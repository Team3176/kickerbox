#include <LiquidCrystal.h>
#include <Servo.h>


Servo motorController;

float mcOutput = 0;
int sliderPort = A0;
int rawSlider = 512;
float percentMotor = 0;
float deadBandLow = 492;
float deadBandHigh =532;
float floatSlider = 0;
int clearEstop = 0;
int seteStop = 1;
int pauseToggle = 2;

enum mode{
  estop=0,
  running=1,
  paused=2
};
mode curMode = estop;

//Declare an LCD, WE are using pins 8,9,4,5,6, and 7 (https://wiki.dfrobot.com/Arduino_LCD_KeyPad_Shield__SKU__DFR0009_#:~:text=The%20arduino%20LCD%20Keypad%20shield,%2C%20right%2C%20down%20and%20left.
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);



void setup()
{
    Serial.begin(9600);


     lcd.begin(16, 2);              // start the library
     lcd.setCursor(0,0);
     lcd.print("LCD BOOT"); // print a simple message
    
    pinMode(2, INPUT);
    //pinMode(4, INPUT); //TODO Relocate
    pinMode(3,INPUT);
    digitalWrite(2,HIGH);
    //digitalWrite(4,HIGH); //TODO Relocate
    digitalWrite(3,HIGH);
    //motorController.attach(9,1000,2000); //TODO RELOCATE
}


void loop()
 { 

  //inputs
  rawSlider = analogRead(sliderPort);
  clearEstop = digitalRead(2);
  //seteStop = digitalRead(4) //TODO relocate
  //******processing*******
  //+Processing Slider Into Motor Percent
  floatSlider = rawSlider;
  if (rawSlider < deadBandLow){
    //If Slider is in the negative range//
    percentMotor = floatSlider*100/492 -100;
  }else if(rawSlider > deadBandHigh){
    // If Slider is in the high range
    percentMotor = floatSlider * 100 /491 -108.4;
  }else{
    //If Slider is in the deadband range
    percentMotor= 0;
  }
  //-Processing Slider Into Motor Percent
  
  //+Processes motor percent into motor outputs+//
  switch(curMode){
  case estop:
    //+Perform Case Actions+//
      //set the  motor output to zero
        mcOutput = 0;
    //+Evaluate Transition
      //If slider in DB, and button pressed
        if(rawSlider<deadBandHigh && rawSlider>deadBandLow && clearEstop == 0){
      //Then set mode = running
              curMode = running;
            }
  break;
  case running:
    //+Perform Case Actions
        //set the motor output to pctMotor
         mcOutput = .9*percentMotor+90;
      //+Evaluate Transition
      //if estop is pressed then set mode=estop
         if (seteStop == 1){
               curMode = estop;
             }
      //if paused is pressed then set mode=paused
      if (pauseToggle == 2){
            curMode = paused;
         }
  break;
  case paused:
    //+Perform Case Actions
      //set motor output to zero
        mcOutput = 0;
    //+Evaluate Transition
      //if estop is pressed then set mode=estop
    if (seteStop == 1){
          curMode = estop;
    }
      //if paused is pressed then set mode=running
    if (pauseToggle == 2){
      curMode = running;
    }
  default:
    //+Perform Case Actions
      //set motor output to zero
        mcOutput = 0;
    //+Evaluate Transition
      //set mode = estop
        curMode = estop;
  break;
  }
 //-Processes motor percent into motor outputs-//

  

  //outputs

 
  motorController.write(mcOutput);


  lcd.setCursor(0,0);
  lcd.print("     HELLO");
  lcd.setCursor(0,1);
  lcd.print("     WORLD");
  
  
  Serial.print("Button ");
  Serial.print(clearEstop);
  Serial.print(" Mode ");
  Serial.print(curMode);
  Serial.print(" The Slider is ");
  Serial.print(rawSlider);
  Serial.print(" The Motor is ");
  Serial.print(percentMotor);
  Serial.print(" The Angle Is "); 
  Serial.print(mcOutput);
  Serial.println(" ");
 }
