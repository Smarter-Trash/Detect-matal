#include <Arduino.h>
#include <ESP32Servo.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Bounce2.h>


const int Proximity=12;
#define trigPIN 5
#define echoPIN 18
#define SOUND_SPEED 0.034
#define SERVO 14
#define BUTTON 15

Bounce button = Bounce();

uint8_t NinaAddress[] = {0x3C, 0x61, 0x05, 0x03, 0x42, 0x70};
uint8_t NunAddress[] = {0xA4, 0xCF, 0x12, 0x8F, 0xBA, 0x18};
uint8_t ViewAddress[] = {0xA4, 0xCF, 0x12, 0x8F, 0xCA, 0x28};
uint8_t FaiiAddress[] = {0x24, 0x6F, 0x28, 0x25, 0x86, 0xDC};

long duration;             
float distance;

int countM = 0;
int countNM = 0;
int active = 0;
int cost = 0;

Servo myservo; 

typedef struct trash_status {
    int state;
    int plastic;
    int metal;
} trash_status;

typedef struct pay_coin { 
  int state;
  int cost;
}pay_coin;

trash_status send_count_trash;
trash_status recv_count_trash;
pay_coin send_cost;
pay_coin send_coin;

esp_now_peer_info_t peerInfo1;
esp_now_peer_info_t peerInfo2;
esp_now_peer_info_t peerInfo3;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);

  if (mac_addr[0] == 0x3C && mac_addr[1] == 0x61 && mac_addr[2] == 0x05 && mac_addr[3] == 0x03 && mac_addr[4] == 0x42 && mac_addr[5] == 0x70){
    memcpy(&recv_count_trash, incomingData, sizeof(recv_count_trash));

    if(recv_count_trash.state == 1){
        Serial.println("START DETECT!");
        countM = 0;
        countNM = 0;

        send_count_trash.state = 2;
        send_count_trash.metal = countM;
        send_count_trash.plastic = countNM;
        esp_err_t result = esp_now_send(NinaAddress, (uint8_t *) &send_count_trash, sizeof(send_count_trash));
        if (result == ESP_OK) {
            Serial.println("Sent with success");
        }
        else {
            Serial.println("Error sending the data");
        }
        active = 1;
    }
    
    else if (recv_count_trash.state == 3){
        active = 0;
        cost = (countM*5) + countNM;

        send_cost.state = 4;
        send_cost.cost = cost;
        esp_err_t result1 = esp_now_send(FaiiAddress, (uint8_t *) &send_cost, sizeof(send_cost));
        if (result1 == ESP_OK) {
            Serial.println("Sent with success");
        }
        else {
            Serial.println("Error sending the data");
        }
        delay(500);

        esp_err_t result2 = esp_now_send(NinaAddress, (uint8_t *) &send_cost, sizeof(send_cost));
        if (result2 == ESP_OK) {
            Serial.println("Sent with success");
        }
        else {
            Serial.println("Error sending the data");
        }
        delay(500);
        cost = 0;
    }
  }
}

void setup() {
    pinMode(trigPIN, OUTPUT);
    pinMode(echoPIN, INPUT);
    pinMode(Proximity, INPUT);

    button.attach(BUTTON, INPUT_PULLUP);
    button.interval(25); 

    myservo.attach(SERVO);  // attaches the servo on pin 13 to the servo object
    /*myservo.write(90);
    delay(1000);*/

    Serial.begin(115200);
    Serial.println("hello");
    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_register_recv_cb(OnDataRecv);
    esp_now_register_send_cb(OnDataSent);
    
    memcpy(peerInfo1.peer_addr, FaiiAddress, 6);
    peerInfo1.channel = 0;  
    peerInfo1.encrypt = false;

    memcpy(peerInfo2.peer_addr, NinaAddress, 6);
    peerInfo2.channel = 0;  
    peerInfo2.encrypt = false;

    memcpy(peerInfo3.peer_addr, ViewAddress, 6);
    peerInfo3.channel = 0;  
    peerInfo3.encrypt = false;
  
    // Add peer        
    if (esp_now_add_peer(&peerInfo1) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }

    if (esp_now_add_peer(&peerInfo2) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }

    if (esp_now_add_peer(&peerInfo3) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
  
}
 
void loop() {
    
    if (active == 1){

        digitalWrite(trigPIN, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPIN, LOW);
        
        duration = pulseIn(echoPIN, HIGH);
        
        // Calculate the distance
        distance = duration * SOUND_SPEED/2;
        Serial.println(distance);
        Serial.println(digitalRead(Proximity));

        if(distance<=8){
            Serial.println("found");
            delay(500);
            if(digitalRead(Proximity)==0){ 
                Serial.println("Metal Object Detected");
                myservo.write(0);
                delay(500);
                myservo.write(90);
                delay(500);
                countM++;
                send_count_trash.state = 2;
                send_count_trash.metal = countM; 
                esp_err_t result = esp_now_send(NinaAddress, (uint8_t *) &send_count_trash, sizeof(send_count_trash));
   
                if (result == ESP_OK) {
                    Serial.println("Sent with success");
                }
                else {
                    Serial.println("Error sending the data");
                }
                delay(500);
            }
            else{
                Serial.println("NonMetal Object Detected");
                myservo.write(180);  
                delay(500);
                myservo.write(90);
                delay(500);
                countNM++;
                send_count_trash.state = 2;
                send_count_trash.plastic = countNM;
                esp_err_t result = esp_now_send(NinaAddress, (uint8_t *) &send_count_trash, sizeof(send_count_trash));
   
                if (result == ESP_OK) {
                    Serial.println("Sent with success");
                }
                else {
                    Serial.println("Error sending the data");
                }
                delay(500);
            }
        }

        else{
            Serial.println("Not found");
        }
        

        delay(1000);
    }

    else if (active == 0){
        button.update();

        if (button.fell()) { 

            send_cost.state = 13;
            send_cost.cost = 0;
            
            esp_err_t result3 = esp_now_send(FaiiAddress, (uint8_t *) &send_cost, sizeof(send_cost));
            if (result3 == ESP_OK) {
                Serial.println("Sent with success");
            }
            else {
                Serial.println("Error sending the data");
            }
            delay(500);

            esp_err_t result4 = esp_now_send(ViewAddress, (uint8_t *) &send_cost, sizeof(send_cost));
             if (result4 == ESP_OK) {
                Serial.println("Sent with success");
            }
            else {
                Serial.println("Error sending the data");
            }
        }
    } 
}
