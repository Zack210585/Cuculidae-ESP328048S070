//ESP32-S3 Blue Pin 17 RX to ESP32 WROOM Pin 17 TX2
//ESP32-S3 Black Pin 18 TX to ESP32 WROOM Pin 16 RX2
//VCC: Connect to 3.3V on the ESP32.
//GND: Connect to GND on the ESP32.
//SDA: Connect to GPIO 21 SDA
//SCL: Connect to GPIO 22 SCL
#include <Wire.h>
#include <stdint.h>
#include <string.h>
#include <U8g2lib.h> 
#include <Arduino.h>
#include <ESP32Time.h> 
#include <TAMC_GT911.h>
#include <HardwareSerial.h>
#include <Arduino_GFX_Library.h>
#define TFT_BL 2
#define TOUCH_SDA 19
#define TOUCH_SCL 20
#define TOUCH_RST 38
#define BLACK 0x0000
#define WHITE 0xFFFF
#define RED   0xF800
#define GREEN 0x07E0
#define BLUE  0x001F
#define YELLOW 0xFFE0
#define GREY  0x4208
#define QR_MATRIX_SIZE 37
unsigned long ScreenLastUpdate = 0;
float last_h_rad = -1, last_m_rad = -1;
bool ChoiceIsBluetooth = false, IsBTOn = false, NewDay = false;
const char* WeekDays[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const char* MonthOfTheYear[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
int CurrentScreen = 0, SelectedSecond = 0, SelectedMinute = 0, SelectedHour = 10, SelectedDay = 1, SelectedMonth = 1, SelectedYear = 2026, SelectedDST = 0, SelectedTimeZone = 0, Selected1224 = 0, SelectedAMPM = 1, STSecond = 0, STMinute  = 0, STHour = 10, STDay = 1, STMonth = 1, STYear = 2026, STDST = 0, STTimeZone = 0, ST1224 = 0,  STAMPM = 0;
HardwareSerial MySerial(2);
TAMC_GT911 ts = TAMC_GT911(TOUCH_SDA, TOUCH_SCL, -1, TOUCH_RST, 800, 480);
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    41, 40, 39, 42, 14, 21, 47, 48, 45, 9, 46, 3, 8, 16, 1, 15, 7, 6, 5, 4,
    0, 210, 4, 43, 0, 22, 4, 12, 1, 12000000
);
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(800, 480, rgbpanel, 3, true);
void DrawScreenZero() 
{
	gfx->fillScreen(BLACK);
	gfx->setTextColor(WHITE); 
	gfx->setFont(u8g2_font_helvB24_tr);
	gfx->setCursor(20, 60); 
	gfx->print("Setup");
	gfx->setFont(u8g2_font_helvB18_tr);
	int off_x = 40, off_y = 120;
	if (!ChoiceIsBluetooth) 
	{
		gfx->fillRect(off_x, off_y, 180, 80, WHITE);
		gfx->setTextColor(BLACK);
	} 
	else 
	{
		gfx->drawRect(off_x, off_y, 180, 80, WHITE);
		gfx->setTextColor(WHITE);
	}
	gfx->setCursor(off_x + 45, off_y + 52); 
	gfx->print("Manual");
	int on_x = 260, on_y = 120;
	if (ChoiceIsBluetooth) 
	{
		gfx->fillRect(on_x, on_y, 180, 80, WHITE);
		gfx->setTextColor(BLACK);
	} 
	else 
	{
		gfx->drawRect(on_x, on_y, 180, 80, WHITE);
		gfx->setTextColor(WHITE);
	}
	gfx->setCursor(on_x + 35, on_y + 52); 
	gfx->print("Bluetooth");
	gfx->drawRect(260, 720, 180, 60, WHITE);
	gfx->setTextColor(WHITE); 
	gfx->setCursor(305, 762); 
	gfx->print("NEXT");
}
void setup() 
{
	Serial.begin(115200);
	MySerial.begin(115200, SERIAL_8N1, 17, 18);
 	pinMode(TFT_BL, OUTPUT); 
	digitalWrite(TFT_BL, HIGH);
	gfx->begin(); 
	gfx->setUTF8Print(true); 
	Wire.begin(TOUCH_SDA, TOUCH_SCL); 
	ts.begin(); 
	ts.setRotation(ROTATION_INVERTED);
	DrawScreenZero();
}
void SerialReceive() 
{
	if (MySerial.available() > 0) 
	{
		String incomingData = MySerial.readStringUntil('\n');
		incomingData.trim(); 
		if (incomingData.startsWith("ST@")) 
		{
			int parsedFields = sscanf(incomingData.c_str() + 3, "%d@%d@%d@%d@%d@%d@%d@%d@%d@%d", 
			&STSecond, &STMinute, &STHour, &STDay, &STMonth, &STYear, &STDST, &STTimeZone, &ST1224, &STAMPM);
			if (parsedFields == 10) 
			{
			
				SelectedYear = STYear;
				SelectedMonth  = STMonth; 
				SelectedDay  = STDay;
				SelectedHour   = STHour; 
				SelectedMinute  = STMinute;
				SelectedSecond = STSecond;
				SelectedDST = STDST;
				SelectedTimeZone = STTimeZone;
				Selected1224  = ST1224;
				SelectedAMPM  = STAMPM;
				Serial.println("Time Set Received!");
				Serial.printf("Date: %02d/%02d/%04d", STDay, STMonth, STYear);
				Serial.printf("... Time: %02d:%02d:%02d", STHour, STMinute, STSecond);
				Serial.printf("... DST: %d", STDST);
				Serial.printf("... Timezone: %d", STTimeZone);
				Serial.printf("... 12/24: %d", ST1224);
				Serial.printf("... AM/PM: %d\n", STAMPM);
			} 
		}
		else if (incomingData.length() > 0)
		{
			Serial.println(incomingData);
		}
	}
}
void loop() 
{
	if (CurrentScreen == 0) 
	{
		ts.read();
		if (ts.isTouched) 
		{
			int tx = 480 - ts.points[0].y;
			int ty = ts.points[0].x;
			if (tx > 40 && tx < 220 && ty > 120 && ty < 200) 
			{
				if (ChoiceIsBluetooth) 
				{
					ChoiceIsBluetooth = false;
					DrawScreenZero();
				}
			}
			else if (tx > 260 && tx < 440 && ty > 120 && ty < 200) 
			{
				if (!ChoiceIsBluetooth) 
				{
					ChoiceIsBluetooth = true;
					DrawScreenZero();
				}
			}
			else if (ty > 720 && tx > 260 && tx < 440) 
			{
				if (!ChoiceIsBluetooth) 
				{
					MySerial.println("WT?");
					Serial.println("Time Inquiry Sent!");
					CurrentScreen = 1; 
					drawFull();
				} 
				else 
				{
					CurrentScreen = 7; 
					drawFull();
				}
			}
			int uc = 0; 
			while (uc < 5) 
			{ 
				ts.read(); 
				if (!ts.isTouched) uc++; 
				else uc = 0; 
        delay(10); 
			}
		}
	}
	else if ((CurrentScreen >= 1 && CurrentScreen <= 7) || CurrentScreen == 9)
	{
		loopManual();
	}
  else if(CurrentScreen == 8)
  {
    loopManual();
	}
  SerialReceive();
}