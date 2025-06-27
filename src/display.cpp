#include "display.h"

constexpr char DisplayHandler::FILE_TEMPLATE[]; // Definition of static constexpr

bool DisplayHandler::begin()
{
    for (int i = 0; i < numOleds; i++)
    {
        pinMode(oledPins[i], OUTPUT);
        digitalWrite(oledPins[i], LOW); // or LOW depending on default state
    }

    // 1. Mount FS
    if (!LittleFS.begin(true))
    { // true = format on failure
        Serial.println("LittleFS mount failed");
        return false;
    }

    // 2. OLED init
    handleCSPin(OLED_CS_ALL, LOW);
    SPI.begin(OLED_CLK, -1, OLED_MOSI, OLED_CS); // SCK, MISO, MOSI, SS (MISO = -1 if unused)

    if (!_oled.begin(SSD1306_SWITCHCAPVCC))
    {
        Serial.println("SSD1306 init failed");
        return false;
    }
    _oled.clearDisplay();
    _oled.setTextColor(SSD1306_WHITE);
    _oled.display();
    handleCSPin(OLED_CS_ALL, HIGH);
    return true;
}

void DisplayHandler::handleCSPin(uint8_t index, bool state)
{
    if (index < numOleds)
    {
        digitalWrite(oledPins[index], state ? HIGH : LOW);
    }
    else
    {
        for (uint8_t i = 0; i < numOleds; i++)
        {
            digitalWrite(oledPins[i], state ? HIGH : LOW);
        }
    }
}

void DisplayHandler::show(uint8_t index)
{
    char path[32];
    snprintf_P(path, sizeof(path), FILE_TEMPLATE, index);

    // Try .bmp first, then .txt
    String bmp = String(path) + F(".bmp");
    String txt = String(path) + F(".txt");
    File f;

    if (LittleFS.exists(bmp) && (f = LittleFS.open(bmp, "r")))
    {
        handleCSPin(index, LOW);
        if (!drawBMP(f))
            Serial.println("BMP draw failed");
        handleCSPin(index, HIGH);
        f.close();
        return;
    }
    if (LittleFS.exists(txt) && (f = LittleFS.open(txt, "r")))
    {
        handleCSPin(index, LOW);
        if (!drawTXT(f))
            Serial.println("TXT draw failed");
        handleCSPin(index, HIGH);
        f.close();
        return;
    }

    // Fallback – file missing
    handleCSPin(index, LOW);
    _oled.clearDisplay();
    _oled.setTextSize(1);
    _oled.setCursor(0, 0);
    _oled.print(F("Missing file:"));
    _oled.setCursor(0, 10);
    _oled.print(String(path) + F(".bmp/txt"));
    _oled.display();
    handleCSPin(index, HIGH);
}

// --- PRIVATE -------------------------------------------------------------

// Minimal 1‑bit, uncompressed 128×64 BMP reader (54‑byte header).
bool DisplayHandler::drawBMP(File &f)
{
    uint8_t header[54];
    if (f.read(header, 54) != 54)
        return false;
    // Very light sanity checks
    if (header[0] != 'B' || header[1] != 'M')
        return false;
    uint32_t offset = *((uint32_t *)&header[10]);
    uint32_t w = *((uint32_t *)&header[18]);
    uint32_t h = *((uint32_t *)&header[22]);
    uint16_t depth = *((uint16_t *)&header[28]);
    if (w != WIDTH || h != HEIGHT || depth != 1)
        return false;

    // Move to bitmap data
    f.seek(offset);
    _oled.clearDisplay();

    // BMP is bottom‑up; we flip Y
    for (int16_t y = HEIGHT - 1; y >= 0; --y)
    {
        for (uint16_t xByte = 0; xByte < WIDTH / 8; ++xByte)
        {
            uint8_t b = f.read();
            for (uint8_t bit = 0; bit < 8; ++bit)
            {
                bool pixel = b & (1 << (7 - bit));
                _oled.drawPixel(xByte * 8 + bit, y, pixel ? SSD1306_WHITE : SSD1306_BLACK);
            }
        }
    }
    _oled.display();
    return true;
}

bool DisplayHandler::drawTXT(File &f)
{
    _oled.clearDisplay();
    _oled.setTextSize(1);
    _oled.setTextColor(SSD1306_WHITE);
    _oled.setCursor(0, 0);

    String line;
    uint8_t lineHeight = 8;
    uint8_t y = 0;

    while (f.available() && y < HEIGHT)
    {
        char c = (char)f.read();
        if (c == '\n' || line.length() >= 20)
        { // crude wrap
            centerText(line, y);
            line = "";
            y += lineHeight;
            if (c == '\n')
                continue;
        }
        line += c;
    }
    if (line.length())
        centerText(line, y);

    _oled.display();
    return true;
}

void DisplayHandler::centerText(const String &line, int16_t y)
{
    int16_t x = (WIDTH - (line.length() * 6)) / 2; // 6 px per glyph at size 1
    _oled.setCursor(max<int16_t>(0, x), y);
    _oled.print(line);
}

void DisplayHandler::displaytext(uint8_t displayIdx, uint8_t x, uint8_t y, uint8_t size, const char data[])
{
    handleCSPin(displayIdx, LOW);
    _oled.clearDisplay();
    _oled.setCursor(x, y);
    _oled.setTextSize(size); // Draw 2X-scale text
    _oled.setTextColor(SSD1306_WHITE);
    _oled.println(F(data));
    _oled.display();
    handleCSPin(displayIdx, HIGH);
}