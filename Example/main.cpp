#include <Arduino.h>

#include <TM1638GS.h>

// I/O pins on the Arduino connected to strobe, clock, data
// (power should go to 3.3v and GND)
TM1638GS tm(8,9,10);

void doLEDs(uint8_t value);

const char string[] = "IP = 192.168.2.71";
int end = sizeof(string) - 8;
char text[9] =  "        ";


void setup() {
  int x = 0;
  tm.reset();

  tm.displayText("abc");
  tm.setColorLED(0, 1);
  
  delay(2000);
  tm.displayASCII(5, 't');
  tm.displayASCII(6, 'u');
  tm.displayASCII(7, 'y');
  tm.setLED(7, 1);

  x = 0;
  do {
  tm.setColorLED(x, 1);
  tm.setBrightness(x);
  delay(1000);
  x++;
  } while(x<8); 

  tm.setBrightness(2);
  tm.clearLEDs();


  delay(1000);

  tm.displayHex(0, 8);
  tm.displayHex(1, 9);
  tm.displayHex(2, 10);
  tm.displayHex(3, 11);
  tm.displayHex(4, 12);
  tm.displayHex(5, 13);
  tm.displayHex(6, 14);
  tm.displayHex(7, 15);

  delay(2000);

  tm.displayText("buttons ");
}

void loop() {

    int x = 0;

    while (x < end)  {

    const char* pos = string + x;

    //display.setChars(pos, 0);
    tm.displayText(pos);
    if (x == 0) delay(1000);
      else  delay(300);
      x++;
    } 

      delay(4000);
      tm.clear();
      tm.setColorLEDs(0xF0, 0x0F);   //<-- Right half green, left half red
      delay(2000);
      tm.setColorLEDs(0x55, 0x00);   //<-- Right half green, left half red
      delay(2000);
      tm.setColorLEDs(0x00, 0xAA);   //<-- Right half green, left half red
      delay(2000);
      tm.clearLEDs();
      x = 0;
      do {
      tm.setColorLED(x, 1);
      delay(150);
      tm.setColorLED(x, 0);
      x++;
      } while(x<8); 
      
      do {
      x--;  
      tm.setColorLED(x, 2);
      delay(150);
      tm.setColorLED(x, 0);
      } while(x != 0); 
  
      x = 0;
      do {
      tm.setColorLED(x, 3);
      delay(150);
      tm.setColorLED(x, 0);
      x++;
      } while(x<8); 







 
 
      tm.displayText("..Ende..");
      delay(2000);


  uint8_t buttons = tm.readButtons();
  doLEDs(buttons);

}

// scans the individual bits of value
void doLEDs(uint8_t value) {
  for (uint8_t position = 0; position < 8; position++) {
    tm.setLED(position, value & 1);
    value = value >> 1;
  }
}
