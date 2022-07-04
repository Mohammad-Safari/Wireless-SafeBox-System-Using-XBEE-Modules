# 1 "d:\\mohammad\\Projects\\CE Labs\\MA Lab\\XBEE\\ARDUINO\\client\\client.ino"
# 2 "d:\\mohammad\\Projects\\CE Labs\\MA Lab\\XBEE\\ARDUINO\\client\\client.ino" 2
# 3 "d:\\mohammad\\Projects\\CE Labs\\MA Lab\\XBEE\\ARDUINO\\client\\client.ino" 2
# 4 "d:\\mohammad\\Projects\\CE Labs\\MA Lab\\XBEE\\ARDUINO\\client\\client.ino" 2





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
Keypad in = Keypad(((char*)keys), rowPins, colPins, ROWS, COLS);

// lcd setup
const int rs = 22, en = 23, d4 = 24, d5 = 25, d6 = 26, d7 = 27;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// sign
char sign[22] = {
    "qwertyuiopqwertyuiop" // ,"asdfghjklasdfghjklas","zxcvbnmzxcvbnmzxcvbn"
};

int auth = 0;
unsigned long authT = 0;

void setup()
{
    Serial.begin(9600);
    for (int i = 0; i < 4; i++)
    {
        pinMode(led[i], 0x1);
        digitalWrite(led[i], 0x0);
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
        else if (millis() - authT > 5 * 1000)
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
    if (auth >= 0 && auth < 3)
    {
        lcd_print("you are now logged in", true, 1, 0);
        delay(100);
        lcd_print("other users can ...", true, 1, 0);
        delay(100);
        lcd_print("press # to enter", true, 1, 0);
        switch (++auth)
        {
        case 1:
            digitalWrite(led[0], 0x1);
            break;
        case 2:
            digitalWrite(led[1], 0x1);
            break;
        case 3:
            digitalWrite(led[2], 0x1);
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
    for (int i = 0; i < 3; i++)
    {
        digitalWrite(led[i], 0x0);
    }
}

void userLogout()
{
    if (auth > 0)
    {
        lcd_print("one is logging out", true, 1, 0);
        delay(100);
        digitalWrite(led[--auth], 0x0);
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
