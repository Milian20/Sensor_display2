#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Water_percent_96.h>
#include <Blume_Gemalt_0_20_127_240.h>
#include <Blume_Gemalt_20_40_127_240.h>
#include <Blume_Gemalt_40_60_127_240.h>
#include <Blume_Gemalt_60_80_127_240.h>
#include <Blume_Gemalt_80_100_127_240.h>



TFT_eSPI tft = TFT_eSPI();
TFT_eSprite Waterpercenticon = TFT_eSprite(&tft);
TFT_eSprite WaterpercentNummber = TFT_eSprite(&tft);
TFT_eSprite Blume = TFT_eSprite(&tft);

#define TFTDISPLAYPOWER 32
#define BUTTON_PIN 39   // Knopf an GPIO 0 

void setup() {
 Serial.begin(115200);
 pinMode(BUTTON_PIN, INPUT_PULLUP);
 tft.writecommand(TFT_DISPON); 
 pinMode(TFTDISPLAYPOWER, OUTPUT);
  // Konfiguriere den Knopf als Eingabegerät mit Pull-up-Widerstand


  // Prüfen, ob das Aufwachen durch den Knopf verursacht wurde
  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
    Serial.println("Aufgewacht durch den Knopf");
  } else {
    Serial.println("Normaler Neustart");
  }

  
  
  delay(2000);  // Kleiner Start-Delay, bevor der Knopf erneut geprüft wird


  

  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_DARKGREY);

  // Wasser Icon einmal zeichnen
  Waterpercenticon.createSprite(96, 96);
  Waterpercenticon.setSwapBytes(true);
  Waterpercenticon.pushImage(0, 0, 96, 96, Water_percent_96);
  Waterpercenticon.pushSprite(140, 110, TFT_BLACK);

  // Sprite für die Nummer einmal erstellen (größere Fläche für größere Zahlen)
  WaterpercentNummber.createSprite(80, 60);
  WaterpercentNummber.setTextSize(7);

  //Blume sprite
  Blume.createSprite(127, 240);

  //Test zwecke
  //tft.drawLine(100,0,100,160, TFT_BLACK);

}


long previuswaterpercentwert = -1;  // Um nur zu aktualisieren, wenn sich die Zahl ändert
//long analogwert= 500;
long prozentwert;

int blumenanzeigenummer;
long previusblumenanzeigenummer = -1;

void loop() {
  digitalWrite(TFTDISPLAYPOWER, HIGH);
  Serial.println();
  
  // Überprüfen, ob der Knopf gedrückt wurde (LOW bedeutet gedrückt)
  if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.println("Knopf gedrückt, gehe in den Deep-Sleep");
    tft.writecommand(TFT_DISPOFF); 
    digitalWrite(TFTDISPLAYPOWER, LOW);
    delay(500);  // Kurze Pause, um Prellen zu vermeiden

    // Konfiguriere den Knopf als Aufweckquelle aus dem Deep-Sleep
    esp_sleep_enable_ext0_wakeup((gpio_num_t)BUTTON_PIN, LOW);  

    // Gehe in den Deep-Sleep-Modus
    esp_deep_sleep_start();
  }

  long analogwert= analogRead(A0);
  Serial.print("Analogwert: ");
  Serial.print(analogwert);
  Serial.println();
  //Sensor Wert bearbeitung
  prozentwert = map(analogwert, 900, 2500, 100, 0);

  Serial.print(digitalRead(TFTDISPLAYPOWER));

  // Prüfen, ob die Zahl sich geändert hat, um unnötiges Zeichnen zu vermeiden
  if (prozentwert != previuswaterpercentwert) {
    // Alte Zahl mit Hintergrundfarbe komplett löschen (Sprite mit Hintergrundfarbe füllen)
    WaterpercentNummber.fillSprite(TFT_DARKGREY); 
    WaterpercentNummber.setTextColor(TFT_BLACK, TFT_DARKGREY);
    
    // Neue Zahl zeichnen
    WaterpercentNummber.drawNumber(prozentwert, 0,0);

    // Neue Zahl auf den Bildschirm pushen
    WaterpercentNummber.pushSprite(140, 48);

    // Speichern der aktuellen Zahl
    previuswaterpercentwert = prozentwert;
  }


  blumenanzeigenummer = map(prozentwert,100, 0, 5, 0);

  if (blumenanzeigenummer != previusblumenanzeigenummer) {
    Blume.fillSprite(TFT_DARKGREY); 
    Blume.pushSprite(0, 0, TFT_WHITE);
    
    switch(blumenanzeigenummer){
      //0-20%
      case(1):
        Blume.setSwapBytes(true);
        Blume.pushImage(0, 0, 127, 240, Blume_Gemalt_0_20_127_240);
        Blume.pushSprite(0, 0, TFT_WHITE);
        break;
    }
    //20-40%
    switch(blumenanzeigenummer){
      case(2):
        Blume.setSwapBytes(true);
        Blume.pushImage(0, 0, 127, 240, Blume_Gemalt_20_40_127_240);
        Blume.pushSprite(0, 0, TFT_WHITE);
        break;
    }
    //40-60%
    switch(blumenanzeigenummer){
      case(3):
        Blume.setSwapBytes(true);
        Blume.pushImage(0, 0, 127, 240, Blume_Gemalt_40_60_127_240);
        Blume.pushSprite(0, 0, TFT_WHITE);
        break;
    }
    //60-80%
    switch(blumenanzeigenummer){
      case(4):
        Blume.setSwapBytes(true);
        Blume.pushImage(0, 0, 127, 240, Blume_Gemalt_60_80_127_240);
        Blume.pushSprite(0, 0, TFT_WHITE);
        break;
    }
    //80-100%
    switch(blumenanzeigenummer){
      case(5):
        Blume.setSwapBytes(true);
        Blume.pushImage(0, 0, 127, 240, Blume_Gemalt_80_100_127_240);
        Blume.pushSprite(0, 0, TFT_WHITE);
        break;
    }

    // Speichern der aktuellen Zahl
    previusblumenanzeigenummer = blumenanzeigenummer;
  }





  delay(500); 
  analogwert--;
  if (prozentwert >99)
  {
    analogwert = 615;
  }
  Serial.print("prozentwert: ");
  Serial.print(prozentwert);
  Serial.println();
  Serial.print("blumenanzeigenummer: ");
  Serial.print(blumenanzeigenummer);
  Serial.println();
  Serial.println();

}
