#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal.h>

#define EEPROM_I2C_ADDRESS 0b1010000

// keypad setup
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {{'1', '2', '3'},
                         {'4', '5', '6'},
                         {'7', '8', '9'},
                         {'*', '0', '#'}};
byte rowPins[ROWS] = {31, 32, 33, 34};
byte colPins[COLS] = {28, 29, 30};
Keypad in = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// lcd setup
const int rs = 22, en = 23, d4 = 24, d5 = 25, d6 = 26, d7 = 27;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// sign
char sign[22] = "qwertyuiopqwertyuiop";
char adminPass[9];
char clientPass[3][9];

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Wire.begin();
  memset(adminPass, 0, 9);
  memset(clientPass, 0, 27);
  load();
}

void loop()
{
  char c;
  if ((c = in.getKey()) > 0)
  {
    if (c == '*')
    {
      sendGeneralExpiryCaution();
    }
    else
    {
      adminAuth();
    }
  }

  if (Serial.available() > 0)
  {
    String req = Serial.readString();
    if (req.substring(0, 5).equals("auth;"))
    {
      char pass[9] = {'\0'};
      req.substring(5, 13).getBytes((unsigned char *)pass, 9);
      if (strcmp(pass, clientPass[0]) == 0)
        Serial.print(sign);
      else
        sendGeneralExpiryCaution();
    }
  }
  else if (Serial1.available() > 0)
  {
    String req = Serial1.readString();
    if (req.substring(0, 5).equals("auth;"))
    {
      char pass[9] = {'\0'};
      req.substring(5, 13).getBytes((unsigned char *)pass, 9);
      if (strcmp(pass, clientPass[1]) == 0)
        Serial1.print(sign);
      else
        sendGeneralExpiryCaution();
    }
  }
  else if (Serial2.available() > 0)
  {
    String req = Serial2.readString();
    if (req.substring(0, 5).equals("auth;"))
    {
      char pass[9] = {'\0'};
      req.substring(5, 13).getBytes((unsigned char *)pass, 9);
      if (strcmp(pass, clientPass[2]) == 0)
        Serial2.print(sign);
      else
        sendGeneralExpiryCaution();
    }
  }
}

void adminAuth()
{
  char pass[9] = {'\0'};
  lcd_print("admin mode pass:", true, 1, 0);
  for (int c = 0; c < 8; c++)
  {
    char ch = in.waitForKey();
    if (ch == '*')
    {
      lcd_print("admin mode exit..", true, 1, 0);
      delay(100);
      lcd_print("", true, 1, 0);
      return;
    }
    else if (ch == '#')
    {
      c = -1;
      lcd_print("input reset..", true, 1, 0);
      delay(100);
      lcd_print("", true, 1, 0);
      delay(100);
      lcd_print("admin mode pass:", true, 1, 0);
      continue;
    }
    else
    {
      pass[c] = ch;
    }
  }
  if (strcmp(pass, adminPass) == 0)
  {
    adminOps();
  }
  else
  {
    lcd_print("wrong admin pass..", true, 1, 0);
    delay(100);
    lcd_print("", true, 1, 0);
  }
}

void adminOps()
{
  char pass[9] = {'\0'};
  lcd_print("which pass is selected to [re]set", true, 1, 0);
  char p;
  do
  {
    p = in.waitForKey();
    if (p == '*')
    {
      lcd_print("admin op exit..", true, 1, 0);
      delay(100);
      lcd_print("", true, 1, 0);
      return;
    }
    else if (p == '#')
    {
      lcd_print("input reset..", true, 1, 0);
      delay(100);
      lcd_print("", true, 1, 0);
      continue;
    }
  } while (p > '4' || p < '0');
  lcd_print("enter new pass:", true, 1, 0);
  for (int c = 0; c < 8; c++)
  {
    char ch = in.waitForKey();
    if (ch == '*')
    {
      lcd_print("pass reset exit..", true, 1, 0);
      delay(100);
      lcd_print("", true, 1, 0);
      return;
    }
    else if (ch == '#')
    {
      c = -1;
      lcd_print("input reset..", true, 1, 0);
      delay(100);
      lcd_print("", true, 1, 0);
      continue;
    }
    else
    {
      pass[c] = ch;
    }
  }
  if (p == 0)
    strcpy(adminPass, pass);
  else
    strcpy(clientPass[p], pass);
  save();
}

void save()
{
  Wire.beginTransmission(EEPROM_I2C_ADDRESS);

  Wire.write(0);
  Wire.write(0);

  for (int i = 0; i < 8; i++)
    Wire.write((uint8_t)(adminPass[i] - '0'));

  for (int k = 0; k < 3; k++)
    for (int i = 0; i < 8; i++)
      Wire.write((uint8_t)(clientPass[k][i] - '0'));

  Wire.endTransmission();
  delay(20);
}

void load()
{
  Wire.beginTransmission(EEPROM_I2C_ADDRESS);

  Wire.write(0);
  Wire.write(0);

  Wire.endTransmission();

  Wire.requestFrom(EEPROM_I2C_ADDRESS, (uint8_t)32);

  for (int i = 0; i < 8; i++)
    adminPass[i] = Wire.read() + '0';
  adminPass[9] = '\0';

  for (int k = 0; k < 3; k++)
  {
    for (int i = 0; i < 8; i++)
      clientPass[k][i] = Wire.read() + '0';
    clientPass[k][9] = '\0';
  }
}

void lcd_print(String s, bool clean, int line, int offset)
{
  if (clean)
    lcd.clear();
  lcd.setCursor(0 + offset, line + 1);
  lcd.print(s);
}

void sendGeneralExpiryCaution()
{
  Serial.print("expireAuth;");
  Serial1.print("expireAuth;");
  Serial2.print("expireAuth;");
  lcd_print("all locked", true, 0, 0);
  delay(100);
  lcd_print("", true, 0, 0);

}