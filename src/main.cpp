/*
  TM1638GS.h - Library for TM1638
  V1.0 
*/

//#ifndef TM1638GS_h
//#define TM1638GS_h

#include "Arduino.h"

#define ACTIVATE_DISPLAY 0x8B
#define BUTTONS 0x42
#define FIXED_ADR 0x44
#define AUTO_INC 0x40


class TM1638GS
{
  public:
    void send_Command(uint8_t value);
    void send_data(const uint8_t addr, const uint8_t data);
    uint8_t get_buttons(void);
    TM1638GS(uint8_t strobe, uint8_t clock, uint8_t data);
    void reset();
    void set_Brightness(uint8_t value);
    void clear_digits();       // Clear the 7-segment displays (only)
    void set_Char(const uint8_t position, const char value);
    void set_Chars(uint8_t position, const char* value);
    void clear_LEDs();   // Turn off all LEDs
    void set_Digit(const uint8_t position, const uint8_t value);
    // Set an LED at the given position to the specified color.
    // LEDs are numbered 0-7, left to right
    void setColorLED(const uint8_t pos, const uint8_t color);
    // Bitmask setting up the color of all LEDs at once.
    // Examples:
    //    setColorLEDs(0xFF, 0x00);   <-- All green
    //    setColorLEDs(0x00, 0xFF);   <-- All red
    //    setColorLEDs(0xFF, 0xFF);   <-- All green+red
    //    setColorLEDs(0xF0, 0x0F);   <-- Right half green, left half red
    void setColorLEDs(uint8_t green, uint8_t red);
    uint8_t readButtons();


};

// map of ASCII values to 7-segment

  const uint8_t ASCII_TABLE[98] = {
  0x00, // (space)   
  0x86, // !   
  0x22, // "   
  0x7E, // #   
  0x6D, // $   
  0xD2, // %   
  0x46, // &   
  0x20, // '   
  0x29, // (   
  0x0B, // )   
  0x21, // *   
  0x70, // +   
  0x10, // ,   
  0x40, // -   
  0x80, // .   
  0x52, // /   
  0x3F, // 0   
  0x06, // 1   
  0x5B, // 2   
  0x4F, // 3   
  0x66, // 4   
  0x6D, // 5   
  0x7D, // 6   
  0x07, // 7   
  0x7F, // 8   
  0x6F, // 9   
  0x09, // :   
  0x0D, // ;   
  0x61, // <   
  0x48, // =   
  0x43, // >   
  0xD3, // ?   
  0x5F, // @   
  0x77, // A   
  0x7C, // B   
  0x39, // C   
  0x5E, // D   
  0x79, // E   
  0x71, // F   
  0x3D, // G   
  0x76, // H   
  0x30, // I   
  0x1E, // J   
  0x75, // K   
  0x38, // L   
  0x15, // M   
  0x37, // N   
  0x3F, // O   
  0x73, // P   
  0x6B, // Q   
  0x33, // R   
  0x6D, // S   
  0x78, // T   
  0x3E, // U   
  0x3E, // V   
  0x2A, // W   
  0x76, // X   
  0x6E, // Y   
  0x5B, // Z   
  0x39, // [   
  0x64, // "\"   
  0x0F, // ]   
  0x23, // ^   
  0x08, // _   
  0x02, // `   
  0x5F, // a   
  0x7C, // b   
  0x58, // c   
  0x5E, // d   
  0x7B, // e   
  0x71, // f   
  0x6F, // g   
  0x74, // h   
  0x10, // i   
  0x0C, // j   
  0x75, // k   
  0x30, // l   
  0x14, // m   
  0x54, // n   
  0x5C, // o   
  0x73, // p   
  0x67, // q   
  0x50, // r   
  0x6D, // s   
  0x78, // t   
  0x1C, // u   
  0x1C, // v   
  0x14, // w   
  0x76, // x   
  0x6E, // y   
  0x5B, // z   
  0x46, // {   
  0x30, // |   
  0x70, // }   
  0x01, // ~   
  0x00, // (del)   
};
//#endif


//  TM1638GS.cpp

//#include "Arduino.h"
//#include "TM1638GS.h"

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
  pinMode(strobe, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);
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






//#include <Arduino.h>

int x;


void setup() {
  // put your setup code here, to run once:

x = 1;

}

void loop() {
  // put your main code here, to run repeatedly:

while (x)  {
delay(10);
}

}