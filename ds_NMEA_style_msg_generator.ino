/*
Generate commands and include NMEA-style 1-byte hex CRC.
CRC (Cyclical Redundancy Check) is a checksum added to a message so that receiver can verify it 
*/

#define P   Serial.print
#define Pln Serial.println
const byte buff_size = 90;
char CRCbuffer[buff_size];

void setup()
{
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo & Teensyduino 3
  }

  Serial.println(F("CRC Starting... "));
  Serial.print("CRC test; should be 42: ");  
  Serial.println(convertToCRC("$GPRMC,023405.00,A,1827.23072,N,06958.07877,W,1.631,33.83,230613,,,A*"),HEX);
}

void loop() {
  delay(1);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// NMEA CRC  
// Sample sentences to XOR
// $test*16
// $GPRMC,023405.00,A,1827.23072,N,06958.07877,W,1.631,33.83,230613,,,A*42
byte convertToCRC(char *buff) {
  //void convertToCRC() {
  char c;
  byte start_with = 0;
  byte end_with = 0;
  byte crc = 0;

  for (byte i = 0; i < buff_size; i++) {
    c = buff[i];
    if(c == '$'){
      start_with = i;
    }
    if(c == '*'){
      end_with = i;
    }      
  }
  if (end_with > start_with){
    for (byte i = start_with+1; i < end_with; i++){ // XOR every character between '$' and '*'
      crc = crc ^ buff[i] ;
    }
  }
  else {
    Pln(end_with);
    Pln (start_with);
    Pln("CRC ERROR");
  }
  return crc;
  
  // based on code by Elimeléc López - July-19th-2013
  // algorithm at http://elimelecsarduinoprojects.blogspot.com/2013/07/nmea-checksum-calculator.html

}


