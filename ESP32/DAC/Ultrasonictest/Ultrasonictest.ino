#include <Ultrasonic.h>

float distancia = 0 ;
#define TRIGGER_PIN 19
#define ECHO_PIN 18

int leds[] = {14, 27, 26, 25, 33, 12, 0};

void loop2(void * p ) {

  Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);
  for (;;) {
    distancia = ultrasonic.convert(ultrasonic.timing(), Ultrasonic::CM);
     
    delay(10);
  }
  vTaskDelete(NULL);
}

void setup() {
  Serial.begin(115200);
  ledcSetup(0, 2500, 8);
  ledcAttachPin(21, 0);

  for (int i = 0 ; leds[i]; i++) {
    pinMode(leds[i], OUTPUT);
  }
  BaseType_t xReturned;
  TaskHandle_t xHandle = NULL;
  xReturned = xTaskCreate(
                loop2,       /* Function that implements the task. */
                "Loop2",          /* Text name for the task. */
                10000,      /* Stack size in words, not bytes. */
                NULL,    /* Parameter passed into the task. */
                tskIDLE_PRIORITY,/* Priority at which the task is created. */
                &xHandle );      /* Used to pass out the created task's handle. */
   delay(500);
  if ( xReturned == pdPASS )
  {
    /* The task was created.  Use the task's handle to delete the task. */
    /*vTaskDelete( xHandle );*/
    Serial.println("tarefa criada");
  }

}

void loop() {
  int pot = 0, i,som = 0 ;

  if (distancia < 50 ){
    pot = map(distancia, 0, 50, 6, 0);
    som = map(distancia,0,50,250,0);
  }
  
  for ( i = 0; i < pot && leds[i] ; i++)
    digitalWrite(leds[i], 1);
  for (; leds[i]; i++)
    digitalWrite(leds[i], 0);
    ledcWrite(0, som);
}
