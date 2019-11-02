/*
   Live View
*/

uint8_t fpsMaxLimit = 200;
uint8_t delayMaxLimit = (uint8_t)(1000.0 / fpsMaxLimit);
String prev_bufVisualization = "default";
uint32_t prev_millis = 0;


String getLiveLights() {
  String bufVisualization = "";
  if ((millis() > prev_millis + delayMaxLimit)  ||  (prev_bufVisualization == "default")) {  // if false serve the old buffer
    for (uint16_t i = 0; i < ledCount; i++) {
      if (i == 0) {  // index 0 will contains the effect number
        bufVisualization = "{\"leds\":[" + String(strip.getMode()) + ",";
      } else {
        bufVisualization += ",";
      }
      CRGB fastled_col = strip.col_to_crgb(strip.getPixelColor(i));
      String col = "";
      bufVisualization += "\"";
      //bufVisualization += strip.getPixelColor(i);
      // create the color in hex notation
      bufVisualization += "#";
      col = "0" + String(fastled_col.red, HEX);
      bufVisualization += col.substring(col.length() - 2);
      col = "0" + String(fastled_col.green, HEX);
      bufVisualization += col.substring(col.length() - 2);
      col = "0" + String(fastled_col.blue, HEX);
      bufVisualization += col.substring(col.length() - 2);
      bufVisualization += "\"";
    }
    if (bufVisualization != prev_bufVisualization ) {  //if there is a new strip state
      prev_bufVisualization = bufVisualization;  //update the previous buffer
    }
  } else {
    bufVisualization = prev_bufVisualization; // serve the old buffer
  }

  // add millis since last update (maybe for future optimizations)
  uint32_t m = millis();
  bufVisualization += "," + String(m - prev_millis);
  prev_millis = m;

  bufVisualization += "]}";
  return bufVisualization;
}

void serveLiveView(AsyncWebServerRequest* request)
{
  request->send_P(200, "text/html", PAGE_liveView);
}
