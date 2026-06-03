// SMART TIMER AUTO POWER ON - ARDUINO VERSION
// FINAL VERSION - EXACT SCHEMATIC CONNECTION
// PINS: D4=TRIGGER, D5=YELLOW, D7=SENSOR, D8=BUZZER

const int triggerPin = 4; // Transistor Base (Controls Main Red PC LED)
const int yellowLED = 5;  // Yellow LED (Timer Active Indicator)
const int statusPin = 7;  // Sensor Signal pin
const int buzzerPin = 8;  // Buzzer Positive Pin

// Timer Configuration
const int countdownSeconds = 5; // Length of the delay per cycle

void setup() {
  // Initialize all pins
  pinMode(triggerPin, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(statusPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  
  // Ensure everything starts turned off safely
  digitalWrite(triggerPin, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(buzzerPin, LOW);
  
  Serial.begin(9600);
  Serial.println("--- Continuous Countdown & Trigger Cycle Active ---");
}

void loop() {
  // ==========================================
  // STEP 1: START COUNTDOWN & YELLOW LED ON
  // ==========================================
  Serial.println("\n--- Starting New Cycle: Safety Delay Mode ---");
  digitalWrite(yellowLED, HIGH); // Turn on yellow indicator
  
  for (int i = countdownSeconds; i > 0; i--) {
    Serial.print("Next trigger sequence starting in: ");
    Serial.print(i);
    Serial.println(" seconds...");
    delay(1000); // Wait exactly 1 second per tick
  }
  
  // ==========================================
  // STEP 2: TIMER EXPIRES -> YELLOW OFF
  // ==========================================
  Serial.println("Timer finished! Turning OFF Yellow LED.");
  digitalWrite(yellowLED, LOW); // Turn off yellow indicator
  delay(200); // Very brief pause before the action
  
  // ==========================================
  // STEP 3: RED LED ON & FAST BUZZER BEEP (1 Second)
  // ==========================================
  Serial.println("Action: Triggering Red LED and Fast Buzzer Beep!");
  digitalWrite(triggerPin, HIGH); // Activates transistor -> Red LED turns ON
  digitalWrite(buzzerPin, HIGH);   // Buzzer turns ON
  
  // Read the status sensor while the Red LED is fully powered
  delay(500); // Wait half a second for voltage to stabilize
  int pcStatus = digitalRead(statusPin);
  Serial.print("[LIVE CHECK] Sensor Readout during trigger: ");
  if (pcStatus == HIGH) {
    Serial.println("[ HIGH ] -> Main Power Detected!");
  } else {
    Serial.println("[ LOW ] -> Main Power is OFF.");
  }
  delay(500); // Wait the remaining half-second of the 1-second trigger

  // ==========================================
  // STEP 4: SHUT DOWN TRIGGER & RESET
  // ==========================================
  Serial.println("Action Finished: Turning OFF Red LED and Buzzer.");
  digitalWrite(triggerPin, LOW);  // Red LED turns OFF
  digitalWrite(buzzerPin, LOW);   // Buzzer turns OFF
  
  delay(1000); // 1-second pause before the whole loop starts over from Step 1
}