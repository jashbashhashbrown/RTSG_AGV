

/*-----( Import needed libraries )-----*/
#include <Servo.h>

/*-----( Declare Constants and Pin Numbers )-----*/
#define leftPIN  10
#define rightPIN 11

#define Distance  97 // Forward: 90 to 180, Backwards: 0 to 90. 

/*-----( Declare objects )-----*/
Servo leftWheels;  // create servo object to control a servo 
Servo rightWheels;

/*-----( Declare Variables )-----*/
int pos = 0;   // variable to store the servo position
int rev = 0;

const int pingFront = 7;
const int pingRight = 8;
const int pingLeft = 9;

int sensorPin_FR = A0;
int sensorPin_FL = A1;
int sensorPin_Right = A2;
int photocellLft = A3; // shortcut to refer to the photocell pin later
int photocellRgt = A4;

int dark_rm_val = 590;
int dark_rm_val2 = 505;
int lftPhoto; // initialize a new integer to store the photocell value
int rgtPhoto;

int lightInitial;
int lightInitialRight;

/*----------------- SETUP + LOOP -------------------*/
void setup()   /****** SETUP: RUNS ONCE ******/
{
  leftWheels.attach(leftPIN);  // attaches the servo on pin 9 to the servo object
  rightWheels.attach(rightPIN);

  pinMode(sensorPin_FR, INPUT); // we will be reading the photocell pin
  pinMode(sensorPin_FL, INPUT);

  pinMode(photocellLft, INPUT); // we will be reading the photocell pin
  pinMode(photocellRgt, INPUT);

  Serial.begin(9600);
  
  lightInitial = analogRead(sensorPin_FR);
  lightInitialRight = analogRead(sensorPin_Right);
  
}//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  int speed = 96;
  
  int ping_right = ping(30, pingRight);
  int tape_right = tapeDetectRight();
  int ping_front = ping(20, pingFront);
  int tape_front = tapeDetectFront();
  Serial.print(ping_right);
  if(ping_right){// || tape_right){
    int ping_left = ping(20, pingLeft);
    if(!ping_front && !tape_front){
      forward(100);
      delay(100);
    } else if(!ping_left){
      reverse(100);
      delay(1000);
      turnLeft();
      delay(1000);
      forward(speed);
      delay(100);
    } else {
      reverse(104);
      delay(1000);
      //reverse(speed);
      //delay(100);
      turnRight();
      delay(1000);
      forward(100);
      delay(100);
    }

  } else {
    turnRight();
    delay(1000);
    if(is_dark_rm()){
      Serial.print("IN DARK ROOM\n");
      Serial.println(lftPhoto);
      Serial.println(rgtPhoto);
      go_twrd_light();
    }
    Serial.print("is dark = ");
    Serial.println(lftPhoto);
    Serial.println(rgtPhoto);
    Serial.print("##");
    int ping_front = ping(20, pingFront);
    while(!ping_right && !ping_front){
      Serial.print(ping_front);
      ping_right = ping(30, pingRight);
      ping_front = ping(20, pingFront);
      forward(100);
      delay(100);
    }
    if(ping_front){
      reverse(98);
      delay(1000);
      turnLeft();
      delay(1000);
    }
  }      
}//--(end main loop )---

/*-----( Declare User-written Functions )-----*/

/* -------------( SENSOR FUNCTIONS ) ------------------*/

/*
 * light_read(): sensor function that detects light and chenged global values.
 */
void light_read(){
  lftPhoto = analogRead(photocellLft); // do the analog read and store the value
  rgtPhoto = analogRead(photocellRgt) + 130;
  //Serial.println("leftPhoto");
  //Serial.println(lftPhoto); // push the most recent value to the computer
  delay(200);
  /*Serial.println("rightPhoto");
  Serial.println(rgtPhoto);*/
  delay(200); // slow the loop down a bit before it repeats*/
}

/*
 * go_twrd_light(): movement function reactionary to light. Detects light in front of, front-left, and front-right of the
 * vehicle. Goes front if there is not a bigger difference of 60 between both sensors, the vehcile will move forward. If
 * the difference is greater, it will turn in the direction with the greater magnitude of light recieved.
 */
void go_twrd_light(){
  while(1){
    //Serial.println("in go_twrd_light");
    light_read();
    Serial.println(lftPhoto);
    Serial.println(rgtPhoto);
    if (lftPhoto >= rgtPhoto+60){
      Serial.println("turn right");
      turnRight();
      delay(650);
      halt();
    } else if (rgtPhoto >= lftPhoto+20){
      Serial.println("turn left");
      turnLeft();
      delay(650);
      halt();
    } else { //(abs(lftPhoto - rgtPhoto) <=60)
      Serial.println("forward");
      forward(100);
      delay(100);
    } 
  }
}

/*
 * is_dark_rm(): boolean function that returns a true if the room is dark; a false is the room is
 * light. The light values are captured through the function 'light_read().'
 */
bool is_dark_rm() {
  bool found = false;
  light_read();
  if (lftPhoto >= dark_rm_val && rgtPhoto >= dark_rm_val2){
   found = true;
  }
  return found;
}

/*
 * ping: A sensor function that detect the distance of the closest obstacle in the inputted
 * direction and returns a boolean value.
 * Input: takes a maximum distance function and a direction. The direction dictates what ping sensor is used,
 * the distance detects the limit of when to return an obstacle has been detected.
 */
int ping (int dist, int dir)
{
  long inches[] = {0};

  inches[0] = objDist(dir);

  if(inches[0] < dist){
    return 1;
  }
  return 0;
}

/*
 * objDist: returns the distance of the obstacle from the designated ping sensor
 */
long objDist(int pingPin) {
  long duration, inches;

  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  inches = microsecondsToInches(duration);
  return inches;
}

long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}

/*
 * tapeDetectRight(): sensor function that detects tape to the right of the vehcile.
 * The sensor was not optimal and this function was not used.
 */
int tapeDetectRight(){
  int lightVal_Right = analogRead(sensorPin_Right);

  Serial.print(lightVal_Right);

  if(lightVal_Right > lightInitialRight + 50){
    return 1;
  } else {
    return 0;
  }
}

int tapeDetectFront(){
  int lightVal_FR = analogRead(sensorPin_FR);
  int lightVal_FL = analogRead(sensorPin_FL);
  int lightVal_avg = (lightVal_FR+lightVal_FL)/2;
  
  Serial.print(lightVal_avg);
  //if lightVal is less than our initial reading (lightInitial) minus 50 it is dark
  //print 'Dark'. 
  //Sensitivity = -50 (smaller, more sensitive)
  
  if (lightVal_avg > lightInitial + 50)
  {
    Serial.println("XXXXXXXXXXXXXXXXXX");
    return 1;
  }

  //else, it is bright, print light
  else
  {
    Serial.println("OOOOOOOOOOOOOOOOOO");
    return 0;
  }
  delay(50); //delay half second between numbers
}

/*-------( MOVEMENT BASED FUNCTIONS ) -------------------*/

/*
 * followWall: movement function that uses ping sensor to detect if there is a wall to the
 * right of the vehcile. If there is a obstacle or wall, the vehicle moves forward. Else, 
 * the vehcile moves straight.
 */
void followWall() {
  int ping_s = ping(20, pingRight);
  Serial.print(ping_s);
  if (ping_s == 1) {
    forward(97);
    delay(1000);
  } else {
    turnRight();
    delay(500);
  }
}

/*
 * obstacle_front: movement function that utilzies the forward ping sensor. Moves forward if the
 * ping sensor does not detect an obstacle within the range of 10. Else, the vehcile turns right.
 */
void obstacle_front() {
  int ping_s = ping(10, pingFront);
  Serial.print(ping_s);
  if (ping_s == 1) {
    halt();
    delay(1000);
    reverse(97);
    delay(1000);
    turnRight();
    delay(1000);
    Serial.print(ping_s);
    Serial.print("\nshould halt");
  } else {
    forward(97);
    delay(100);
    Serial.print("\nshould go forward");
    //Serial.print(ping());
  }
}

void forward(int dist)
{
  leftWheels.write(dist);
  rightWheels.write(dist);                                 
}

void reverse(int dist)
{
  int converted = 90 - (dist-90);
  leftWheels.write(converted);
  rightWheels.write(converted);

}

void halt()
{
  leftWheels.write(90);
  rightWheels.write(90);
}

void turnRight()
{
  leftWheels.write(108); //112
  rightWheels.write(90); //66
}

void turnLeft()
{
  rightWheels.write(114); //126
  leftWheels.write(90);
}
