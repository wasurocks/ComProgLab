#include <M5Stack.h>
// #include "esp32_digital_led_lib.h"

// This part of the code is for a different version of the M5 stack (with LED):
// strand_t m_sLeds = {.rmtChannel = 0, .gpioNum = 15, .ledType = LED_WS2812B_V3, .brightLimit = 32, .numPixels = 10, .pixels = nullptr, ._stateVars = nullptr};
// Extern means that the data is provided in a different file - the array of the logo bitmap is imported from logo.c
extern unsigned char logo[];
// Specify screen size
int screen_width = 320;
int screen_height = 240;
// Declares variables (uninitialized)
// The location of the pingpong racket
int racket_position;
int racket2_position;
// Set amount in seconds
long interval;
// Set the coordinates of the ball
int ball_x;
int ball_y;
// Ball size specified by its radius
int ball_r;
// Acceleration
int accel_x;
int accel_y;
// The player's score
int score;
// Second player's score
int score2;

/*void ledBar(int R, int G, int B, int M) {
  if ((M < 0) || (M > 12)) return;
  if (M == 11) // right
  {
    for (int i = 0; i < 5; i++)
    {
       m_sLeds.pixels[i] = pixelFromRGBW(R, G, B, 0);
    }
  }
  else if (M == 10) // left
  {
    for (int i = 5; i < 10; i++)
    {
       m_sLeds.pixels[i] = pixelFromRGBW(R, G, B, 0);
    }
  }
  else if (M == 12) // all
  {
    for (int i = 0; i < 10; i++)
    {
       m_sLeds.pixels[i] = pixelFromRGBW(R, G, B, 0);
    }
  }
  else
  {
     m_sLeds.pixels[M] = pixelFromRGBW(R, G, B, 0);
  }
  digitalLeds_updatePixels(&m_sLeds);
  }*/

void start() {
  // Resets the screen to BLACK
  M5.Lcd.fillScreen(0x0000);
  // Set how long an interval is
  interval = 60;
  // Set the ball to be at the screen center
  ball_x = 160;
  ball_y = 120;
  ball_r = 6;
  // Ball starts moving from left top corner to right bottom corner
  accel_x = 5;
  accel_y = 5;
  score = 0;
  score2 = 0;
  // Draws the logo
  M5.Lcd.drawBitmap(0, 0, screen_width, screen_height, (uint16_t *)logo);
  // Set the size of the score text
  M5.Lcd.setTextSize(2);
  // Resets time count
  long previousMillis = 0;
  // Initializes and declares local interval variable
  long interval = 500;
  int color = 0x0000;
  // This while loop is the HOME menu of the game which has a text that alternates colors every interval count of seconds
  while (true)
  {
    unsigned long currentMillis = millis();
    // If the time counter exceeds the limit, reset time count
    if (currentMillis - previousMillis > interval) {
      previousMillis = currentMillis;
      color = (color == 0x7bef) ? 0xffff : 0x7bef;
      M5.Lcd.setCursor(35, 200);
      M5.Lcd.setTextColor(color);
      M5.Lcd.print("press any key to play");
    }
    M5.update();
    if (M5.BtnA.wasPressed() || M5.BtnB.wasPressed() || M5.BtnC.wasPressed()) break;
  }
  // Sets the screen to black and plays a sequence of sounds after the user STARTs the game
  M5.Lcd.fillScreen(0x0000);
  M5.Speaker.tone(800);
  delay(100);
  M5.Speaker.tone(1200);
  delay(100);
  M5.Speaker.mute();
  delay(500);
  M5.Lcd.fillScreen(0xffff);
  M5.Lcd.setCursor(120, 0);
  M5.Lcd.setTextColor(0x0000);
  M5.Lcd.print(0);
  M5.Lcd.setCursor(200, 0);
  M5.Lcd.setTextColor(0x0000);
  M5.Lcd.print(0);
}

// This is the game loop
void game() {
  // Draws the ball with set properties
  M5.Lcd.fillCircle(ball_x, ball_y, ball_r, 0x7bef);
  long previousMillis = 0;
  while (true)
  {
    // Converts the voltage scale?? only for cases where voltage is limited
    int voltage = analogRead(36) * 3400 / 4096;
    int voltage2 = analogRead(35) * 3400 / 4096;
    int percentage = voltage * 100 / 3400;
    int percentage2 = voltage2 * 100 / 3400;
    // Scaling to integer values e.g 0 to 100 to 0 to 10 instead (32 percent becomes 3)
    racket_position = map(percentage, 0, 100, 0, 10);
    racket2_position = map(percentage2, 0, 100, 0, 10);
    // Declare the racket's properties
    int racket_width = 20;
    int racket_height = 40;
    int racket_margin = 10;
    int racket2_width = 20;
    int racket2_height = 40;
    int racket2_margin = 10;
    int x, y, color, x2, y2, color2, racket_x, racket_y, racket2_x, racket2_y;
    // Enables the racket to slide
    for (int i = 0; i < 10; i++)
    {
      // Condition for rendering the racket for both players/sides
      // This is for the left side
      if (i < 5)
      {
        x = 0;
        y = i * (racket_height + racket_margin);
      }
      // This is for the right side;
      else
      {
        x2 = screen_width - racket2_width;
        y2 = (9 - i) * (racket2_height + racket2_margin);
      }
      color = (i == racket_position) ? RED : WHITE;
      color2 = (i == racket2_position) ? RED : WHITE;
      M5.Lcd.fillRect(x, y, racket_width, racket_height, color);
      M5.Lcd.fillRect(x2, y2, racket2_width, racket2_height, color2);
      // ledBar(0, 0, 0, 12);
      // ledBar(255, 0, 0, 9 - racket_position);
      if (i == racket_position)
      {
        racket_x = x;
        racket_y = y;
      }
      if (i == racket2_position)
      {
        racket2_x = x2;
        racket2_y = y2;
      }
    }
    unsigned long currentMillis = millis();
    // This is where the global variable is used
    if (currentMillis - previousMillis > interval) {
      previousMillis = currentMillis;
      // Sets the previous ball position as WHITE, so it disappears
      M5.Lcd.fillCircle(ball_x, ball_y, ball_r, WHITE);
      ball_x += accel_x;
      ball_y += accel_y;
      // Sets new ball position
      M5.Lcd.fillCircle(ball_x, ball_y, ball_r, 0x7bef);
      if (ball_y <= 0)
        accel_y *= -1;
      if (ball_y >= 240)
        accel_y *= -1;
      // Makes the ball bounce off the racket
      if ((ball_x + ball_r >= racket2_x) && (ball_y + ball_r >= racket2_y) && (ball_y - ball_r <= racket2_y + racket2_height))
      {
        accel_x *= -1;
        accel_y *= 1;
        repulse();
      }
      if ((ball_x - ball_r <= racket_x + racket_width) && (ball_y + ball_r >= racket_y) && (ball_y - ball_r <= racket_y + racket_height))
      {
        accel_x *= -1;
        accel_y *= 1;
        repulse();
      }
      // PLAYER 1 WINS
      if (ball_x > screen_width) {
        score++;
  
        
        // Set the ball to be at the screen center
        ball_x = 160;
        ball_y = 120;
        ball_r = 6;
        // Ball starts moving from left top corner to right bottom corner
        accel_x = 5;
        accel_y = 5;
      }
      // PLAYER 2 WINS
      if (ball_x < 0) {
        score2++;
        
        
        // Set the ball to be at the screen center
        ball_x = 160;
        ball_y = 120;
        ball_r = 6;
        // Ball starts moving from left top corner to right bottom corner
        accel_x = -5;
        accel_y = -5;
      }
      p1wins();
      p2wins();
    }
  }
}

// What happens after a rebound
void repulse() {
  // Speeds up the game after each point earned
  interval -= (interval >= 25) ? 5 : 0;
}

void p1wins() {
  M5.Lcd.setCursor(120, 0);
  M5.Lcd.setTextColor(0xffff);
  score == 0? M5.Lcd.print(0): M5.Lcd.print(score - 1);
  M5.Lcd.setCursor(120, 0);
  M5.Lcd.setTextColor(0x0000);
  M5.Lcd.print(score);
}

void p2wins() {
  M5.Lcd.setCursor(200, 0);
  M5.Lcd.setTextColor(0xffff);
  score2 == 0? M5.Lcd.print(0): M5.Lcd.print(score2 - 1);
  M5.Lcd.setCursor(200, 0);
  M5.Lcd.setTextColor(0x0000);
  M5.Lcd.print(score2);
}

void setup() {
  M5.begin();
  pinMode(36, INPUT);
  pinMode(35, INPUT);
  // pinMode(15, OUTPUT);
  // digitalWrite (15, LOW);
  /*
    if (digitalLeds_initStrands(&m_sLeds, 1)) {
    Serial.println("Can't init LED driver().");
    }
    digitalLeds_resetPixels(&m_sLeds);
  */
}

void loop() {
  start();
  game();
}
