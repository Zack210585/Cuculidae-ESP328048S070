int getDayOfWeek(int y, int m, int d) //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
	int m_calc = m + 1;
	int y_calc = y;
	if (m_calc < 3) 
	{
		m_calc += 12;
		y_calc--; 
	}
	return (d + (13 * (m_calc + 1)) / 5 + y_calc + y_calc / 4 - y_calc / 100 + y_calc / 400 + 6) % 7;
}
int getMaxDays() //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
	if (SelectedMonth == 2) return ((SelectedYear % 4 == 0 && SelectedYear % 100 != 0) || (SelectedYear % 400 == 0)) ? 29 : 28;
	if (SelectedMonth == 4 || SelectedMonth == 6 || SelectedMonth == 9 || SelectedMonth == 11) return 30;
	return 31;
}
void drawLine(int x1, int y1, int x2, int y2, int weight, uint16_t color) 
{
	int dx = abs(x2 - x1), dy = abs(y2 - y1);
	int sx = (x1 < x2) ? 1 : -1, sy = (y1 < y2) ? 1 : -1;
	int err = dx - dy;
	while (true) 
	{
		gfx->fillCircle(x1, y1, weight / 2, color);
		if (x1 == x2 && y1 == y2) break;
		int e2 = 2 * err;
		if (e2 > -dy) 
		{ 
			err -= dy;
			x1 += sx; 
		}
		if (e2 < dx) 
		{ 
			err += dx;
			y1 += sy; 
		}
	}
}
void drawGearIcon(int x, int y, int size, uint16_t color) 
{
	int cx = x + size/2, cy = y + size/2;
	int r_outer = size/2 - 5;
	int tooth_length = 6; 
	for (int i = 0; i < 360; i += 45) 
	{
		float a = i * (3.14159265f / 180.0f);
		float a_tip_left  = a - 0.12f;
		float a_tip_right = a + 0.12f;
		float a_base_left  = a - 0.22f;
		float a_base_right = a + 0.22f;
		int x_t1 = cx + (r_outer + tooth_length) * sin(a_tip_left);
		int y_t1 = cy - (r_outer + tooth_length) * cos(a_tip_left);
		int x_t2 = cx + (r_outer + tooth_length) * sin(a_tip_right);
		int y_t2 = cy - (r_outer + tooth_length) * cos(a_tip_right);
		int x_b1 = cx + r_outer * sin(a_base_left);
		int y_b1 = cy - r_outer * cos(a_base_left);
		int x_b2 = cx + r_outer * sin(a_base_right);
		int y_b2 = cy - r_outer * cos(a_base_right);
		gfx->fillTriangle(x_t1, y_t1, x_b1, y_b1, x_b2, y_b2, color);
		gfx->fillTriangle(x_t1, y_t1, x_t2, y_t2, x_b2, y_b2, color);
	}
	gfx->fillCircle(cx, cy, r_outer, color);
	gfx->fillCircle(cx, cy, size/4, BLACK);
}
void drawBluetoothIcon(int x, int y, int size, uint16_t color) 
{
    int cx = x + size / 2;
    int cy = y + size / 2;
    int topY    = cy - 13; 
    int bottomY = cy + 13;
    int midY    = cy;
    int rightX  = cx + 7;
    int leftX   = cx - 7;
    auto drawLine = [&](int x1, int y1, int x2, int y2) {
        gfx->drawLine(x1, y1, x2, y2, color);
        if (abs(x2 - x1) > abs(y2 - y1)) {
            gfx->drawLine(x1, y1 + 1, x2, y2 + 1, color); 
			} else {
            gfx->drawLine(x1 + 1, y1, x2 + 1, y2, color); 
		}
	};
    gfx->drawLine(cx - 1, topY, cx - 1, bottomY, color);
    gfx->drawLine(cx,     topY, cx,     bottomY, color);
    gfx->drawLine(cx + 1, topY, cx + 1, bottomY, color);
    drawLine(cx, topY,    rightX, cy - 6);
    drawLine(cx, midY,    rightX, cy - 6);
    drawLine(cx, midY,    rightX, cy + 6);
    drawLine(cx, bottomY, rightX, cy + 6);
    drawLine(leftX, cy - 7, cx, midY);
    drawLine(leftX, cy + 7, cx, midY);
}
void drawDSTIcon(int x, int y, int size, uint16_t color) 
{
    int cx = x + size / 2;
    int cy = y + size / 2;
    int sx = cx - 5;
    int sy = cy - 5;
    int r_sun = 8;
    for (int i = 0; i < 360; i += 45) {
        float rad = i * 0.0174533f;
        int x1 = sx + r_sun * sin(rad);
        int y1 = sy - r_sun * cos(rad);
        int x2 = sx + (r_sun + 5) * sin(rad);
        int y2 = sy - (r_sun + 5) * cos(rad);
        gfx->drawLine(x1, y1, x2, y2, color);
        gfx->drawLine(x1 + 1, y1, x2 + 1, y2, color);
	}
    gfx->fillCircle(sx, sy, r_sun - 1, color);
    gfx->fillCircle(sx, sy, r_sun - 3, BLACK); 
    int clock_x = cx + 5;
    int clock_y = cy + 5;
    int r_clock = 10;
    gfx->fillCircle(clock_x, clock_y, r_clock + 2, BLACK);
    gfx->drawCircle(clock_x, clock_y, r_clock, color);
    gfx->drawCircle(clock_x, clock_y, r_clock - 1, color);
    gfx->fillCircle(clock_x, clock_y, 2, color);
    gfx->drawLine(clock_x, clock_y, clock_x, clock_y - 6, color);   
    gfx->drawLine(clock_x, clock_y, clock_x + 1, clock_y - 6, color); 
    gfx->drawLine(clock_x, clock_y, clock_x + 6, clock_y, color);    
    gfx->drawLine(clock_x, clock_y, clock_x + 6, clock_y + 1, color); 
    gfx->drawLine(clock_x - r_clock - 1, clock_y, clock_x - r_clock + 1, clock_y, BLACK);
    gfx->drawLine(clock_x + r_clock - 1, clock_y, clock_x + r_clock + 1, clock_y, BLACK); 
    gfx->fillTriangle(clock_x + 6,  clock_y - 11, clock_x + 11, clock_y - 8, clock_x + 6,  clock_y - 5,  color);
    gfx->fillTriangle(clock_x - 6,  clock_y + 11, clock_x - 11, clock_y + 8, clock_x - 6,  clock_y + 5,  color);
}
void drawOptionsIcon(int x, int y, int size, uint16_t color) 
{
    int cx = x + size / 2;
    int cy = y + size / 2;
    auto drawLine = [&](int x1, int y1, int x2, int y2) {
        gfx->drawLine(x1, y1, x2, y2, color);
        if (x1 == x2) {
            gfx->drawLine(x1 + 1, y1, x2 + 1, y2, color); 
			} else {
            gfx->drawLine(x1, y1 + 1, x2, y2 + 1, color); 
		}
	};
    int rayStart = 11;
    int rayEnd = 15;
    drawLine(cx, cy - rayEnd, cx, cy - rayStart);
    drawLine(cx, cy + rayStart, cx, cy + rayEnd);
    drawLine(cx - rayEnd, cy, cx - rayStart, cy);
    drawLine(cx + rayStart, cy, cx + rayEnd, cy);
    drawLine(cx - 11, cy - 11, cx - 8, cy - 8);
    drawLine(cx + 8, cy - 8, cx + 11, cy - 11);
    drawLine(cx - 11, cy + 11, cx - 8, cy + 8);
    drawLine(cx + 8, cy + 8, cx + 11, cy + 11);
    gfx->drawCircle(cx, cy, 9, color);
    gfx->drawCircle(cx, cy, 8, color);
    gfx->fillCircle(cx, cy, 6, BLACK); 
    for (int i = -6; i <= 6; i++) {
        int w = sqrt(36 - (i * i)); 
        if (w > 0) {
            gfx->drawLine(cx, cy + i, cx + w, cy + i, color);
		}
	}
    gfx->drawLine(cx, cy - 6, cx, cy + 6, color);
}
void drawNotificationIcon(int x, int y, int size, uint16_t color) 
{
    int cx = x + size / 2;
    int cy = y + size / 2;
    gfx->fillCircle(cx, cy - 11, 3, color);
    gfx->fillCircle(cx, cy - 11, 1, BLACK);
    gfx->fillCircle(cx, cy - 3, 8, color); 
    gfx->fillRect(cx - 8, cy - 3, 16, 9, color); 
    int rim_y = cy + 5;
    int rim_height = 3;
    gfx->fillRect(cx - 11, rim_y, 22, rim_height, color); 
    gfx->fillTriangle(cx - 11, rim_y, cx - 11, rim_y + rim_height, cx - 8, rim_y, color); // Left flare
    gfx->fillTriangle(cx + 11, rim_y, cx + 11, rim_y + rim_height, cx + 8, rim_y, color); // Right flare
    gfx->fillCircle(cx, cy + 10, 3, color);
}
void drawDashboard(bool full) 
{
	float aspect = 0.92;      
	int r = 220, cx = 240, cy = 578; 
	if (full || (STMinute == 0 && NewDay))
	{
		NewDay = false;
		gfx->fillRect(0, 75, 480, 70, BLACK);
		gfx->setFont(u8g2_font_helvB24_tr);
		gfx->setTextColor(WHITE, BLACK);
		int CurMon = STMonth - 1; 
		int Dow = getDayOfWeek(STYear, CurMon, STDay);
		gfx->setCursor(20, 115); gfx->print(WeekDays[Dow]);
		char dateBuf[32];
		sprintf(dateBuf, "%s %02d, %d", MonthOfTheYear[CurMon], STDay, STYear);
		int16_t x1, y1; uint16_t w, h;
		gfx->getTextBounds(dateBuf, 0, 0, &x1, &y1, &w, &h);
		gfx->setCursor(460 - w, 115); gfx->print(dateBuf);
	}
	if (full) 
	{
		for (int i = 0; i < 60; i++) 
		{
			float rad = i * 6 * 0.0174533;
			int x1 = cx + r * sin(rad), y1 = cy - (r * aspect) * cos(rad);
			if (i % 5 == 0) 
			{
				drawLine(x1, y1, cx + (r-35)*sin(rad), cy - ((r-35)*aspect)*cos(rad), 6, WHITE);
			} 
			else 
			{
				drawLine(x1, y1, cx + (r-15)*sin(rad), cy - ((r-15)*aspect)*cos(rad), 3, WHITE);
			}
		}
		gfx->drawRect(23, 20, 50, 50, WHITE);
		drawGearIcon(33, 30, 30, WHITE);
		gfx->drawRect(119, 20, 50, 50, WHITE);
		drawBluetoothIcon(129, 30, 30, IsBTOn ? BLUE : WHITE);
		gfx->drawRect(215, 20, 50, 50, WHITE);
		if(SelectedDST==1) drawDSTIcon(225, 30, 30, BLUE);
		else drawDSTIcon(225, 30, 30, WHITE);
		gfx->drawRect(311, 20, 50, 50, WHITE);
		drawOptionsIcon(321, 30, 30, WHITE); 
		gfx->drawRect(407, 20, 50, 50, WHITE);
		drawNotificationIcon(417, 30, 30, WHITE);
	}
	float h_rad = ((STHour % 12) + (STMinute / 60.0)) * 30 * 0.0174533;
	float m_rad = STMinute * 6 * 0.0174533;
	if (h_rad != last_h_rad || m_rad != last_m_rad || full) 
	{
		if (last_h_rad != -1 && !full) 
		{
			drawLine(cx, cy, cx + 120 * sin(last_h_rad), cy - (120 * aspect) * cos(last_h_rad), 13, BLACK);
			drawLine(cx, cy, cx + 170 * sin(last_m_rad), cy - (170 * aspect) * cos(last_m_rad), 8, BLACK);
		}
		drawLine(cx, cy, cx + 120 * sin(h_rad), cy - (120 * aspect) * cos(h_rad), 13, WHITE);
		drawLine(cx, cy, cx + 170 * sin(m_rad), cy - (170 * aspect) * cos(m_rad), 8, WHITE);
		gfx->fillCircle(cx, cy, 15, WHITE); 
		last_h_rad = h_rad;
		last_m_rad = m_rad;
	}
}
void drawNavigation() 
{
	gfx->setFont(u8g2_font_helvB18_tr);
	if (CurrentScreen > 0 && CurrentScreen < 9) 
	{
		gfx->drawRect(40, 720, 180, 60, WHITE);
		gfx->setTextColor(WHITE); gfx->setCursor(85, 762); gfx->print("BACK");// BACK
	}
	if (CurrentScreen < 7) 
	{
		gfx->drawRect(260, 720, 180, 60, WHITE);
		gfx->setTextColor(WHITE); gfx->setCursor(305, 762); gfx->print("NEXT");// Next
	}
	if (CurrentScreen == 7) 
	{
		gfx->drawRect(260, 720, 180, 60, WHITE);
		gfx->setTextColor(WHITE); gfx->setCursor(310, 762); gfx->print("Agree");// Next
	}
}
void drawElements() 
{
	gfx->setTextColor(WHITE); gfx->setFont(u8g2_font_helvB24_tr);
	if (CurrentScreen == 0) 
	{ 
		DrawScreenZero();
	}
	if (CurrentScreen == 1) 
	{
		gfx->setCursor(20, 60); gfx->print("Year");
		gfx->setFont(u8g2_font_helvB18_tr);
		for(int i = 0; i < 10; i++) 
		{
			int x=40+(i%2)*220, y=120+(i/2)*100;
			if (SelectedYear == 2026 + i) 
			{ 
				gfx->fillRect(x, y, 180, 80, WHITE); 
				gfx->setTextColor(BLACK);
			} 
			else 
			{ 
				gfx->drawRect(x, y, 180, 80, WHITE); 
				gfx->setTextColor(WHITE); 
			}
			gfx->setCursor(x + 48, y + 52); gfx->print(2026 + i);
		}
	} 
	else if (CurrentScreen == 2) 
	{
		gfx->setCursor(20, 60); gfx->print("Month");
		gfx->setFont(u8g2_font_helvB18_tr);
		for(int i = 0; i < 12; i++) 
		{
    		int x=20+(i%3)*150, y=100+(i/3)*75;
    		if (SelectedMonth == (i + 1))  // Changed: Match 1-12 range
    		{ 
        		gfx->fillRect(x, y, 140, 65, WHITE); 
        		gfx->setTextColor(BLACK); 
    		} 
    		else 
    		{
        		gfx->drawRect(x, y, 140, 65, WHITE);
        		gfx->setTextColor(WHITE); 
    		}
    		gfx->setCursor(x + 42, y + 45); gfx->print(MonthOfTheYear[i]); // Keep array index 0-11
		}
	}
	else if (CurrentScreen == 3) 
	{
		gfx->setCursor(20, 60); gfx->print("Day");
		gfx->setFont(u8g2_font_helvB18_tr);
		int md = getMaxDays();
		if (SelectedDay > md) SelectedDay = md;
		for(int i = 0; i < md; i++) 
		{
			int x=15+(i%5)*92, y=90+(i/5)*70;
			if (SelectedDay == i + 1) 
			{ 
				gfx->fillRect(x, y, 85, 60, WHITE); 
				gfx->setTextColor(BLACK); 
			}
			else 
			{ 
				gfx->drawRect(x, y, 85, 60, WHITE); 
				gfx->setTextColor(WHITE); 
			}
			gfx->setCursor(x + 30, y + 42); gfx->print(i + 1);
		}
	}
	else if (CurrentScreen == 4) 
	{
		gfx->setCursor(20, 60); gfx->print("Zone & DST");
		gfx->setFont(u8g2_font_helvB18_tr);
		gfx->drawRect(40, 140, 80, 80, WHITE);
		gfx->setCursor(72, 192); gfx->print("-");
		gfx->drawRect(140, 140, 200, 80, WHITE);
		gfx->setFont(u8g2_font_helvB14_tr);
		gfx->setCursor(155, 188);
		gfx->print("UTC ");
		if (SelectedTimeZone >= 0) gfx->print("+");
		gfx->print(SelectedTimeZone);
		gfx->setFont(u8g2_font_helvB18_tr);
		gfx->drawRect(360, 140, 80, 80, WHITE);
		gfx->setCursor(390, 192); gfx->print("+");
		if (SelectedDST == 1) 
		{
			gfx->fillRect(40, 260, 400, 80, WHITE);
			gfx->setTextColor(BLACK);
		} 
		else 
		{
			gfx->drawRect(40, 260, 400, 80, WHITE);
			gfx->setTextColor(WHITE);
		}
		gfx->setCursor(140, 312); gfx->print("Daylight Saving");
	}
	else if (CurrentScreen == 5) 
	{
		gfx->setCursor(20, 60); gfx->print("Hour");
		gfx->drawRect(20, 70, 200, 50, WHITE);
		gfx->drawRect(246, 70, 200, 50, WHITE);
		if(Selected1224 == 0)
		{
			gfx->fillRect(20, 70, 200, 50, WHITE); 
			gfx->setTextColor(BLACK); 
			gfx->setFont(u8g2_font_helvB14_tr); gfx->setCursor(70, 105); gfx->print("12H Mode");
			gfx->setTextColor(WHITE); 
			gfx->setFont(u8g2_font_helvB14_tr); gfx->setCursor(304, 105); gfx->print("24H Mode");
		}
		else
		{
			gfx->setTextColor(WHITE); 
			gfx->setFont(u8g2_font_helvB14_tr); gfx->setCursor(70, 105); gfx->print("12H Mode");
			gfx->fillRect(246, 70, 200, 50, WHITE); 
			gfx->setTextColor(BLACK); 
			gfx->setFont(u8g2_font_helvB14_tr); gfx->setCursor(304, 105); gfx->print("24H Mode");
		}

		gfx->setFont(u8g2_font_helvB18_tr);
		int Count = 0, Cols = 0;
		if(Selected1224 == 0)
		{
			Count = 12;
			Cols = 3;
		}
		else
		{
			Count = 24;
			Cols = 4;
		}
		int bw = (440 / Cols) - 10;
		for(int i = 0; i < Count; i++) 
		{
			int CurVal = 0;
			if(Selected1224 == 0)
			{
				CurVal = i + 1;
			}
			else
			{
				CurVal = i;
			}
			int x = 20 + (i % Cols) * (440 / Cols), y = 140 + (i / Cols) * 70;
			if(Selected1224 == 0)
			{
				if(SelectedHour>12)
				{
					if (SelectedHour-12 == CurVal) 
					{		 
						gfx->fillRect(x, y, bw, 60, WHITE); 
						gfx->setTextColor(BLACK); 
					} 
					else
					{
						gfx->drawRect(x, y, bw, 60, WHITE); 
						gfx->setTextColor(WHITE); 
					}
					gfx->setCursor(x + 28, y + 42); gfx->print(CurVal);
				}
				else
				{
					if (SelectedHour == CurVal) 
					{		 
						gfx->fillRect(x, y, bw, 60, WHITE); 
						gfx->setTextColor(BLACK); 
					} 
					else
					{
						gfx->drawRect(x, y, bw, 60, WHITE); 
						gfx->setTextColor(WHITE); 
					}
					gfx->setCursor(x + 28, y + 42); gfx->print(CurVal);
				}
			}
			else
			{
				if(SelectedHour>24)SelectedHour-=12;
				if (SelectedHour == CurVal) 
				{	 
					gfx->fillRect(x, y, bw, 60, WHITE); 
					gfx->setTextColor(BLACK); 
				} 
				else
				{
					gfx->drawRect(x, y, bw, 60, WHITE); 
					gfx->setTextColor(WHITE); 
				}
				gfx->setCursor(x + 28, y + 42); gfx->print(CurVal);
			}
		}
		if (Selected1224 == 0) 
		{
			int am_x = 20 + (9 % 3) * (440 / 3), am_y = 140 + (9 / 3) * 70 + 70;
			int pm_x = 20 + (11 % 3) * (440 / 3), pm_y = 140 + (11 / 3) * 70 + 70;
			if (SelectedAMPM == 0) 
			{
				gfx->fillRect(am_x, am_y, bw, 60, WHITE); 
				gfx->setTextColor(BLACK); 
			} 
			else 
			{ 
				gfx->drawRect(am_x, am_y, bw, 60, WHITE); 
				gfx->setTextColor(WHITE); 
			}
			gfx->setCursor(am_x + 26, am_y + 42); gfx->print("AM");
			if (SelectedAMPM == 1) 
			{ 
				gfx->fillRect(pm_x, pm_y, bw, 60, WHITE); 
				gfx->setTextColor(BLACK); 
			} 
			else 
			{ 
				gfx->drawRect(pm_x, pm_y, bw, 60, WHITE); 
				gfx->setTextColor(WHITE); 
			}
			gfx->setCursor(pm_x + 26, pm_y + 42); gfx->print("PM");
		}
	}
	else if (CurrentScreen == 6) 
	{
		gfx->setCursor(20, 60); gfx->print("Minute");
		gfx->setFont(u8g2_font_helvB14_tr);
		for(int i = 0; i < 60; i++) 
		{
			int dispVal = (i + 1) % 60;
			int x = 10 + (i % 6) * 78, y = 75 + (i / 6) * 65;
			if (SelectedMinute == dispVal) 
			{ 
				gfx->fillRect(x, y, 72, 55, WHITE); 
				gfx->setTextColor(BLACK); 
			} 
			else 
			{
				gfx->drawRect(x, y, 72, 55, WHITE); 
				gfx->setTextColor(WHITE); 
			}
			gfx->setCursor(x + 20, y + 38); if (dispVal < 10) gfx->print("0"); gfx->print(dispVal);
		}
	}
	else if (CurrentScreen == 7) 
	{
		gfx->fillScreen(BLACK);
		gfx->setFont(u8g2_font_helvB24_tr);
		gfx->setTextColor(WHITE);
		gfx->setCursor(20, 60);
		gfx->print("Agreement");
		gfx->setFont(u8g2_font_helvB14_tr);
		gfx->setCursor(20, 140);
		gfx->print("Agreement");
	}
	else if (CurrentScreen == 8) 
	{
		gfx->setCursor(20, 60); 
		gfx->print("Install App");
		gfx->setFont(u8g2_font_helvB12_tr);
		gfx->setTextColor(GREY);
		gfx->setCursor(20, 100); 
		gfx->print("Scan with your Android-Phone to download the App");
		const int blockSize = 9; 
		const int qrSizeModules = 37;
		const int offsetX = 55;
		const int offsetY = 370;
		/*gfx->fillRect(50 , 365, 380, 343, WHITE);
			for (int y = 0; y < QR_MATRIX_SIZE; y++) 
			{
			for (int x = 0; x < QR_MATRIX_SIZE; x++) 
			{
			uint8_t bitState = pgm_read_byte(&(qrModules[y][x]));
			uint16_t color = (bitState == 1) ? BLACK : WHITE;
			gfx->fillRect(offsetX + (x * (blockSize+1)), offsetY + (y * blockSize), blockSize+1, blockSize, color);
			}
		}*/
	}
	drawNavigation();
}
void drawFull() 
{
	gfx->fillScreen(BLACK);
	last_h_rad = -1; 
	last_m_rad = -1;
	if (CurrentScreen == 9) drawDashboard(true); 
	else drawElements();
}
void loopManual() 
{
	if (CurrentScreen == 9 && millis() - ScreenLastUpdate > 1000) 
	{
		if(STMinute == 59)
		{
			NewDay = true;
		}
		drawDashboard(false); 
		ScreenLastUpdate = millis();
	}
	ts.read();
	if (ts.isTouched) 
	{
		int tx = 480 - ts.points[0].y, ty = ts.points[0].x;
		bool back = (ty > 720 && tx > 40 && tx < 220);
		bool next = (ty > 720 && tx > 260 && tx < 440);
		bool GearIcon         = ( tx > 23  && tx < 73  && ty > 20 && ty < 70);
		bool BluetoothIcon    = ( tx > 119 && tx < 169 && ty > 20 && ty < 70);
		bool DSTIcon          = ( tx > 215 && tx < 265 && ty > 20 && ty < 70);
		bool OptionsIcon      = ( tx > 311 && tx < 361 && ty > 20 && ty < 70);
		bool NotificationIcon = ( tx > 407 && tx < 457 && ty > 20 && ty < 70);
		bool actionTriggered = false;
		if (CurrentScreen == 9 && GearIcon) 
		{
			CurrentScreen = 0; 
			SelectedYear = STYear;
			SelectedMonth  = STMonth; 
			SelectedDay  = STDay;
			SelectedHour   = STHour; 
			SelectedMinute  = STMinute;
			actionTriggered = true;
		}
		else if (CurrentScreen == 9 && BluetoothIcon)
		{
			IsBTOn = !IsBTOn;
			if(!IsBTOn)
			{
			}
			else
			{
			}
			actionTriggered = true;
		}
		else if (CurrentScreen == 9 && DSTIcon)
		{

			if (SelectedDST == 1) 
			{
				STHour = (STHour + 1) % 24;
				SelectedDST = 0;
			} 
			else 
			{
				STHour = (STHour - 1 + 24) % 24;
				SelectedDST = 1;
			}
			STDST = SelectedDST;
			MySerial.println(String("ST") + "\x1F" + STSecond + "\x1F" + STMinute + "\x1F" + STHour + "\x1F" + STDay + "\x1F" + STMonth + "\x1F" + STYear + "\x1F" + STDST + "\x1F" + STTimeZone+ "\x1F" + ST1224 + "\x1F" + STAMPM);
			Serial.println("DST Toggle, Time Set Sent!");
			actionTriggered = true;
		}
		else if (CurrentScreen == 9 && OptionsIcon)
		{
			actionTriggered = true;
		}
		else if (CurrentScreen == 9 && NotificationIcon)
		{
			actionTriggered = true;
		}
		else if (back && CurrentScreen > 0 && CurrentScreen < 7) 
		{ 
			CurrentScreen--; 
			actionTriggered = true;
		}
		else if (back && CurrentScreen == 7) 
		{ 
			if(ChoiceIsBluetooth)
			{
				CurrentScreen = 0;
				actionTriggered = true;
			}
			else
			{
				CurrentScreen--; 
				actionTriggered = true;
			}
		}
		else if (back && CurrentScreen == 8) 
		{ 
			CurrentScreen = 7;
			actionTriggered = true;
		}
		else if (next && CurrentScreen < 6) 
		{ 
			CurrentScreen++; 
			actionTriggered = true;
		}
		else if (next && CurrentScreen == 6) 
		{ 
			int SelectedHourMilitary = SelectedHour;
			if (Selected1224 == 0) 
			{
				if (SelectedAMPM == 1 && SelectedHour < 12) SelectedHourMilitary += 12; 
				if (SelectedAMPM == 0 && SelectedHour == 12) SelectedHourMilitary = 0; 
			}
			MySerial.println(String("ST") + "\x1F" + SelectedSecond + "\x1F" + SelectedMinute + "\x1F" + SelectedHourMilitary + "\x1F" + SelectedDay + "\x1F" + SelectedMonth + "\x1F" + SelectedYear + "\x1F" + SelectedDST + "\x1F" + SelectedTimeZone + "\x1F" + Selected1224 + "\x1F" + SelectedAMPM);
			Serial.println("Setup Done, Time Set Sent!");
			CurrentScreen++; 
			actionTriggered = true;
		}
		else if (next && CurrentScreen == 7) 
		{
			if(ChoiceIsBluetooth)
			{
				CurrentScreen = 8; 
				actionTriggered = true;
			}
			else
			{
				CurrentScreen = 9; 
				actionTriggered = true;
			}
		}
		else if (CurrentScreen == 1) 
		{
			for(int i=0; i<10; i++) 
			{
				int bx=40+(i%2)*220, by=120+(i/2)*100; 
				if(tx>bx && tx<bx+180 && ty>by && ty<by+80) 
				{
					if (SelectedYear != 2026+i) {
						SelectedYear = 2026+i; 
						actionTriggered = true;
					}
				} 
			} 
		}
		else if (CurrentScreen == 2) 
		{
    		for(int i=0; i<12; i++) 
    		{ 
        		int bx=20+(i%3)*150, by=100+(i/3)*75; 
        		if(tx>bx && tx<bx+140 && ty>by && ty<by+65) 
        		{ 
            		if (SelectedMonth != (i + 1)) 
								{ 
                		SelectedMonth = i + 1;
                		actionTriggered = true;
            		}
        		} 
    		}
		}
		else if (CurrentScreen == 3) 
		{
			int md = getMaxDays(); 
			for(int i=0; i<md; i++) 
			{ 
				int bx=15+(i%5)*92, by=90+(i/5)*70; 
				if(tx>bx && tx<bx+85 && ty>by && ty<by+60) 
				{
					if (SelectedDay != i+1) {
						SelectedDay = i+1; 
						actionTriggered = true;
					}
				} 
			} 
		}
		else if (CurrentScreen == 4) 
		{
			// Timezone Minus Button Touch Target (-12 limit)
			if(tx > 40 && tx < 120 && ty > 140 && ty < 220) 
			{
				if(SelectedTimeZone > -12) {
					SelectedTimeZone--;
					actionTriggered = true;
				}
			}
			// Timezone Plus Button Touch Target (+14 limit)
			else if(tx > 360 && tx < 440 && ty > 140 && ty < 220) 
			{
				if(SelectedTimeZone < 14) {
					SelectedTimeZone++;
					actionTriggered = true;
				}
			}
			// DST Toggle Box Touch Target
			else if(tx > 40 && tx < 440 && ty > 260 && ty < 340) 
			{
			  if (SelectedDST == 1) 
			  {
				  SelectedDST = 0;
			  } 
			  else 
			  {
				  SelectedDST = 1;
			  }
				actionTriggered = true;
			}
		}
		else if (CurrentScreen == 5) 
		{
			if(tx>20 && tx<220 && ty>70 && ty<120) 
			{
				Selected1224 = 0; 
				if(SelectedHour>12)
				{
					SelectedAMPM = 1;
					SelectedHour -= 12;
				}
				else
				{
					SelectedAMPM = 0;
					SelectedHour=SelectedHour;
				}
				actionTriggered = true;
			}
			if(tx>246 && tx<446 && ty>70 && ty<120) 
			{
				Selected1224 = 1; 
				if(SelectedAMPM == 1)
				{
					SelectedHour += 12;
				}
				actionTriggered = true;
			}
			if(Selected1224 == 0) 
			{
				int bw = (440 / 3) - 10;
				int am_x = 20 + (9 % 3) * (440 / 3), am_y = 140 + (9 / 3) * 70 + 70;
				int pm_x = 20 + (11 % 3) * (440 / 3), pm_y = 140 + (11 / 3) * 70 + 70;
				if(tx>am_x && tx<am_x+bw && ty>am_y && ty<am_y+60) 
				{
						SelectedAMPM = 0; 
						actionTriggered = true;
				}
				if(tx>pm_x && tx<pm_x+bw && ty>pm_y && ty<pm_y+60) 
				{ 
						SelectedAMPM = 1;
						actionTriggered = true;
				}
			}
		int Count = 0, Cols = 0;
		if(Selected1224 == 0)
		{
			Count = 12;
			Cols = 3;
		}
		else
		{
			Count = 24;
			Cols = 4;
		}
		for(int i=0; i<Count; i++) 
		{
				int bx = 20+(i%Cols)*(440/Cols), by = 140+(i/Cols)*70;
				if(tx>bx && tx<bx+(440/Cols)-10 && ty>by && ty<by+60) 
				{
					int TargetHr = 0;
					if(Selected1224 == 1)
					{
							TargetHr = i;
					}
					else
					{
							TargetHr = i+1;
					}
					if (SelectedHour != TargetHr) 
					{
						SelectedHour = TargetHr; 
						actionTriggered = true;
					}
				} 
		}
		}
		else if (CurrentScreen == 6) 
		{
			for(int i=0; i<60; i++) 
			{ 
				int dispVal = (i + 1) % 60;
				int bx=10+(i%6)*78, by=75+(i/6)*65; 
				if(tx>bx && tx<bx+72 && ty>by && ty<by+55) 
				{ 
					if (SelectedMinute != dispVal) {
						SelectedMinute = dispVal; 
						actionTriggered = true;
					}
				} 
			} 
		}
		else if (CurrentScreen == 7) 
		{
		}
		if (actionTriggered) 
		{
			drawFull();
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