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

const uint32_t SIXTEENTHS = 1000;
uint32_t timeNow;
uint32_t prevTime = 0;

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
   Serial.begin(9600);
}

void loop() {
   digitalWrite(S2,LOW);     // Setting RED (R) filtered photodiodes to be read
   digitalWrite(S3,LOW);
   redFrequency = pulseIn(sensorOut, LOW, 185000);  // Reading the output frequency
   Serial.print("R = ");
   Serial.print(redFrequency);
   redColor = map(redFrequency,32,8,46,66);
   Serial.print(redColor);
   MIDI.sendNoteOn(redColor, 127, 1);
   MIDI.sendNoteOff(redColor, 127, 1);
}
