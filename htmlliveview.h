/*
 * LiveView html
 */

// Live View
const char PAGE_liveView[] PROGMEM = R"=====(<!DOCTYPE html>
<html><head><meta name="viewport" content="width=500" charset="utf-8" >
<meta http-equiv="refresh" content="3600">
<style>html, body {margin: 0;padding: 0;justify-content: center;}canvas {display: block;}</style>
<title>Live View</title>
<body>
<canvas id="liveCanvas"  width="304" height="16" style="width: 304px; height: 16px; position: absolute; top: 0px; left: 0px; right: 0px;  margin: auto;"></canvas>
<script>
var namespace = {};
url = "/json/live";  // live strip state json format 
lightSize = 16; // light box size
numLights = 1; 
prev_leds = ([0,"#000000",-1]);  //previous strip state
namespace.strip = {"leds":[0,"#ffffff",0]}; //arbitrary strip state
var canvas = document.getElementById("liveCanvas");  

function loadJSON(url,cbk,cbkError){
    var httpReq = new XMLHttpRequest(); 
    httpReq.overrideMimeType("application/json");
    httpReq.addEventListener("error", cbkError, false);
    httpReq.open("GET",url,true);
    httpReq.responseType="json";
    httpReq.onload=function(e) {
      var json= httpReq.response;
      cbk(json);
    }
    httpReq.onerror=function(e) {
     cbkError(httpReq.error);
    }
    httpReq.send();   
}

loadJSON(url,drawLights,errorDrawLights); //get actual strip state

function resizeCanvas(w,h,noRedraw){
 canvas.width=w;
 canvas.height=h;
 canvas.style.width=w+"px";
 canvas.style.height=h+"px";
}

function renderLight(light, col) {
  var ctx = canvas.getContext("2d");
  ctx.fillStyle = col;
  ctx.fillRect(light * lightSize, 0, lightSize, lightSize); //create the square representing the light (left, top, width, height)
  ctx.strokeStyle = "color";
  ctx.lineWidth = 1;
  ctx.strokeRect(light * lightSize, 0, lightSize, lightSize); //create the stroke (left, top, width, height)
}

function errorDrawLights(err){
  window.location.reload();  //reload the page
  console.log("frame reloaded");
}

function drawLights(strip) {
  if( prev_leds==strip.leds) {
    mi=0; //millis since last frame
  } else {
    if ( numLights != strip.leds.length-2) {
      numLights = strip.leds.length-2; 
      resizeCanvas(numLights * lightSize, lightSize, true);
    }
    for (i = 0; i < numLights+2; i++) {
      if(i==0) { 
        m=strip.leds[i];  // get effect mode index
      } else if (i==numLights+1){
          mi=strip.leds[i]; //millis since last frame
      } else {
      renderLight(i-1, strip.leds[i]);
      }
    }
    prev_leds=strip.leds; //update previpus strip state with actual state
    delete strip;  // local variable to the GC
    delete namespace.strip; // global variable to the GC
  }
  loadJSON(url,drawLights,errorDrawLights);  //request new  strip state
}
</script>
</body>
</html>)=====";
