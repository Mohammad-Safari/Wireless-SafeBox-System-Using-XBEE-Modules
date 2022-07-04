#include <Arduino.h>
#line 1 "d:\\mohammad\\Projects\\CE Labs\\MA Lab\\XBEE\\ARDUINO\\client\\client.ino"
#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal.h>

#define MAX_USERS 3
#define AUTH_EXP_PERIOD 5


// led pins
const int led[4] = {35, 36, 37};

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
char sign[22] = {
    "qwertyuiopqwertyuiop" // ,"asdfghjklasdfghjklas","zxcvbnmzxcvbnmzxcvbn"
};

int auth = 0;
unsigned long authT = 0;

#line 35 "d:\\mohammad\\Projects\\CE Labs\\MA Lab\\XBEE\\ARDUINO\\client\\client.ino"
void setup();
#line 46 "d:\\mohammad\\Projects\\CE Labs\\MA Lab\\XBEE\\ARDUINO\\client\\client.ino"
void loop();
#line 85 "d:\\mohammad\\Projects\\CE Labs\\MA Lab\\XBEE\\ARDUINO\\client\\client.ino"
int userAuth();
#line 138 "d:\\mohammad\\Projects\\CE Labs\\MA Lab\\XBEE\\ARDUINO\\client\\client.ino"
void userOps();
#line 167 "d:\\mohammad\\Projects\\CE Labs\\MA Lab\\XBEE\\ARDUINO\\client\\client.ino"
void expireAuth();
#line 176 "d:\\mohammad\\Projects\\CE Labs\\MA Lab\\XBEE\\ARDUINO\\client\\client.ino"
void userLogout();
#line 191 "d:\\mohammad\\Projects\\CE Labs\\MA Lab\\XBEE\\ARDUINO\\client\\client.ino"
void lcd_print(String s, bool clean, int line, int offset);
#line 35 "d:\\mohammad\\Projects\\CE Labs\\MA Lab\\XBEE\\ARDUINO\\client\\client.ino"
void setup()
{
    Serial.begin(9600);
    for (int i = 0; i < 4; i++)
    {
        pinMode(led[i], OUTPUT);
        digitalWrite(led[i], LOW);
    }
    auth = 0;
}

void loop()
{
    if (auth == 0)
    {
        lcd_print("press any key to start..", true, 1, 0);
        delay(10);
        if (in.getKey() > 0)
        {
            if (userAuth() != -1)
            {
                authT = millis();
                userOps();
            }
        }
    }
    else
    {
        if (Serial.available())
        {
            // if (Serial.readString().substring(0, 11).equals("expireAuth;"))
            // {
                expireAuth();
            // }
        }
        else if (millis() - authT > AUTH_EXP_PERIOD * 1000)
        {
            expireAuth();
        }
        else if (in.getKey() == '#')
        {
            userOps();
        }
        else if (in.getKey() == '*')
        {
            userLogout();
        }
    }
}

int userAuth()
{
    char pass[9] = {'\0'};
    lcd_print("enter pass:", true, 1, 0);
    for (int c = 0; c < 8; c++)
    {
        char ch = in.waitForKey();
        if (ch == '*')
        {
            lcd_print("user login exit..", true, 1, 0);
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
            lcd_print("enter pass:", true, 1, 0);
            continue;
        }
        else
        {
            pass[c] = ch;
        }
    }
    Serial.write("auth;");
    Serial.flush();
    delay(10);
    Serial.write(pass);
    Serial.flush();
    while (Serial.available() == 0)
        lcd_print("please wait", false, 1, 0);
    String serverSign = Serial.readString();
    if (serverSign.substring(0, 21).equals(sign))
    {
        return 0;
    }
    else
    {
        lcd_print("wrong pass..", true, 1, 0);
        delay(100);
        lcd_print("all users must re-auth", true, 1, 0);
        delay(100);
        lcd_print("", true, 1, 0);
        expireAuth();
    }
    return -1;
}

void userOps()
{
    if (auth >= 0 && auth < MAX_USERS)
    {
        lcd_print("you are now logged in", true, 1, 0);
        delay(100);
        lcd_print("other users can ...", true, 1, 0);
        delay(100);
        lcd_print("press # to enter", true, 1, 0);
        switch (++auth)
        {
        case 1:
            digitalWrite(led[0], HIGH);
            break;
        case 2:
            digitalWrite(led[1], HIGH);
            break;
        case 3:
            digitalWrite(led[2], HIGH);
            break;
        }
    }
    else
    {
        lcd_print("maximum users are served", true, 1, 0);
        delay(100);
    }
}

void expireAuth()
{
    auth = 0;
    for (int i = 0; i < MAX_USERS; i++)
    {
        digitalWrite(led[i], LOW);
    }
}

void userLogout()
{
    if (auth > 0)
    {
        lcd_print("one is logging out", true, 1, 0);
        delay(100);
        digitalWrite(led[--auth], LOW);
    }
    else
    {
        lcd_print("no users is logged in", true, 1, 0);
        delay(100);
    }
}

void lcd_print(String s, bool clean, int line, int offset)
{
    if (clean)
        lcd.clear();
    lcd.setCursor(0 + offset, line + 1);
    lcd.print(s);
}

