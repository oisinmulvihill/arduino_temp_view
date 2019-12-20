/* Temperature View

This uses the LCD to show the temperature and humidity of the room. The DHT11
sensor provides this via a third-party library I use to get the temperature,
humidity and comfort level. There is a lot of information to display, so I
rotate every two seconds through five "pages" of information.

DHTesp DHT11 library
- https://github.com/beegee-tokyo/DHTesp

Oisin Mulvihill
2019-12-19

*/
#include <LiquidCrystal.h>
#include "DHTesp.h"

#define DELAY 2000
#define DHT_PIN 2

int DISPLAY_PAGE = 0;

DHTesp dht;

ComfortState comfortState;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);


void setup() {
  Serial.begin(9600);

  dht.setup(DHT_PIN, DHTesp::DHT11);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  lcd.print("Room Information");
}


void loop() {
  float temperature = dht.getTemperature();
  float humidity = dht.getHumidity();
  char report[16];
  // Size 5: XX.Y0
  char value[5];

  if(isnan(temperature) || isnan(humidity))
  {
    snprintf(report, sizeof(report), "No temp data :(");
  }
  else {
    // Clear out previous displayed values:
    memset(value, 0, sizeof(value));
    memset(report, 0, sizeof(report));

    if (DISPLAY_PAGE > 4) {
      DISPLAY_PAGE = 0;
    }

    switch(DISPLAY_PAGE) {
      case 0:
        dtostrf(temperature, 4, 1, value);
        snprintf(report, sizeof(report), "Temp. %sC", value);
        break;

      case 1:
        dtostrf(humidity, 4, 1, value);
        snprintf(report, sizeof(report), "Humidity %s%%", value);
        break;

      case 2:
        dtostrf(dht.computeDewPoint(temperature, humidity), 4, 1, value);
        snprintf(report, sizeof(report), "Dew Point %sC", value);
        break;

      case 3:
        dtostrf(dht.computeHeatIndex(temperature, humidity), 4, 1, value);
        snprintf(report, sizeof(report), "Heat Index %s", value);
        break;

      case 4:
        dht.getComfortRatio(comfortState, temperature, humidity);
        switch(comfortState) {
          case Comfort_OK:
            snprintf(report, sizeof(report), "Feels %s", "OK");
            break;
          case Comfort_TooHot:
            snprintf(report, sizeof(report), "Feels %s", "Too Hot");
            break;
          case Comfort_TooCold:
            snprintf(report, sizeof(report), "Feels %s", "Too Cold");
            break;
          case Comfort_TooDry:
            snprintf(report, sizeof(report), "Feels %s", "Too Dry");
            break;
          case Comfort_TooHumid:
            snprintf(report, sizeof(report), "Feels %s", "Too Humid");
            break;
          case Comfort_HotAndHumid:
            snprintf(report, sizeof(report), "Feels %s", "Hot&Humid");
            break;
          case Comfort_HotAndDry:
            snprintf(report, sizeof(report), "Feels %s", "Hot&Dry");
            break;
          case Comfort_ColdAndHumid:
            snprintf(report, sizeof(report), "Feels %s", "Cold&Humid");
            break;
          case Comfort_ColdAndDry:
            snprintf(report, sizeof(report), "Feels %s", "Cold&Dry");
            break;
          default:
            break;
        };
        break;

      default:
        break;
    }

    Serial.print("\nDISPLAY_PAGE: ");
    Serial.print(DISPLAY_PAGE);

    DISPLAY_PAGE += 1;
  }

  Serial.print("\nRoom Information\n");
  Serial.print(report);
  Serial.print("\n");

  // Clear out previous display content:
  lcd.setCursor(0, 1);
  lcd.print("                ");
  // Now print out the new content:
  lcd.setCursor(0, 1);
  lcd.print(report);

  delay(DELAY);
}
