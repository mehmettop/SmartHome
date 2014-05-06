#include <Keypad_I2C.h> // Keypad_I2C kütüphanesini bağlıyoruz.
#include <Keypad.h> // Keypad kütüphanesini bağlıyoruz.
#include <Wire.h> // Wire kütüphanesini bağlıyoruz.
#include <PinChangeInt.h>


// Tuş takımı için kullandığımız PCF8574 için adres.
// PCF8574ün 1,2 ve 3. pinleri için toprak bağlantısı yapılırsa
// bağlantı adresi 0x20 oluyor.
#define I2CADDR 0x38

const byte ROWS = 4; // Tuş takımındaki satır sayısı
const byte COLS = 4; // Tuş takımındaki sütun sayısı
const int B=A1;
const int G=A2;
const int R=A3;
const int Buzzer=8;
int passwordMode = 0;
String password ="1234";
String inputPassword ="";
unsigned long  startTime=0;
unsigned long  stopTime=0;



// Tuş takımı üzerindeki butonları matris şeklinde yazıyoruz.
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {3, 2, 1, 0}; // Satıların konnektördeki pin numaraları
byte colPins[COLS] = {7, 6, 5, 4}; // Sütunların konnektördeki pin numaraları

// Yeni bir tuş takımı sınıfı oluşturuyoruz.
Keypad_I2C customKeypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS, I2CADDR); 
void setup(){
  customKeypad.begin( );
  attachInterrupt(0,enterPassword,RISING);
  pinMode(Buzzer,OUTPUT);
  pinMode(R,OUTPUT);
  pinMode(G,OUTPUT);
  pinMode(B,OUTPUT);
  // Seri iletişimi başlatıyoruz.
  Serial.begin(9600);
}


void loop(){
  // Tuş takımından gelen değeri customkey değişkenine atıyoruz.
while(passwordMode==1){
  //Serial.print("Current Time => ");
  //Serial.print(millis());
  if(millis()-startTime >30000){
     turnOnRedLED();
     alarm();
  }
  else{
    turnOnGreenLED();
  }
  
  getKey();
}  
delay(100);
turnOnBlueLED();
delay(100);
turnOffLED();


} 



char getKey(){
  
  
  char customKey = customKeypad.getKey(); 
 
  if(customKey=='A'){
    if(inputPassword == password){
     Serial.println("Password O.K.");
     tone(Buzzer,80,200);
     passwordMode=0;
     
    }
    else{
     Serial.println("Password Fail");
     tone(Buzzer,100,400);
     inputPassword="";
    }
  }
   else if(customKey=='B'){
     //SIFIRLA
      Serial.println("Password Reset");
      inputPassword="";
    }else if (customKey != NO_KEY){
       //Serial.print(customKey);
       tone(Buzzer,460,80);
       inputPassword += customKey;
    }
    
    return customKey;
}
 
 
 


void turnOnRedLED(){
  analogWrite(R,0);//M
  analogWrite(G,255);
  analogWrite(B,255);
}
void turnOnGreenLED(){
  analogWrite(R,255);
  analogWrite(G,0);
  analogWrite(B,255);
}
void turnOnBlueLED(){
  analogWrite(R,255);
  analogWrite(G,255);
  analogWrite(B,0);
}
void turnOffLED(){
  analogWrite(R,255);
  analogWrite(G,255);
  analogWrite(B,255);
}
void enterPassword(){
if(passwordMode !=1){
  passwordMode=1;
turnOnGreenLED();
startTime=millis();
//Serial.print("DOOR JUST OPENED TIME => ");
//Serial.println(startTime);

 }
}

void alarm(){
tone(Buzzer,1000,1000);
}


  
