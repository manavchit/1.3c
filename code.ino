// Define pin connections for PIR sensor, LEDs, and Ultrasonic sensor 
#define PIR_PIN 3          // PIR motion sensor input pin
#define ONBOARD_LED 13     // Onboard LED for motion indication
#define ALERT_LED 11       // External alert LED for motion detection
#define DISTANCE_LED 10    // LED to indicate object detection by ultrasonic sensor
#define ULTRASONIC_TRIG 5  // Ultrasonic sensor trigger pin


// Flags to track sensor statuses
volatile bool isMotionDetected = false;
volatile bool isObjectDetected = false;

// Interrupt Service Routine (ISR) for PIR sensor motion detection
void motionISR() {
  isMotionDetected = true;  // Set flag when motion is detected
}

// Interrupt Service Routine (ISR) for Ultrasonic sensor detection
void objectISR() {
  float distance = measureDistance();
  if (distance < 10) {
    isObjectDetected = true;
  } else {
    isObjectDetected = false;
  }
}

void setup() {
  // Configure pin modes
  pinMode(DISTANCE_LED, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(ONBOARD_LED, OUTPUT);
  pinMode(ALERT_LED, OUTPUT);
  pinMode(ULTRASONIC_TRIG, OUTPUT);
 

  // Initialize serial communication
  Serial.begin(9600);

  // Attach interrupts to sensors
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), motionISR, RISING);
  
}

void loop() {
  // Handle motion detection
  if (isMotionDetected) {
    digitalWrite(ALERT_LED, HIGH);
    digitalWrite(ONBOARD_LED, HIGH);
    Serial.println("Motion detected! LEDs turned ON");

    delay(500); // Stability delay to prevent false triggers

    digitalWrite(ALERT_LED, LOW);
    digitalWrite(ONBOARD_LED, LOW);
    Serial.println("No motion detected. LEDs turned OFF");

    isMotionDetected = false; // Reset flag
  }

  // Handle object detection
  if (isObjectDetected) {
    digitalWrite(DISTANCE_LED, HIGH);
    Serial.println("Object detected within 10 cm. LED ON");
  } else {
    digitalWrite(DISTANCE_LED, LOW);
    Serial.println("No object detected. LED OFF");
  }

  delay(1000); // Delay to control sensor reading frequency
}

// Function to measure distance using ultrasonic sensor
float measureDistance() {
  digitalWrite(ULTRASONIC_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG, LOW);

  long duration = pulseIn(ULTRASONIC_TRIG, HIGH);
  float distance = duration * 0.034 / 2;  // Convert time to distance
  return distance;
}
