// SMART TIMER AUTO POWER ON - ESP32 VERSION
// FINAL VERSION - EXACT SCHEMATIC CONNECTION
// PINS: GPIO4=TRIGGER, GPIO5=YELLOW, GPIO21=SENSOR(PULLDOWN), GPIO19=BUZZER

const int triggerPin = 4;   // GPIO 4 -> Transistor Base (Triggers Motherboard PWR_SW & Red LED)
const int yellowLED = 5;    // GPIO 5 -> Standby Countdown Timer Indicator
const int statusPin = 21;   // GPIO 21 -> Single Sensor Signal Input for Bench Test
const int buzzerPin = 19;   // GPIO 19 -> Buzzer Positive Pin

// Timer Configuration
const int countdownSeconds = 5; // Safety delay before attempting to boot the PC

void setup() {
  // Initialize all pins
  pinMode(triggerPin, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  
  // INPUT_PULLDOWN keeps GPIO 21 at LOW automatically when no wire is connected
  pinMode(statusPin, INPUT_PULLDOWN); 
  pinMode(buzzerPin, OUTPUT);
  
  // Ensure everything starts completely turned off safely
  digitalWrite(triggerPin, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(buzzerPin, LOW);
  
  Serial.begin(115200);
  Serial.println("======================================================");
  Serial.println("--- ESP32 SINGLE-PIN (GPIO 21) SIMULATION MODE ---");
  Serial.println("--- RED LED & BUZZER SYNCHRONIZED TOGETHER ---");
  Serial.println("======================================================");
}

void loop() {
  // ===================================================
  // STEP 1: SAFETY COUNTDOWN (YELLOW LED ON)
  // ===================================================
  Serial.println("\n--- Starting Loop Cycle ---");
  digitalWrite(yellowLED, HIGH); 
  
  for (int i = countdownSeconds; i > 0; i--) {
    Serial.print("Simulated boot attempt in: ");
    Serial.print(i);
    Serial.println(" seconds...");
    delay(1000); 
  }
  
  digitalWrite(yellowLED, LOW); 
  delay(200); 

  // ===================================================
  // STEP 2: CHECK SIMULATED POWER STATE (GPIO 21)
  // ===================================================
  int pcPower = digitalRead(statusPin);
  
  if (pcPower == LOW) { 
    Serial.println("Status: [ GPIO 21 IS LOW -> PC IS OFF ]");
    Serial.println("Action: Activating Transistor (Red LED) and Buzzer together for 2.5 seconds...");
    
    // --- START TOGETHER ---
    digitalWrite(triggerPin, HIGH); // Turn on Red LED / Transistor Switch
    digitalWrite(buzzerPin, HIGH);  // Turn on Buzzer
    
    // Hold them both active at the same time
    delay(2500);                    // 2.5 seconds hold duration
    
    // --- STOP TOGETHER ---
    digitalWrite(triggerPin, LOW);  // Turn off Red LED / Transistor Switch
    digitalWrite(buzzerPin, LOW);   // Turn off Buzzer
    
    Serial.println("Action: Trigger sequence complete.");
  } else {
    Serial.println("Status: [ GPIO 21 IS HIGH -> PC IS RUNNING ]");
    Serial.println("Action: Skipping button press loop to prevent accidental shutdown.");
  }

  // ===================================================
  // STEP 3: 5-SECOND BENCH OBSERVATION WINDOW
  // ===================================================
  Serial.println(">>> Entering 5-second observation window.");
  
  for (int check = 1; check <= 5; check++) {
    int liveRead = digitalRead(statusPin);
    
    Serial.print("   [Live Check ");
    Serial.print(check);
    Serial.print("/5] Sensor Input (GPIO 21): ");
    Serial.println(liveRead == HIGH ? "HIGH (ON)" : "LOW (OFF)");
    
    delay(1000); 
  }
  
  Serial.println("Observation window closed. Resetting loop in 1 second...");
  delay(1000);
}

