#include<LiquidCrystal.h>
LiquidCrystal lcd(A3, A4, A5, A0, A1, 13);  // sets the interfacing pins

// PINS
int P1 = A6;
int P2 = A7;
int P3 = A2;
int trigPin_1 = 7;
int echoPin_1 = 8;
int trigPin_2 = 9;
int echoPin_2 = 10;
int trigPin_3 = 11;
int echoPin_3 = 12;
int Fan = 4;
int Motor1 = 5;
int Motor2 = 6;
int buzzerPin = 3; //CT

// HYPER-PARAMETERS
int TrigDist = 15; // in CM
float Motor1_min_time = 0; // soap min time
float Motor1_max_time = 10; // soap max time
float Motor2_min_time = 0; // water min time
float Motor2_max_time = 30; // water max time
float Motor3_min_time = 0; // blower min time
float Motor3_max_time = 30; // blower max time

// SYSTEM-VARIABLES
int Motor1_curr_time;
int Motor2_curr_time;
int Motor3_curr_time;
float buzzerTime = 0.1; // in seconds
String L1 = "   DISPENSOR    ";
String L2 = "";
long duration;
int distance;


void BUZZ(int k)
{
  for(int i=0;i<k;i++)
  {
    Serial.println("BUZZ_HIGH");
    digitalWrite(buzzerPin,HIGH);
    delay(buzzerTime * 1000.0);
    Serial.println("BUZZ_LOW");
    digitalWrite(buzzerPin,LOW);
    delay(buzzerTime * 1000.0);    
    }
 }

void Refresh_display()
{
  lcd.setCursor(0,0);           //sets the cursor at row 0 column 0
  lcd.print(L1); // prints 16x2 LCD MODULE
  lcd.setCursor(0,1);           //sets the cursor at row 1 column 2
  lcd.print(L2);     // prints HELLO WORLD
}

int Dist(int trigPin, int echoPin)
{
// Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*0.034/2;
//Serial.println("Returning dist "+String(distance));
delay(100);
return distance;
}
  
void setup()
{
  Serial.begin(9600);
 lcd.begin(16, 2);  // initializes the 16x2 LCD
 Refresh_display();
 pinMode(trigPin_1,OUTPUT);
  pinMode(trigPin_2,OUTPUT);
 pinMode(trigPin_3,OUTPUT);
 pinMode(Fan,OUTPUT);
pinMode(echoPin_1, INPUT);
pinMode(echoPin_2, INPUT);
pinMode(echoPin_3, INPUT);
 pinMode(buzzerPin,OUTPUT);

INTRUPT();
}

void INTRUPT()
{
  Motor1_curr_time = Motor1_min_time + (Motor1_max_time - Motor1_min_time)*float((1024.0 - analogRead(P1))/1024.0);
  Motor2_curr_time = Motor2_min_time + (Motor2_max_time - Motor2_min_time)*float((1024.0 - analogRead(P2))/1024.0);
  Motor3_curr_time = Motor3_min_time + (Motor3_max_time - Motor3_min_time)*float((1024.0 - analogRead(P3))/1024.0);
  }

void loop()
{
INTRUPT();
if (Dist(trigPin_1,echoPin_1) <= TrigDist)
{
  Serial.println("SOAP DISPENSING ");
  L2 = "SOAP DISPENSING ";
  Refresh_display();
  digitalWrite(Motor1,HIGH);
    BUZZ(1);
  delay(Motor1_curr_time*1000);
    digitalWrite(Motor1,LOW);
      BUZZ(1);
          L2 = "                ";
      Refresh_display();
  }

else if (Dist(trigPin_2,echoPin_2) <= TrigDist)
{
  Serial.println("WATER DISPENSING");
    L2 = "WATER DISPENSING";
      Refresh_display();
  digitalWrite(Motor2,HIGH);
    BUZZ(1);
  delay(Motor2_curr_time*1000);
      digitalWrite(Motor2,LOW);
        BUZZ(1);
          L2 = "                ";
      Refresh_display();
  }  

else if (Dist(trigPin_3,echoPin_3) <= TrigDist)
{
  Serial.println("AIR BLOWER ON  ");
    L2 = " AIR BLOWER ON  ";
      Refresh_display();
  digitalWrite(Fan,HIGH);
  BUZZ(1);
  delay(Motor3_curr_time*1000);
      digitalWrite(Fan,LOW);
        BUZZ(1);
          L2 = "                ";
      Refresh_display();
  } 
}
