// GSM/GPRS module interface

#include "Main.h"
#define RXD2 33            
#define TXD2 32              
#define GSM_Serial_Baud 9600 

void setGSM(){
  Serial2.begin(GSM_Serial_Baud, SERIAL_8N1, RXD2, TXD2);
    delay(1000);  //Intervalo de 1 segundo

  /*Serial2.println("AT"); //Teste de conexão 
  updateSerial(); //Chamada da função updateSerial()
  
  Serial2.println("AT+CMGF=1"); //Configuração do modo SMS text
  updateSerial(); //Chamada da função updateSerial()
  
  Serial2.println(“AT+CMGS=\”+ZZ XXXXXXXXXXX\””); //Número de telefone que irá receber a mensagem, “ZZ” corresponde ao código telefônico do pais e “XXXXXXXXXXX” corresponde ao número de telefone com o DDD
  updateSerial(); //Chamada da função updateSerial()
  
  Serial2.print("Testando oie"); //Texto que será enviado para o usúario
  updateSerial(); //Chamada da função updateSerial()
  
  Serial2.write(26); //confirmação das configurações e envio dos dados para comunicação serial.*/
}

/*void updateSerial() {
  
  while (Serial2.available()) //Verifica se a comunicação serial está disponível
  {
    Serial.write(Serial2.read()); //Realiza leitura serial dos dados de entrada Arduino
  }
  delay(500); //Intervalo de 0,5 segundos
}*/