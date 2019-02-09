/**************************** Define *****************************/
#define       pushButton            5
#define       LDR_ANALOG            A0
#define       MOIS_ANALOG           A1
#define       WATER_FLOW            2


/************************* Global Var. ***************************/
int           LDR_THRESHOLD;
int           MOIS_THRESHOLD;
int           WTRFLOW_THRESHOLD;
volatile int  NbTopsFan;
/************************** Functions ****************************/
void          rpm();

void setup() {
    Serial.begin(9600);
    attachInterrupt(0, rpm, RISING);

    Serial.println("[Cal]: put LDR Sensor in dark room .. Simulate night");
    Serial.println("[Cal]: press the push button when ready");
    while(digitalRead(pushButton) == LOW) {}
    LDR_THRESHOLD = analogRead(LDR_ANALOG);
    for(int i=0; i<5; i++){
        delay(200);
        LDR_THRESHOLD += analogRead(LDR_ANALOG);
        LDR_THRESHOLD /= 2;
    }



    Serial.println("[Cal]: put moisture sensor in a wet soil..simulate fully watered soil");
    Serial.println("[Cal]: press the push button when ready");
    while(digitalRead(pushButton) == LOW) {}
    MOIS_THRESHOLD = analogRead(MOIS_ANALOG);
    for(int i=0; i<5; i++){
        delay(200);
        MOIS_THRESHOLD += analogRead(MOIS_ANALOG);
        MOIS_THRESHOLD /= 2;
    }


    Serial.println("Showing Results:");
    Serial.print("\tLDR_THRESHOLD = "); Serial.println(LDR_THRESHOLD);
    Serial.print("\tMOIS_THRESHOLD ="); Serial.println(MOIS_THRESHOLD);
}

void loop() {
  // put your main code here, to run repeatedly:

}

/*********************** Func. Definition *************************/
void rpm(){ 
    NbTopsFan++; 
}
