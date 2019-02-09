#include <SoftwareSerial.h>

/*     
 *      PINOUT: 
 *        _____________________________
 *       |  ARDUINO UNO >>>   SIM800L  |
 *        -----------------------------
 *            GND      >>>   GND
 *        RX  10       >>>   TX    
 *        TX  11       >>>   RX
 *       RESET 2       >>>   RST 
 *                 
 *  
 */


 //send "AT" from serial monitor (without quotes)
 //if you get "OK" answer.. every thing is fine 

String Arsp, Grsp;
SoftwareSerial gsm(10, 11); // RX, TX

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial.println("Testing GSM SIM800L");
  gsm.begin(4800);

}

void loop() {
  // put your main code here, to run repeatedly:

  if(gsm.available())
  {
    Grsp = gsm.readString();
    Serial.println(Grsp);
  }

  if(Serial.available())
  {
    Arsp = Serial.readString();
    gsm.println(Arsp);
  }

}
