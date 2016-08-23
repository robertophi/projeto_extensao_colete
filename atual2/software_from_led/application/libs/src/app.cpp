#include "app.h"
#include <iostream>
//#include <bitset>
#define SSID "Hunger"
#define PASSWORD "12345678"

/*Static*/
Motors	*App::motors = Motors::getSingleton();
FFT	*App::fft = FFT::getSingleton();
App::Buffer	App::buffer;

App::App() {
	wifi = WiFi::getSingleton();
}

App::~App() {
	wifi->stopServer();
}

void App::setup() {
	wifi->config(SSID, PASSWORD);
	wifi->startServer();
	fft->setInterruptHandler(App::fftHandler);
}

void App::fftHandler(unsigned int output) {
	printf("Got FFT output %d\n", output);

	//	motors->write(output);
	printf("%i\n", buffer.length());
	fft->read();

	if(buffer.length() > 0)
		fft->write(buffer.pop());
}

void App::writeCompass(unsigned char direction) {
	switch(direction) {
		case 'N':
			printf("Sending direction as North.\n");
			break;
		case 'W':
			printf("Sending direction as West.\n");
			break;
		case 'E':
			printf("Sending direction as East.\n");
			break;
		case 'S':
			printf("Sending direction as South.\n");
			break;
	}
}

void App::writeGyroscope(int xAngle, int yAngle) {
	printf("Sending angle to vest.\n");
	printf("%i\n", xAngle);
	printf("%i\n", yAngle);
}

void App::run() {
	unsigned char *data = new unsigned char[MAX_SIZE];
	unsigned int *size;
	int controle = 0;
	while (1) {

		//wifi->receive(data, size);

		char type = data[0];   
		type    = 'm';
		//data[1] = char(0);
		//data[2] = char(0);
		//data[3] = char(0);
		//data[4] = char(50);

		if(controle == 0){
			int command = (1<<24)|(255<<16)|(255<<8)|(1);
			motors->write(command);
			command = (0<<24)|(255<<16)|(255<<8)|(255);
			motors->write(command);
			command = (2<<24)|(255<<16)|(255<<8)|(1);
			motors->write(command);
			printf("Feito comandos variação all e potencia all e decaimento all");
			controle = 1;
		}


		//wifi->receive(data, size);
		//char type = data[0];
		switch (type) {
		case 'm': { /* motors */
            printf("Motors received. Sending it to the motors...\n");
			/*
            int op    = (int)(data[1]);
            int linha = (int)(data[2]);
            int col   = (int)(data[3]);
            int val   = (int)(data[4]);
			int command = (op<<24)|(linha<<16)|(col<<8)|(val);
            
            //int command = (data[1] << 24) |(data[2] << 16) | (data[3]) << 8)| data[4];
            //Trocado o "OR LOGICAL ||" por "OR BITWISE |" na formação do comando



			motors->write(command);
			*/
		}
			break;
		case 'a': { /*audio*/
			printf("Audio received. Sending it to the FFT...\n");
			printf("%s\n", data);
			buffer.push(data);

			if(!fft->isProcessing()) {
				fft->write(buffer.pop());
			}
		}
			break;
		case 'c': { /*compass*/
			printf("Compass received. Sending to the motors...\n");
			printf("%s\n", data);
			writeCompass(data[1]);
		}
			break;
		case 'g': { /*gyroscope*/
			printf("Gyroscope received. Sending to the motors...\n");
			printf("%s\n", data);
			writeGyroscope(int(data[1]), int(data[2]));
		}
			break;
		default: {
			printf("Some data received. Don't know what to do...\n");
			printf("%s\n", data);
		}
			break;
		}
	}
}
