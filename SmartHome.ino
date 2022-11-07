#include<Servo.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
Servo myservo;
Servo myservo1;
SoftwareSerial mySerial(9, 10);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
LiquidCrystal lcd1(34, 33, 31, 30, 29, 28);

int ldr = A2;
int value = 0;

const int flamePin = 50;
int Flame = 0;

int pirPin = 20;
int pirState = 0;

int pos = 0;
int pos1 = 0;

int val;
int b1 = 22;

int ledr = 25;
int ledg = 21;
int ledy = 40;

int cnt = 0, cnt2;
int timer = 0;
byte tx = 1;

int Buzzer = 6;
int Buzzer1 = 53;
int Buzzer2 = 44;
int Buzzer3 = 46;
int Buzzer4 = 52;


const int trigPin = 37;
const int echoPin = 36;
long duration;
int distance;

int gasC_1 = 0;
int smkC_1 = 0;
const int SensorPin1 = A0;
const int SensorPin2 = A1;
String textForSMS;



void setup()
{
  lcd.begin(16, 2);
  lcd1.begin(16, 2);
  delay(100);
  mySerial.begin(9600);
  Serial.begin(9600);

  pinMode(flamePin, INPUT);
  pinMode(tx, OUTPUT);
  pinMode(SensorPin1, INPUT);
  pinMode(SensorPin2, INPUT);
  pinMode(pirPin, INPUT);
  pinMode(b1, INPUT_PULLUP);
  pinMode(ledr, OUTPUT);
  pinMode(ledg, OUTPUT);
  pinMode(ledy, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //pinMode(ledb, OUTPUT);


  myservo.attach(7);
  myservo1.attach(47);
  cnt = 0;

}
void loop()
{

  ////////////////////////**********AUTO LIGHT*********//////////////////////


  value = analogRead(ldr);
  
  if (value >= 300)
  {
    digitalWrite(ledy, HIGH);
  }

  else 
  {
    digitalWrite(ledy, LOW);
  }


  ////////////////////////**********GAS LEAKAGE ALERT*********//////////////////////

  int gasC_1 = analogRead(SensorPin1);
  int SmkC_1 = analogRead(SensorPin2);

  gasC_1 = analogRead(SensorPin1);
  gasC_1 = (5.0 * gasC_1 * 100.0) / 1024.0;

  smkC_1 = analogRead(SensorPin2);
  smkC_1 = (5.0 * smkC_1 * 100.0) / 1024.0;
  delay(50);

  if (gasC_1 >= 102 || smkC_1 >= 102)
  {
    tone(Buzzer, 500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  THERE IS SMOKE ");
    lcd.setCursor(0, 1);
    lcd.print(" NOT SAFE HERE ");
    delay(100);
    lcd.clear();
    lcd.print("Sending SMS...");
    delay(100);

    Serial.print("AT+CMGF=1\r");
    delay(100);
    Serial.print("AT+CMGS=\"+01840413707\"\r");
    Serial.print("GAS LEAKAGE ALERT! Please Be Informed that gas leakage has Occured!\r");
    delay(200);
    Serial.println((char)26); // End AT command with a ^Z, ASCII code 26
    delay(200);
    Serial.println();

    for (pos = 0; pos <= 90; pos = pos + 5)
    {
      myservo1.write(pos);
      delay(100);

    }



  }
  else
  {


    noTone(Buzzer);
    delay(200);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("    NO SMOKE    ");
    lcd.setCursor(0, 1);
    lcd.print(" KITCHEN IS SAFE ");
    delay(100);


  }

  ////////////////////////**********VIBRATION ALERT(DOOR/WINDOW BROKER)*********//////////////////////

  if (digitalRead(b1) == HIGH) {


    tone(Buzzer3, 500);
    delay(200);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WINDOW IS NOT SAFE");
    lcd.setCursor(0, 1);
    lcd.print("   PLEASE CHECK  ");
    digitalWrite(ledr, HIGH);
    delay(300);               // wait for a second
    digitalWrite(ledr, LOW);
    delay(300);               // wait for a second


  }
  else
  {
    noTone(Buzzer3);
    delay(50);
    lcd.clear();
  }

  ////////////////////////**********PIR INTRUDER*********//////////////////////


  pirState = digitalRead(pirPin);

  lcd1.setCursor(0, 0);
  lcd1.print("SMART HOME");
  if (pirState == LOW)
  {

    noTone(Buzzer1);
    delay(100);
    //lcd1.clear();
    lcd1.setCursor(0, 1);
    lcd1.print("NO INTRUDER DETECT");
    digitalWrite(ledr, LOW);


  }
  else if (pirState == HIGH) {

    tone(Buzzer1, 500);
    lcd1.setCursor(0, 1);
    lcd1.print("ALARM INTRUDER");
    delay(100);

    digitalWrite(ledr, HIGH);
    delay(100);               // wait for a second
    digitalWrite(ledr, LOW);
    delay(100);
    lcd1.clear();

  }



  ////////////////////////**********ULTRASONIC INTRUDER*********//////////////////////


  digitalWrite(trigPin, LOW);
  delay(2);
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;



  if (distance <= 100)
  {

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("INTRUDER IS NEAR");
    tone(Buzzer2, 500);
    delay(200);
    digitalWrite(ledr, HIGH);

  }
  else
  {
    noTone(Buzzer2);
    delay(200);
    digitalWrite(ledr, LOW);
  }

  ////////////////////////**********FIRE ALERT*********//////////////////////


  Flame = digitalRead(flamePin);

  if (Flame == LOW)
  {
    digitalWrite(ledr, LOW);
    noTone(Buzzer4);
    delay(100);

  }

  else if (Flame == HIGH)
  {
    digitalWrite(ledr, HIGH);
    tone(Buzzer4, 500);
    delay(200);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   ALERT   ");
    lcd.setCursor(0, 1);
    lcd.print("FIRE HAS OCCURED");

    for (pos1 = 90; pos1 >= 0; pos1 = pos1 - 5)
    {
      myservo.write(pos1);
      delay(100);
    }


  }






}
