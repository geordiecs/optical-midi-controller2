#include <MIDI.h>

#if defined(ARDUINO_SAM_DUE) || defined(SAMD_SERIES)
   /* example not relevant for this hardware (SoftwareSerial not supported) */
   MIDI_CREATE_DEFAULT_INSTANCE();
#else
   #include <SoftwareSerial.h>
   using Transport = MIDI_NAMESPACE::SerialMIDI<SoftwareSerial>;
   int rxPin = 8;
   int txPin = 9;
   SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);
   Transport serialMIDI(mySerial);
   MIDI_NAMESPACE::MidiInterface<Transport> MIDI((Transport&)serialMIDI);
#endif
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 13

int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;//store frequency read by the photodiods

int redColor = 0;
int greenColor = 0;
int blueColor = 0;//store the red, green and blue colors

void setup()
{
   // Setting the outputs for the color sensor
   pinMode(S0, OUTPUT);
   pinMode(S1, OUTPUT);
   pinMode(S2, OUTPUT);
   pinMode(S3, OUTPUT);
   // Setting the sensorOut for the color sensor as an input
   pinMode(sensorOut, INPUT);
  
   // Setting frequency scaling for the color sensor to 100%
   digitalWrite(S0,HIGH);
   digitalWrite(S1,HIGH);

   pinMode(LED_BUILTIN, OUTPUT);
   MIDI.begin(1);                      // Launch MIDI and listen to channel 4
}

void loop()
{
    // Setting RED (R) filtered photodiodes to be read
  {
   digitalWrite(S2,LOW);
   digitalWrite(S3,LOW);
  
  // Reading the output frequency
   redFrequency = pulseIn(sensorOut, LOW);
  // Remaping the value of the RED (R) frequency from 0 to 255
  // You must replace with your own values. Here's an example: 
  // redColor = map(redFrequency, 70, 120, 255,0);
   redColor = map(redFrequency,1150,100,0,127);
  
  // Printing the RED (R) value
   Serial.print(" R = ");
   Serial.print(redColor);
  
  // Setting GREEN (G) filtered photodiodes to be read
   digitalWrite(S2,HIGH);
   digitalWrite(S3,HIGH);
  
  // Reading the output frequency
   greenFrequency = pulseIn(sensorOut, LOW);
  // Remaping the value of the GREEN (G) frequency from 0 to 255
  // You must replace with your own values. Here's an example: 
  // greenColor = map(greenFrequency, 100, 199, 255, 0);
   greenColor = map(greenFrequency, 8000, 127, 0, 127);
  
  // Printing the GREEN (G) value  
   Serial.print(" G = ");
   Serial.print(greenColor);
 
  // Setting BLUE (B) filtered photodiodes to be read
   digitalWrite(S2,LOW);
   digitalWrite(S3,HIGH);
  
  // Reading the output frequency
   blueFrequency = pulseIn(sensorOut, LOW);
  // Remaping the value of the BLUE (B) frequency from 0 to 255
  // You must replace with your own values. Here's an example: 
  // blueColor = map(blueFrequency, 38, 84, 255, 0);
   blueColor = map(blueFrequency, 1000, 74, 0, 127);
  
  // Printing the BLUE (B) value 
   Serial.print(" B = ");
   Serial.print(blueColor);
  }
  // Checks the current detected color and prints
  // a message in the serial monitor
   if(redColor > greenColor && redColor > blueColor){
      Serial.println(" - RED detected!");
   }
   if(greenColor > redColor && greenColor > blueColor){
    Serial.println(" - GREEN detected!");
   }
   if(blueColor > redColor && blueColor > greenColor){
    Serial.println(" - BLUE detected!");
   }
   {
      digitalWrite(LED_BUILTIN, HIGH);
      MIDI.sendNoteOn(redColor, 127, 1);    // Send a Note (pitch 42, velo 127 on channel 1)
      delay(1000);		            // Wait for a second
      MIDI.sendNoteOff(42, 0, 1);     // Stop the note
      digitalWrite(LED_BUILTIN, LOW);
      delay(500);
   }
}