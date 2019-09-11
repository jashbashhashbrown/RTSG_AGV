

/*----------------------( Import needed libraries )-------------------------*/
#include <Servo.h>

/*----------------------( Declare Constants and Pin Numbers )----------------------------*/
#define leftPIN  10
#define rightPIN 11

#define Distance  98 // Forward: 90 to 180, Backwards: 0 to 90. 

/*---------------------------( Declare objects )--------------------*/
Servo leftWheels;  // create servo object to control a servo 
Servo rightWheels;

/*-------------------------------( Declare Variables )-----------------------*/
int pos = 0;   // variable to store the servo position
int rev = 0;

const int pingFront = 7;
const int pingRight = 8;
const int pingLeft = 9;


/****** SETUP: RUNS ONCE ******/

void setup()   
{
  leftWheels.attach(leftPIN);  // attaches the servo on pin 9 to the servo object
  rightWheels.attach(rightPIN);

  Serial.begin(9600);
  
}//--(end setup )---



/****** LOOP: RUNS CONSTANTLY ******/
void loop()   
{
  followWall();      
}
//--(end main loop )---



/*-----------------( Declare User-written Functions )---------------------*/

/*---------------------------( ALGORITHMS )-------------------------------*/

void followWall() {
  while(ping(10, pingRight)) {
    Serial.print("Go forward \n");
    forward(97);
    delay(100);
  }
  Serial.print("Turn right \n");
  turnRight();
  delay(1000);
  
//  int ping_s = ping(20, pingRight);
//  Serial.print(ping_s);
//  if (ping_s == 1) {
//    forward(105);
//    delay(1000);
//  } else {
//    turnRight();
//    delay(500);
//  }
}

void obstacle_front() {
while (ping(10, pingFront)) {
    Serial.print("Halting \n");
    halt();
    delay(100);
  }
  Serial.print("Forward \n");
  forward(97);
  delay(100);
}

/*--------------------( Declare Movement Functions) --------------------- */

/*
 * FORWARD function: moves vehicle forward
 * Input: Pos defines angular rotation speed
 * Input and Delay can varu the distance of the vehicle
 * 90 < Pos <= 180 for Foward movement
 * Closer to 90, slower the speed
 */
void forward(int pos)
{
  leftWheels.write(pos);
  // delay(10); // Is this needed? Test
  rightWheels.write(pos);
  // delay(10); // Is this needed? Test
}

/*
 * REVERSE Function: moves vehicle backward
 * Input: pos defines rotation speed and direction
 * 0 <= pos <= 90
 * Closer to 90, slower
 */
void reverse(int pos)
{
  leftWheels.write(pos);
  // delay(10);
  rightWheels.write(pos);
  // delay(10);
}

/*
 * HALT function: Halts the vehcile by writing 90 to each wheel
 * 90 is a complete stop and no rotation.
 */
void halt()
{
  leftWheels.write(90);
  // delay(10);
  rightWheels.write(90);
  // delay(10);
}

/*
 * TURN RIGHT function: Turns vehicle right 90 degrees
 * May need to tweak write inputs for more accurate turn
 */
void turnRight()
{
  leftWheels.write(120);
  // delay(10);
  rightWheels.write(90);
  // delay(10);
}

/*
 * TURN LEFT function: Turns vehcile left 90 degrees
 * May need to tweak write inputs for more accurate turn
 */
void turnLeft()
{
  rightWheels.write(121);
  // delay(10);
  leftWheels.write(90);
  // delay(10);
}

/*-----------------------(Declare Sensor Functions) ----------------------------*/

/*
 * Ping funcitons: uses ping sensor to detect real obstacles to front, left, right of vehicle
 * Distance input: defines set distance to detect wall
 * direction: defines which ping sensor to utilize 
 *  (pingFront, pingRight, pingLeft) only
 * Returns Boolean (0 or 1)
 * Output 0: pinged distance is greater than trigger (NOTHING DETECTED)
 * Output 1: pinged distance is less than trigger (DETECTED OBSTACLE)
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
 * objDistance: finds closest obstacle in direction of inputted ping direction
 * Returns direction
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





/* -------------------(NOTES)----------------------*/
//  int ping_right = ping(20, pingRight);
//  Serial.print(ping_right);
//  if(ping_right == 1){
//    int ping_front = ping(20, pingFront);
//    int ping_left = ping(20, pingLeft);
//    if(ping_front == 0){
//      forward();
//      delay(1000);
//    } else if(ping_left == 0){
//      reverse();
//      delay(1000);
//      turnLeft();
//      delay(1000);
//    } else {
//      reverse();
//      delay(1000);
//      turnRight();
//      delay(1000);
//    }
//
//  } else {
//    reverse();
//    delay(500);
//    turnRight();
//    delay(1000);
//    forward();
//    delay(1000);
//    halt();
//    delay(1000);
