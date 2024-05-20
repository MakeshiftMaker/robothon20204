#include <Wire.h>
#include <MeAuriga.h>

// Create an instance of MeGyro for the gyro sensor
MeGyro gyro(0, 0x69);
MeEncoderOnBoard motor_left(SLOT1);
MeEncoderOnBoard motor_right(SLOT2);
MeLineFollower lineFinder(PORT_6);

void setup() {
  // Initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  // Initialize the gyro sensor
  gyro.begin();
  // Optional: Set the initial values for the gyro (zero calibration)
  gyro.fast_update();
  
}

void move(int speed){
  motor_left.setMotorPwm(-speed);
  motor_right.setMotorPwm(speed); //right motor is built in reverse so it has to spin the other direction
}

void turn(int direction, int deg){
  move(0);
  int rotation = gyro.getAngleZ()+180;

  if(direction == 0){//turn right
    if((rotation+deg)%360 < gyro.getAngleZ()){
      while(gyro.getAngleZ() > (rotation+deg)%360 ){
        gyro.update();
        // Serial.print(gyro.getAngleZ()+180);
        // Serial.print(" ");
        // Serial.println((rotation + deg) % 360);
        motor_left.setMotorPwm(30);
        motor_right.setMotorPwm(30);
      }
    }
    
      while((int)gyro.getAngleZ()+180 <= ((rotation + deg)%360)){
        gyro.update();
        // Serial.print(gyro.getAngleZ()+180);
        // Serial.print(" ");
        // Serial.println((rotation + deg) % 360);
        motor_left.setMotorPwm(30);
        motor_right.setMotorPwm(30);
      }
    
  }
  else{//turn left
    while(gyro.getAngleZ()+180 >= ((rotation-deg)%360)){
      gyro.update();
      // Serial.print(gyro.getAngleZ()+180);
      // Serial.print(" ");
      // Serial.println((rotation + deg) % 360);
      motor_left.setMotorPwm(-30);
      motor_right.setMotorPwm(-30);
    }
  }
  move(0);
}

void lineFollow(){
  int sensorState = lineFinder.readSensors();
  Serial.println(sensorState);
  switch(sensorState)
  {
    case 0:
      Serial.println("Neither is white"); //both black
      move(30);
      break; //forward
    case 1:
      Serial.println("Left is white"); //right black
      while(lineFinder.readSensors() == 1){
        turn(0,1);
      }  
      break; // turn right
    case 2:
      Serial.println("Right is white"); //left black
      while(lineFinder.readSensors() == 2){
        turn(1,1);
      }
      break; // turn left //left
    case 3:
      Serial.println("Left Right is white"); //neither black
      move(30);
      break;//forward
    default: break;
  }
}

// void balance(){
//   float angleX = gyro.getAngleX();
//   //the higher the angle the proportionally larger the tourque has to be
//   //max angle is 30 degrees (pulled out of my ass)
//   int speed = angleX / 30 * 255;
//   if(abs(angleX) > 5){
//     move(0speed);
//   }
//   else{
//     move(0);
//   }
//   Serial.print("balance-speed ");
//   Serial.print(speed);
// }

void loop() {
  // Update the gyro sensor readings
  gyro.update();
  
  // Get the angle values
  // float angleX = gyro.getAngleX();
  // float angleY = gyro.getAngleY();
  // float angleZ = gyro.getAngleZ();
  
  //lineFollow();
  turn(0, 90);
  //Serial.println("got out");
  delay(1000);
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
