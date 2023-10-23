#include "ST7735_TEE.h"

TEE_ST7735 lcd(9, 10, 11, 12, 13); // Arduino csk, sda, A0, rst, cs

#define BUTTON_CHANGE 5
#define BUTTON_SHOOT 3
#define PIN_ANALOG_X 0
#define PIN_ANALOG_Y 1

int x = 50;
int y = 150;

int x_star[15];
int y_star[15];

int life = 5;
int score = 0;

const int MAX_BULLETS = 5;
const int BULLET_SPEED = 3;
boolean bulletsActive[MAX_BULLETS];
int bulletsX[MAX_BULLETS];
int bulletsY[MAX_BULLETS];

uint16_t currentColor = WHITE;

void setup()
{
  Serial.begin(9600);
  lcd.init(lcd.VERTICAL);
  lcd.fillScreen(BLACK);
  randomSeed(analogRead(0));

  for (int i = 0; i < 15; i++)
  {
    x_star[i] = (int)random(15, 115);
    y_star[i] = (int)random(30, 120);
    lcd.drawString(x_star[i], y_star[i], "*", YELLOW, 1);
  }

  for (int i = 0; i < MAX_BULLETS; i++)
  {
    bulletsActive[i] = false;
  }
}



void loop()
{
  lcd.drawString(x, y, "<-0->", currentColor, 1);

  lcd.drawString(10, 10, ("Life : " + String(life)).c_str(), CYAN, 1);
  lcd.drawString(10, 20, ("Score : " + String(score)).c_str(), RED, 1);

  int x_pos = analogRead(PIN_ANALOG_X);

  if (analogRead(PIN_ANALOG_X) < 341 && x > 0)
  {
    lcd.drawString(x, y, "<-0->", BLACK, 1);
    x = x - 5;
  }
  if (analogRead(PIN_ANALOG_X) > 628 && x < 95)
  {
    lcd.drawString(x, y, "<-0->", BLACK, 1);
    x = x + 5;
  }
  if (analogRead(PIN_ANALOG_Y) < 341 && y < 150)
  {
    lcd.drawString(x, y, "<-0->", BLACK, 1);
    y = y + 5;
  }
  if (analogRead(PIN_ANALOG_Y) > 628 && y > 30)
  {
    lcd.drawString(x, y, "<-0->", BLACK, 1);
    y = y - 5;
  }

  if (digitalRead(BUTTON_CHANGE) == LOW)
  {
    currentColor = random(0x0000, 0xFFFF);
    lcd.drawString(x, y, "<-0->", currentColor, 1);
    delay(600);
  }

  for (int i = 0; i < MAX_BULLETS; i++)
  {
    if (bulletsActive[i])
    {
      lcd.drawString(bulletsX[i], bulletsY[i], "o", BLACK, 1);

      bulletsY[i] -= BULLET_SPEED;

      if (bulletsY[i] < 30)
      {
        bulletsActive[i] = false;
      }
      else
      {
        bool bulletCollided = false;
        for (int j = 0; j < 15; j++)
        {
          if (bulletsX[i] >= x_star[j] - 5 && bulletsX[i] <= x_star[j] + 10 && bulletsY[i] >= y_star[j] && bulletsY[i] <= y_star[j] + 10)
          {
            bulletCollided = true;
            lcd.drawString(x_star[j], y_star[j], "*", BLACK, 1);
            x_star[j] = (int)random(15, 115);
            y_star[j] = (int)random(30, 120);
            lcd.drawString(x_star[j], y_star[j], "*", YELLOW, 1);
            //lcd.drawString(70, 20, ("Score: " + String(score)).c_str(), MAGENTA, 1);
            lcd.drawString(10, 20, ("Score : " + String(score)).c_str(), BLACK, 1);
            score += 10;
          }
        }
        if (bulletCollided)
        {
          bulletsActive[i] = false;
        }
      }
      for (int j = 0; j < 15; j++)
{
    if (x + 15 >= x_star[j] - 10 && x - 15 <= x_star[j] + 10 && y >= y_star[j] - 10 && y <= y_star[j] + 10)
    {
        lcd.drawString(10, 10, ("Life : " + String(life)).c_str(), BLACK, 1);
        life--;
        lcd.fillRect(x, y, 10, 10, BLACK);
        lcd.drawString(x_star[j], y_star[j], "*", BLACK, 1);
        x_star[j] = (int)random(15, 115);
        y_star[j] = (int)random(30, 120);
        lcd.drawString(x_star[j], y_star[j], "*", YELLOW, 1);
        x = 50;
        y = 150;
        lcd.drawString(10, 10, ("Life : " + String(life)).c_str(), YELLOW, 1);
    }
}

      if (bulletsActive[i])
      {
        lcd.drawString(bulletsX[i], bulletsY[i], "o", currentColor, 1);
      }
    }
  }

  if (digitalRead(BUTTON_SHOOT) == LOW)
  {
    for (int i = 0; i < MAX_BULLETS; i++)
    {
      if (!bulletsActive[i])
      {
        bulletsX[i] = x + 15;
        bulletsY[i] = y - 5;
        bulletsActive[i] = true;
        break;
      }
    }
  
    
  if (life == 0)
  {
    lcd.fillScreen(BLACK);
    lcd.drawString(35, 75, "GAME OVER!", RED, 1);
    delay(5000);
    
    life = 5;
    score = 0;
    x = 50;
    y = 150;
    currentColor = WHITE;
    setup();
  }
}
