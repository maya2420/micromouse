unsigned long int lasttick = 0;

double time = 0.0;
double dis = 0.0, pos = 0.0;



void setup() {
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  digitalWrite(4,HIGH);

  digitalWrite(9, HIGH);
  digitalWrite(10,LOW);



}

void loop() {
  if(millis() - lasttick >= 1 ){
    lasttick = millis();
    time = time + .001;
  }
  if(pos < .125){
    dis = .5*.25*time*time;
  }
  if((pos >= .125) && (pos < .75)){
    dis = .125 + .25*(time-1);
  }
  
  
  if((dis - pos > .000236)){
    digitalWrite(5,HIGH);
    digitalWrite(5, LOW);
    pos = pos + .000236;
  }
  /*if((dis - pos > .000236) && (pos >= .75) && (pos < 1.00)){
    digitalWrite(5,HIGH);
    digitalWrite(5, LOW);
    pos = pos + .000236;
  }*/

}
