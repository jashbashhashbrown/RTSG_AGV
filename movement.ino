

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

//    if(!ping_front) { /* && !tape_front){*/
//      turnRight();
//      delay(1000);
//      forward(104);
//      delay(2000);
//    } else {
//      //reverse(97);
//      //delay(1000);
//      turnRight();
//      delay(1000);
//      forward(104);
//      delay(2000);
//    }

    //forward(speed);
    //halt();
    //delay(100);
  }
      
}//--(end main loop )---

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


bool is_dark_rm() {
  bool found = false;
  light_read();
  if (lftPhoto >= dark_rm_val && rgtPhoto >= dark_rm_val2){
   found = true;
  }
  return found;
}

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

/*-----( Declare User-written Functions )-----*/
//none
