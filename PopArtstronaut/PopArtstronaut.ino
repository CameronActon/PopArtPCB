/*
  "PopArtstronaut"
  PopArt PCB
  Cameron Acton
*/

#include <Adafruit_NeoPixel.h>
#include <Bounce2.h>
#include "frames.h";

int state = 0; //State of LEDs (which state is currently displayed)

//Inputs and Outputs
Adafruit_NeoPixel strip(24, 0);
Bounce but = Bounce();

//List of colors used in frame animation 
int colors[4][3] {
  {0, 0, 0},        //BLACK 0
  {255, 0, 0},      //RED   1
  {255, 255, 255},  //WHITE 2
  {0, 0, 255},      //BLUE  3
};

//Timer Vars
long sTime;
int interval;

void setup() {
  //Adressing button
  but.attach(1, INPUT);
  but.interval(5);

  //Addressing NeoPixels
  strip.begin();
  strip.clear();
  strip.show();

  sTime = 0; //Start timer with value of 0
}

void loop() {
  but.update();

  if (but.rose()) {
    strip.setBrightness(255); //Update brightness between state
    strip.clear();            //Clear NeoPixels between states
    state = (state + 1) % 4;  //Change states: 0-3
  }

  //Finite State Machine
  switch (state) {
    case 0: off(); break;
    case 1: on(); break;
    case 2: codeRed(); break;
    case 3: crewmate(); break;
  }
}

//Function that turns the board off
void off() {
  strip.clear();
  strip.show();
}

//Function that turns Astronaut faceplate white and arms red & blue
void on() {
  //Faceplate: White
  for (int i = 0; i < strip.numPixels() - 4; i++) {
    strip.setPixelColor(i, 255, 255, 255);
  }

  //Upper Arm: Red
  //Lower Arm: Blue
  strip.setPixelColor(20, 255, 0, 0); 
  strip.setPixelColor(21, 255, 0, 0); 
  strip.setPixelColor(22, 0, 0, 255);
  strip.setPixelColor(23, 0, 0, 255);
  strip.show();
}

// Function that causes the Astronaut faceplate to blink red, mimicing a warning light
void codeRed() {
  static bool intensify = true; //Boolean that controls whether the brightness is increasing or decreasing
  int brightness;               //Brightness value of NeoPixels
  interval = 10;                //Interval between changes in brightness

  //Timer that increases/decreases brightness
  if (millis() - sTime >= interval) {
    if (intensify) {
      brightness += 1;
    } else {
      brightness -= 1;
    }
    sTime = millis();
  }

  if (brightness == 255) {
    intensify = false;
  } else if (brightness == 0) {
    intensify = true;
  }

  strip.setBrightness(brightness);

  for (int i = 0; i < strip.numPixels() - 4; i++) {
    strip.setPixelColor(i, 255, 0, 0);
  }
  strip.show();
}

void crewmate() {
  interval = 500;       //Interval between change in frame

  static int frame = 0; //Current frame of animation
  
  for (int i = 0; i < strip.numPixels(); i++) {
    int pixAddress = pixelMap[i];
    int whichCol = animation[frame][i];

    int r = colors[whichCol][0];
    int g = colors[whichCol][1];
    int b = colors[whichCol][2];

    if (pixAddress != -1) {
      strip.setPixelColor(pixAddress, r, g, b);
    }
  }

  //Timer that controls update of frame 
  if (millis() - sTime >= interval) {
    strip.show();
    frame = (frame + 1) % 5;

    sTime = millis();
  }
}
