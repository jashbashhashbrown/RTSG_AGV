int photocellone = A0; // shortcut to refer to the photocell pin later
int photocelltwo = A1;

void setup() {
  pinMode(photocellone, INPUT); // we will be reading the photocell pin
  pinMode(photocelltwo, INPUT);
  Serial.begin(9600); // starts serial communication at 9,600 baud (the rate)
}

void loop() {
  int readValone; // initialize a new integer to store the photocell value
  int readValtwo;
  readValone = analogRead(photocellone); // do the analog read and store the value
  readValtwo = analogRead(photocelltwo);
  Serial.println(readValone); // push the most recent value to the computer
  delay(50);
  Serial.println(readValtwo);
  delay(50);} // slow the loop down a bit before it repeats
