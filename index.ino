#include <SPI.h>
#include <MFRC522.h>

// RFID Pin Definitions
#define RST_PIN 3
#define SS_PIN 10
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Ultrasonic Pin Definitions
#define TRIG_PIN 8
#define ECHO_PIN 9

// Motor Driver Pin Definitions
#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7

// State Variable
bool robotEnabled = false;
const int obstacleDistanceThreshold = 20; // Distance in cm to trigger turning

// REPLACE these with the actual UIDs of your cards
// You can find your card UID by running the "DumpInfo" example in the MFRC522 library
byte whiteCardUID[] = {0x11, 0x22, 0x33, 0x44}; // Authorized start card
byte blueTagUID[] = {0x55, 0x66, 0x77, 0x88};   // Stop/Disable tag

void setup() {
  Serial.begin(9600);
  
  // Initialize SPI bus and RFID reader
  SPI.begin();
  mfrc522.PCD_Init();
  
  // Initialize Ultrasonic Pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Initialize Motor Pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  stopMotors(); 
  Serial.println("System Ready. Tap the White Card to start the robot.");
}

void loop() {
  // 1. Check for RFID tags
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    if (checkUID(mfrc522.uid.uidByte, whiteCardUID)) {
      robotEnabled = true;
      Serial.println("White Card Authorized. Robot is now ON.");
    } 
    else if (checkUID(mfrc522.uid.uidByte, blueTagUID)) {
      robotEnabled = false;
      stopMotors();
      Serial.println("Blue Tag Scanned. Robot is now OFF.");
    } 
    else {
      Serial.println("Unauthorized Card. Access Denied.");
    }
    mfrc522.PICC_HaltA(); // Stop reading to prevent multiple triggers
  }

  // 2. Obstacle Avoidance Logic (Only runs if enabled)
  if (robotEnabled) {
    long distance = getDistance();
    
    // If an object is detected within 20cm (and > 0 to filter false readings)
    if (distance > 0 && distance <= obstacleDistanceThreshold) {
      Serial.println("Obstacle detected! Evading...");
      
      // Stop, reverse a bit, and turn right to avoid
      stopMotors();
      delay(300);
      
      reverseMotors();
      delay(400); 
      
      turnRight();
      delay(500); // Adjust this delay to control how sharp the turn is
    } else {
      // Path is clear, keep moving
      moveForward();
    }
  }
}

// Function to calculate distance from ultrasonic sensor
long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Measure the time for the echo to bounce back
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout
  
  // Calculate distance in centimeters
  long distance = duration * 0.034 / 2;
  return distance;
}

// Function to compare scanned RFID tag with our saved tags
bool checkUID(byte *scannedUID, byte *targetUID) {
  for (byte i = 0; i < 4; i++) {
    if (scannedUID[i] != targetUID[i]) {
      return false;
    }
  }
  return true;
}

// --- Motor Control Functions ---
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void reverseMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnRight() {
  // Left wheels forward, right wheels reverse
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
