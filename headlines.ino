// The Fonts file (M5EPD_TTF) used in the example is in the same path as the
// program, please install Fonts file in TF card or SPIFFS before compiling.

#include "M5EPD.h"
#include "FS.h"
#include "SPIFFS.h"
#include "binaryttf.h"


M5EPD_Canvas canvas(&M5.EPD);
void shutdown()
{
  M5.disableEPDPower();  // digitalWrite(M5EPD_EPD_PWR_EN_PIN, 0);
  M5.disableEXTPower();  // digitalWrite(M5EPD_EXT_PWR_EN_PIN, 0);
  M5.disableMainPower(); // digitalWrite(M5EPD_MAIN_PWR_PIN, 1);
  esp_deep_sleep_start();
}

void displayClock() {
  M5.EPD.Clear(true);
  canvas.deleteCanvas();
  canvas.createCanvas(520, 940);
  canvas.createRender(64, 256);
  canvas.setTextSize(64);
  canvas.setTextDatum(MC_DATUM);
  canvas.drawString("10:15 PM", 270, 480);
  canvas.pushCanvas(10, 10, UPDATE_MODE_GC16);
}
void displayStory() {
    String text = "Once upon a time, in a peaceful forest, lived a swift rabbit and a slow tortoise. The rabbit, proud of his speed, often teased the tortoise for his slowness. Irritated by the rabbit's arrogance, the tortoise proposed a race to settle the matter once and for all. The rabbit, amused by the idea, readily agreed, confident in his ability to win easily. As the race commenced, the rabbit dashed ahead, leaving the tortoise far behind. His confidence soared as he watched the slow progress of his competitor. Feeling assured of victory, the rabbit decided to take a nap under a shady tree, believing he had ample time to rest and still win. Meanwhile, the tortoise trudged along steadily, undeterred by the rabbit's lead. Slow and steady, he focused on the path ahead, determined to reach the finish line. As the sun climbed higher, the rabbit's slumber deepened, oblivious to the passing time. Unexpectedly, the rabbit awoke to find the tortoise near the finish line. Shocked and panicked, he sprinted with all his might, but it was too late. With a final surge of determination, the tortoise crossed the finish line, winning the race. The forest erupted in cheers as the tortoise celebrated his victory.";
    canvas.createRender(32, 256);
    canvas.setTextSize(32);
    canvas.setTextDatum(MC_DATUM);
    canvas.print(text);
    canvas.pushCanvas(10, 10, UPDATE_MODE_GC16);
}
void setup() {
    M5.begin(false, false, false, false, false);
    M5.EPD.SetRotation(90);
    M5.EPD.Clear(true);
    canvas.loadFont(binaryttf, sizeof(binaryttf));  // Load font files from binary data
    canvas.createCanvas(520, 940);
   
    displayStory();
    
    delay(10000);

    displayClock();

    delay(2000);

    shutdown();
}

void loop() {
}