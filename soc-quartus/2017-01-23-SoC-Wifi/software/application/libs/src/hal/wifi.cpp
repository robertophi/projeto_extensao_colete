#include "wifi.h"


/* static */
WiFi *WiFi::wifi = 0;

/* public */
WiFi::~WiFi() {
}

WiFi *WiFi::getSingleton() {
	if (wifi == 0)
		wifi = new WiFi();
	return wifi;
}

void WiFi::config(char* name, char* password) {
	usleep(1000000);

	//Wifi mode - mode 2 = AP mode (host)
	writeCommand("AT+CWMODE=2");
	readUART2();
	alt_putchar('0');
	usleep(1000000);

	//Set multiple connection or not - mode 1 = multiple conn. enable
	writeCommand("AT+CIPMUX=1");
	readUART2();
	alt_putchar('1');
	usleep(1000000);
	//Set IP address of ESP8266 softAP
	writeCommand("AT+CIPAP=\"192.168.4.1\"");
	readUART2();
	alt_putchar('2');
	usleep(1000000);
	//Generic configuration - ssid, pwd, ch, ecn
	writeCommand("AT+CWSAP=\"ColeteWifi\",\"123456789\",5,3");
	readUART2();
	alt_putchar('3');
	usleep(1000000);
	//Configure as a server - mode 1 = create server
	//						- port 80 (default 333)
	writeCommand("AT+CIPSERVER=1,80");
	readUART2();
	alt_putchar('4');
	usleep(1000000);


	//Server timeout, max7200 segundos
	writeCommand("AT+CIPSTO=600");
	readUART2();
	alt_putchar('5');
	usleep(1000000);



	//----------
	//telnet 192.168.4.1 80
	//para mandar dados para o wifi
}




void WiFi::stopServer() {
	char stop[] = "AT+CIPSERVER=0";
	char rst[] = "AT+RST";
	//to do
}

//Função chamada para ler a uart
void WiFi::readUART(char *data, int *size){
	char ch;
	int i = 0;
	do{
		waitForStatusReady(FIFOED_AVALON_UART_STATUS_RRDY_MSK);
		ch = IORD_FIFOED_AVALON_UART_RXDATA(UART_WIFI_BASE);
	}while(ch !='\n' && ch != ':');

	if(ch == ':'){
		do{
			waitForStatusReady(FIFOED_AVALON_UART_STATUS_RRDY_MSK);
			ch = IORD_FIFOED_AVALON_UART_RXDATA(UART_WIFI_BASE);
			*data=ch;
			data++;
			i++;
		}while(ch != '\n');
		*size = i-2;
	}
	else{
		*data = 0;
		*size = 0;
	}
}


//Funcao chamada para verificar resposta do módulo aos comandos
void WiFi::readUART2(){
	char ch;
	do{
		waitForStatusReady(FIFOED_AVALON_UART_STATUS_RRDY_MSK);
		ch = IORD_FIFOED_AVALON_UART_RXDATA(UART_WIFI_BASE);
		alt_putchar(ch);
	}while(ch !='\n' && ch != ':');

	if(ch == ':'){
		alt_putchar('\n');
		do{
			waitForStatusReady(FIFOED_AVALON_UART_STATUS_RRDY_MSK);
			ch = IORD_FIFOED_AVALON_UART_RXDATA(UART_WIFI_BASE);
			alt_putchar(ch);

		}while(ch != '\n');
		alt_putchar('\n');

	}
}

/* private */
WiFi::WiFi() {
}
//Função usada para enviar os comandos para o módulo
//Envia os caractéres de final de comando (\r e \n) automaticamente
volatile void WiFi::writeCommand(char message[]){
	int i=0;
	char M=message[i];
	//disable interrupts
	//IOWR(UART_WIFI_BASE, 3, 0);


	while(M != '\0'){
		//Envia todos os caractéres do comando, até encontrar '\0'
		waitForStatusReady(FIFOED_AVALON_UART_STATUS_TRDY_MSK);
		IOWR_FIFOED_AVALON_UART_TXDATA(UART_WIFI_BASE,M);
		i=i+1;
		M=message[i];
	}

	//Send end of package characters
	//Padrão do esp8266
		//Send '\r'
		waitForStatusReady(FIFOED_AVALON_UART_STATUS_TRDY_MSK);
		IOWR_FIFOED_AVALON_UART_TXDATA(UART_WIFI_BASE,'\r');
		//Send '\n'
		waitForStatusReady(FIFOED_AVALON_UART_STATUS_TRDY_MSK);
		IOWR_FIFOED_AVALON_UART_TXDATA(UART_WIFI_BASE,'\n');

	//re-enable interrupts
	//IOWR(UART_WIFI_BASE, 3, (FIFOED_AVALON_UART_CONTROL_RRDY_MSK|FIFOED_AVALON_UART_CONTROL_E_MSK));

}

void WiFi::waitForStatusReady(unsigned int mask){
	char status;
	do{
		status = IORD_FIFOED_AVALON_UART_STATUS(UART_WIFI_BASE);
		status = status & mask;
	}while(status==0x00);

}


