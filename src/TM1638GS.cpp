//  TM1638GS.cpp

#include "Arduino.h"
#include "TM1638GS.h"

// Register mappings
#define REG_MAX                 0x0F
#define REG_LED_OFFSET          1
#define LED_TO_REG(p)           (REG_LED_OFFSET + (p << 1))


uint8_t STROBE_IO;
uint8_t DATA_IO;
uint8_t CLOCK_IO;

TM1638lite::TM1638lite(uint8_t strobe, uint8_t clock, uint8_t data) {
  STROBE_IO = strobe;
  DATA_IO = data;
  CLOCK_IO = clock;
  pinMode(strobe, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);
  sendCommand(ACTIVATE);
  reset();
}

void TM1638lite::sendCommand(uint8_t value)
{
  digitalWrite(STROBE_IO, LOW);
  shiftOut(DATA_IO, CLOCK_IO, LSBFIRST, value);
  digitalWrite(STROBE_IO, HIGH);
}

void TM1638lite::reset() {
  sendCommand(WRITE_INC); // set auto increment mode
  digitalWrite(STROBE_IO, LOW);
  shiftOut(DATA_IO, CLOCK_IO, LSBFIRST, 0xc0);   // set starting address to 0
  for (uint8_t i = 0; i < 16; i++)
  {
    shiftOut(DATA_IO, CLOCK_IO, LSBFIRST, 0x00);
  }
  digitalWrite(STROBE_IO, HIGH);
}

uint8_t TM1638lite::readButtons()
{
  uint8_t buttons = 0;
  digitalWrite(STROBE_IO, LOW);
  shiftOut(DATA_IO, CLOCK_IO, LSBFIRST, BUTTONS);
  pinMode(DATA_IO, INPUT);

  for (uint8_t i = 0; i < 4; i++)
  {
    uint8_t v = shiftIn(DATA_IO, CLOCK_IO, LSBFIRST) << i;
    buttons |= v;
  }

  pinMode(DATA_IO, OUTPUT);
  digitalWrite(STROBE_IO, HIGH);
  return buttons;
}

void TM1638lite::setBrightness(uint8_t value)
{
  sendCommand(0x88 | (value & 7));
}


void TM1638lite::displayText(String text) {
  uint8_t length = text.length();
  for (uint8_t position = 0; position < 8 && position < length; position++)
	displayASCII(position, text[position]);
}


void TM1638lite::displaySS(uint8_t position, uint8_t value) { // call 7-segment
  sendCommand(WRITE_LOC);
  digitalWrite(STROBE_IO, LOW);
  shiftOut(DATA_IO, CLOCK_IO, LSBFIRST, 0xC0 + (position << 1));
  shiftOut(DATA_IO, CLOCK_IO, LSBFIRST, value);
  digitalWrite(STROBE_IO, HIGH);
}

void TM1638lite::displayASCII(uint8_t position, uint8_t ascii) {
  displaySS(position, ss[ascii]);
}

void TM1638lite::displayHex(uint8_t position, uint8_t hex) {
  displaySS(position, hexss[hex]);
}


void TM1638lite::clear()
{
  for (uint8_t position = 0; position < 8; position++)
	displayASCII(position, 0x20);
}


void TM1638lite::clearLEDs()
{
    for (uint8_t a = REG_LED_OFFSET; a <= REG_MAX; a += 2)
   {       
//        send_data(a, 0x00);
    sendCommand(WRITE_LOC);
    digitalWrite(STROBE_IO, LOW);
    shiftOut(DATA_IO, CLOCK_IO, LSBFIRST, (0xC0 | a));
    shiftOut(DATA_IO, CLOCK_IO, LSBFIRST, 0x00);
    digitalWrite(STROBE_IO, HIGH);
   }
}

void TM1638lite::setColorLED(const uint8_t pos, const uint8_t color)
{
//    send_data(LED_TO_REG(pos), color);
    sendCommand(WRITE_LOC);
    digitalWrite(STROBE_IO, LOW);
    shiftOut(DATA_IO, CLOCK_IO, LSBFIRST, 0xC1 + (pos << 1));
    shiftOut(DATA_IO, CLOCK_IO, LSBFIRST, color & 3);
    digitalWrite(STROBE_IO, HIGH);
}

void TM1638lite::setColorLEDs(uint8_t green, uint8_t red)
{
    for (uint8_t a = REG_LED_OFFSET; a <= REG_MAX; a += 2, green >>= 1, red >>= 1)
  //      send_data(a, (green & 1) | ((red & 1) << 1));
  {
    sendCommand(WRITE_LOC);
    digitalWrite(STROBE_IO, LOW);
    shiftOut(DATA_IO, CLOCK_IO, LSBFIRST, (0xC0 | a));
    shiftOut(DATA_IO, CLOCK_IO, LSBFIRST, (green & 1) | ((red & 1) << 1));
    digitalWrite(STROBE_IO, HIGH);
  }



}

