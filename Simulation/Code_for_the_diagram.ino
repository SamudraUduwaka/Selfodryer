#define TRIG_PIN1 6
#define ECHO_PIN1 7
#define TRIG_PIN2 8
#define ECHO_PIN2 12
#define OUTPUT_1  2
#define OUTPUT_2  3
#define OUTPUT_3  4
#define OUTPUT_4  5
#define BUZZER 10

float ldr1, ldr2, ldr3, ldr4, x;

void setup() {
  pinMode(OUTPUT_1, OUTPUT);
  pinMode(OUTPUT_2, OUTPUT);
  pinMode(OUTPUT_3, OUTPUT);
  pinMode(OUTPUT_4, OUTPUT);
  pinMode(TRIG_PIN1, OUTPUT);
  pinMode(ECHO_PIN1, INPUT);
  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);
  Serial.begin(9600);
}

float readDistanceCM(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  int duration = pulseIn(echo, HIGH);
  return duration * 0.034 / 2;
}

void movements(){
  float distance1 = readDistanceCM(TRIG_PIN1, ECHO_PIN1);
  float distance2 = readDistanceCM(TRIG_PIN2, ECHO_PIN2);
  
  if (ldr1<=512 or ldr2<=512 or ldr3<=512 or ldr4<=512){
    x = min(ldr1, min(ldr2, min(ldr3, ldr4)));
    if (x==ldr1){    //Forawrd
      if (distance1>15){
        forward(); 
      }
      else{
        stop();
      }
  } 
    else if (x==ldr2){   //Backward
      if (distance2>15){
        backward(); 
      }
      else{
        stop();
      }
  }
    else if (x==ldr3){   //Right
      right();
  } 
    else if (x==ldr4){   //Left
      left();
  }
  }
  else if (ldr1>512 and ldr2>512 and ldr3>512 and ldr4>512){
      stop();
  }
  delay(100);
}

void forward(){
  digitalWrite(OUTPUT_1, HIGH);
  digitalWrite(OUTPUT_2, HIGH);
  digitalWrite(OUTPUT_3, LOW);
  digitalWrite(OUTPUT_4, LOW);
}

void backward(){
  digitalWrite(OUTPUT_1, LOW);
  digitalWrite(OUTPUT_2, LOW);
  digitalWrite(OUTPUT_3, HIGH);
  digitalWrite(OUTPUT_4, HIGH);
}

void right(){
  digitalWrite(OUTPUT_1, HIGH);
  digitalWrite(OUTPUT_2, LOW);
  digitalWrite(OUTPUT_3, LOW);
  digitalWrite(OUTPUT_4, HIGH);
}
  
void left(){
  digitalWrite(OUTPUT_1, LOW);
  digitalWrite(OUTPUT_2, HIGH);
  digitalWrite(OUTPUT_3, HIGH);
  digitalWrite(OUTPUT_4, LOW);
}
  
void stop(){
  digitalWrite(OUTPUT_1, LOW);
  digitalWrite(OUTPUT_2, LOW);
  digitalWrite(OUTPUT_3, LOW);
  digitalWrite(OUTPUT_4, LOW);
}
  
  
void loop()
{
  ldr1 = analogRead(1);
  ldr2 = analogRead(2);
  ldr3 = analogRead(3);
  ldr4 = analogRead(4);

  movements();
}
  