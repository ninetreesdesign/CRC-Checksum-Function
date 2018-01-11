// Make a function to calculate the NMEA type 8-bit CRC code
// - A message string is built
// - The checksum is computed, and formatted to two chars
// - The full string is printed
//   EX1:  $test*   crc = 16
//   EX2:  $GPRMC,023405.00,A,1827.23072,N,06958.07877,W,1.631,33.83,230613,,,A*  crc = 42

// 2017-10-19 streamlined this to be a clearer example, posted to github
// 2018-01-11 renamed functions; add fctn to calculate string length, eliminated globals


void setup() {
    Serial.begin(115200); // Init and set rate for serial output
    // wait for serial port to connect. Needed for Leonardo and Teensyduino 3
    while (!Serial && (millis() < 5000)) {} // include timeout if print console isn't opened

    Serial.println("NMEA CRC Demo");
    // build msg
    char msg_array[100] = "$MSGXY,9.1,25.5*";
    Serial.printf("%s\n", msg_array);

    byte len = stringLength(msg_array);
    byte crc = calculateCRC(msg_array,len);
    // print msg with crc
    Serial.printf("%s%02X\n", msg_array, crc); // crc result must render as two hex characters
}


void loop() {
}


/// Calculate a 1-byte CRC code as used in NMEA messages
byte calculateCRC(char *str, byte str_len) {
    char c;
    byte start_pos = 0;
    byte end_pos = 0;
    byte crc = 0;
    //byte buff_size = calculate here
    for (byte i = 0; i < str_len; i++) {
        c = str[i];
        if(c == '$') {
            start_pos = i;
        }
        if(c == '*') {
            end_pos = i;
        }
    }
    if (end_pos > start_pos) {
        for (byte i = start_pos+1; i < end_pos; i++) { // XOR every character between '$' and '*'
            crc = crc ^ str[i] ;
        }
    }
    else {
        Serial.printf("CRC Error: start:%d > end:%d \n",start_pos, end_pos);
    }
    return crc;

    // based on code by Elimeléc López - July-19th-2013
    // algorithm at http://elimelecsarduinoprojects.blogspot.com/2013/07/nmea-checksum-calculator.html
}


byte stringLength(char* p_str) {
    byte offset = 0;

    //While loop that tests whether the end of the array has been reached
    while (*(p_str + offset) != '\0' && offset < 255) { // check for max
        //advance to the next element of the array
        ++offset;
    }
    return offset;
}


