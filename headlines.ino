// The Fonts file (M5EPD_TTF) used in the example is in the same path as the
// program, please install Fonts file in TF card or SPIFFS before compiling.

#include "M5EPD.h"
#include "FS.h"
#include "SPIFFS.h"
#include "binaryttf.h"
#include <WiFi.h>
#include "config.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>

M5EPD_Canvas canvas(&M5.EPD);
rtc_time_t RTCtime;
DynamicJsonDocument doc(1024); // Adjust size as needed
int feedIndex = 0;

void shutdown()
{
  M5.disableEPDPower();  // digitalWrite(M5EPD_EPD_PWR_EN_PIN, 0);
  M5.disableEXTPower();  // digitalWrite(M5EPD_EXT_PWR_EN_PIN, 0);
  M5.disableMainPower(); // digitalWrite(M5EPD_MAIN_PWR_PIN, 1);
  esp_deep_sleep_start();
}

void displayClock()
{
  M5.EPD.Clear(true);
  canvas.deleteCanvas();
  canvas.createCanvas(520, 940);
  canvas.createRender(64, 256);
  canvas.setTextSize(64);
  canvas.setTextDatum(MC_DATUM);
  char timeStrbuff[64];
  M5.RTC.getTime(&RTCtime);
  sprintf(timeStrbuff, "%02d:%02d", RTCtime.hour, RTCtime.min);
  Serial.print(timeStrbuff);
  canvas.drawString(timeStrbuff, 270, 480);
  canvas.pushCanvas(10, 10, UPDATE_MODE_GC16);
}

void displayImage()
{
  M5.EPD.Clear(true);
  canvas.deleteCanvas();
  canvas.createCanvas(540, 960);
  // canvas.drawJpgUrl("https://i.imgur.com/wBmYZJn.jpeg"); // shajan
  canvas.drawJpgUrl("https://i.imgur.com/5en8XJ1.jpeg"); // leaf
  canvas.pushCanvas(0, 0, UPDATE_MODE_GC16);
}

void displayStory(String text)
{
  M5.EPD.Clear(true);
  // String text = "Once upon a time, in a peaceful forest, lived a swift rabbit and a slow tortoise. The rabbit, proud of his speed, often teased the tortoise for his slowness. Irritated by the rabbit's arrogance, the tortoise proposed a race to settle the matter once and for all. The rabbit, amused by the idea, readily agreed, confident in his ability to win easily. As the race commenced, the rabbit dashed ahead, leaving the tortoise far behind. His confidence soared as he watched the slow progress of his competitor. Feeling assured of victory, the rabbit decided to take a nap under a shady tree, believing he had ample time to rest and still win. Meanwhile, the tortoise trudged along steadily, undeterred by the rabbit's lead. Slow and steady, he focused on the path ahead, determined to reach the finish line. As the sun climbed higher, the rabbit's slumber deepened, oblivious to the passing time. Unexpectedly, the rabbit awoke to find the tortoise near the finish line. Shocked and panicked, he sprinted with all his might, but it was too late. With a final surge of determination, the tortoise crossed the finish line, winning the race. The forest erupted in cheers as the tortoise celebrated his victory.";
 canvas.deleteCanvas();
  canvas.createCanvas(540, 960);
  canvas.createRender(32, 256);
  canvas.setTextSize(32);
  canvas.setTextDatum(MC_DATUM);
  canvas.print(text);
  canvas.pushCanvas(10, 10, UPDATE_MODE_GC16);
}

void connectToWifi()
{
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void setTime()
{
 
  RTCtime.hour = 16;
  RTCtime.min = 19;
  RTCtime.sec = 00;
  M5.RTC.setTime(&RTCtime);
}

void setup()
{
  M5.begin(false, false, true, false, false);
  M5.RTC.begin();
  Serial.begin(115200);

  connectToWifi();
  getAndSetFeedData();
  M5.EPD.SetRotation(90);
  M5.EPD.Clear(true);
  canvas.loadFont(binaryttf, sizeof(binaryttf)); // Load font files from binary data
  canvas.createCanvas(520, 940);

  displayStory(doc[feedIndex]["content"]);
}

void getAndSetFeedData(){
  // Your API endpoint
  String url = "https://solstice-ahead-swallow.glitch.me/feed";
  
  // Send the HTTP request
  HTTPClient http;
  http.begin(url);
  int httpResponseCode = http.GET();
  
  // Check for successful response
  if (httpResponseCode > 0) {
    // Parse JSON
    String payload = http.getString();
    deserializeJson(doc, payload);
    
    // Loop through the array
    for (int i = 0; i < doc.size(); i++) {
      JsonObject obj = doc[i];
      // Extract values
      String category = obj["category"];
      String icon = obj["icon"];
      String content = obj["content"];
      
      // Print values
      Serial.print("Category: ");
      Serial.println(category);
      Serial.print("Icon URL: ");
      Serial.println(icon);
      Serial.print("Content: ");
      Serial.println(content);
    }
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}

void loop()
{
  M5.update();
  
  if (M5.BtnP.wasPressed())
  {
    Serial.println("Left button pressed");
    displayImage();
    delay(1000);
    shutdown();
  }

  if (M5.BtnR.wasPressed())
  {
    Serial.println("Left button pressed");
    displayStory(doc[feedIndex]["content"]);
    Serial.println(feedIndex);
    feedIndex++;
    if(feedIndex == doc.size()) {
      feedIndex = 0;
    }
  }

  delay(50);
}