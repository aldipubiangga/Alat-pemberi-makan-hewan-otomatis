#include <Servo.h>
#include <DS3231.h> 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#define trig 6
#define echo 7
#define pinSensor A1

LiquidCrystal_I2C lcd(0x27,20,4);
DS3231  rtc(SDA, SCL); 
Time  t;
Servo myservo;
Servo myservo1;

int jam;
int menit;
int detik;
int nilai= 0;
int buzzer = 5;
int minum_servo = 9;
int makan_servo = 10;

int bacaSensor(){
  nilai = analogRead(pinSensor);
  return nilai;
}

void setup() {
  //Serial
  Serial.begin(115200);

  //Buzzer
  pinMode(buzzer, OUTPUT);

  //LCD
  lcd.init();
  lcd.init();
  lcd.backlight();

  //Ultrasonic
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  //Servo
  myservo.attach(minum_servo);
  myservo1.attach(makan_servo);

  //RTC
  rtc.begin();
  rtc.setDate(28, 11, 2021);   //menset tanggal 
  rtc.setTime(5, 59, 00);     //menset jam 
  rtc.setDOW(2);     //menset hari 
}

void loop() {
  //Kondisi RTC
  t = rtc.getTime();
  jam = t.hour;
  menit = t.min;
  detik = t.sec;
  
  Serial.print(rtc.getDOWStr()); //prosedur pembacaan hari
  Serial.print(" ");
  
  Serial.print(rtc.getDateStr()); //prosedur pembacaan tanggal
  Serial.print(" -- ");
 
  Serial.println(rtc.getTimeStr()); //prosedur pembacaan waktu

  if (jam == 06 & menit == 00 & detik >= 00){
    myservo.write(0);
    delay(1000);
    myservo.write(180);
    delay(1000);
    digitalWrite(buzzer, HIGH);
    delay(500);
  }
  
  //Kondisi Sensor ultrasonic
  digitalWrite(trig,LOW);
  delay(2);
  digitalWrite(trig,HIGH);
  delay(10);
  digitalWrite(trig,LOW);
  
  long durasi;
  float jarak;

  durasi = pulseIn(echo,HIGH);
  jarak = durasi * 0.034/2;
  Serial.print("jarak (cm): ");
  Serial.println(jarak);
  lcd.setCursor(0,0);
  lcd.print("Mknan: "); 
  if(jarak<7){
      lcd.setCursor(8,0);
      lcd.print("Habis");
      tone(buzzer,300);
      delay(500);
    }
   else if(jarak>=7){
      lcd.setCursor(8,0);
      lcd.print("Aman");
      digitalWrite(buzzer,LOW);
      delay(500);
    }

  //Kondisi Water Level Sensor
  int tinggi = bacaSensor();
  lcd.setCursor(0,1);
  lcd.print("Minum: ");
  if (tinggi<=100){
    lcd.setCursor(8,1);
    lcd.print("Habis");
    Serial.println("Jumlah air : Habis");
    myservo1.write(0);
    delay(1000);
    myservo1.write(180);
    delay(1000);
    tone(buzzer,400);
    delay(250);
  }
  else if (tinggi>100 & tinggi<=500){
    lcd.setCursor(8,1);
    lcd.print("Sedikit");
    Serial.println("Jumlah Air : Sedikit");
  }
  else if (tinggi>500){
    lcd.setCursor(8,1);
    lcd.print("Banyak");
    Serial.println("Jumlah Air : Banyak");
  }
  delay (1000);
}
