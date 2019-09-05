

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

const int pingPin = 7;

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
  leftWheels.write(120);
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

int ping (int dist)
{
  long inches[] = {0};

  inches[0] = objDist();

  if(inches[0] < dist){
    return 1;
  }
  return 0;
}

long objDist() {
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

void setup()   /****** SETUP: RUNS ONCE ******/
{
  leftWheels.attach(leftPIN);  // attaches the servo on pin 9 to the servo object
  rightWheels.attach(rightPIN);

  Serial.begin(9600);
  
}//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  followWall();
      
}//--(end main loop )---

void followWall() {
  int ping_s = ping(20);
  Serial.print(ping_s);
  if (ping_s == 1) {
    forward();
    delay(1000);
  } else {
    turnRight();
    delay(500);
  }
}

void detect_turnRight() {
  int ping_s = ping(10);
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