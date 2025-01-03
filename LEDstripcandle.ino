#include <FastLED.h>

// Configurazione LED
#define LED_PIN 4
#define LED_COUNT 3
#define BRIGHTNESS 255
#define GROUP_SIZE 3

CRGB leds[LED_COUNT];
CRGB ledsNext[LED_COUNT]; // Configurazione successiva dei LED

// Sequenza di luminosità randomizzata
int candleBrightness[] = {
  255, 240, 255, 220, 255, 230, 255, 190, 170, 255, 255, 210, 255, 255, 200, 180
};
int brightnessSize = sizeof(candleBrightness) / sizeof(candleBrightness[0]);

// Modalità debug
bool debugMode = false;

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_COUNT);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setCorrection(TypicalLEDStrip); // Abilita correzione colori per striscia LED
  FastLED.clear();
  FastLED.show();

  // Inizializza la configurazione successiva
  for (int i = 0; i < LED_COUNT; i++) {
    ledsNext[i] = CRGB::Black;
  }

  if (debugMode) {
    Serial.begin(9600);
  }
}

void loop() {
  buildNextConfiguration();
  smoothTransition();

  if (debugMode) {
    printLedStatus();
  }

  delay(50); // Ogni configurazione dura 50 ms inclusa la transizione
}

void buildNextConfiguration() {
  for (int i = 0; i < LED_COUNT; i++) {
    ledsNext[i] = CRGB::Black; // Spegni tutti i LED
  }

  for (int g = 0; g < LED_COUNT / GROUP_SIZE; g++) {
    int groupStart = g * GROUP_SIZE;
    if (groupStart + GROUP_SIZE <= LED_COUNT) {
      // Scegli un valore casuale dall'array di luminosità
      int brightness = candleBrightness[random(brightnessSize)];

      // Colori base con variazione casuale per un effetto più naturale
      int baseRed = 240 + random(-15, 16);   // Varia tra 225 e 255
      int baseGreen = 50 + random(-10, 11); // Varia tra 40 e 60
      int baseBlue = 10 + random(-5, 6);    // Varia tra 5 e 15

      for (int j = 0; j < GROUP_SIZE; j++) {
        ledsNext[groupStart + j] = CRGB(baseRed, baseGreen, baseBlue);
        ledsNext[groupStart + j].fadeLightBy(255 - brightness); // Applica la luminosità al colore
      }
    }
  }
}

void smoothTransition() {
  for (int step = 0; step < 8; step++) { // 8 passi per una transizione fluida
    for (int i = 0; i < LED_COUNT; i++) {
      // Interpolazione lineare tra la configurazione corrente e la successiva
      leds[i].r = (leds[i].r * (8 - step) + ledsNext[i].r * step) / 8;
      leds[i].g = (leds[i].g * (8 - step) + ledsNext[i].g * step) / 8;
      leds[i].b = (leds[i].b * (8 - step) + ledsNext[i].b * step) / 8;
    }
    FastLED.show();
    delay(50 / 8); // Dividi i 50 ms tra i passi
  }
}

void printLedStatus() {
  Serial.println("Stato dei LED:");
  for (int i = 0; i < LED_COUNT; i++) {
    Serial.print("LED ");
    Serial.print(i);
    Serial.print(": R=");
    Serial.print(leds[i].r);
    Serial.print(" G=");
    Serial.print(leds[i].g);
    Serial.print(" B=");
    Serial.println(leds[i].b);
  }
  Serial.println();
}
