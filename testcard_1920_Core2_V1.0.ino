/* April 2023
   M5stack Core 2 with M5Stack Display Module 13.2
   Compiled with Arduino IDE 2.04
   Testcard generation 1920x1080 at 24 frames/sec
   with true time clock using internet through wifi.
   Please input in the sofware : your call, wifi name and wifi password
   The ntp server name can be changed for a more near home location.
   Do not use this software for commercial use
   alain.fort.f1cjn at gmail.com
*/

#include <M5ModuleDisplay.h>
#include <M5Unified.h>
#include <stdio.h>
#include <WiFi.h>

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define GREY 0x7BEF
#define TURQUOISE 0X0410
#define AZURE 0x041F


// Set the name and password of the wifi to be connected.
const char* call = " CALL";              //  set text centered, up to 7 characters
const char* ssid = "My SSID";              //  set  your wifi ssid
const char* password = "password";     //  set your wifi password
const char* ntpServer = "ntp.unice.fr";  //  set your Internet server address

const long gmtOffset_sec = 0;      // french GMT offset, zero for UTC time
const int daylightOffset_sec = 0;  //3600 for french summer time //0 for french winter time

void setup(void) {
  auto cfg = M5.config();
  cfg.module_display.logical_width = 1920;
  cfg.module_display.logical_height = 1080;
  cfg.module_display.refresh_rate = 24;  // seul 24 fonctionne ?
  M5.begin(cfg);

  // Change the primary display to ModuleDisplay.
  M5.setPrimaryDisplayType(m5gfx::board_M5ModuleDisplay);

  Serial.begin(115200);
  M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(GREY);

  Serial.printf("\nConnecting to %s ", ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);              // Connect wifi and return connection status.
  while (WiFi.status() != WL_CONNECTED) {  // If the wifi connection fails.
    delay(100);                            // delay 0.1s
    Serial.print(".");
    M5.Lcd.setTextColor(BLUE, BLACK);
    M5.Lcd.setCursor(100, 600);
    M5.Lcd.setTextSize(4);
    M5.Lcd.print(" WIFI NOT CONNECTED");
  }
  Serial.println(" WIFI CONNECTED");  // serial monitor

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);  // init and get the time.
  printLocalTime();

  WiFi.disconnect(true);  // Disconnect wifi.
  WiFi.mode(WIFI_OFF);    // Set the wifi mode to off.
  delay(20);
  M5.Lcd.fillScreen(GREY);
}

void loop(void) {
  int16_t x, y, dx, dy, radius = 480;
  uint16_t w, h, len, mask, idx;
  uint16_t colors[8] = { BLACK, WHITE, YELLOW, CYAN, GREEN, MAGENTA, RED, BLUE };

  M5.Lcd.fillRect(0, 0, 1920, 19, WHITE);  // haut
  for (x = 41, y = 0; x < 1920 - 81; x += 160) {
    M5.Lcd.fillRect(x, y, 78, 18, BLACK);
  }  //rectangles noirs tout en haut

  M5.Lcd.fillRect(0, 1061, 1920, 19, WHITE);  //bas
  for (x = 41, y = 1061; x < 1920 - 81; x += 160) {
    M5.Lcd.fillRect(x, y, 78, 18, BLACK);
  }  //rectangles noirs tout en bas

  M5.Lcd.fillRect(0, 0, 39, 1080, WHITE);  // gauche
  for (y = 21, x = 0; y < 1080 - 81; y += 160) {
    M5.Lcd.fillRect(x, y, 39, 78, BLACK);
  }  //rectangles noirs tout à gauche

  M5.Lcd.fillRect(1881, 0, 39, 1080, WHITE);  // Droite
  for (y = 21, x = 1881; y < 1081 - 81; y += 160) {
    M5.Lcd.fillRect(x, y, 39, 78, BLACK);
  }  //rectangles noirs tout à droite


  for (x = 39, y = 0, w = 2, h = 1080; x < 1920; x += 80) {
    M5.Lcd.fillRect(x, y, w, h, WHITE);  // Lignes fines verticales
  }
  for (x = 0, y = 19, w = 1920, h = 2; y < 1080; y += 80) {
    M5.Lcd.fillRect(x, y, w, h, WHITE);  // ligne fines horizontales
  }
  M5.Lcd.fillRect(361, 101, 78, 438, TURQUOISE);
  M5.Lcd.fillRect(361, 541, 78, 438, PINK);
  M5.Lcd.fillRect(439, 101, 80, 158, AZURE);
  M5.Lcd.fillRect(439, 821, 80, 158, ORANGE);
  M5.Lcd.fillRect(1401, 101, 80, 158, AZURE);
  M5.Lcd.fillRect(1401, 821, 80, 158, ORANGE);
  M5.Lcd.fillRect(1481, 101, 78, 438, OLIVE);
  M5.Lcd.fillRect(1481, 541, 78, 438, PURPLE);

  for (dx = radius; dx > -radius; dx--) {
    w = sqrt(radius * radius - dx * dx);
    y = 540 - dx;        // centre cercle - dx
    dy = (y - 20) / 80;  // step  avec calage des lignes Horizontales du cercle en tranches horizontales de 80 pixels de haut
    //Serial.println(dx);
    //Serial.println(dy);
    mask = 7;
    colors[0] = (dy == 3) ? LIGHTGREY : BLACK;  // Si ligne 3 alors on utilise les deux couleurs définies
    switch (dy) {  // Switch selon le numero de tranche
      case 0:
      case 1:
        idx = 1;
        len = 0;
        break;
      case 2:
        idx = 0;
        len = 0;
        break;
      case 3:
        idx = 0;
        len = 55;
        mask = 1;
        break;  //len == longueur segment  idx == couleur
      case 4:
      case 5:
        idx = 2;
        len = 160;
        break;
      case 6:
      case 7:
      case 8:
        idx = 0;
        len = 0;
        break;
      case 9:
        for (idx = 2; idx < 8; idx++) {  // degradé de gris
          dy = (idx - 2) * 51;
          colors[idx] = M5.Lcd.color565(dy, dy, dy);
        }
        idx = 2;
        len = 160;
        break;  // len = longueur des segments gris
      case 10:
        idx = 1;
        len = 0;
        break;
      case 11:
      case 12:
        colors[2] = YELLOW;
        idx = 2;
        len = 0;
        break;
    }
    if (len == 0)  // pas de segments = ligne pleine
      M5.Lcd.fillRect(960 - w, y, w * 2, 1, colors[idx]);

    else {

      if (mask == 1) idx = 0 + (w) / len;
      if (dy == 3) {
        colors[0] = LIGHTGREY;
        colors[1] = BLACK;
      } else {
        colors[0] = WHITE;
      }  // correctif
      dy = w % len;
      for (x = 960 - w; x < 960 + w; idx++) {
        M5.Lcd.fillRect(x, y, dy, 1, colors[idx & mask]);  // couleur selon bloc pair/impair si mask = 1
        x += dy;
        if (x + len > 960 + w) dy = w % len;
        else dy = len;
      }
    }
  }


  for (x = 560, y = 580, dx = 10, dy = 0; dx > 0; x += 2 * dx) {  // Barres verticales noir et blancs dégressives
    M5.Lcd.fillRect(x, y, dx, 160, WHITE);
    dy += dx * 4;
    if (dy >= 160) {
      dy = 0;
      dx--;
      x = x + 1;
    }  // x+1 pour maintenir l'écart visuel
  }

  M5.Lcd.fillRect(921, 420, 78, 240, BLACK);  // rectange noir vertical au centre
  for (x = 519, y = 500, w = 2, h = 80; x < 1450; x += 80) {
    M5.Lcd.fillRect(x, y, w, h, WHITE);
  }  // petites lignes verticales dans le bandeau noir horizontal

  M5.Lcd.fillRect(480, 540, 960, 2, WHITE);   // Ligne horizontale blanche au centre
  M5.Lcd.fillRect(960, 420, 2, 240, WHITE);   // ligne verticale blanche au centre
  M5.Lcd.fillRect(720, 180, 480, 80, WHITE);  // Carré blanc sous indicatif
  M5.Lcd.fillRect(920, 900, 80, 120, RED);    // bloc rouge dans le jaune

  M5.Lcd.fillCircle(200, 180, 118, WHITE);     // Cercle en haut à gauche
  M5.Lcd.fillRect(119, 99, 162, 162, BLACK);   // Carré
  M5.Lcd.drawLine(119, 180, 281, 180, WHITE);  //ligne h
  M5.Lcd.drawLine(200, 99, 200, 261, WHITE);   //ligne v

  M5.Lcd.fillCircle(1720, 180, 118, BLACK);      // Cercle en haut à droite
  M5.Lcd.fillRect(1640, 99, 162, 162, WHITE);    // Carré
  M5.Lcd.drawLine(1640, 180, 1801, 180, BLACK);  //ligne h
  M5.Lcd.drawLine(1720, 99, 1720, 261, BLACK);   //ligne v

  M5.Lcd.fillCircle(200, 900, 118, BLACK);     // Cercle en bas à gauche
  M5.Lcd.fillRect(119, 819, 162, 162, WHITE);  // Carré
  M5.Lcd.drawLine(119, 900, 281, 901, BLACK);  //ligne h
  M5.Lcd.drawLine(200, 820, 200, 981, BLACK);  //ligne v

  M5.Lcd.fillCircle(1720, 900, 118, WHITE);      // Cercle en bas à droite
  M5.Lcd.fillRect(1640, 819, 162, 162, BLACK);   // Carré
  M5.Lcd.drawLine(1640, 900, 1901, 900, WHITE);  //ligne h
  M5.Lcd.drawLine(1720, 820, 1720, 981, WHITE);  //ligne v

  M5.Lcd.fillRect(780, 100, 360, 80, BLACK);  // pour indicatif / for Call
  M5.Lcd.setCursor(830, 110);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(9);
  M5.Lcd.print("F1CJN");

  while (true) {
    printLocalTime();  //boucle infinie
    delay(900);
  }
}

void printLocalTime() {  // Output current time.
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {  // Return 1 when the time is successfully obtained.
    Serial.println("Failed to obtain time");
    return;
  }

  //Serial.println(&timeinfo,"%H:%M:%S");  //Serial print date and time.
  M5.Lcd.fillRect(720, 820, 520, 80, BLACK);
  M5.Lcd.setCursor(740, 830);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.print(&timeinfo, "%H:%M:%S");
}
