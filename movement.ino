

/*-----( Import needed libraries )-----*/
#include <Servo.h>

/*-----( Declare Constants and Pin Numbers )-----*/
#define leftPIN  10
#define rightPIN 11

#define Distance  98 // Forward: 90 to 180, Backwards: 0 to 90. 

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

int lightInitial;
int lightInitialRight;

void forward()
{
  for(pos = 90; pos<=Distance; pos+=1)     
  {
    leftWheels.write(pos);
    delay(10);
    rightWheels.write(pos);            
    delay(10);                       
  }
}

void reverse()
{
  for(pos = 90; pos >= 80; pos-=1)
  {
    leftWheels.write(pos);
    delay(10);
    rightWheels.write(pos);
    delay(10);
  }
}

void halt()
{
  leftWheels.write(90);
  rightWheels.write(90);
}

void turnRight()
{
  leftWheels.write(121);
  delay(10);
  rightWheels.write(90);
  delay(10);
}

void turnLeft()
{
  rightWheels.write(121);
  delay(10);
  leftWheels.write(90);
  delay(10);
}

int ping (int dist, int dir)
{
  long inches[] = {0};

  inches[0] = objDist(dir);

  if(inches[0] < dist){
    return 1;
  }
  return 0;
}

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

void setup()   /****** SETUP: RUNS ONCE ******/
{
  leftWheels.attach(leftPIN);  // attaches the servo on pin 9 to the servo object
  rightWheels.attach(rightPIN);

  pinMode(sensorPin_FR, INPUT); // we will be reading the photocell pin
  pinMode(sensorPin_FL, INPUT);

  Serial.begin(9600);
  
  lightInitial = analogRead(sensorPin_FR);
  lightInitialRight = analogRead(sensorPin_Right);
  
}//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  int ping_right = ping(30, pingRight);
  int tape_right = tapeDetectRight();
  Serial.print(ping_right);
  if(ping_right || tape_right){
    int ping_front = ping(20, pingFront);
    int tape_front = tapeDetectFront();
    int ping_left = ping(20, pingLeft);
    if(!ping_front && !tape_front){
      forward();
      delay(1000);
    } else if(!ping_left){
      reverse();
      delay(1000);
      turnLeft();
      delay(1000);
    } else {
      reverse();
      delay(1000);
      reverse();
      delay(1000);
      turnRight();
      delay(1000);
    }

  } else {
    reverse();
    delay(500);
    turnRight();
    delay(1000);
    forward();
    delay(1000);
    halt();
    delay(1000);
  }
      
}//--(end main loop )---

void followWall() {
  int ping_s = ping(20, pingRight);
  Serial.print(ping_s);
  if (ping_s == 1) {
    forward();
    delay(1000);
  } else {
    turnRight();
    delay(500);
  }
}

void obstacle_front() {
  int ping_s = ping(10, pingFront);
  Serial.print(ping_s);
  if (ping_s == 1) {
    halt();
    delay(1000);
    reverse();
    delay(1000);
    turnRight();
    delay(1000);
    Serial.print(ping_s);
    Serial.print("\nshould halt");
  } else {
    forward();
    delay(1000);
    Serial.print("\nshould go forward");
    //Serial.print(ping());
  }
}

/*-----( Declare User-written Functions )-----*/
//none
