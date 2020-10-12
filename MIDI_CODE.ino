//new code for nymphy MIDI pad
// For ArduinoMega 

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define midichannel 1

//qikoo import below

#define PINS 14
//#define threshold 100
//#define sensitivityCap 800
#define maxPlayTime 30


int threshold[14] = {100,100,100,100,100,100,100,100,100,100,100,100,100,100} //change individual threshold here
int sensitivityCap[14] = {800,800,800,800,800,800,800,800,800,800,800,800,800,800} //change sensi here

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

int pinInput = 0;
bool activePad[PINS] = {false,false,false,false,false,false,false,false,false,false,false,false,false,false};
byte PadNote[14] =       { 52 , 51 , 50 , 49 , 48 , 47 , 46 , 45, 44 , 43, 42, 41 , 40 , 39 };
byte status1;
unsigned long timer[PINS] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int highScore[PINS] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};





int pin_Out_S0 = 2;
int pin_Out_S1 = 3;
int pin_Out_S2 = 4;
int pin_In_Mux1 = A15;
int Mux1_State[4] = {0};

//qikoo import above




void setup(){
  Serial.begin(256000);
   // initialize the LCD
  lcd.begin();
  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.print("New Instrument");
  lcd.setCursor  (0,1);
    delay (500);
  lcd.print  ("ON");


}


//qikoo code below this

void loop() {

  for(int pin = 0; pin < PINS; pin++ ){
    
    pinInput = analogRead(pin);       //input taken from the pin
    
    if(pinInput > threshold[pin] ){
      if(activePad[pin] == false)
      {
        
        if(millis() - timer[pin]  >= maxPlayTime )
        {
          
          float velocity = ((pinInput)/float(sensitivityCap[pin] - threshold[pin]))*127;
        
          if(velocity > 127) velocity = 127;
        
          if(velocity > highScore[pin]) highScore[pin] = velocity;
          
          activePad[pin] = true;
          
          
        }
      }   
    }

    else if(pinInput > threshold[pin]){
      if(activePad[pin] == true){
          
        float velocity = ((pinInput)/float(sensitivityCap[pin] - threshold[pin]))*127;
        
        if(velocity > 127) velocity = 127;
        
        if(velocity > highScore[pin]) highScore[pin] = velocity;
          
      }
    }

    else if (pinInput < threshold[pin]){
      if(activePad[pin] == true ){
          
        MIDI_TX(144, PadNote[pin], highScore[pin]);   //sending signal to midi
          
        highScore[pin] = 0;               //resettting variables
        activePad[pin] = false;
        timer[pin]= millis();
          
      }   
    }
  } 

}


//core qikoo code above this



//functions sending data to midi 
void MIDI_TX(byte MESSAGE, byte PITCH, byte VELOCITY) 
{
  status1 = MESSAGE + midichannel;
  Serial.write(status1);
  Serial.write(PITCH);
  Serial.write(VELOCITY);
}


