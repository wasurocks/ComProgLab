// COUNTDOWN TIMER
#include <M5Stack.h>

#define TONE 262
#define NOTE_B1 262
#define NOTE_Cd1 277
#define NOTE_D1 294
#define NOTE_E1 330
#define NOTE_F1 349
#define NOTE_G1 392

#define TFT_BLACK 0x0000
#define TFT_YELLOW 0xFFE0

int music[] = {
  NOTE_E1, NOTE_E1, NOTE_E1,
  NOTE_E1, NOTE_E1, NOTE_E1,
  NOTE_E1, NOTE_G1, NOTE_Cd1, NOTE_D1, NOTE_E1,
  NOTE_F1, NOTE_F1, NOTE_F1, NOTE_F1,
  NOTE_F1, NOTE_E1, NOTE_E1,
  NOTE_E1, NOTE_D1, NOTE_D1, NOTE_E1, NOTE_D1,
  NOTE_G1
};

uint32_t targetTime = 0;
uint8_t mm = 0;
uint8_t ss = 0;
bool clockRunning = false;

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
  M5.Lcd.drawChar('M', 55, 210, 4);
  M5.Lcd.drawChar('S', 150, 210, 4);
  M5.Lcd.drawString("START", 240, 210, 1);
  M5.Lcd.drawString("STOP", 245, 230, 1);
  drawTime(mm, ss);
}

void loop() {
  // put your main code here, to run rep0.eatedly:
  // testing
  while (clockRunning) {
    // This exits the alarm clock
    if (ss == 0 && mm == 0) {
      mm = 0;
      ss = 0;
      drawTime(mm, ss);
      alarm();
      clockRunning = false;
      break;
    } else if (ss == 0 && mm != 0) {
      drawTime(mm, 00);
      mm--;
      ss = 59;
    } else {
      drawTime(mm, ss);
      ss--;
    }

    if (M5.BtnC.wasPressed()) {
      M5.update();
      clockRunning = false;
      break;
    } else if (M5.BtnC.pressedFor(5000)) {
      M5.update();
      mm = 0;
      ss = 0;
      drawTime(mm, ss);
      clockRunning = false;
      break;
    }

    delay(1000);
    M5.update();
  }

  if (M5.BtnA.wasPressed()) {
    mm++;
    drawTime(mm, ss);
  } else if (M5.BtnA.pressedFor(5000)) {
    mm = 0;
    drawTime(0, ss);
  }
  else if (M5.BtnA.wasReleased()) {
    M5.Speaker.mute();
  }
  if (M5.BtnB.wasPressed()) {
    if (ss < 60) ss++;
    else {
      ss = 0;
      mm++;
    }
    drawTime(mm, ss);
  }
  else if (M5.BtnB.pressedFor(5000)) {
    ss = 0;
    drawTime(mm, 0);
  }
  else if (M5.BtnB.wasReleased()) {
    M5.Speaker.mute();
  }
  if (M5.BtnC.wasPressed()) {
    clockRunning = true;
  }
  M5.update();
}

void drawTime(uint8_t minutes, uint8_t seconds) {
  M5.Lcd.drawString("     ", 10, 80, 8);
  if (minutes < 10) {
    M5.Lcd.drawNumber(0, 30, 80, 8);
    M5.Lcd.drawNumber(minutes, 90, 80, 8);
  }
  else M5.Lcd.drawNumber(minutes, 30, 80, 8);
  M5.Lcd.drawChar(':', 150, 80, 8);
  if (seconds < 10) {
    M5.Lcd.drawNumber(0, 180, 80, 8);
    M5.Lcd.drawNumber(seconds, 240, 80, 8);
  }
  else M5.Lcd.drawNumber(seconds, 180, 80, 8);
}

void alarm() {
  int i = 0;
  while (i < 24
         && !(M5.BtnA.wasPressed()
              || M5.BtnB.wasPressed()
              || M5.BtnC.wasPressed())) {
    M5.Speaker.tone(music[i]);
    delay(200);
    M5.Speaker.tone(-1);
    delay(200);
    i++;
    M5.update();
  }
  M5.Speaker.mute();
}
