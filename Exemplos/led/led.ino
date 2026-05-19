int LDR_Val = 0;    
int sensor =34;      
int led= 25;         
void setup() {
    Serial.begin(9600);     
    pinMode(led, OUTPUT);  
}
void loop() {
    LDR_Val = analogRead(sensor); 
    Serial.print("LDR Output Value: ");
    Serial.println(LDR_Val); 
    if(LDR_Val > 2000) {    
        Serial.println(" High intensity ");
        digitalWrite(led,LOW);
    }
    else {
        Serial.println("LOW Intensity ");
        digitalWrite(led,HIGH); 
    delay(1000); 
}