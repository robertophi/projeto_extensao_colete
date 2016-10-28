#include "app.h"

#define SSID "ColeteWifi"
#define PASSWORD "12345678"
#define COLUMNS 5
#define LINES 8
//Senha não pode ter menos de 8 caracteres

/*Static*/
Motors	*App::motors = Motors::getSingleton();
FFT	*App::fft = FFT::getSingleton();
App::Buffer	App::buffer;

App::App() {
	wifi = WiFi::getSingleton();
	compass_vib_value = 25;
}

App::~App() {
	wifi->stopServer();
}

void App::setup() {
	wifi->config(SSID, PASSWORD);
	wifi->startServer();
	//fft->setInterruptHandler(App::fftHandler);
}

void App::fftHandler(unsigned int output) {

	//	motors->write(output);
	fft->read();

	if(buffer.length() > 0)
		fft->write(buffer.pop());
}

//If we have 5 motors in a column, each motor is responsible for 72� each; We receive a float in (-180�, 180�]. To simplify we add 180 to it.
void App::writeCompass(int direction) {
	int responsible = 360/COLUMNS;
	direction += 180;
	int motor = direction/responsible;
	int neighbor;
	float remainder = direction % responsible;
	if(remainder > 0.5) {
		remainder = 1-remainder;
		neighbor = motor;
		motor--;
	} else {
		neighbor = motor - 1;
	}
	int cmd = 1 << 24;
	int line = (LINES-1) << 16;
	int column = (motor-1) << 8;
	neighbor = (neighbor-1) << 8;
	int neighbor_remainder = compass_vib_value*remainder;

	motors->write((int)( cmd | line |  column	|  2 ));
	motors->write((int)( cmd | line | neighbor	|  2 ));
	motors->write((int)(  0  | line |  column	| compass_vib_value ));
	motors->write((int)(  0  | line | neighbor	| neighbor_remainder ));
	cmd = 2 << 24;
	motors->write((int)( cmd | line |  column	|  1 ));
	motors->write((int)( cmd | line | neighbor	|  1 ));
}

void App::writeGyroscope(int xAngle, int yAngle, int zAngle) {
	int x = defineIndex(xAngle) << 16;
	int y = defineIndex(yAngle) << 8;
	motors->write(( (1 << 24) | x | y | 2 ));
	motors->write((     0     | x | y | (zAngle > 100 ? 100 : zAngle)));
	motors->write(( (2 << 24) | x | y | 1 ));
}

int App::defineIndex(int value) {
	float line_int = 200/LINES-2;
	// each motor has an interval defined by this size
	int half_mot = (LINES - 1)%2 == 0 ? LINES/2 : ((int)(LINES - 1)/2) + 1;
	// the motor in the middle of the line/column
	float half_int_end = line_int*(half_mot-1);
	// the point in the end of the middle motor interval
	if((-1)*half_int_end/2 < value && value < half_int_end/2) {
		return half_mot;
		// if the value is in the middle interval, return the middle identifier
	}
	float mot_int_b = half_int_end/2;
	// the begin of the interval that will be tested
	int x = -1;
	if(value >= 0) { // if is in the positive part
		for(int i = 1; 2*i < LINES-2 && x == -1; i++) {
			// for each motor in the positive part but the last
			if(mot_int_b <= value && value < mot_int_b + line_int) {
				x = i + half_mot;
				// if the value is in the interval of the motor being tested,
				// return it's identifier
			} else {
				mot_int_b += line_int; // else, just set to test the next motor
			}
		}
		if (x == -1) {
			x = LINES-1; // if it is in the positive part and the responsible
			// motor was not found, then the signal is bigger than 100
		}
		return x;
	}
	mot_int_b *= -1; // set to test in the negative part
	line_int *= -1;
	for(int i = 1; 2*i < LINES-2 && x == -1; i++) {
		//for each motor in the negative part
		if(mot_int_b >= value && value > mot_int_b + line_int) {
			x = i + half_mot; // if the value is in the interval of the motor
			// being tested, return it's identifier
		} else {
			mot_int_b += line_int; // else, just set to test the next motor
		}
	}
	if (x == -1) {
		x = 0; // if it is in the negative part and the responsible
		// motor was not found, then the signal is smaller than -100
	}
	return x;
}

void App::run() {
	unsigned char *data;
	unsigned int *size;

	//Shift amount de 10, Decaimento de 15 e potencia de 255 em cada motor, durante inicialização do programa.
	motors->write((1<<24)|(255<<16)|(255<<8)|(10));
	motors->write((2<<24)|(255<<16)|(255<<8)|(15));
	motors->write((0<<24)|(255<<16)|(255<<8)|(255));

	while (1) {
		wifi->receive(data, size);
		char type = data[0];
		switch (type) {
		case 'm': { /* motors */
			int cmd = (int)(data[1]);
			int linha = (int)(data[2]);
			int coluna = (int)(data[3]);
			int valor = (int)(data[4]);
//			alt_putstr("Motors received. Sending it to the motors...\n");
			int command = (  (cmd << 24) | (linha << 16) | (coluna << 8) | (valor) );
//			alt_printf("Comando: %d %d %d %d = %d\n" ,cmd, linha, coluna, valor, command );
			motors->write(command);
		}
			break;
		case 'a': { /*audio*/
//			alt_putstr("Audio received. Sending it to the FFT...\n");
//			alt_printf("%s\n", data);
			//buffer.push(data);
			motors->write_to_next_line();


//			|           _
//			|    _     / \
//		____|___/_\___/___\_____
//			|\_/   \_/     \   /
//			|               \_/

			//if(!fft->isProcessing()) {
			//	fft->write(buffer.pop());
			//}


		}
			break;
		case 'c': { /*compass*/
//			alt_putstr("Compass received. Sending to the motors...\n");
//			alt_printf("%s\n", data);

			writeCompass((int)data[1]);
		}
			break;
		case 'g': { /*gyroscope*/
//			alt_putstr("Gyroscope received. Sending to the motors...\n");
//			alt_printf("%s\n", data);
			writeGyroscope(int(data[1]), int(data[2]), int(data[3]));
		}
			break;
		default: {
//			alt_putstr("Some data received. Don't know what to do...\n");
//			alt_printf("%s\n", data);
		}
			break;
		}
	}
}
