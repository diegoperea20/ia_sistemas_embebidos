#include <Arduino.h>

//Variables asociadas a los dos LEDs que se van a controlar
int led_1 = 2;//Banana
int led_2 = 3;//naranja
int led_3 = 4;//limon
int led_4 = 5;//fresa
int led_5 = 6;//manzana
char valor;  //Variable para indicar que llega una orden

void setup() {

  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);
  pinMode(led_4, OUTPUT);
  pinMode(led_5, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  if (Serial.available())  //Si el puerto serie (Bluetooth) está disponible
  {
    valor = Serial.read();  //Lee el dato entrante via Bluetooth
    //Banana
    if (valor == 'A')   //Si el dato que llega es una A
    {
      digitalWrite(led_1, HIGH);  //Enciende el LED 1
    }
    if (valor == 'B')   //Si el dato que llega es una B
    {
      digitalWrite(led_1, LOW);   //Apaga el LED 1
    }
    //Naranja
    if (valor == 'C')   //Si el dato que llega es una B
    {
      digitalWrite(led_2, HIGH);   //Apaga el LED 1
    }
    if (valor == 'D')   //Si el dato que llega es una B
    {
      digitalWrite(led_2, LOW);   //Apaga el LED 1
    }
    //Limon
    if (valor == 'E')   //Si el dato que llega es una B
    {
      digitalWrite(led_3, HIGH);   //Apaga el LED 1
    }
    if (valor == 'F')   //Si el dato que llega es una B
    {
      digitalWrite(led_3, LOW);   //Apaga el LED 1
    }
    //Fresa
    if (valor == 'G')   //Si el dato que llega es una B
    {
      digitalWrite(led_4, HIGH);   //Apaga el LED 1
    }
    if (valor == 'H')   //Si el dato que llega es una B
    {
      digitalWrite(led_4, LOW);   //Apaga el LED 1
    }
    //Manzana
    if (valor == 'I')   //Si el dato que llega es una B
    {
      digitalWrite(led_5, HIGH);   //Apaga el LED 1
    }
    if (valor == 'J')   //Si el dato que llega es una B
    {
      digitalWrite(led_5, LOW);   //Apaga el LED 1
    }
    
  }
}
