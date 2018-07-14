#include <SoftwareSerial.h>

const char* lighton = "light on";
const char* lightoff = "light off";


int buttonState = 0;
float temp;



void setup()
{
  pinMode(5, INPUT);
  pinMode(9, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  // read the state of the pushbutton value
  buttonState = digitalRead(5);
  temp = ((analogRead(1) * 0.004882814) - 0.5) * 100;
  if (temp > 25 && buttonState == HIGH){
    digitalWrite(9, HIGH);
  } else {
    // turn light off
    digitalWrite(9, LOW);
  }
  delay(10);
  
  
  if (digitalRead(9) == HIGH){
  	Serial.println(lighton);
    delay(1000);
  }else{
  	Serial.println(lightoff);
    delay(1000);
  }
}
