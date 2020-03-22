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

struct Motor {
  int pinA;
  int pinB;
  int pinPwm;
  bool flipped;
};

Motor motorLeftFront = { MOTOR_LEFT_FRONT_A, MOTOR_LEFT_FRONT_B, MOTOR_LEFT_FRONT_PWM, false };
Motor motorLeftBack = { MOTOR_LEFT_BACK_A, MOTOR_LEFT_BACK_B, MOTOR_LEFT_BACK_PWM, true };
Motor motorRightFront = { MOTOR_RIGHT_FRONT_A, MOTOR_RIGHT_FRONT_B, MOTOR_RIGHT_FRONT_PWM, true };
Motor motorRightBack = { MOTOR_RIGHT_BACK_A, MOTOR_RIGHT_BACK_B, MOTOR_RIGHT_BACK_PWM, false };

void setup() {
  setupMotor(motorLeftFront);
  setupMotor(motorLeftBack);
  setupMotor(motorRightFront);
  setupMotor(motorRightBack);

  Serial.begin(9600);
}

void setupMotor(Motor motor) {
  pinMode(motor.pinA, OUTPUT);
  pinMode(motor.pinB, OUTPUT);
  pinMode(motor.pinPwm, OUTPUT);
}

void loop() {
  motorStop(motorLeftFront);
  motorStop(motorLeftBack);
  motorStop(motorRightFront);
  motorStop(motorRightBack);

  delay(500);
  
  motorForward(motorLeftFront);
  motorForward(motorLeftBack);
  motorForward(motorRightFront);
  motorForward(motorRightBack);

  for(int i = 0; i < 100; i++) {
    motorSpeed(motorLeftFront, i);
    motorSpeed(motorLeftBack, i);
    motorSpeed(motorRightFront, i);
    motorSpeed(motorRightBack, i);

    delay(50);
  }

  motorStop(motorLeftFront);
  motorStop(motorLeftBack);
  motorStop(motorRightFront);
  motorStop(motorRightBack);

  delay(500);

  motorBackward(motorLeftFront);
  motorBackward(motorLeftBack);
  motorBackward(motorRightFront);
  motorBackward(motorRightBack);

  for(int i = 0; i < 100; i++) {
    motorSpeed(motorLeftFront, i);
    motorSpeed(motorLeftBack, i);
    motorSpeed(motorRightFront, i);
    motorSpeed(motorRightBack, i);

    delay(50);
  }
}

void motorForward(Motor motor) {
  digitalWrite(motor.pinA, motor.flipped ? LOW : HIGH);
  digitalWrite(motor.pinB, motor.flipped ? HIGH : LOW);
}

void motorBackward(Motor motor) {
  digitalWrite(motor.pinA, motor.flipped ? HIGH : LOW);
  digitalWrite(motor.pinB, motor.flipped ? LOW : HIGH);
}

void motorStop(Motor motor) {
  digitalWrite(motor.pinA, LOW);
  digitalWrite(motor.pinB, LOW);
}

void motorSpeed(Motor motor, int speedPercentage) {
  int pwmValue = map(speedPercentage, 0, 100, MOTOR_MIN_PWM, MOTOR_MAX_PWM);
  analogWrite(motor.pinPwm, pwmValue);

}
