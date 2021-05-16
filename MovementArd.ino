#include <stdio.h>
#include <VNH3SP30.h>


/*******************************************************************************************
 * Define Statements                                                                       *
 *******************************************************************************************/
//compensation value : used to subtract from motor 1 to make the motors equal
#define COMP_SPEED 10

//motor 1 pins
#define M1_PWM 9    
#define M1_INA 8    
#define M1_INB 7
//motor 2 pins
#define M2_PWM 5
#define M2_INA 6
#define M2_INB 4
#include <stdio.h>
#include <VNH3SP30.h>
//ping sensors
#define echoPinL 10 
#define trigPinL 11 
#define echoPinC 13 
#define trigPinC 12 
#define echoPinR 3 
#define trigPinR 2

/*******************************************************************************************
 * Global Variables                                                                        *
 *******************************************************************************************/
char buff [50];

long durationL; // variable for the duration of sound wave travel
int distanceL; // variable for the distance measurement
long durationC; // variable for the duration of sound wave travel
int distanceC; // variable for the distance measurement
long durationR; // variable for the duration of sound wave travel
int distanceR; // variable for the distance measurement

/*******************************************************************************************
 * Objects                                                                                 *
 *******************************************************************************************/
VNH3SP30 motor_1;
VNH3SP30 motor_2; 

/*******************************************************************************************
 * Function Prototypes                                                                     *
 *******************************************************************************************/
//Main Functions
void setup(void);
void loop(void);

//Motor Functions
void turn(int spd, int in_place);
void brake(int brakePower);
void setSpeed(int speed);


/*******************************************************************************************
 * Void setup/ Void Loop                                                                   *
 *******************************************************************************************/
//Main functions
void setup() {

  //Init motors
  motor_1.begin(M1_PWM, M1_INA, M1_INB, -1, -1);
  motor_2.begin(M2_PWM, M2_INB, M2_INA, -1, -1); 
  pinMode(trigPinL, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPinL, INPUT); // Sets the echoPin as an INPUT
  pinMode(trigPinC, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPinC, INPUT); // Sets the echoPin as an INPUT
  pinMode(trigPinR, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPinR, INPUT); // Sets the echoPin as an INPUT

  Serial.begin(9600);

  int findface=0;

  while (!Serial) {
    ; // wait for serial port to connect.
  }

}

void loop() {
// Clears the trigPin condition
  digitalWrite(trigPinL, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPinL, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinL, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  durationL = pulseIn(echoPinL, HIGH);
  // Calculating the distance
  distanceL = durationL * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance LEFT: ");
  Serial.print(distanceL);
  Serial.println(" cm");

    // Clears the trigPin condition
  digitalWrite(trigPinC, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPinC, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinC, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  durationC = pulseIn(echoPinC, HIGH);
  // Calculating the distance
  distanceC = durationC * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance CENTER: ");
  Serial.print(distanceC);
  Serial.println(" cm");

      // Clears the trigPin condition
  digitalWrite(trigPinR, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPinR, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinR, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  durationR = pulseIn(echoPinR, HIGH);
  // Calculating the distance
  distanceR = durationR * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance RIGHT: ");
  Serial.print(distanceR);
  Serial.println(" cm");

  char buffer[16];

  if (Serial.available() > 0) {
    int size = Serial.readBytesUntil('\n', buffer, 12);
    if (buffer[0] == 'Y') {//when image is found move forward
      setspeed(100);
      delay(2000);
      break(50);
      finface=1;
    }
    else{
      if(findface=='1'){ //face is found flag is set, so robot doesn't move anymore
        brake(50);
      }
      else{
      if (buffer[0] == 'N') {
      digitalWrite(LED_BUILTIN, LOW);
      }

      if (distanceC > 5){
      setSpeed(-50);
      delay(500);

      turn(-100,1); //turn left
      delay(500);        
      }

      else if (distanceR > 5) {
      turn(-50,1); //turn left
      delay(500);    
      }
  
      else if (distanceL > 5){
      turn(50,1); //turn right
      delay(500);    
    }
  
    else {
    setSpeed(50);
    }
    }
    }
}



/*******************************************************************************************
 * Motor Constroller Functions                                                             *
 *******************************************************************************************/
/*
 * Sets the speed of both motors 
 * Used for moving the bot forward and backwards
 * range between 0 and 400
 */
void setSpeed(int speed) {
  motor_1.setSpeed(speed - COMP_SPEED);
  motor_2.setSpeed(speed);
}

/*
 * Brings the robot to a stop in a more natural way
 * range between 0 and 400
 */
void brake(int brakePower) {
  motor_1.brake(brakePower);
  motor_2.brake(brakePower);
}

/*
 * providing different speeds to the motors, you can slighty turn if need be
 * Parameter in_place determines if the robot will do a slight angle (0) or if
 * it should be turning in place (1)
 * spd > 0 (meaning its positve)  turn right
 * spd < 0 (meaning its negative) turn left
 */
void turn(int spd, int in_place) {
  //turn in place
  if (in_place) {
    motor_1.setSpeed(spd);
    motor_2.setSpeed(-spd);    
  }

  //doing a slight turn 
  else {
    int currentSpeed = (motor_1.speed + motor_2.speed) / 2;
    
    motor_1.setSpeed(currentSpeed + spd);
    motor_2.setSpeed(currentSpeed - spd); 
  }
}
