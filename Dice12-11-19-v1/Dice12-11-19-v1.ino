 /*
 * Tiny4kOLED - Drivers for SSD1306 controlled dot matrix OLED/PLED 128x32 displays
 *
 * Based on ssd1306xled, re-written and extended by Stephen Denne
 * from 2017-04-25 at https://github.com/datacute/Tiny4kOLED
 *
 */

// INCLUDES
// ============
#include <Tiny4kOLED_TinyWireM.h>

// DEFINITIONS
// ============


// VARIABLES
// ===========
int menuVal = 3;               // Default menu on start
int xPos;                      // Number offset on x
int maxVal;                    // 2 4 6 8 10 12 20 100
long randNumber;


// SETUP
// ===========

void setup() 
{
  randomSeed(analogRead(PB2));
  menu();
  randomNumber();
  oled.begin();                // Send the initialization sequence to the oled. This leaves the display turned off.
  initDisplay();               // Setup the first half of memory.
  oled.switchRenderFrame();    // Switch the half of RAM that we are writing to, to be the half that is non currently displayed.
  initDisplay();               // Setup the second half of memory.
  updateDisplay();             // Call your own display updating code.
  oled.on();                   // Turn on the display.
}

// DISPLAY
// =========
void initDisplay() {
  oled.clear();                // Clear whatever random data has been left in memory.
  oled.setFont(FONT6X8);       // The characters in the 8x16 font are 8 pixels wide and 16 pixels tall.
  oled.setCursor(xPos, 0);     // Position the text cursor
  oled.print(maxVal);          // Write text to oled RAM (which is not currently being displayed).
}

void updateDisplay() {
  oled.setFont(FONT11X16);     // The characters in the 11x16 font are 11 pixels wide and 16 pixels tall.
  oled.setCursor(5,1);         // Position the text cursor
  oled.print(randNumber);      // Write the number of milliseconds since power on.
  oled.switchFrame();          // Swap which half of RAM is being written to, and which half is being displayed.
}

// LOOP
// ===========
uint8_t swMode = 0;
uint8_t swRandom = 0;
void loop()
{
  readButtonMode();
  updateMode();

  readButtonRandom();
  updateRandom();
}

void readButtonMode()
{
  if(digitalRead(PB3)==0)
  {
    swMode=swMode<<1 | 1;
  }
  else if(digitalRead(PB3)==1)
  {
    swMode=swMode<<1 | 0;
  }
}

void updateMode()
{
  if (swMode==0x7f)
  {
    menuVal++;
    if (menuVal == 9)
    {
      menuVal = 1;
    }
    menu();
    initDisplay();
    updateDisplay();
  }
}

void readButtonRandom()
{
  if(digitalRead(PB4)==0)
  {
    swRandom=swRandom<<1 | 1;
  }
  else if(digitalRead(PB4)==1)
  {
    swRandom=swRandom<<1 | 0;
  }
}

void updateRandom()
{
  if (swRandom==0x7f)
  {
    randomNumber();
    initDisplay();
    updateDisplay();
  }
}

long randomNumber()
{
  randNumber = random(0, maxVal)+1;
}

void menu()
{

  switch(menuVal)
  {
    case 1: 
        setPosAndMaxVal(0,2);
        break;
    case 2: 
        setPosAndMaxVal(5,4);
        break;
    case 3: 
        setPosAndMaxVal(10,6);
        break;
    case 4:
        setPosAndMaxVal(15,8);
        break;
    case 5:
        setPosAndMaxVal(25,10);
        break;
    case 6:
        setPosAndMaxVal(35,12);
        break;  
    case 7:
        setPosAndMaxVal(45,20);
        break;
    case 8:
        setPosAndMaxVal(55,100);
        break;
  }
  
}

void setPosAndMaxVal(int pos, int val)
{
  xPos = pos;
  maxVal = val;
}
