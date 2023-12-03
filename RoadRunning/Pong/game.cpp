

#include "daisy_seed.h"

#include "daisy.h"
#include "dev/oled_ssd130x.h"
#include <string>

#include "patch_bay.h"

/* DIGITAL INPUT PINS */
#define BUTTON_1_PIN    D25 // pin 32
#define BUTTON_2_PIN    D26 // pin 33
#define ENC_A_PIN       D27 // pin 34
#define ENC_B_PIN       D28 // pin 35
#define ENC_BUTTON_PIN  D29 // pin 36
/* ANALOG INPUT PINS */
#define JOY_X_PIN       A0//A0
#define JOY_Y_PIN       A1//A1

#define BUTTON_UPDATE_RATE  10000.0f
#define ENCODER_UPDATE_RATE 1000.0f       // in 
#define POT_UPDATE_RATE     0.0005f     // in seconds
#define SAMPLE_RATE         48000.0f    // in Hz
using namespace daisy;

Patch_Bay    pa;
//Mydisplay display;
//DaisySeed   hw;

#define HEIGHT 128
#define WIDTH 64

void Patch_Bay::InitDisplay()
{
    /** OLED display configuration */
    /** Configure the Display */
	Mydisplay::Config display_cfg;
	//OledDisplay<SSD130xI2c128x64Driver>::Config display_cfg;

        display_cfg.driver_config.transport_config.i2c_address               = 0x3C;
    display_cfg.driver_config.transport_config.i2c_config.periph         = I2CHandle::Config::Peripheral::I2C_1;
    display_cfg.driver_config.transport_config.i2c_config.speed          = I2CHandle::Config::Speed::I2C_100KHZ;
    display_cfg.driver_config.transport_config.i2c_config.mode           = I2CHandle::Config::Mode::I2C_MASTER;
    display_cfg.driver_config.transport_config.i2c_config.pin_config.scl = {DSY_GPIOB, 8};    display_cfg.driver_config.transport_config.i2c_config.pin_config.sda = {DSY_GPIOB, 9};
    /** OLED_Init_BEGIN **/
    display.Init(display_cfg);
    display.Fill(false);
    display.Update();
    /** OLED Init_END **/
}




int max(int a, int b) {
  if (a > b) {
    return a;
  } else {
    return b;
  }
}

int min (int a, int b) {
  if (a < b) {
    return a;
  } else {
    return b;
  }
}


void drawFastVLine (int16_t x, int16_t y, uint8_t h, uint8_t color)
{
    	
  int end = y+h;
  for (int a = max(0,y); a < min(end,HEIGHT); a++)
  {
pa.display.DrawPixel(x,a,color);
  }
}

void fillRectangle(int16_t x, int16_t y, uint8_t w, uint8_t h, bool on)
{

  for (int16_t i=x; i<x+w; i++)
  {
    drawFastVLine(i, y, h, on);
  }
}

void drawFastHLine(int16_t x, int16_t y, uint8_t w, bool on)
{
  int end = x+w;
  for (int a = max(0,x); a < min(end,WIDTH); a++)
  {
    pa.display.DrawPixel(a,y,on);
  }
}

 void drawRectangle (int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color)
{
  drawFastHLine(x, y, w, color);
  drawFastHLine(x, y+h-1, w, color);
  drawFastVLine(x, y, h, color);
  drawFastVLine(x+w-1, y, h, color);
}









void Patch_Bay::InitButtons()
{
      button1.Init(seed::BUTTON_1_PIN, BUTTON_UPDATE_RATE, daisy::Switch::TYPE_MOMENTARY, daisy::Switch::POLARITY_INVERTED, daisy::Switch::PULL_UP);
    //button1.Init(hw.GetPin(25), 1000);
      //button2.Init(hw.GetPin(26),1000);

   
  button2.Init(seed::BUTTON_2_PIN, BUTTON_UPDATE_RATE, daisy::Switch::TYPE_MOMENTARY, daisy::Switch::POLARITY_INVERTED, daisy::Switch::PULL_UP);
}







void Patch_Bay::Init()
{
    Patch_Bay::InitButtons();

    //Patch_Bay::InitEncoder();
    //Patch_Bay::InitSD();
};