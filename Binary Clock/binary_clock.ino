#include <Time.h>

#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by Unix time_t as ten ASCII digits
#define TIME_HEADER  'T'   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

/*
  Author: Dane Miller
  Title: Binary Clock
  Description: Simple ditial clock with LEDs used to represent the digits as a binary number.
*/


void setup() {
  // put your setup code here, to run once:
  int hour_1 = 7;
  int hour_2 = 6;
  int hour_4 = 5;
  int hour_8 = 4;
  
  int minute_1 = 13;
  int minute_2 = 12;
  int minute_4 = 11;
  int minute_8 = 10;
  int minute_16 = 9;
  int minute_32= 8;
  
  pinMode(hour_1, OUTPUT);
  pinMode(hour_2, OUTPUT);
  pinMode(hour_4, OUTPUT);
  pinMode(hour_8, OUTPUT);
  
  pinMode(minute_1, OUTPUT);
  pinMode(minute_2, OUTPUT);
  pinMode(minute_4, OUTPUT);
  pinMode(minute_8, OUTPUT);
  pinMode(minute_16, OUTPUT);
  pinMode(minute_32, OUTPUT);

  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  
   if(Serial.available() ) 
  {
    processSyncMessage();
  }
  if(timeStatus() == timeNotSet) 
    Serial.println("waiting for sync message");
  
  time_t t = now(); // Store the current time in time 
                    //  variable t 
  int hours = hour(t); //Get the hour
  hours += -5;         // To switch to local time I am GMT -5 
  hours = hours % 12;  // To switch to 12 hour mode
  if(hours == 0){
     hours += 12;       // This is so Midnight and noon are represented as 12 and not 0
  }
  hours += 16;          // Add an extra outer bit to the binary hour so we dont have to add padding with 0's at the beginning 
  int mins = minute(t) +64; // Add an extra outer bit to the binary minute so we dont have to add padding with 0's at the beginning 
  bool am = isAM();      // To possibly add an extra LED to show whether it is morning or evening
  int i;
  
  String hour_s =  String(hours, BIN);  //Converting the hour to binary
  String mins_s = String(mins, BIN);    // Converting the minutes to binary

  
  for (i = 1; i < 5; i++){

    if(hour_s[i] == '0'){
      digitalWrite(3+i, LOW);  // If the bit is zero the corresponding LED is off 
    } else {
      digitalWrite(3+i, HIGH);   // If the bit is one the corresponding LED is on
    }
  }


  for (i = 1; i < 7; i++){
     if(mins_s[i] == '0'){
      digitalWrite(7+i, LOW);   // If the bit is zero the corresponding LED is off 
    } else {
      digitalWrite(7+ i, HIGH);   // If the bit is one the corresponding LED is on 
    }
  }

}

/*
  This function gets a time from the serial port and sets the arduino time.
  We will be using a python function to send the current time. 
*/
void processSyncMessage() {
  // if time sync available from serial port, update time and return true
  while(Serial.available() >=  TIME_MSG_LEN ){  // time message consists of header & 10 ASCII digits
    char c = Serial.read() ; 
    Serial.print(c);  
    if( c == TIME_HEADER ) {       
      time_t pctime = 0;
      for(int i=0; i < TIME_MSG_LEN -1; i++){   
        c = Serial.read();          
        if( c >= '0' && c <= '9'){   
          pctime = (10 * pctime) + (c - '0') ; // convert digits to a number    
        }
      }   
      setTime(pctime);   // Sync Arduino clock to the time received on the serial port
    }  
  }
}
