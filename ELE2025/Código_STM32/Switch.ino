//Switches Code

void setSwitches(){
  pinMode(SW0, OUTPUT);
  pinMode(SW1, OUTPUT);
  pinMode(SW2, OUTPUT);
  pinMode(SW3, OUTPUT);
  pinMode(SW4, OUTPUT);
  pinMode(SW5, OUTPUT);
  for(int i=0; i<6; i++){
    turnSwitch(i, LOW);
  }
}

void turnSwitch(char switchNo, bool state){
  switch(switchNo){ 
    case 0:
      digitalWrite(SW0, state);
    case 1:
      digitalWrite(SW1, state);   
    case 2:
      digitalWrite(SW2, state);   
    case 3:
      digitalWrite(SW3, state);  
    case 4:
      digitalWrite(SW4, state);   
    case 5:
      digitalWrite(SW5, state);                           
  }
}