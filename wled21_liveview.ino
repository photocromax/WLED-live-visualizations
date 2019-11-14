/*
   Live View
*/
#ifndef WLED_DISABLE_LIVEVIEW

uint16_t fpsMaxLimit = 60;
uint16_t delayMaxLimit = 0;//(uint16_t)(1000.0 / ((float)fpsMaxLimit*ledCount / multipartSize));

String prev_bufVisualization = "default";
uint32_t prev_millis = 0;
uint32_t multipartIndex = 0;
uint32_t maxCacheLife = 1000;

String gammaCorrectHex(uint8_t col) {
  String sCol = "0" + String((uint8_t)(pow(((float)col  / 255), 2.2) * 255), HEX);
  return sCol.substring(sCol.length() - 2);
}

String getLiveLights() {
  String bufVisualization = "";
  if ((millis() > prev_millis + delayMaxLimit)  ||  (prev_bufVisualization == "default")) {  // if false serve empty buffer
    for (uint16_t i = 0; (i < multipartSize) && (i + multipartIndex * multipartSize < ledCount); i++) {
      if (i == 0) {  // add extra fields
        bufVisualization = "{\"fx\":" + String(strip.getMode()) + ", \"pal\":" + strip.getPalette() + ", \"lc\":" + ledCount + ", \"leds\":[";
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
      //col = gammaCorrectHex(fastled_col.red);
      bufVisualization += col.substring(col.length() - 2);
      
      col = "0" + String(fastled_col.green, HEX);
      //col = gammaCorrectHex(fastled_col.green);
      bufVisualization += col.substring(col.length() - 2);

      col = "0" + String(fastled_col.blue, HEX);
      //col = gammaCorrectHex(fastled_col.blue);
      bufVisualization += col.substring(col.length() - 2);
      bufVisualization += "\"";
    }
    if (bufVisualization != prev_bufVisualization || ledCount > multipartSize || millis()>prev_millis+maxCacheLife ) { //if there is a new strip state or strip is longer than multipartiSize
      prev_bufVisualization = bufVisualization;  //update the previous buffer
      bufVisualization += "], \"mpi\":" + String(multipartIndex) + "";  
      if (ledCount > multipartSize ) {
        multipartIndex++;
        if (multipartIndex * multipartSize >= ledCount) {
          multipartIndex = 0;  
        }
      } else {
        multipartIndex = 0;
      }
       // add millis since last update (maybe for future optimizations)
      uint32_t m = millis();
      bufVisualization += ", \"mi\":" + String(m - prev_millis);
      bufVisualization += "}";
      prev_millis = m;
      return bufVisualization;
    }
  }
  return String("");
}

/*
  String getLiveLightsJson() {
  //StaticJsonDocument<JSONlive> jsonLive;

  // StaticJsonObject allocates memory on the stack, it can be
  // replaced by DynamicJsonDocument which allocates in the heap.
  //
  StaticJsonDocument<JSON_OBJECT_SIZE(10)> jsonLive;
  if ((millis() > prev_millis + delayMaxLimit)  ||  (prev_bufVisualization == "default")) {  // if false serve the old buffer
    JsonArray leds;
    for (uint16_t i = 0; i < multipartSize && i + multipartIndex * multipartSize < ledCount; i++) {
      if (i == 0) {  // add extra fields
        jsonLive["m"] = strip.getMode();
        jsonLive["lc"] = ledCount;
        jsonLive["mpi"] = multipartIndex;
        leds = jsonLive.createNestedArray("leds");
      }
      CRGB fastled_col = strip.col_to_crgb(strip.getPixelColor(i + multipartIndex * multipartSize));
      // create the color in hex notation
      String col = "";
      String bufCol = "";
      col = "0" + String(fastled_col.red, HEX);
      bufCol += col.substring(col.length() - 2);
      col = "0" + String(fastled_col.green, HEX);
      bufCol += col.substring(col.length() - 2);
      col = "0" + String(fastled_col.blue, HEX);
      bufCol += col.substring(col.length() - 2);
      leds.add(bufCol);
    }
    prev_jsonLive = jsonLive;

    // add millis since last update (maybe for future optimizations)
    uint32_t m = millis();
    jsonLive["mpi"] = multipartIndex;
    jsonLive["mi"] = m - prev_millis;
    prev_millis = m;
    multipartIndex++;
    if (multipartIndex * multipartSize >= ledCount) {
      multipartIndex = 0;
    }
  }
  uint32_t serJsonSize = measureJson(jsonLive);
  char ret[serJsonSize];
  serializeJson(jsonLive, ret[serJsonSize]);
  return ret;

  }
*/
  #ifndef WLED_DISABLE_INTERNAL_LIVEVIEW
void serveLiveView(AsyncWebServerRequest * request)
{
  request->send_P(200, "text/html", PAGE_liveView, pageProcessor);
}
  #endif
#endif
