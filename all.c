#include <DHT.h>
#include <LiquidCrystal.h>
#include <Key.h>
#include <Keypad.h>
#define DHTPIN A0
#define DHTTYPE DHT11
#define ALARMPIN A1
LiquidCrystal led(12,13,11,10,9,8);
DHT dht(DHTPIN, DHTTYPE);

char key [4][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

byte colPin[3] = {5, 6, 7};
byte rowPin[4] = {0, 2, 3, 4};
Keypad keypad = Keypad(makeKeymap(key), rowPin, colPin, 4, 3);
bool acceptKey = true;

int limit = 65;
String lcd_input = "";
int input_limit = 0;
int flag = 0;

void clearRow(byte n) {
    byte last = 16 - n;
    lcd.setCursor(n, 1);

    for (byte i = 0; i < last; ++i) {
        lcd.print(" ");
    }
    lcd.setCursor(n, 1);
}

void reset_Pincode() {
    if(flag == 0) {
        acceptKey = false;...
        lcd.clear();
        lcd.print("Enter Newlimit(%:");
        lcd.setCursor(0, 1);
        lcd.print("PIN:");
        lcd.cursor();

        lcd_input = "";
        inputlimit = 0;
        acceptKey = true;
        flag = 1;
    }
    else if(flag == 1)
    {
        acceptKey = false;
        limit = inputlimit;

        lcd.clear();
        lcd.print("Newlimit:");
        lcd.setCursor(0, 1);
        lcd.print(limit);
        lcd.print("%");
        delay(3000);

        flag = 0;
        reset_humidity();
    }
}

void reset_humidity()
{
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Humidity");
    lcd.setCursor(0, 0);
    lcd.print("Temp");

    acceptKey = true;
    lcd_input = "";
    inputlimit = 0;
}

void setup()
{
    lcd.begin(16, 2);
    pinMode(ALARMPIN, OUTPUT);
    lcd.setCursor(0, 1);
    lcd.print("Humidity");
    lcd.setCursor(0, 0);
    lcd.print("Temp");
    Serial.begin(9600);
    dht.begin();
}

void loop()
{
    char key = keypad.getKey();
    if(flag == 0)
    {
        float h = dht.readHumidity();
        float t = dht.readTemperature();
        
        Serial.print("Humidity: ");
        Serial.print(h);
        Serial.print(" %\t");
        Serial.print("Temperature: ");
        Serial.print(t);
        Serial.println(" *C ");

        lcd.setCursor(9, 0);
        lcd.print((t), 2);
        lcd.print((char)0xDF);
        lcd.print("C");
        if(h > limit)
        {
            digitalWrite(ALARMPIN, HIGH);
        }
        else
        {
            digitalWrite(ALARMPIN, LOW);
        }
        delay(1000);
        lcd.setCursor(9, 1);
        lcd.print((h), 2);
        lcd.print("%");
        delay(1000);
        switch(key)
        {
            case '#':
                lcd_input = "";
                inputlimit = 0;
                digitalWrite(ALARMPIN, LOW);
                reset_Pincode();
                break;
        }
    else
    {
        switch(key)
        {
            case '0':
                lcd_input += '0';
                inputlimit = inputlimit * 10;
                lcd.print(0);
                break;
            case '1':
                lcd_input += '1';
                inputlimit = inputlimit * 10 + 1;
                lcd.print(1);
                break;
            case '2':
                lcd_input += '2';
                inputlimit = inputlimit * 10 + 2;
                lcd.print(2);
                break;
            case '3':
                lcd_input += '3';
                inputlimit = inputlimit * 10 + 3;
                lcd.print(3);
                break;
            case '4':
                lcd_input += '4';
                inputlimit = inputlimit * 10 + 4;
                lcd.print(4);
                break;
            case '5':
                lcd_input += '5';
                inputlimit = inputlimit * 10 + 5;
                lcd.print(5);
                break;
            case '6':
                lcd_input += '6';
                inputlimit = inputlimit * 10 + 6;
                lcd.print(6);
                break;
            case '7':
                lcd_input += '7';
                inputlimit = inputlimit * 10 + 7;
                lcd.print(7);
                break;
            case '8':
                lcd_input += '8';
                inputlimit = inputlimit * 10 + 8;
                lcd.print(8);
                break;
            case '9':
                lcd_input += '9';
                inputlimit = inputlimit * 10 + 9;
                lcd.print(9);
                break;
            case '#':
                reset_Pincode();
                break;
            case '*':
                clearRow(4);
                lcd_input = "";
                inputlimit = 0;
                break;
        }
    }
}