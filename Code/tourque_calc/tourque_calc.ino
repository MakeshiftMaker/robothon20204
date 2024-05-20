#include <Wire.h>
#include <MeAuriga.h>

// Create an instance of MeGyro for the gyro sensor
MeGyro gyro(0, 0x69);
MeEncoderOnBoard motor_left(SLOT1);
MeEncoderOnBoard motor_right(SLOT2);

void setup() {
  // Initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  // Initialize the gyro sensor
  gyro.begin();
  // Optional: Set the initial values for the gyro (zero calibration)
  gyro.fast_update();
  
}

void move(int direction, int speed){
  int left = 0;
  int right = 0;
  switch(direction){
    case 0:
      left = speed;
      right = speed;
      break;
  }
  // Serial.print("motor-speed ");
  // Serial.print(speed/100*255);
  motor_left.setMotorPwm(-speed);
  motor_right.setMotorPwm(speed); //right motor is built in reverse so it has to spin the other direction
}

void balance(){
  float angleX = gyro.getAngleX();
  //the higher the angle the proportionally larger the tourque has to be
  //max angle is 30 degrees (pulled out of my ass)
  int speed = angleX / 30 * 255;
  if(abs(angleX) > 5){
    move(0, speed);
  }
  else{
    move(0,0);
  }
  Serial.print("balance-speed ");
  Serial.print(speed);
}

void turnTest(){
  gyro.fast_update();
  int time = millis();
  while(time%1000 != 0);
  time = millis();
  Serial.println(time);
  float startAngle = gyro.getAngleX();
  float endAngle;
  move(0,255);
  
  if(time % 1000 == 0){
    gyro.update();
    endAngle = gyro.getAngleX();
  }
  // move(0,0);
  float anglePerSecond = endAngle - startAngle;
  if(anglePerSecond > 10){
    Serial.println(anglePerSecond);
  }
  move(0,0);
}

void loop() {
  // Update the gyro sensor readings
  gyro.update();
  turnTest();
  // Get the angle values
  // float angleX = gyro.getAngleX();
  // float angleY = gyro.getAngleY();
  // float angleZ = gyro.getAngleZ();
  
  
  // // Print the angle values to the Serial Monitor
  // Serial.print("Angle X: ");
  // Serial.print(angleX);
  // Serial.print("  Angle Y: ");
  // Serial.print(angleY);
  // Serial.print("  Angle Z: ");
  // Serial.println(angleZ);
  
  // Add a small delay to make the output readable
  delay(100);
}
