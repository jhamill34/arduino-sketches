#define LED_PIN 2
#define SWITCH_PIN 3

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int value = digitalRead(SWITCH_PIN);
  digitalWrite(LED_PIN, value);   
}
