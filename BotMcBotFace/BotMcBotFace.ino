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

#define STOP_DISTANCE 200

#define DEFAULT_SPEED 65

#define FULL_TURN_DURATION_MILLIS 3400

#define CLOCKWISE 1
#define COUNTERCLOCKWISE 2
#define DIRECTION_CHANGE_CHANCE 25

#define EVADE_ANGLE_MAX 180
#define EVADE_ANGLE_MIN 45
#define EVADE_ANGLE_DIFF 180 - 45

struct Motor {
  const byte pinA;
  const byte pinB;
  const byte pinPwm;
  const bool flipped;
};

struct Sonic {
  const byte pinTrigger;
  const byte pinEcho;
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

byte evadeDirection = CLOCKWISE;
short evadeAngle = -1;

void setup() {
  Serial.begin(9600);

  randomSeed(analogRead(0));
  
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
    evade();
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

void setSpeed(byte speedPercentage) {
  for(Motor motor : motors) {
    setSpeed(motor, speedPercentage);
  }
}

void setSpeed(Motor motor, byte speedPercentage) {
  int pwmValue = map(speedPercentage, 0, 100, MOTOR_MIN_PWM, MOTOR_MAX_PWM);
  analogWrite(motor.pinPwm, pwmValue);
}

void turnLeft(short angle) {
  setSpeed(DEFAULT_SPEED);
  
  driveForward(motorRightFront);
  driveForward(motorRightBack);
  driveBackwards(motorLeftFront);
  driveBackwards(motorLeftBack);  

  delay(FULL_TURN_DURATION_MILLIS * (angle / (float)360));  

  stop();
}

void turnRight(short angle) {
  setSpeed(DEFAULT_SPEED);
  
  driveBackwards(motorRightFront);
  driveBackwards(motorRightBack);
  driveForward(motorLeftFront);
  driveForward(motorLeftBack);  

  delay(FULL_TURN_DURATION_MILLIS * (angle / (float)360));  

  stop();
}

void evade() {
  if(random(100) < DIRECTION_CHANGE_CHANCE) {
    evadeDirection = evadeDirection == CLOCKWISE ? COUNTERCLOCKWISE : CLOCKWISE;
  }

  evadeAngle = random(EVADE_ANGLE_DIFF) + EVADE_ANGLE_MIN;
  if(evadeDirection == CLOCKWISE) {
    turnRight(evadeAngle);
  }
  else {
    turnLeft(evadeAngle);
  }
}
