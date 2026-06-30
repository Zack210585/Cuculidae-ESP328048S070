const uint8_t finderPattern[7][7] = {
    {1,1,1,1,1,1,1},
    {1,0,0,0,0,0,1},
    {1,0,1,1,1,0,1},
    {1,0,1,1,1,0,1},
    {1,0,1,1,1,0,1},
    {1,0,0,0,0,0,1},
    {1,1,1,1,1,1,1}
};
const uint8_t alignmentPattern[5][5] = {
    {1,1,1,1,1},
    {1,0,0,0,1},
    {1,0,1,0,1},
    {1,0,0,0,1},
    {1,1,1,1,1}
};
uint8_t qrModules[QR_MATRIX_SIZE][QR_MATRIX_SIZE];
const char* textToEncode = "https://play.google.com/store/apps/details?id=com.google.android.calendar&hl=en";
void applyFinderPatterns() 
{
    // 1. Top-Left Finder & its 9x9 white separator zone
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            if (y < 7 && x < 7) qrModules[y][x] = finderPattern[y][x];
            else qrModules[y][x] = 0; // Clear separator to white
		}
	}
    // 2. Top-Right Finder & its 9x9 white separator zone
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            int targetX = QR_MATRIX_SIZE - 9 + x;
            if (y < 7 && x >= 2) qrModules[y][targetX] = finderPattern[y][x - 2];
            else qrModules[y][targetX] = 0; // Clear separator to white
		}
	}
    // 3. Bottom-Left Finder & its 9x9 white separator zone
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            int targetY = QR_MATRIX_SIZE - 9 + y;
            if (y >= 2 && x < 7) qrModules[targetY][x] = finderPattern[y - 2][x];
            else qrModules[targetY][x] = 0; // Clear separator to white
		}
	}
}
void applyAlignmentPatterns() 
{
    // Version 5 center coordinate is exactly (30, 30)
    int startX = 30 - 2;
    int startY = 30 - 2;
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            qrModules[startY + y][startX + x] = alignmentPattern[y][x];
		}
	}
}
void applyTimingPatterns() 
{
    // Timing lines run between the finder patterns (indices 8 to 28)
    for (int i = 8; i <= QR_MATRIX_SIZE - 9; i++) {
        // Alternating black (1) on even indices, white (0) on odd indices
        qrModules[6][i] = (i % 2 == 0) ? 1 : 0;
        qrModules[i][6] = (i % 2 == 0) ? 1 : 0;
	}
}
void encodePayloadData(const char* text) 
{
    int textLength = strlen(text);
    if (textLength == 0) return;
    int bitIndex = 0;
    int direction = -1; // -1 = Upward traversal, 1 = Downward traversal
    int y = QR_MATRIX_SIZE - 1; // Start at the absolute bottom row
    // Step across the matrix right-to-left in 2-module wide columns
    for (int x = QR_MATRIX_SIZE - 1; x > 0; x -= 2) 
    {
        // Skip the vertical timing column completely
        if (x == 6) {
            x--;
		}
        while (true) 
        {
            // Read column right-to-left within the 2-module wide track
            for (int i = 0; i < 2; i++) 
            {
                int currentX = x - i;
                int currentY = y;
                // Condition A: Skip Finder Patterns + 1 module white separators
                if ((currentX < 9 && currentY < 9) || 
                    (currentX > QR_MATRIX_SIZE - 9 && currentY < 9) || 
                    (currentX < 9 && currentY > QR_MATRIX_SIZE - 9)) {
                    continue; 
				}
                // Condition B: Skip Format Info modules around Finders
                if ((currentY == 8 && currentX < 9) || (currentX == 8 && currentY < 9) ||
                    (currentY == 8 && currentX > QR_MATRIX_SIZE - 9) || (currentX == QR_MATRIX_SIZE - 8 && currentY < 9) ||
                    (currentX == 8 && currentY > QR_MATRIX_SIZE - 9) || (currentY == QR_MATRIX_SIZE - 8 && currentX < 9)) {
                    continue;
				}
                // Condition C: Skip Timing Lines
                if (currentX == 6 || currentY == 6) {
                    continue;
				}
                // Condition D: Skip Version 5 Alignment Pattern
                if (currentX >= 28 && currentX <= 32 && currentY >= 28 && currentY <= 32) {
                    continue;
				}
                // Process and map raw data bits
                int bytePos = (bitIndex / 8) % textLength;
                int bitPos = 7 - (bitIndex % 8);
                uint8_t activeBit = (text[bytePos] >> bitPos) & 0x01;
                // Apply Standard QR Mask 000: ((x + y) % 2 == 0)
                uint8_t maskPattern = ((currentX + currentY) % 2 == 0); 
                qrModules[currentY][currentX] = activeBit ^ maskPattern;
                bitIndex++;
			}
            // Move pointer vertically
            y += direction;
            // Reverse direction if vertical bounds are breached
            if (y < 0 || y >= QR_MATRIX_SIZE) {
                direction = -direction; 
                y += direction; 
                break; // Break the vertical loop to shift to the next 2-column strip left
			}
		}
	}
}
//memset(qrModules, 0, sizeof(qrModules));
//applyFinderPatterns();
//applyAlignmentPatterns();
//applyTimingPatterns();
//encodePayloadData(textToEncode);