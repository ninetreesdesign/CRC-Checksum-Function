// Demonstrate the use of a function to generate the NMEA 8-bit CRC
// - A message is built and placed into a buffer string
// - The checksum is computed, and formatted to two chars
// - The full string is printed to the IDE monitor
//   EX1:  $test*   crc = 16
//   EX2:  $GPRMC,023405.00,A,1827.23072,N,06958.07877,W,1.631,33.83,230613,,,A*  crc = 42

// 2017-10-19 streamlined this to be a clearer example, posted to github

const byte buff_len = 90;
char CRCbuffer[buff_len];

// create pre-defined strings to control flexible output formatting
String sp = " ";
String delim = ",";
String splat = "*";
String msg = "";

// -----------------------------------------------------------------------
void setup() {
  Serial.begin(115200); // Init and set rate for serial output
  // wait for serial port to connect. Needed for Leonardo and Teensyduino 3
  while (!Serial && (millis() < 5000)) {} // include timeout if print console isn't opened

  // build msg
  Serial.println("NMEA CRC Demo");
  char strX[8];
  char strY[8];
  float x = 10.1;
  float y = 20;
  String cmd = "$XYVALUE";    // a command name
  dtostrf(x, 4, 1, strX);   // format float value to string XX.X
  dtostrf(y, 4, 1, strY);
  msg = cmd + delim + strX + delim + strY + splat;
  outputMsg(msg); // print the entire message string, and append the CRC
}

// -----------------------------------------------------------------------
void loop() {
}

// -----------------------------------------------------------------------
byte convertToCRC(char *buff) {
  // NMEA CRC: XOR each byte with previous for all chars between '$' and '*'
  char c;
  byte i;
  byte start_with = 0;    // index of starting char in msg
  byte end_with = 0;      // index of starting char in msg
  byte crc = 0;

  for (i = 0; i < buff_len; i++) {
    c = buff[i];
    if (c == '$') {
      start_with = i;
    }
    if (c == '*') {
      end_with = i;
    }
  }
  if (end_with > start_with) {
    for (i = start_with + 1; i < end_with; i++) { // XOR every character between '$' and '*'
      crc = crc ^ buff[i] ;  // xor the next char
    }
  }
  else { // else if error, print a msg  
    Serial.println("CRC ERROR");
  }
  return crc;
  
  // based on code by Elimeléc López - July-19th-2013
}

// -----------------------------------------------------------------------
void outputMsg(String msg) {
  msg.toCharArray(CRCbuffer, sizeof(CRCbuffer)); // put complete string into CRCbuffer
  byte crc = convertToCRC(CRCbuffer); // call function to compute the crc value

  Serial.print(msg);
  if (crc < 16) Serial.print("0"); // add leading 0 if needed
  Serial.println(crc, HEX);
}


