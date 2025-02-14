// Define pin connections for PIR sensor, LEDs, and Ultrasonic sensor
#define PIR_PIN 3          // PIR motion sensor input pin
#define ONBOARD_LED 13     // Onboard LED for motion indication
#define ALERT_LED 11       // External alert LED for motion detection
#define DISTANCE_LED 10    // LED to indicate object detection by ultrasonic sensor
#define ULTRASONIC_TRIG 5  // Ultrasonic sensor trigger pin

// Flag to track motion detection status
bool isMotionDetected = false;

// Interrupt Service Routine (ISR) for PIR sensor motion detection
void motionISR() {
  isMotionDetected = true;  // Set flag when motion is detected
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

  // Attach interrupt to PIR sensor for motion detection
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), motionISR, RISING);
}

void loop() {
  // Measure distance using ultrasonic sensor
  float distance = measureDistance();
  Serial.print("Measured Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Turn on distance LED if an object is detected within 10 cm
  if (distance < 10) {
    digitalWrite(DISTANCE_LED, HIGH);
    Serial.println("Object detected. LED ON");
  } else {
    digitalWrite(DISTANCE_LED, LOW);
    Serial.println("No object detected. LED OFF");
  }

  // Check if motion was detected by the PIR sensor
  if (isMotionDetected) {
    // Turn on LEDs to indicate motion
    digitalWrite(ALERT_LED, HIGH);
    digitalWrite(ONBOARD_LED, HIGH);

    Serial.println("Motion detected! LEDs turned ON");

    delay(500); // Stability delay to prevent false triggers

    // Turn off LEDs
    digitalWrite(ALERT_LED, LOW);
    digitalWrite(ONBOARD_LED, LOW);

    Serial.println("No motion detected. LEDs turned OFF");

    // Reset motion detection flag
    isMotionDetected = false;
  }

  delay(1000); // Delay to control sensor reading frequency
}

// Function to measure distance using ultrasonic sensor
float measureDistance() {
  pinMode(ULTRASONIC_TRIG, OUTPUT);
  digitalWrite(ULTRASONIC_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG, LOW);

  pinMode(ULTRASONIC_TRIG, INPUT);
  long duration = pulseIn(ULTRASONIC_TRIG, HIGH);
  float distance = duration * 0.034 / 2;  // Convert time to distance
  return distance;
}
