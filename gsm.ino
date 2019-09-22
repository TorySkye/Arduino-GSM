#include <SoftwareSerial.h>
#include <string.h>

// PWM outputs
#define PWM1 5
#define PWM2 6
#define PWM3 9
#define PWM4 10

// LED
#define LED1 4
#define LED2 7
#define LED3 8
#define LED4 11

// serial port for communication with GSM module GA6
SoftwareSerial mySerial(3, 2); // GA6 (Tx,Rx) -> Arduino (3,2)

String number = "0977931800"; 

String textMessage;
String part1;
String part2;

int percent;
int pwm_val; // pwm value 

void setup()
{
  // begin serial communication with Arduino and serial monitor
  Serial.begin(9600);

  // begin serial communication with Arduino and GA6 module
  mySerial.begin(9600);

  Serial.println("Initialization...");
  delay(1000);

  mySerial.println("AT"); // once the handshake test is successful, it will back to OK
  updateSerial(); 
  mySerial.println("AT+CSQ"); // signal quality test, value range is 0-31, 31 is the best
  updateSerial();
  mySerial.println("AT+CCID"); // SIM card check, it will back OK if SIM is plugged
  updateSerial();
  mySerial.println("AT+CREG?"); // check whether it has registered in the network
  updateSerial();

  // set LED pins as outputs
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(PWM3, OUTPUT);
  pinMode(PWM4, OUTPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);

  // set LED pins low
  digitalWrite(PWM1, LOW);
  digitalWrite(PWM2, LOW);
  digitalWrite(PWM3, LOW);
  digitalWrite(PWM4, LOW);
 
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);

  digitalWrite(LED_BUILTIN, LOW);

  mySerial.println("AT+CMGD=1,4");  // delete all SMS messages stored in the message storage area
  delay(100);
  updateSerial();

  read_sms(); 
}

void loop()
{
    if(mySerial.available() >0) { 
	
      textMessage = mySerial.readString(); // store serial data to string
      Serial.print(textMessage);
      delay(100);
      digitalWrite(LED_BUILTIN, HIGH);
      mySerial.println("AT+CMGD=1");  
      delay(100);
      updateSerial();
    }
   else
    {
      digitalWrite(LED_BUILTIN, LOW);
    }
 
   if (textMessage.indexOf("PWM1")>= 0){  
	   
    part1 = textMessage.substring(0, textMessage.indexOf("-"));
    part2 = textMessage.substring(textMessage.indexOf("-")+1);

    Serial.print(part1);
    Serial.print('\n');
    Serial.print(part2);
    
	percent = part2.toInt();
    Serial.print(percent);
    
	pwm_val = (percent*255)/100;    
    Serial.print('\n');
    Serial.print(pwm_val);
    analogWrite(PWM1, pwm_val);
   }

   if (textMessage.indexOf("PWM2") >= 0){

    part1 = textMessage.substring(0, textMessage.indexOf("-"));
    part2 = textMessage.substring(textMessage.indexOf("-")+1);

    Serial.print(part1);
    Serial.print('\n');
    Serial.print(part2);
    
	percent = part2.toInt();
    Serial.print(percent);
   
    pwm_val = (percent*255)/100;    
    Serial.print('\n');
    Serial.print(pwm_val);
    analogWrite(PWM2, pwm_val);
   }

   if (textMessage.indexOf("PWM3") >= 0){
	   
    part1 = textMessage.substring(0, textMessage.indexOf("-"));
    part2 = textMessage.substring(textMessage.indexOf("-")+1);

    Serial.print(part1);
    Serial.print('\n');
    Serial.print(part2);
    
	percent = part2.toInt();
    Serial.print(percent);
    
	pwm_val = (percent*255)/100;    
    Serial.print('\n');
    Serial.print(pwm_val);
    analogWrite(PWM3, pwm_val);
   }

   if (textMessage.indexOf("PWM4") >= 0){
    
    part1 = textMessage.substring(0, textMessage.indexOf("-"));
    part2 = textMessage.substring(textMessage.indexOf("-")+1);

    Serial.print(part1);
    Serial.print('\n');
    Serial.print(part2);

	percent = part2.toInt();
    Serial.print(percent);

    pwm_val = (percent*255)/100;    
    Serial.print('\n');
    Serial.print(pwm_val);
    analogWrite(PWM4, pwm_val);
   }

   if (textMessage.indexOf("LED1 ON") >= 0){
      
    digitalWrite(LED1, HIGH);
   }

   if (textMessage.indexOf("LED2 ON") >= 0){
    
    digitalWrite(LED2, HIGH);
    Serial.println("LED2 ON");
   }

   if (textMessage.indexOf("LED3 ON") >= 0){
     
    digitalWrite(LED3, HIGH);
    Serial.println("LED3 ON");
   }

   if (textMessage.indexOf("LED4 ON") >= 0){
    
    digitalWrite(LED4, HIGH);
    Serial.println("LED4 ON");
   }

   if (textMessage.indexOf("LED1 OFF") >= 0){

	digitalWrite(LED1, LOW);
    Serial.println("LED1 OFF");
   }

   if (textMessage.indexOf("LED2 OFF") >= 0){
      
    digitalWrite(LED2, LOW);
    Serial.println("LED2 OFF");
   }

   if (textMessage.indexOf("LED3 OFF") >= 0){
     
    digitalWrite(LED3, LOW);
    Serial.println("LED3 OFF");
   }

   if (textMessage.indexOf("LED4 OFF") >= 0){
     
    digitalWrite(LED4, LOW);
    Serial.println("LED4 OFF");
   }

   textMessage = "";

}

void updateSerial() 
{
  delay(500);
  while (Serial.available())
  {
    mySerial.write(Serial.read()); 
  }
  while(mySerial.available())
  {
    Serial.write(mySerial.read());
  }
}

void read_sms() 
{
  // configuring TEXT mode
  mySerial.print("AT+CMGF=1\r");
  delay(100);
  // decides how newly arrived SMS messages should be handled
  mySerial.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);
}

void send_sms(String message) 
{
  Serial.println ("Sending SMS...");
  mySerial.println("AT+CMGF=1");   // configuring TEXT mode
  delay(100);
  Serial.println ("Set phone number");
  mySerial.println("AT+CMGS=\"" + number + "\"\r");
  delay(100);
  mySerial.println(message);
  delay(100);
  mySerial.println((char)26);
  delay(1000);
}
