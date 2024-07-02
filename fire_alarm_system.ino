   #include <SoftwareSerial.h>
   #include<LiquidCrystal.h>

   SoftwareSerial mysim(8,9);                           // cdule ononnect gsm on this pin
   String smstext1;
     String smstext2;
   int buz = 10;                                        //buzzer connected to pin 10
   int led = 11;                                        //led connected to pin 11
   int mq135 = A1;                             //output of mq135 connected to A1 pin of Arduino
   int lm35 =A0;
   int threshold = 250;                                 //Threshold level for Air Quality
   int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;  //lcd pins connection to arduino pins
   LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
   //int lm35 =A2;

void setup() {
      Serial.begin(9600);
      mysim.begin(9600);
      lcd.begin(16,2);
      lcd.clear();
      Serial.println("System is Initializing..");
      updateSerial();
      pinMode (buz,OUTPUT);                       // buzzer is connected as Output from Arduino
      pinMode (led,OUTPUT);                      // led is connected as output from Arduino
      pinMode (mq135,INPUT);                    // MQ135 is connected as INPUT to arduino
      pinMode (lm35,INPUT);                    // lm35 is connected as INPUT to arduino
      analogWrite(buz, LOW);
      digitalWrite(led, LOW); 
      delay(500); // wait for 5 seconds
}

void loop() {
      
      int temprature=analogRead(lm35);   //read lm35 analog outputs at A0 and store it on temprature
      int milivolts=(temprature*5/1024)*1000;
      int celsius=milivolts/10;
     
      lcd.setCursor(0,0);
      lcd.print("temprature is:");
      lcd.setCursor(0,1);
      lcd.print(celsius);
      lcd.print("*c");
      delay(500);
      lcd.clear();
      
      
      int ppm = analogRead(mq135);           //read MQ135 analog outputs at A1 and store it in ppm
      Serial.println(ppm);                     //print value of ppm in serial monitor  
      lcd.setCursor(0,0);         
      lcd.print("Air Quality is: ");
      lcd.setCursor(0,1);
      lcd.print(ppm);
      delay(200);
      lcd.clear();
      
      if (ppm > threshold || celsius > 40)  // check is ppm is greater than threshold or not 
                                            //    and temprature level is greater than 40 *c
      {
      smstext1 =  "\nFire Detected..!";
      smstext2 = "\nLocation: house no 0000 in kitchen";
      lcd.print(smstext1);
       lcd.print(smstext2);
      sendSMS(smstext1); 
      sendSMS(smstext2); 
      Serial.println(smstext1);  
        Serial.println(smstext2);
      digitalWrite(buz,HIGH);     //Turn ON Buzzer
      digitalWrite(led,HIGH);     //Turn ON LED
      delay(500);
      
      mysim.println("ATD+251927485352;"); 
      updateSerial();
      
      Serial.println("Calling  ");
      delay(500);                // wait for 5 seconds...
      mysim.println("ATH");        //hang up
      updateSerial();
      Serial.println("Hangup Call");

    }
     
     
     else
    {
      digitalWrite(led,LOW);   //jump here if ppm is not greater than threshold and turn off LED
      digitalWrite(buz,LOW);   //Turn off Buzzer
      lcd.setCursor(0,0);
      lcd.print("temprature is:");
      lcd.setCursor(0,1);
      lcd.print(celsius);
      lcd.print("*c");
      delay(500);
      lcd.clear();
       lcd.setCursor(0,0);         
      lcd.print("Air Quality is: ");
      lcd.setCursor(0,1);
      lcd.print(ppm);
      delay(200);
      Serial.println("No Fire detected");
    } 
     
  delay (50);
}


void sendSMS(String message)
{
  mysim.print("AT+CMGF=1\r");                     // AT command to send SMS message
  updateSerial();
  delay(100);
  
  mysim.println("AT + CMGS = \"+251942587445\"");  // recipient's mobile number, in international format
  updateSerial();
  delay(100);
  
  mysim.println(message);                         // message to send
  
  updateSerial();
  delay(100);
  
  mysim.println((char)26);                        // End AT command with a ^Z, ASCII code 26
  updateSerial();
  delay(100); 
  
  mysim.println();
  delay(100);                                     // give module time to send SMS
 
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mysim.write(Serial.read());   //Forward what Serial received to Software Serial Port
  }
  while(mysim.available()) 
  {
    Serial.write(mysim.read());  //Forward what Software Serial received to Serial Port
  }
}
