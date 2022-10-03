#include "Wire.h" 


//all global variables
const int MPU_ADDR = 0x68; 

//manage the accelerometer
int16_t accelerometer_x, accelerometer_y, accelerometer_z; 
int16_t gyro_x, gyro_y, gyro_z; 
int16_t temperature;

char tmp_str[7];

char* convert_int16_to_str(int16_t i) {
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

//manage the input pins and 4 seven segment displays
#define SERIESRESISTOR 10000    

float reading;
#define THERMISTORPIN A0 
int holdornot = 1;
int pinFirst = A1;
int pinA = 2;
int pinB = 3;
int pinC = 4;
int pinD = 5;
int pinE = 6;
int pinF = 7;
int pinG = 8;
int D1 = 34 ;
int D2 = 10;
int D3 = 11;
int D4 = 12;
int arr[11];

//manage button
int buttonP = 13;

//setup
void setup(void) {
  //starting up and managing pins again
  digitalWrite(53, HIGH);
  Serial.begin(9600);
  Wire.begin();
  
  //accelerometer stuff
  Wire.beginTransmission(MPU_ADDR); 
  Wire.write(0x6B); 
  Wire.write(0);
  Wire.endTransmission(true);

  //back to pins
  arr[0] = 2;
  arr[1] = 3;
  arr[2] = 4;
  arr[3] = 5;
  arr[4] = 6;
  arr[5] = 7;
  arr[6] = 8;
  arr[7] = 9;
  arr[8] = 10;
  arr[9] = 11;
  arr[10] = 12; 
  pinMode(pinFirst, OUTPUT);
  pinMode(pinA, OUTPUT);     
  pinMode(pinB, OUTPUT);     
  pinMode(pinC, OUTPUT);     
  pinMode(pinD, OUTPUT);     
  pinMode(pinE, OUTPUT);     
  pinMode(pinF, OUTPUT);     
  pinMode(pinG, OUTPUT);   
  pinMode(D1, OUTPUT);  
  pinMode(D2, OUTPUT);  
  pinMode(D3, OUTPUT);  
  pinMode(D4, OUTPUT);  
  pinMode(buttonP, INPUT);

  //setup delay time
  int timea = 50;

  //animation and load up
  while (timea > 10){ 
    reading = analogRead(THERMISTORPIN);
    Serial.print("Analog reading "); 
    Serial.println(reading);
   
    // convert the value to resistance
    reading = (1023 / reading)  - 1;     
    reading = SERIESRESISTOR / reading; 
    Serial.print("Thermistor resistance "); 
    Serial.println(reading);
    int n = reading;
    int num1 = n%10;
    n/=10;
    int num2 = n%10;
    n/=10;
    int num3 = n%10;
    n/=10;
    int num4 = n%10;
  
    seg1(num1);
    delay(timea);
    seg2(num2);
    delay(timea);
    seg3(num3);
    delay(timea);
    seg4(num4);
    delay(timea);
    timea = timea - 5;
  }
}//end of setup

//set up some useful values ahead of time
int val = 0;
bool top = false;

//the main loop
void loop(void) {
  //manage accelerometer
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 7*2, true);
  
  accelerometer_x = Wire.read()<<8 | Wire.read(); 
  accelerometer_y = Wire.read()<<8 | Wire.read();
  accelerometer_z = Wire.read()<<8 | Wire.read(); 
  temperature = Wire.read()<<8 | Wire.read(); 
  gyro_x = Wire.read()<<8 | Wire.read(); 
  gyro_y = Wire.read()<<8 | Wire.read();
  gyro_z = Wire.read()<<8 | Wire.read();

  //figure out y position
  if (accelerometer_y < 0){
    top = true;
    Serial.println("top");
  }
  else{
    top = false;
  }

  //read thermistor
  val = digitalRead(buttonP);
  Serial.println(val); //print out value (unneccesary when in operation, just for testing)
  Serial.println(accelerometer_y);//print out y position (unneccesary when in operation, just for testing)

  //read thermistor
  reading = analogRead(THERMISTORPIN);
  reading = (1023 / reading)  - 1;  //convert thermistor value to readable value
  reading = SERIESRESISTOR / reading; //accounting for resistor
  //Serial.print("Thermistor resistance "); (just for testing)
  //Serial.println(reading); (just for testing)

  //reading broken down into single digits for a 7 segment display  
  int n = reading;
  int timeb = 15;
  int num1 = n%10;
  n/=10;
  int num2 = n%10;
  n/=10;
  int num3 = n%10;
  n/=10;
  int num4 = n%10;
  
  //if button pressed
  while (val != 1){
    //read everything
    Serial.println(gyro_y);
    gyro_x = Wire.read()<<8 | Wire.read();
    gyro_y = Wire.read()<<8 | Wire.read(); 
    gyro_z = Wire.read()<<8 | Wire.read(); 
    //figure out orientation (up or down)
    if (accelerometer_y < 0){
      top = true;
      Serial.println("top");
    }
    else{
      top = false;
    }

    //flash LED (wiring also allows for buzzes from piezo speaker when this is activated)
    digitalWrite(pinFirst, HIGH);
    delay(1);
    digitalWrite(pinFirst, LOW);

    //flip dislay to match orientation (up or down)
    if (top == false){
      seg1(num1);
      delay(timeb);
      seg2(num2);
      delay(timeb);
      seg3(num3);
      delay(timeb);
      seg4(num4);
      delay(timeb); 
    }
    else{
      Aseg4(num1);
      delay(timeb);
      Aseg3(num2);
      delay(timeb);
      Aseg2(num3);
      delay(timeb);
      Aseg1(num4);
      delay(timeb);
    }
    val = digitalRead(buttonP);
  }

  //if button not pressed
  //account for orentation (flip display upside down if the machine is oriented upside down, otherwise leave display as normal orientation)
  if (top == false){
    seg1(num1);
    delay(timeb);
    seg2(num2);
    delay(timeb);
    seg3(num3);
    delay(timeb);
    seg4(num4);
    delay(timeb); 
  }
  else{
    Aseg4(num1);
    delay(timeb);
    Aseg3(num2);
    delay(timeb);
    Aseg2(num3);
    delay(timeb);
    Aseg1(num4);
    delay(timeb);
  }
  
  
}

//7 segment display section 1 (orentation up)
void seg1(int input){
  digitalWrite(D1, LOW);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);
  digitalWrite(arr[0], LOW);
  digitalWrite(arr[1], LOW);
  digitalWrite(arr[2], LOW);
  digitalWrite(arr[3], LOW);
  digitalWrite(arr[4], LOW);
  digitalWrite(arr[5], LOW);
  digitalWrite(arr[6], LOW);
  if (input == 9){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 8){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 7){
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
  }
  else if (input == 6){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 5){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 4){
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 3){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 2){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 1){
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
  }
  else if (input == 0){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
  }
}

//7 segment display section 2 (orentation up)
void seg2(int input){
  digitalWrite(D1, HIGH);
  digitalWrite(D2, LOW);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);
  digitalWrite(arr[0], LOW);
  digitalWrite(arr[1], LOW);
  digitalWrite(arr[2], LOW);
  digitalWrite(arr[3], LOW);
  digitalWrite(arr[4], LOW);
  digitalWrite(arr[5], LOW);
  digitalWrite(arr[6], LOW);
  if (input == 9){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 8){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 7){
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
  }
  else if (input == 6){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 5){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 4){
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 3){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 2){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 1){
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
  }
  else if (input == 0){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
  }
}

//7 segment display section 3 (orentation up)
void seg3(int input){
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, LOW);
  digitalWrite(D4, HIGH);
  digitalWrite(arr[0], LOW);
  digitalWrite(arr[1], LOW);
  digitalWrite(arr[2], LOW);
  digitalWrite(arr[3], LOW);
  digitalWrite(arr[4], LOW);
  digitalWrite(arr[5], LOW);
  digitalWrite(arr[6], LOW);
  if (input == 9){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 8){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 7){
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
  }
  else if (input == 6){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 5){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 4){
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 3){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 2){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 1){
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
  }
  else if (input == 0){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
  }
}

//7 segment display section 4 (orentation up)
void seg4(int input){
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, LOW);
  digitalWrite(arr[0], LOW);
  digitalWrite(arr[1], LOW);
  digitalWrite(arr[2], LOW);
  digitalWrite(arr[3], LOW);
  digitalWrite(arr[4], LOW);
  digitalWrite(arr[5], LOW);
  digitalWrite(arr[6], LOW);
  if (input == 9){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 8){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 7){
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
  }
  else if (input == 6){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 5){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 4){
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 3){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 2){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 1){
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
  }
  else if (input == 0){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
  }
}

//7 segment display section 1 (orentation down)
void Aseg1(int input){
  digitalWrite(D1, LOW);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);
  digitalWrite(arr[0], LOW);
  digitalWrite(arr[1], LOW);
  digitalWrite(arr[2], LOW);
  digitalWrite(arr[3], LOW);
  digitalWrite(arr[4], LOW);
  digitalWrite(arr[5], LOW);
  digitalWrite(arr[6], LOW);
  if (input == 9){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 8){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 7){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
  }
  else if (input == 6){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 5){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 4){
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 3){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 2){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 1){
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
  }
  else if (input == 0){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
  }
}

//7 segment display section 2 (orentation down)
void Aseg2(int input){
  digitalWrite(D1, HIGH);
  digitalWrite(D2, LOW);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);
  digitalWrite(arr[0], LOW);
  digitalWrite(arr[1], LOW);
  digitalWrite(arr[2], LOW);
  digitalWrite(arr[3], LOW);
  digitalWrite(arr[4], LOW);
  digitalWrite(arr[5], LOW);
  digitalWrite(arr[6], LOW);
  if (input == 9){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 8){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 7){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
  }
  else if (input == 6){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 5){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 4){
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 3){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 2){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 1){
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
  }
  else if (input == 0){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
  }
}

//7 segment display section 3 (orentation down)
void Aseg3(int input){
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, LOW);
  digitalWrite(D4, HIGH);
  digitalWrite(arr[0], LOW);
  digitalWrite(arr[1], LOW);
  digitalWrite(arr[2], LOW);
  digitalWrite(arr[3], LOW);
  digitalWrite(arr[4], LOW);
  digitalWrite(arr[5], LOW);
  digitalWrite(arr[6], LOW);
  if (input == 9){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 8){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 7){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
  }
  else if (input == 6){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 5){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 4){
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 3){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 2){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 1){
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
  }
  else if (input == 0){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
  }
}

//7 segment display section 4 (orentation down)
void Aseg4(int input){
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, LOW);
  digitalWrite(arr[0], LOW);
  digitalWrite(arr[1], LOW);
  digitalWrite(arr[2], LOW);
  digitalWrite(arr[3], LOW);
  digitalWrite(arr[4], LOW);
  digitalWrite(arr[5], LOW);
  digitalWrite(arr[6], LOW);
  if (input == 9){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 8){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 7){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
  }
  else if (input == 6){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 5){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 4){
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[5], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 3){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 2){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[6], HIGH);
  }
  else if (input == 1){
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
  }
  else if (input == 0){
    digitalWrite(arr[0], HIGH);
    digitalWrite(arr[1], HIGH);
    digitalWrite(arr[2], HIGH);
    digitalWrite(arr[3], HIGH);
    digitalWrite(arr[4], HIGH);
    digitalWrite(arr[5], HIGH);
  }
}
