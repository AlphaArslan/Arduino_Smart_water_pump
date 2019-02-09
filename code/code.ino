/******************************************************************
 * Coded by m.Said (Alpha Arslan)
 * https://github.com/AlphaArslan
 * Date: 9/2/2019 
******************************************************************/

/**************************** Include ****************************/
#include <Sim800l.h>
#include <SoftwareSerial.h> 

/**************************** Define *****************************/
//constants
#define       DEBUG             0           //make it 1 to see debug messages in serial monitor 
#define       DELAY_TIME        5000        //5 seconds
#define       PUMP_DELAY        5000        //5 seconds
#define       FIX_PUMP_DELAY    600000      //10 minutes
#define       LDR_THRESHOLD     0           //obtain from Tuning_Code
#define       MOIS_THRESHOLD    0           //obtain from Tuning_Code
#define       WTRFLOW_THRESHOLD 0           //obtain from Tuning_Code

//pin coonection
#define       LDR_ANALOG        A0          //LDR sensor module analog output
#define       MOIS_ANALOG       A1          //soil moisture sensor analog output
#define       PUMP_CONTROL      3           //we consider it LED or any representation
#define       IS_FIXED          4           //this is a push button.. you press it after fixing problems 
#define       WATER_FLOW        2           //this is the output from water flow sensor module

/************************* Global Var. ***************************/
Sim800l       Sim800l;                      //an object for the GSM module 
char          text[]="Water is not flowing.. we have a problem";                 
char          number[]="+201207903371";
volatile int  NbTopsFan;                    //measuring the rising edges of the signal

/************************** Functions ****************************/
bool          is_night();                   //returns TRUE if it's night.. it uses LDR sensor..
bool          need_water();                 //returns TRUE if the land is dry and needs water
void          start_pump();                 //set pump output to HIGH
void          stop_pump();                  //set pump output to LOW
bool          is_water_flowing();           //uses water folw sensor 
void          send_warning();               //sends SMS warning 
void          rpm();                        //needed for water flow sensor interrupt 
/**************************** Setup() ****************************/
void setup(){
    if(DEBUG){Serial.begin(9600); Serial.println("Debug Enabled");}
    
    Sim800l.begin();

    attachInterrupt(0, rpm, RISING);        //excutes rpm() whenever a RISING occurs on pin 2
}
/**************************** loop() *****************************/
void loop(){
    //wait until it's not night
    //pump doesn't work during night
    while(is_night()){
        delay(DELAY_TIME);
    }
    
    //yaaaay.. it's light.. the day has come ^_^
    //let's see if the land is thirsty 
    if(need_water()){
        //it needs water.. lets turn on the pump
        start_pump();
        delay(PUMP_DELAY);

        //check if water made it through
        if(is_water_flowing()){
            //keep it ON as long as it needs water and it still is day
            while(need_water() && !is_night() && is_water_flowing()){
                delay(DELAY_TIME);
            }
            stop_pump();
        }else{
            stop_pump();
            //water is not flowing .. trouble!!
            send_warning();
            while(digitalRead(IS_FIXED) == LOW){}       //wait until problem is fixed
        }
        
            
    }
}



/*********************** Func. Definition *************************/
bool is_night(){
    //get reading from LDR sensor
    //if greater than LDR_THRESHOLD
    //it's day .. return FALSE
    //if not, it's night .. return TRUE
    if(analogRead(LDR_ANALOG)>LDR_THRESHOLD)  return false;
    return true;
}

bool need_water(){
    //get reading from moisture sensor
    //if it's lower than MOIS_THRESHOLD
    //we consider it dry and return TRUE
    //return FALSE otherwise
    if(analogRead(MOIS_ANALOG)<MOIS_THRESHOLD) return true;
    return false;
}

void start_pump(){
    digitalWrite(PUMP_CONTROL, HIGH);
}

void stop_pump(){
    digitalWrite(PUMP_CONTROL, LOW);
}

bool is_water_flowing(){
    NbTopsFan = 0;                          //Set NbTops to 0 ready for calculations
    sei();                                  //Enables interrupts
    delay (1000);                           //Wait 1 second
    cli();                                  //Disable interrupts
    Calc = (NbTopsFan * 60 / 7.5);          //(Pulse frequency x 60) / 7.5Q, = flow rate in L/hour 
    if(calc > WTRFLOW_THRESHOLD) return true;
    return false;
}

void send_warning(){
    if(Sim800l.sendSms(number,text)){
        Serial.println("Error Sending message");
    }esle{
        Serial.println("Message sent..");
    }
    
}

void rpm(){ 
    NbTopsFan++;  //This function measures the rising and falling edge of the hall effect sensors signal
} 
