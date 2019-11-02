wled00

------
add

#ifndef WLED_DISABLE_LIVEVIEW
  #include "html_liveview.h"
#endif

------
------

FX.h

------
function moved to public
CRGB 
      col_to_crgb(uint32_t);

------
------

wled18_server

------
add

server.on("/liveview", HTTP_GET, [](AsyncWebServerRequest *request){
    serveLiveView(request);
  });

------
------


index.html (to be added before "FX mode" or in any other useful place)

------
<iframe id="lv" scrolling="no" src="/liveview" style=" border-style: none;  justify-content: center; width: 100%; height: 35px; "></iframe>
    
------
