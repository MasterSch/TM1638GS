//  TM1638GS.cpp

#include "Arduino.h"
#include "TM1638GS.h"

uint8_t STROBE;
uint8_t DATA;
uint8_t CLOCK;



void TM1638GS::send_Command(uint8_t value)
{
  digitalWrite(STROBE, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, value);
  digitalWrite(STROBE, HIGH);
}


void TM1638GS::send_data(const uint8_t addr, const uint8_t data)
{
  digitalWrite(STROBE, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, FIXED_ADR);
  digitalWrite(STROBE, HIGH);
  digitalWrite(STROBE, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, 0xC0 | addr);
  shiftOut(DATA, CLOCK, LSBFIRST, data);
  digitalWrite(STROBE, HIGH);
}


uint8_t TM1638GS::get_buttons(void)
{
  uint8_t buttons = 0;
  uint8_t x = 0;
  digitalWrite(STROBE, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, BUTTONS);
  pinMode(DATA, INPUT);
  do  {
    uint8_t temp = shiftIn(DATA, CLOCK, LSBFIRST) << x;
    buttons |= temp;
    x++;
  } while (x < 4);
  pinMode(DATA, OUTPUT);
  digitalWrite(STROBE, HIGH);
  return buttons;
}




TM1638GS::TM1638GS(uint8_t strobe, uint8_t clock, uint8_t data) 
{
  STROBE = strobe;
  DATA = data;
  CLOCK = clock;
  pinMode(STROBE, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA, OUTPUT);
  send_Command(ACTIVATE_DISPLAY);
  reset();
}


void TM1638GS::reset() {
  send_Command(AUTO_INC); // set auto increment mode
  digitalWrite(STROBE, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, 0xc0);   // set starting address to 0
  for (uint8_t i = 0; i < 16; i++)
  {
    shiftOut(DATA, CLOCK, LSBFIRST, 0x00);
  }
  digitalWrite(STROBE, HIGH);
}


void TM1638GS::set_Brightness(uint8_t value)
{
  send_Command(0x88 | (value & 7));
}



void TM1638GS::clear_digits()
{
  for (uint8_t position = 0; position < 8; position++)
{
  send_Command(FIXED_ADR );
  digitalWrite(STROBE, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, 0xC0 + (position << 1));
  shiftOut(DATA, CLOCK, LSBFIRST, 0x00);
  digitalWrite(STROBE, HIGH);
}
}


void TM1638GS::clear_LEDs()
{
    for (uint8_t a = 1; a <= 0x0F; a += 2)
   {       
//        send_data(a, 0x00);
    send_Command(FIXED_ADR);
    digitalWrite(STROBE, LOW);
    shiftOut(DATA, CLOCK, LSBFIRST, (0xC0 | a));
    shiftOut(DATA, CLOCK, LSBFIRST, 0x00);
    digitalWrite(STROBE, HIGH);
   }
}


void TM1638GS::set_Digit(const uint8_t position, const uint8_t value)
{
    send_data((position & 0x07) << 1, value);
}

void TM1638GS::set_Hex(uint8_t position, uint8_t hex) {
  hex &= 0x0f;
  if (hex > 9) hex +=7;
  set_Digit(position, ASCII_TABLE[hex + 0x10]);
}

void TM1638GS::set_Char(const uint8_t position, const char value)
{
    uint8_t temp = 0;
    if ((value < 0x20) | (value > 0x7F)) temp = 0x0;
    else temp = ASCII_TABLE[value - 0x20];
    set_Digit(position, temp);
}

void TM1638GS::set_Chars(uint8_t position, const char* value)
{
    while (position < 8) {
    if (*value) set_Char(position++, *value++);
    else break;
    }   
}


void TM1638GS::setColorLED(const uint8_t position, const uint8_t color)
{
//    send_data(LED_TO_REG(pos), color);
    send_Command(FIXED_ADR);
    digitalWrite(STROBE, LOW);
    shiftOut(DATA, CLOCK, LSBFIRST, 0xC1 + (position << 1));
    shiftOut(DATA, CLOCK, LSBFIRST, color & 3);
    digitalWrite(STROBE, HIGH);
}

void TM1638GS::setColorLEDs(uint8_t green, uint8_t red)
{
    for (uint8_t a = 1; a <= 0x0F; a += 2, green >>= 1, red >>= 1)
  //      send_data(a, (green & 1) | ((red & 1) << 1));
  {
    send_Command(FIXED_ADR);
    digitalWrite(STROBE, LOW);
    shiftOut(DATA, CLOCK, LSBFIRST, (0xC0 | a));
    shiftOut(DATA, CLOCK, LSBFIRST, (green & 1) | ((red & 1) << 1));
    digitalWrite(STROBE, HIGH);
  }
}

uint8_t TM1638GS::readButtons()
{
    return get_buttons();
}
