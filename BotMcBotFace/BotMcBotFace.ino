#define MOTOR_LEFT_FRONT_A 43
#define MOTOR_LEFT_FRONT_B 45
#define MOTOR_LEFT_FRONT_PWM 12

#define MOTOR_LEFT_BACK_A 39
#define MOTOR_LEFT_BACK_B 41
#define MOTOR_LEFT_BACK_PWM 13

#define MOTOR_RIGHT_FRONT_A 47
#define MOTOR_RIGHT_FRONT_B 49
#define MOTOR_RIGHT_FRONT_PWM 10

#define MOTOR_RIGHT_BACK_A 51
#define MOTOR_RIGHT_BACK_B 53
#define MOTOR_RIGHT_BACK_PWM 11

#define MOTOR_MIN_PWM 100
#define MOTOR_MAX_PWM 255

#define SONIC_TRIGGER 5
#define SONIC_ECHO 6

#define STOP_DISTANCE 40

#define DEFAULT_SPEED 75

#define FULL_TURN_DURATION_MILLIS 3400

struct Motor {
  const int pinA;
  const int pinB;
  const int pinPwm;
  const bool flipped;
};

struct Sonic {
  const int pinTrigger;
  const int pinEcho;
};

struct SonicValues {
  float duration;
  float distance;
};

const Motor motorLeftFront = { MOTOR_LEFT_FRONT_A, MOTOR_LEFT_FRONT_B, MOTOR_LEFT_FRONT_PWM, false };
const Motor motorLeftBack = { MOTOR_LEFT_BACK_A, MOTOR_LEFT_BACK_B, MOTOR_LEFT_BACK_PWM, true };
const Motor motorRightFront = { MOTOR_RIGHT_FRONT_A, MOTOR_RIGHT_FRONT_B, MOTOR_RIGHT_FRONT_PWM, true };
const Motor motorRightBack = { MOTOR_RIGHT_BACK_A, MOTOR_RIGHT_BACK_B, MOTOR_RIGHT_BACK_PWM, false };

const Motor motors[4] = { motorLeftFront, motorLeftBack, motorRightFront, motorRightBack };

const Sonic sonic = { SONIC_TRIGGER, SONIC_ECHO };
SonicValues sonicValues = { -1, -1 };

void setup() {
  Serial.begin(9600);
  
  setupMotor(motorLeftFront);
  setupMotor(motorLeftBack);
  setupMotor(motorRightFront);
  setupMotor(motorRightBack);

  setupSonic(sonic);
}

void setupMotor(Motor motor) {
  pinMode(motor.pinA, OUTPUT);
  pinMode(motor.pinB, OUTPUT);
  pinMode(motor.pinPwm, OUTPUT);

}

void setupSonic(Sonic sonic) {
  pinMode(sonic.pinTrigger, OUTPUT);
  pinMode(sonic.pinEcho, INPUT);
}

void loop() {
  measureDistance(sonic, sonicValues);

  if(sonicValues.distance < 15) {
    stop();
  }
  else {
    setSpeed(DEFAULT_SPEED);
    driveForward();
  }
}

void measureDistance(Sonic sonic, SonicValues &values) {
  digitalWrite(sonic.pinTrigger, LOW);
  delayMicroseconds(2);
  digitalWrite(sonic.pinTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(sonic.pinTrigger, LOW);

  values.duration = pulseIn(sonic.pinEcho, HIGH);
  values.distance = values.duration * 0.01715;
}

void driveForward() {
  for(Motor motor : motors) {
    driveForward(motor);
  }
}

void driveForward(Motor motor) {
  digitalWrite(motor.pinA, motor.flipped ? LOW : HIGH);
  digitalWrite(motor.pinB, motor.flipped ? HIGH : LOW);
}

void driveBackwards() {
  for(Motor motor : motors) {
    driveBackwards(motor);
  }
}

void driveBackwards(Motor motor) {
  digitalWrite(motor.pinA, motor.flipped ? HIGH : LOW);
  digitalWrite(motor.pinB, motor.flipped ? LOW : HIGH);
}

void stop() {
  for(Motor motor : motors) {
    stop(motor);
  }
}

void stop(Motor motor) {
  digitalWrite(motor.pinA, LOW);
  digitalWrite(motor.pinB, LOW);
}

void setSpeed(int speedPercentage) {
  for(Motor motor : motors) {
    setSpeed(motor, speedPercentage);
  }
}

void setSpeed(Motor motor, int speedPercentage) {
  int pwmValue = map(speedPercentage, 0, 100, MOTOR_MIN_PWM, MOTOR_MAX_PWM);
  analogWrite(motor.pinPwm, pwmValue);
}

void turnLeft(int angle) {
  setSpeed(DEFAULT_SPEED);
  
  driveForward(motorRightFront);
  driveForward(motorRightBack);
  driveBackwards(motorLeftFront);
  driveBackwards(motorLeftBack);  

  delay(FULL_TURN_DURATION_MILLIS * (angle / (float)360));  

  stop();
}

void turnRight(int angle) {
  setSpeed(DEFAULT_SPEED);
  
  driveBackwards(motorRightFront);
  driveBackwards(motorRightBack);
  driveForward(motorLeftFront);
  driveForward(motorLeftBack);  

  delay(FULL_TURN_DURATION_MILLIS * (angle / (float)360));  

  stop();
}
