// Include the SoftwareSerial library for serial communication
#include <SoftwareSerial.h>
SoftwareSerial mySerial(11, 12);

// Define pin assignments for ultrasonic sensors
uint8_t TRIG_PIN1 = 2;  
uint8_t ECHO_PIN1 = 3;
uint8_t TRIG_PIN2 = 4;
uint8_t ECHO_PIN2 = 5;

// Define pin assignments for motor outputs
uint8_t OUTPUT_1 = 6;  //Right forward
uint8_t OUTPUT_2 = 7;  //Right backward
uint8_t OUTPUT_3 = 9;  //Left forward
uint8_t OUTPUT_4 = 10;  //Left backward

// Define pin assignment for buzzer
uint8_t BUZZER = 13;

// Declare variables for LDR values, distance measurements, and threshold
float ldr1, ldr2, ldr3, ldr4, x, distance1, distance2;
int threshold = 650;

void setup() {
  // Set pin modes for motor outputs
  pinMode(OUTPUT_1, OUTPUT);
  pinMode(OUTPUT_2, OUTPUT);
  pinMode(OUTPUT_3, OUTPUT);
  pinMode(OUTPUT_4, OUTPUT);

  // Set pin modes for ultrasonic sensors
  pinMode(TRIG_PIN1, OUTPUT);
  pinMode(ECHO_PIN1, INPUT);
  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);

  // Set pin modes for buzzer
  pinMode(BUZZER, OUTPUT);

  // Initialize serial communication for debugging and SoftwareSerial for SIM800L module
  Serial.begin(9600);
  mySerial.begin(9600);
}

// Function to read distance using ultrasonic sensor
float readDistanceCM(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  int duration = pulseIn(echo, HIGH);
  delay(100);
  return duration * 0.034 / 2;
}

// Function to send a message using the SIM800L module
void Sendmessage(){
  mySerial.println("AT+CMGF=1");
  delay(1000);
  mySerial.println("AT+CMGs=\"+94762811062\"\r");
  delay(1000);
  mySerial.println("Selfodryer: I'm in the extreme darkness");
  delay(100);
  mySerial.println((char)26);
  delay(1000);
}

// Function to control robot movements based on sensor readings
void movements() {
  // Check if all LDR values are below or equal to 100 (indicating extreme darkness)
  if (ldr1 <= 100 && ldr2 <= 100 && ldr3 <= 100 && ldr4 <= 100) {
    stop();
    digitalWrite(BUZZER, HIGH);
    Sendmessage();  // Function to send a message
    delay(2000);
    digitalWrite(BUZZER, LOW);
    delay(3000);
  }
  // Check if there a significant difference between adjacent LDR values
  else if (abs(ldr1 - ldr2) > 80 || abs(ldr3 - ldr4) > 80) {
    // Determine the minimum LDR value among all
    x = min(ldr1, min(ldr2, min(ldr3, ldr4)));

    // Check if any LDR value is below or equal to the threshold
    if (ldr1 <= threshold || ldr2 <= threshold || ldr3 <= threshold || ldr4 <= threshold) {
      // Adjust robot movements based on LDR and ultrasonic sensor readings
      if (x == ldr1) {
        // Check if distance from ultrasonic sensor 1 is greater than 40 cm
        if (distance1 > 40) { forward(); }
        else { stop(); }
      }
      else if (x == ldr2) {
        // Check if distance from ultrasonic sensor 2 is greater than 40 cm
        if (distance2 > 40) { backward(); }
        else { stop(); }
      }
      else if (x == ldr3) {
        // Turn right
        right();
      }
      else if (x == ldr4) {
        // Turn left
        left();
      }
    } 
    else {
      // Stop the robot if all LDR values are above the threshold (indicating sufficient light conditions)
      stop();
    }
  }
  else {
    // Stop the robot if the lighting conditions are sufficient 
    stop();
  }
}


// Functions to control robot movements
void forward() {
  digitalWrite(OUTPUT_1, HIGH);
  digitalWrite(OUTPUT_2, HIGH);
  digitalWrite(OUTPUT_3, LOW);
  digitalWrite(OUTPUT_4, LOW);
}

void backward() {
  digitalWrite(OUTPUT_1, LOW);
  digitalWrite(OUTPUT_2, LOW);
  digitalWrite(OUTPUT_3, HIGH);
  digitalWrite(OUTPUT_4, HIGH);
}

void right() {
  digitalWrite(OUTPUT_1, HIGH);
  digitalWrite(OUTPUT_2, LOW);
  digitalWrite(OUTPUT_3, LOW);
  digitalWrite(OUTPUT_4, HIGH);
}

void left() {
  digitalWrite(OUTPUT_1, LOW);
  digitalWrite(OUTPUT_2, HIGH);
  digitalWrite(OUTPUT_3, HIGH);
  digitalWrite(OUTPUT_4, LOW);
}

void stop() {
  digitalWrite(OUTPUT_1, LOW);
  digitalWrite(OUTPUT_2, LOW);
  digitalWrite(OUTPUT_3, LOW);
  digitalWrite(OUTPUT_4, LOW);
}

void loop() {
  // Read LDR values and ultrasonic sensor distances
  ldr1 = analogRead(2);
  ldr2 = analogRead(3);
  ldr3 = analogRead(4);
  ldr4 = analogRead(5);
  distance1 = readDistanceCM(TRIG_PIN1, ECHO_PIN1);
  distance2 = readDistanceCM(TRIG_PIN2, ECHO_PIN2);

  // Print sensor values for debugging
  Serial.println(distance1);
  Serial.println(distance2); 

  // Delay before calling the movements function
  delay(100);
  movements();
}
