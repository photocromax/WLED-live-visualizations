/*
   Live View
*/

uint16_t fpsMaxLimit = 60;
uint16_t delayMaxLimit = 0;//(uint16_t)(1000.0 / ((float)fpsMaxLimit*ledCount / multipartSize));

String prev_bufVisualization = "default";
uint32_t prev_millis = 0;
uint32_t multipartIndex = 0;





String getLiveLights() {
  String bufVisualization = "";
  if ((millis() > prev_millis + delayMaxLimit)  ||  (prev_bufVisualization == "default")) {  // if false serve empty buffer
    for (uint16_t i = 0; i < multipartSize && i + multipartIndex * multipartSize < ledCount; i++) {
      if (i == 0) {  // add extra fields
        bufVisualization = "{\"m\":" + String(strip.getMode()) + ", \"lc\":" + ledCount + ", \"leds\":[";
      } else {
        bufVisualization += ",";
      }
      CRGB fastled_col = strip.col_to_crgb(strip.getPixelColor(i + multipartIndex * multipartSize));
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
    if (bufVisualization != prev_bufVisualization || ledCount > multipartSize ) { //if there is a new strip state or strip is longer than multipartiSize
      prev_bufVisualization = bufVisualization;  //update the previous buffer
      if (ledCount > multipartSize ) {
        multipartIndex++;
        if (multipartIndex * multipartSize >= ledCount) {
          multipartIndex = 0;
        }
      }
      // add millis since last update (maybe for future optimizations)
      uint32_t m = millis();
      bufVisualization += "], \"mpi\":" + String(multipartIndex) + "";
      bufVisualization += ", \"mi\":" + String(m - prev_millis);
      bufVisualization += "}";
      prev_millis = m;
      return bufVisualization;
    }
  }
  return String("");
}



void serveLiveView(AsyncWebServerRequest * request)
{
  request->send_P(200, "text/html", PAGE_liveView, settingsProcessor);
}
