#define TRIG_PIN 9
#define ECHO_PIN 10
#define SPEED_SOUND 0.0343 // cm / us
#define CM_TO_IN 0.39370   // in / cm

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  float duration, distance;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);

  // SPEED = DISTANCE / TIME
  // We divide by two because echo tells us round trip time
  distance = (duration * SPEED_SOUND) / 2;

  Serial.print("Distance: ");
  Serial.println(distance * CM_TO_IN);
  delay(100);
}
