#include <Wire.h>
#include <MeAuriga.h>
#include <math.h>

// Create an instance of MeGyro for the gyro sensor
MeGyro gyro(0, 0x69);
MeEncoderOnBoard Encoder_1(SLOT1); //motor Left
MeEncoderOnBoard Encoder_2(SLOT2); //motor right
MeLineFollower lineFinder(PORT_6);

void setup() {
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);
  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);

  attachInterrupt(Encoder_1.getIntNum(), isr_process_encoder1, RISING);
  attachInterrupt(Encoder_2.getIntNum(), isr_process_encoder2, RISING);

  Serial.begin(115200);
  // Initialize the gyro sensor
  gyro.begin();
  // Optional: Set the initial values for the gyro (zero calibration)
  gyro.fast_update();
  Encoder_1.setTarPWM(200);
  Encoder_2.setTarPWM(200);
  delay(1000);
  Encoder_1.setTarPWM(0);
  Encoder_2.setTarPWM(0);
  // Encoder_1.move(0.0, 150);
  // Encoder_2.move(0.0, 150);

  // delay(1000);

  // Encoder_1.move(360.0, 150);
  // Encoder_2.move(360.0, 150);

  // delay(1000);

  // Encoder_1.move(720.0, 150);
  // Encoder_2.move(720.0, 150);

  // delay(1000);

  // Encoder_1.move(0.0, 150);
  // Encoder_2.move(0.0, 150);
  
}

void isr_process_encoder1(void) //interrupt service routine
{
  if(digitalRead(Encoder_1.getPortB()) == 0){
    Encoder_1.pulsePosMinus();
  }else{
    Encoder_1.pulsePosPlus();
  }
}
void isr_process_encoder2(void)
{
  if(digitalRead(Encoder_2.getPortB()) == 0){
    Encoder_2.pulsePosMinus();
  }else{
    Encoder_2.pulsePosPlus();
  }
}

void move(int speed){
  Encoder_1.setMotorPwm(-speed); //left motor is built in reverse so it has to spin the other direction
  Encoder_2.setMotorPwm(speed);
  // Encoder_1.updateSpeed();
  // Encoder_2.updateSpeed();
}

float angle1 = 0.0;
float angle2 = 0.0;

void turn(int direction, int deg, int speed){
  float botCirc = 11.5 * M_PI;
  float wheelCirc = 6.5 * M_PI;
  //stop movement
  //calculate number of wheel turns according to int deg
  float turns = (botCirc/wheelCirc) * (float)(deg/360);
  angle1 += turns * 360.0 * direction;
  angle2 -= turns * 360.0 * direction;
  //calculate how long the wheels gotta be turning
  //float turnTime = (speed/60)*turns;
  //turn that amount
  Encoder_1.move(angle1, speed);
  Encoder_2.move(angle2, speed);
  //check how close we got and adjust if off
}

void loop() {
  // Update the gyro sensor readings
  //gyro.update();
  
  // Get the angle values
  // float angleX = gyro.getAngleX();
  // float angleY = gyro.getAngleY();
  // float angleZ = gyro.getAngleZ();
  
  //lineFollow();
  //move(30);
  // delay(5000);
  //turn(1, 90, 30);
  Encoder_1.loop();
  Encoder_2.loop();
  // Serial.println("got out");
  // delay(1000);
  // Print the angle values to the Serial Monitor
  // Serial.print("Angle X: ");
  // Serial.print(angleX);
  // Serial.print("  Angle Y: ");
  // Serial.print(angleY);
  // Serial.print("  Angle Z: ");
  //Serial.println(angleZ);
  
  // Add a small delay to make the output readable
  // delay(100);
}
