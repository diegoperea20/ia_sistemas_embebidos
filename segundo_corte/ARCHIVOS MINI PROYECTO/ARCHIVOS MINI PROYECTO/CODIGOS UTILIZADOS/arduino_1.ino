#include <SoftwareSerial.h>
SoftwareSerial BT1(10, 11); // RX | TX
uint8_t a=0;
void setup()
  { 
    Serial.begin(115200);
    BT1.begin(57600); 

    pinMode(2,INPUT);
    pinMode(3,INPUT);
    pinMode(4,INPUT);
    pinMode(5,INPUT);
    pinMode(6,INPUT);
  }
 
void loop()
  {  

    if(digitalRead(2)==HIGH){
      a=1;
      BT1.write("1");
      //tenis
      }
      if(digitalRead(3)==HIGH){
        a=2;
      BT1.write("2");
      //golf
      }
      if(digitalRead(4)==HIGH){
        a=3;
      BT1.write("3");
      //baloncesto
      }
      if(digitalRead(5)==HIGH){
        a=4;
      BT1.write("4");
      //correr
      }
      if(digitalRead(6)==HIGH){
        a=5;
      //tiro con arco
      }
      switch(a){
        case 1:
        BT1.write("1");
        break;

        case 2:
        BT1.write("2");
        break;

        case 3:
        BT1.write("3");
        break;

        case 4:
        BT1.write("4");
        break;

        case 5:
        BT1.write("5");
        break;
        }
    //Serial.println(a);
    delay(1000);

    /*//Serial.println()
    if (BT1.available())
           Serial.write(BT1.read());
     if (Serial.available())
        BT1.write(Serial.read());*/
  }
