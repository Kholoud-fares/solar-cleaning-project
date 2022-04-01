/*code for solar cleaning project 
which include two motors for wheels, motor for brush and pump and be accessd with RFID and password

 component used
 1-Arduino mega
 2- Dual monster motor driver
 3- 4 Relay module for pump and brush
 4- 2 Ultrasonic to sense the end of solar cell
 5- Keypad
 6- RFID
 7- Water level sensor
 8- LCD to dispaly water level sensor reading and if password right or wrong
 */

 //kholoud shaker 
 //(4/1/2022)

#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 53
#define RST_PIN 47
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
bool flag=false;

//keypad
#include <Keypad.h>
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {23,25,27,29}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {31,33,35}; //connect to the column pinouts of the keypad
char pass[6];
byte count=0;
String password="1234";
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//motors
#define MOTOR_A1_PIN 7
#define MOTOR_B1_PIN 8 
//MOTOR 2
#define MOTOR_A2_PIN 4
#define MOTOR_B2_PIN 9

#define PWM_MOTOR_1 5
#define PWM_MOTOR_2 6
#define EN_PIN_1 A0
#define EN_PIN_2 A1

//ultrasonic
//ultrasonic1
int trigPin1 =39;
int echoPin1 =41 ;
long distance1;
long duration1;
//ultrasonic2
int trigPin2 =40;
int echoPin2 =38;
long distance2;
long duration2;

//LCD
#include <LiquidCrystal.h>
const int rs = 16, en = 17, d4 = 18, d5 = 19, d6 = 20, d7 = 21;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//water level
int sensorPower= A15;
int sensorPin= A14;
int val;
int x;

//brush
int rb1=A8;
int rb2=A9;
//pump
int rp1=A10;
int rp2=A11;


void setup() 
{
  pinMode(sensorPower, OUTPUT);
  digitalWrite(sensorPower, LOW);
  pinMode(rb1, OUTPUT);       
  pinMode(rb2, OUTPUT);
  pinMode(rp1, OUTPUT);
  pinMode(rp2, OUTPUT);
  pinMode(MOTOR_A1_PIN, OUTPUT);
  pinMode(MOTOR_B1_PIN, OUTPUT);
  pinMode(MOTOR_A2_PIN, OUTPUT);
  pinMode(MOTOR_B2_PIN, OUTPUT);
  pinMode(PWM_MOTOR_1, OUTPUT);
  pinMode(PWM_MOTOR_2, OUTPUT);
  pinMode(EN_PIN_1, OUTPUT);
  pinMode(EN_PIN_2, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin2, OUTPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  SPI.begin();      // Initiate  SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  
}
void loop(){
  lcd.clear();
  lcd.print("Put your Card");
  delay(1000);
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
  String UID="";
  for (byte i=0;i<rfid.uid.size;i++) 
  {
     Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");// E8 07 C7 D2
     Serial.print(rfid.uid.uidByte[i], HEX);
     UID.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ")); // 0E c2 12 
     UID.concat(String(rfid.uid.uidByte[i], HEX));
  }
  UID.toUpperCase();
  // E8 07 C7 D2
  if (UID.substring(1) == "77 29 54 27") //change here the UID of the card/cards that you want to give access
  { lcd.clear();
    lcd.setCursor(0,0);
    lcd.println("enter password");
    while(count<=3){
     int key = keypad.getKey();
     if (key){
       pass[count] = key;
       count++;
        }}
      Serial.println(pass);
      lcd.clear();
     if (password == pass){
       lcd.print("Right");
       flag=true;
       delay(2000);
       
       }
     else{
      lcd.print("Wrong");
    }
  }
  while(flag){
    lcd.clear();
    digitalWrite(sensorPower, HIGH);  // turn the sensor ON
    delay(10);                      // wait 10 milliseconds
    val = analogRead(sensorPin); // read the analog value from sensor
    digitalWrite(sensorPower, LOW);   // turn the sensor OFF
    x= map(val,0,1023,0,40);
    lcd.print("water level=");
    lcd.print(x);
    lcd.println("mm");
    
    digitalWrite(trigPin1, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin1, LOW);
    duration1 = pulseIn(echoPin1, HIGH);
    distance1 = duration1*0.034/2; 
    /*lcd.clear();
    lcd.print("d1=");
    lcd.print(distance1);*/
    digitalWrite(rb1,HIGH);
    delay(1000);
    digitalWrite(rp1,HIGH);
    delay(1000);
    digitalWrite(EN_PIN_1, HIGH);
    digitalWrite(MOTOR_A1_PIN, HIGH);
    digitalWrite(MOTOR_B1_PIN, LOW);
    analogWrite(PWM_MOTOR_1, 200);
    digitalWrite(EN_PIN_2, HIGH);
    digitalWrite(MOTOR_A2_PIN, HIGH);
    digitalWrite(MOTOR_B2_PIN, LOW);
    analogWrite(PWM_MOTOR_2,200); 
    while(distance1<20){
         digitalWrite(trigPin1, LOW);
         delayMicroseconds(2);
         digitalWrite(trigPin1, HIGH);
         delayMicroseconds(10);
         digitalWrite(trigPin1, LOW);
         duration1 = pulseIn(echoPin1, HIGH);
         distance1 = duration1*0.034/2;
         /*lcd.clear(); 
         lcd.print("d1=");
         lcd.print(distance1);  */       
       }
       
    digitalWrite(trigPin2, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin2, LOW);
    duration2 = pulseIn(echoPin2, HIGH);
    distance2 = duration2*0.034/2; 
    /*lcd.clear();
    lcd.print("d2=");
    lcd.print(distance2);*/
    digitalWrite(EN_PIN_1, HIGH);
    digitalWrite(MOTOR_A1_PIN, LOW);
    digitalWrite(MOTOR_B1_PIN,HIGH);
    analogWrite(PWM_MOTOR_1, 200);
    digitalWrite(EN_PIN_2, HIGH);
    digitalWrite(MOTOR_A2_PIN, LOW);
    digitalWrite(MOTOR_B2_PIN, HIGH);
    analogWrite(PWM_MOTOR_2, 200);
    delay(500);
    while(distance2<20){
         digitalWrite(trigPin2, LOW);
         delayMicroseconds(2);
         digitalWrite(trigPin2, HIGH);
         delayMicroseconds(10);
         digitalWrite(trigPin2, LOW);
         duration2 = pulseIn(echoPin2, HIGH);
         distance2 = duration2*0.034/2; 
         /*lcd.clear();
         lcd.print("d2=");
         lcd.print(distance2); */        
       }

     digitalWrite(EN_PIN_1, LOW);
     digitalWrite(MOTOR_A1_PIN, LOW);
     digitalWrite(MOTOR_B1_PIN, LOW);
     analogWrite(PWM_MOTOR_1, 130);
     digitalWrite(EN_PIN_2, LOW);
     digitalWrite(MOTOR_A2_PIN, LOW);
     digitalWrite(MOTOR_B2_PIN, LOW);
     analogWrite(PWM_MOTOR_2, 150);
     digitalWrite(rb1,LOW);
     digitalWrite(rb2,LOW);
     digitalWrite(rp1,LOW);
     
     digitalWrite(rp2,LOW);
     lcd.clear();
     lcd.print("stop");
     delay(1000);
     flag=false;
  }
  rfid.PICC_HaltA();
  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();

  
}}
