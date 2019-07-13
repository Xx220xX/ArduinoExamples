#include "BluetoothSerial.h"
#define PONTOS  1000
#define PS 999
unsigned char  valor[PONTOS] = {0};
double teta;
int letra;
char edit = 0;
BluetoothSerial SerialBT;
void SendData(int n);
void SendData(String s);

void gerarNumeros(double teta) {
  Serial.println("\n gerando numeros para teta = " + String(teta));
  int i = 0;
  double angle = PI * teta / 180.0;
  SendData("fazendo pontos:\n");
  String s = "";
  double delta = 2*PI/PONTOS;
  for (; i < PONTOS; i++) {
    valor[i] = 127.5* ( sin(angle * i * 0.006)+1 ); //112 pontos de 0 a 255
    s += String(valor[i]) +(i%12==0?"\n":" ");
  }
  SendData(s);
}
void musica(void *p) {
  configASSERT (((uint32_t) p) == 1);
  int i = 0;
 
    for (i = 0; ;) {
      dacWrite(25, valor[i]);
      i=i>=PS?0:i+1;
    }
  
  vTaskDelete(NULL);

}
void setup() {
  gerarNumeros(150);
  Serial.begin(115200);
  SerialBT.begin("Musiquinha");
  BaseType_t xReturned;
  TaskHandle_t xHandle = NULL;
  xReturned = xTaskCreate(
                musica,       /* Function that implements the task. */
                "MUSICA",          /* Text name for the task. */
                10000,      /* Stack size in words, not bytes. */
                ( void * ) 1,    /* Parameter passed into the task. */
                tskIDLE_PRIORITY,/* Priority at which the task is created. */
                &xHandle );      /* Used to pass out the created task's handle. */

  if ( xReturned == pdPASS )
  {
    /* The task was created.  Use the task's handle to delete the task. */
    /*vTaskDelete( xHandle );*/
    Serial.println("tarefa criada");
  }

}

void loop() {

  if (SerialBT.available()) {
    edit = 1;
    teta = 0;
    int decimais = 0;
    while (SerialBT.available()) {
      letra = SerialBT.read();
      Serial.print((char)letra);
      if (letra >= '0' && letra <= '9') {
        teta = teta * 10 + (letra - '0');
        if (decimais)decimais++;
      } else if (letra = '.' ) {
        decimais++;
      } else {
        break;
      }
    }
    edit = 0;
    gerarNumeros(teta * pow(10, -decimais));
  }

  delay(500);
}
void SendData(int n) {
  SendData(String(n, DEC));
}
void SendData(String s) {
  int len = s.length();
  char *buff = (char *)calloc(len + 1, sizeof(char));
  s.toCharArray(buff, len);
  if (SerialBT.hasClient()) {
    SerialBT.write((uint8_t*)buff, len);
  }
  Serial.println(buff);
  free(buff);

}
