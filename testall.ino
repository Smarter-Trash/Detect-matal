#include <Servo.h>
#define TRIG_PIN 23 // ESP32 pin GIOP23 connected to Ultrasonic Sensor's TRIG pin
#define ECHO_PIN 22 // ESP32 pin GIOP22 connected to Ultrasonic Sensor's ECHO pin
Servo myservo;  // create servo object to control a servo
float duration_us, distance_cm;
int pos = 0;    // variable to store the servo position
const int Proximity=12;
int countM = 0;
int countNM = 0;
int found = 0;
int proxValue = 99;
void setup() {
    pinMode(TRIG_PIN, OUTPUT);// configure the trigger pin to output mode
    pinMode(ECHO_PIN, INPUT);  // configure the echo pin to input mode
    pinMode(Proximity, INPUT);
    myservo.attach(13);  // attaches the servo on pin 13 to the servo object
    myservo.write(90);
    Serial.begin(9600);
}
 
void loop() {
    // generate 10-microsecond pulse to TRIG pin
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    
    // measure duration of pulse from ECHO pin
    duration_us = pulseIn(ECHO_PIN, HIGH);
  
    // calculate the distance
    distance_cm = 0.017 * duration_us;
    
    if(distance_cm<=2){
      Serial.println("found");
      if(digitalRead(Proximity)==HIGH){ 
          Serial.println("Metal Object Detected");
          myservo.write(0);
          delay(1000);
          countM++;
          
      }
      else{
          Serial.println("NonMetal Object Detected");
          myservo.write(180);  
          delay(1000);
          countNM++;
      }
      myservo.write(90);
      delay(2000);
   }
   else{
    delay(2000);   
   }
}
