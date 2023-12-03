#include "daisy_seed.h"
#include "daisysp.h"
//#include "dev/oled_ssd130x.h"
#include <string>
#include "game.cpp"
#include "patch_bay.h"


using namespace daisy;
using namespace daisysp;

DaisySeed   hw;
Patch_Bay    pb;




void drawField(int score1,int score2);
void collisionControl();
void drawBall();

/* OLED PINS */

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

bool    right = 0, 
        left = 0, 
        up = 0, 
        down = 0;

int     joyReadX = 0, //change these to float, and ADC read will be in float
        joyReadY = 0;

int     encoderVal = 0;



Switch          button1,
                button2;
Parameter       xPos, 
                yPos;
AnalogControl   pot1, 
                pot2;


//variables we will want


// Define the PINS you're goint to use on your Arduino Nano
int controller1 = 2;               // ANALOG 2
int controller2 = 3;               // ANALOG 3
int ledPin = 4;                    // DIGITAL 4
int btnPin = 5;                    // DIGITAL 5

// Define variables
int buttonState = 0;               // HIGH = Pressed
int gameState = 0;                 // 0 = Home, 1 = Game, 2 = End

int controllerValue1 = 0;          // variable to store the value coming from the potentiometer
int controllerValue2 = 0;          // variable to store the value coming from the potentiometer

int paddlePositionPlayer1 = 0;
int paddlePositionPlayer2 = 0;  

int scorePlayer1 = 0;    
int scorePlayer2 = 0;

int ballX = 128/2;      
int ballY = 32/2;
int ballSpeedX = 4;
int ballSpeedY = 2;

#define OLED_RESET 4


 


void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		out[0][i] = in[0][i];
		out[1][i] = in[1][i];
	}
}

int main() {
   // pb.hw.Configure();
	//hw.Init();

 hw.StartLog(false); //start Serial Comm
    //OledDisplay<SSD130xI2c128x64Driver>::Config display_cfg;

   //     display_cfg.driver_config.transport_config.i2c_address               = 0x3C;
    //display_cfg.driver_config.transport_config.i2c_config.periph         = I2CHandle::Config::Peripheral::I2C_1;
    //display_cfg.driver_config.transport_config.i2c_config.speed          = I2CHandle::Config::Speed::I2C_100KHZ;
    //display_cfg.driver_config.transport_config.i2c_config.mode           = I2CHandle::Config::Mode::I2C_MASTER;
    //display_cfg.driver_config.transport_config.i2c_config.pin_config.scl = {DSY_GPIOB, 8};    display_cfg.driver_config.transport_config.i2c_config.pin_config.sda = {DSY_GPIOB, 9};
    /** OLED_Init_BEGIN **/
   // display.Init(display_cfg);

    //display.Fill(false);
    //display.Update();
    /** OLED Init_END **/


    hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.StartAudio(AudioCallback);


//buttons

  /** DIGTAL I/O Init_BEGIN **/
    button1.Init(seed::BUTTON_1_PIN, BUTTON_UPDATE_RATE, daisy::Switch::TYPE_MOMENTARY, daisy::Switch::POLARITY_INVERTED, daisy::Switch::PULL_UP);
    //button1.Init(hw.GetPin(25), 1000);
      //button2.Init(hw.GetPin(26),1000);

   
  button2.Init(seed::BUTTON_2_PIN, BUTTON_UPDATE_RATE, daisy::Switch::TYPE_MOMENTARY, daisy::Switch::POLARITY_INVERTED, daisy::Switch::PULL_UP);


    /** DIGITAL I/O Init_END **/

    /** ANALOG I/O Init_BEGIN **/
   // AdcChannelConfig adcConfig [2];


    const int num_adc_channels = 2;
AdcChannelConfig my_adc_config[num_adc_channels];
// Initialize the first one connected to A0
my_adc_config[0].InitSingle(seed::A0);
// Initialize the second one connected to A4
my_adc_config[1].InitSingle(seed::A1);

hw.adc.Init(my_adc_config, num_adc_channels);
   // adcConfig[0].InitSingle(seed::JOY_X_PIN);
    //adcConfig[1].InitSingle(seed::JOY_Y_PIN);
    //hw.adc.Init(adcConfig, 2);
    hw.adc.Start();
   //pot1.Init(hw.adc.GetPtr(0), SAMPLE_RATE, false, false, POT_UPDATE_RATE);
   // pot1.Init(hw.adc.GetRawValue(0), SAMPLE_RATE, false, false, POT_UPDATE_RATE);
     //pot2.Init(hw.adc.GetRawValue(1), SAMPLE_RATE, false, false, POT_UPDATE_RATE);



   // pot2.Init(hw.adc.GetPtr(1), SAMPLE_RATE, false, false, POT_UPDATE_RATE);
   // xPos.Init(pot1, 0, 128, Parameter::LINEAR);
    //yPos.Init(pot2, 0, 64, Parameter::LINEAR);

  // Read controller value and calculate paddle position
  //controllerValue1 = xPos.Process();
  //controllerValue2 =  yPos.Process();



  

  // Set button state
  //joyReadX = xPos.Process();
     //   joyReadY = yPos.Process();

     //   hw.StartLog(false); //start Serial Comm
 

  
  while(1) {

    int value1 = hw.adc.Get(0);
    int value2 = hw.adc.Get(1);


     paddlePositionPlayer1 = value1 * (46.0 / 65536);
  paddlePositionPlayer2 = value2 * (46.0 / 65536);
         button1.Debounce();
        button2.Debounce();

       // buttonState = button1.Pressed();



		
        if(button1.RisingEdge())
        {
           //hw.PrintLine("%d", button1.Pressed());
           hw.PrintLine("/// BUTTON 1 RSISING EDGE///");
        }



        if(button1.FallingEdge())
        {
           //hw.PrintLine("%d", button1.Pressed());
           hw.PrintLine("/// BUTTON 1 falling EDGE///");
        }


        

  if(button1.Pressed())
        {
           //hw.PrintLine("%d", button1.Pressed());
           hw.PrintLine("/// BUTTON 1 PRESSED///");
        }
        if(button2.RisingEdge())
        {
  hw.PrintLine("/// BUTTON 2///");
        }


        if(button2.RisingEdge())
        {
           //hw.PrintLine("%d", button1.Pressed());
           hw.PrintLine("/// BUTTON 2 RSISING EDGE///");
        }



        if(button2.FallingEdge())
        {
           //hw.PrintLine("%d", button1.Pressed());
           hw.PrintLine("/// BUTTON 2 falling EDGE///");
        }

	//uint32_t now      = System::GetNow();
   // uint32_t log_time = System::GetNow();

 /** This method generates a range based on param.Init() values **/
      



  if (button1.FallingEdge() && gameState == 0) {

      gameState = 1;
   // system.delay(100);
    System::Delay(10);
  } else if (button1.FallingEdge() && (gameState == 1 || gameState == 2)) {
      gameState = 0;
      scorePlayer1 = 0;
      scorePlayer2 = 0;
      ballX = 128/2;
      ballY = 64/2;
   System::Delay(10);
  }
  
  if (gameState == 0) {
   // display.setTextSize(2);
   // display.setTextColor(WHITE);
    //display.setCursor(37, 6);
     hw.PrintLine("gameState=0");
    pb.display.SetCursor(37, 6);
   // display.println("PONG");
    pb.display.WriteString("PONG ", Font_6x8, true);
    //display.setTextSize(1);
    //display.setTextColor(WHITE);
   // display.setCursor(27, 20);
       pb.display.SetCursor(27, 20);
   // display.println("press start");
        pb.display.WriteString("press start ", Font_7x10, true);
    //display.display();
    //display.clearDisplay();    
    pb.display.Update();  
    pb.display.Fill(false);
   
     
  }

  if (gameState == 1) {

    //  hw.PrintLine("gameState=1");
    drawField(scorePlayer1, scorePlayer2);
    
    collisionControl();
    drawBall();
    
   // display.display();
   // display.clearDisplay();
   pb.display.Update();  
     pb.display.Fill(false);
  
  }

  if (gameState == 2) {

      //hw.PrintLine("gameState=2");
    drawField(scorePlayer1, scorePlayer2);

   // display.setTextSize(1);
   // display.setTextColor(WHITE);
    
    if (scorePlayer1 == 2) {
      //display.setCursor(7, 15);
      pb.display.SetCursor(7, 30);
       pb.display.Update();  
    } else if (scorePlayer2 == 2) {
     //display.setCursor(38, 15);
       pb.display.SetCursor(38, 30);
       // display.Update();  
    }
   // display.println("winner!");
   pb.display.Fill(false);
        pb.display.WriteString("Winner!!!", Font_6x8, true);
   // display.display();
    //display.clearDisplay();   
    pb.display.Update();  
    pb.display.Fill(false);
  }
}
}





void drawField(int score1, int score2) {
 // display.fillRect(0, round(paddlePositionPlayer1), 2, 9, 1);
    // display.DrawRect(round(paddlePositionPlayer1),3,2 , 2, true, false);
    fillRectangle(0,round(paddlePositionPlayer1), 2, 9, 1);
  
  // display.fillRect(126, round(paddlePositionPlayer2), 2, 9, 1);
    // display.DrawRect(round(paddlePositionPlayer2),126,2 , 2, true, false);
   fillRectangle(126, round(paddlePositionPlayer2), 2, 9, 1);
  
  


    //SCORE CODE

  //  CONVERT INT SCORE TO STRING
     pb.display.SetCursor(55, 0);
  std::string score1Str = std::to_string(score1);
pb.display.WriteString(score1Str.c_str(), Font_11x18, true);
    pb.display.WriteString(":", Font_6x8, true);
    std::string score2Str = std::to_string(score1);
pb.display.WriteString(score2Str.c_str(), Font_11x18, true);
        

 fillRectangle(63, 12, 1, 5, 1); 

fillRectangle(63, 22, 1, 5, 1);
 
fillRectangle(63, 32, 1, 5, 1);
 
fillRectangle(63, 42, 1, 5, 1);
 
fillRectangle(63, 52, 1, 5, 1);
 
fillRectangle(63, 62, 1, 5, 1);
 
   
        
// hw.PrintLine("RECT drawn");
   //   display.Update();  
   
  // System::Delay(10);
      
}

void collisionControl() {
  //bounce from top and bottom
  if (ballY >= 32 - 2 || ballY <= 0) {
    ballSpeedY *= -1;
  } 

  //score points if ball hits wall behind player
  if (ballX >= 128 - 2 || ballX <= 0) {
    if (ballSpeedX > 0) {
      scorePlayer1++;
    //  hw.PrintLine("%d", scorePlayer1);
      ballX = 128 / 4;
    }
    if (ballSpeedX < 0) {
      scorePlayer2++;
     // hw.PrintLine("%d", scorePlayer1);
      ballX = 128 / 4 * 3;
    }  
    if (scorePlayer1 == 4 || scorePlayer2 == 4) {     //set the score to win
      gameState = 2;
    }
  }

  //bounce from player1
  if (ballX >= 0 && ballX <= 2 && ballSpeedX < 0) {
    if (ballY > round(paddlePositionPlayer1) - 2 && ballY < round(paddlePositionPlayer1) + 9) {
      ballSpeedX *= -1;
    }
  }
  //bounce from player2
  if (ballX >= 128-2-2 && ballX <= 128-2 && ballSpeedX > 0) {
    if (ballY > round(paddlePositionPlayer2) - 2 && ballY < round(paddlePositionPlayer2) + 9) {
      ballSpeedX *= -1;
    }

  }
}

void drawBall() {

 // display.fillRect(ballX, ballY, 2, 2, 1);
fillRectangle(ballX, ballY, 2, 2, 1);

  
  ballX += ballSpeedX;
  ballY += ballSpeedY;
}

