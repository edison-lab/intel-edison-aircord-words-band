
#include <iostream>
#include <iomanip>
#include <queue>
#include <stdlib.h>
#include "OLED.h"
#include "systemFunctions.h"
#include "logo.h"

using namespace std;

// ------------------------------------------------------------------------------
// using shared memory map for communication for processes
#include "mmap.h"
struct mmapData* p_mmapData;

// ------------------------------------------------------------------------------
// MODE
enum {
        MODE_STOP = 0,
        MODE_MUSIC,
        MODE_VOICE,
        MODE_INIT
};
const char mode_chars[3] = {';', '*', ':'};
int getMode(char c) {
  switch(c){
  case mode_chars[0]: return MODE_STOP;
  case mode_chars[1]: return MODE_MUSIC;
  case mode_chars[2]: return MODE_VOICE;
  }
};
int mode = 0;

// ------------------------------------------------------------------------------
// data from ultrasonic
#define MYBUFSZ 1024
char mmapStr[MYBUFSZ+1] = {'\0'};
char ResultStr[MYBUFSZ+1] = {'\0'};
int mmapStrArrow = 0;

// ------------------------------------------------------------------------------
// Misc
int posX = 0;
int preposX = 0;
int posY = 0;
int preposY = 0;

// for logo
float stop_x = -104;

// for voice recognition
std::queue<std::string> strQueue;

// ------------------------------------------------------------------------------
void setup() 
{
  mode = MODE_INIT;

  setupMemoryMap();

  Serial.begin(9600);
    
  // init OLED
  OLED_init();

  // display Host Name
  OLED_locate(10,10);
  OLED_write(getHostname().c_str());

  // display Network SSID info
  OLED_locate(10,25);
  OLED_write(getSSID().c_str()); // SSID
  OLED_locate(3,25);

  // display Network IP info
  OLED_locate(10,40);
  OLED_write(getIpAddress().c_str()); // IP
  OLED_locate(3,40);

  // wait
  delay(3000); 
  
  // clear OLED
  OLED_background( 0x0000 );
  OLED_cls();

  {
    // check process
    if(isRunning("decoder")) { Serial.println("killall decoder"); system("killall decoder"); }
    delay(1000);
    if(isRunning("mmap/mmap")) { Serial.println("killall mmap"); system("killall mmap"); }
    delay(1000); 
    
    // kick C programs
    system("/home/root/ultrasonic/decoder plughw:2,0 | /home/root/mmap/mmap &");
  }

} // setup

// ------------------------------------------------------------------------------
int CheckMode()
{
  int m;
  
  if (pthread_mutex_lock(&(p_mmapData->mutex)) != 0) exitError("pthread_mutex_lock");
  
  m = getMode(p_mmapData->mode);
  
  if (pthread_mutex_unlock(&(p_mmapData->mutex)) != 0) exitError("pthread_mutex_unlock");
 
  return m;
}

// ------------------------------------------------------------------------------
void loop() 
{
  OLED_locate(0,0);

  mode = CheckMode();

  switch(mode)
  {
    case MODE_STOP:
    {
      // display Logo (marquee left to right)
      {
        static int stop_count = 100;
        if(stop_x <= 8.99 && stop_x >= 8.0 && stop_count >= 0)
        {
          stop_count --;
          if(stop_count < 0)
          { 
            stop_count = 100;
            stop_x += 0.5; 
          }
        }
        else
        {
          stop_x += 0.5;
        }
        if(stop_x > 128) stop_x = -104;
        int stop_y = 60;

        // set OLED color
        OLED_foreground(0x0415);
        
        // LOGO data transfer 
        OLED_bitblit(stop_x, stop_y, logoWidth, logoHeight, logoData);
        OLED_bitblit(stop_x-230, stop_y, logoWidth, logoHeight, logoData);

      }

      break;
    }
    case MODE_MUSIC:
    {
      DisplayLyrics();
      
      break;
    }
    case MODE_VOICE:
    {
      DisplayVoiceRecog();

      break;
    }
    
  } // switch(mode)

} // loop()
 



