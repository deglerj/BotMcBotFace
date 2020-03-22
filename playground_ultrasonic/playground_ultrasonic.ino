#define SONIC_TRIGGER 5
#define SONIC_ECHO 6

void setup() {
  pinMode(SONIC_TRIGGER, OUTPUT);
  pinMode(SONIC_ECHO, INPUT);
  
  Serial.begin(9600);
}

float duration;
float distance;

void loop() {
  digitalWrite(SONIC_TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(SONIC_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(SONIC_TRIGGER, LOW);

  duration = pulseIn(SONIC_ECHO, HIGH);
  distance = duration * 0.01715;
  Serial.print("Distance: ");
  Serial.println(distance);

  delay(500);
}
