#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Water_percent_96.h>
#include <Blume_Gemalt_0_20_127_240.h>
#include <Blume_Gemalt_20_40_127_240.h>
#include <Blume_Gemalt_40_60_127_240.h>
#include <Blume_Gemalt_60_80_127_240.h>
#include <Blume_Gemalt_80_100_127_240.h>


TFT_eSPI tft = TFT_eSPI();
TFT_eSprite BG = TFT_eSprite(&tft);
TFT_eSprite Waterpercenticon = TFT_eSprite(&tft);
TFT_eSprite WaterpercentNummber = TFT_eSprite(&tft);
TFT_eSprite Blume = TFT_eSprite(&tft);

#define TFTDISPLAYPOWER 32 // PIN from ESP
#define BUTTON_PIN 39   // PIN from ESP for deep slep and wacke up
#define timer_deep_sleep_in_hours 10//*3600 // hour * 3600 seconds for 1 hour

void setup() {
 Serial.begin(115200);
 pinMode(BUTTON_PIN, INPUT_PULLUP);
 tft.writecommand(TFT_DISPON); 
 pinMode(TFTDISPLAYPOWER, OUTPUT);



  // Check for button press
  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
    Serial.println("Aufgewacht durch den Knopf");
  } else {
    Serial.println("Normaler Neustart");
    Serial.print("hallo");
  }
delay(2000); 
  



  
// configur thr TFT Display
  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_DARKGREY);

  BG.createSprite(113,240);
  BG.setSwapBytes(true);
  BG.fillSprite(TFT_TRANSPARENT);
  BG.pushSprite(127, 0);

  // draw water icon on tft
  Waterpercenticon.createSprite(96, 96);
  Waterpercenticon.setSwapBytes(true);
  Waterpercenticon.pushImage(0, 0, 96, 96, Water_percent_96);
  Waterpercenticon.pushSprite(140, 110, TFT_BLACK);

  // create spritr for tft Display
  WaterpercentNummber.createSprite(80, 60);
  WaterpercentNummber.setTextSize(7);

  //create spritr for tft Display
  Blume.createSprite(127, 240);

}


long previuswaterpercentwert = -1;  // for chek if the nummber changet
long prozentwert;

int blumenanzeigenummer;
long previusblumenanzeigenummer = -1;

void loop() {
  digitalWrite(TFTDISPLAYPOWER, HIGH);
  Serial.println();

    // deepsleep check
  if (digitalRead(BUTTON_PIN) == LOW && blumenanzeigenummer != 1) {
    Serial.println("Knopf gedrückt, gehe in den Deep-Sleep");
    tft.writecommand(TFT_DISPOFF); 
    digitalWrite(TFTDISPLAYPOWER, LOW);
    delay(2000);  

    // deepsleep wacke up per putton
    esp_sleep_enable_ext0_wakeup((gpio_num_t)BUTTON_PIN, LOW);
    // 
    uint64_t timer_wakeup_deepsleep = static_cast<uint64_t>(timer_deep_sleep_in_hours) * 1000000;  
    // deepsleep wacke up after timmer after button press
    esp_sleep_enable_timer_wakeup(timer_wakeup_deepsleep); // 1000000 micro seconds for 1 second 

    // Go to deepsleep
    esp_deep_sleep_start();
  }



// water Sennsor input
  long analogwert= analogRead(A0);
  Serial.print("Analogwert: ");
  Serial.print(analogwert);
  Serial.println();
  prozentwert = map(analogwert, 900, 2500, 100, 0);

  // change watter icon if needed
  blumenanzeigenummer = map(prozentwert,100, 0, 5, 0);

  // change watterpercent number if nedet
  if (prozentwert != previuswaterpercentwert) {
    
    if (blumenanzeigenummer ==1 ){
      WaterpercentNummber.fillSprite(TFT_RED); 
      WaterpercentNummber.setTextColor(TFT_BLACK, TFT_RED);

      BG.setSwapBytes(true);
      BG.fillSprite(TFT_RED);
      BG.pushSprite(127, 0);
      Waterpercenticon.pushSprite(140, 110, TFT_BLACK);
    } 
    else{
      WaterpercentNummber.fillSprite(TFT_DARKGREY); 
      WaterpercentNummber.setTextColor(TFT_BLACK, TFT_DARKGREY);

      BG.setSwapBytes(true);
      BG.fillSprite(TFT_DARKGREY);
      BG.pushSprite(127, 0);
      Waterpercenticon.pushSprite(140, 110, TFT_BLACK);
    }
    
    
    // draw new number
    WaterpercentNummber.drawNumber(prozentwert, 0,0);

    // new nummber to display
    WaterpercentNummber.pushSprite(140, 48);

    // save ald nummber
    previuswaterpercentwert = prozentwert;
  }



  if (blumenanzeigenummer != previusblumenanzeigenummer) {
    if (blumenanzeigenummer == 1){
      Blume.fillSprite(TFT_RED); 
    }
    else
      Blume.fillSprite(TFT_DARKGREY); 
    Blume.pushSprite(0, 0, TFT_WHITE);
    
    switch(blumenanzeigenummer){
      //0-20%
      case(1):
        Blume.setSwapBytes(true);
        Blume.fillSprite(TFT_RED);
        Blume.pushImage(0, 0, 127, 240, Blume_Gemalt_0_20_127_240);
        Blume.pushSprite(0, 0, TFT_WHITE);
        break;
    }
    //20-40%
    switch(blumenanzeigenummer){
      case(2):
        Blume.setSwapBytes(true);
        Blume.fillSprite(TFT_DARKGREY);
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

    // save icon nummber 
    previusblumenanzeigenummer = blumenanzeigenummer;
  }


delay(200);
  Serial.print("prozentwert: ");
  Serial.print(prozentwert);
  Serial.println();
  Serial.print("blumenanzeigenummer: ");
  Serial.print(blumenanzeigenummer);
  Serial.println();
  Serial.println();

}
