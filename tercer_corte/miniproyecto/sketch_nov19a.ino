int ventilador =9;
int a=HIGH;

void setup() {
  // put your setup code here, to run once:
    pinMode(7,INPUT);
    pinMode(ventilador,OUTPUT);
    Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  a=digitalRead(7);
  if(a==HIGH){
    digitalWrite(ventilador,HIGH);
    }else{
      digitalWrite(ventilador,LOW);
      }

  Serial.println(a);
}
