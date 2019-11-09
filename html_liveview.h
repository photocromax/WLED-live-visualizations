/*
   LiveView html
*/

// Live View
const char PAGE_liveView[] PROGMEM = R"=====(<!DOCTYPE html>
<html><head><meta name="viewport" content="width=500" charset="utf-8" >
<meta http-equiv="refresh" content="3600">
<style>html, body {margin: 0;padding: 0;justify-content: center;} canvas{display:block;}</style>
<title>Live View</title>
<body style="background-color:#333;font-family:Helvetica,Verdana,sans-serif;color:white" >
<canvas id="liveCanvas"  width="5" height="5" style="width: 5px; height: 5px; position: absolute; top: 0px; left: 0px; right: 0px;  margin: auto;"></canvas>

<script>

var windowWidth = 500;
var windowHeight = 500;
var lightSize = 16; // light box size
var lines=0;
var multipartSize=%MPS%;  //refer to wled21_liveview multipartSize global variable. (it has to be the same value)
var lightPerLines=1;
var numLights = 0; 
var canvas ;  
var ctx ;
var fx=-1;
var pal=-1;
var mi=-1;
var mpi=0;

prev_leds = (["#000000"]);  //previous strip state
host="";
function pcm_loadJSON(cbk,cbkError){
    var httpReq = new XMLHttpRequest(); 
    httpReq.overrideMimeType("application/json");
    httpReq.addEventListener("error", cbkError, false);
    httpReq.open("GET",host+"/json/live",true);
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
function pcm_resizeCanvas(noRedraw){
  if (!noRedraw) {
   pcm_clearCanvas(canvas);
  }
  lightsPerLine = Math.floor(windowWidth / lightSize ); 
  lines = Math.ceil(numLights / lightsPerLine)
  if (lines == 0) {
    lines=1;
  }  
  w = lightsPerLine*lightSize+1;
  h = lines*lightSize+1;
  canvas.width=w;
  canvas.height=h;
  canvas.style.width=w+"px";
  canvas.style.height=h+"px";
}


function pcm_clearCanvas(cnv) {
 ctx.clearRect(0,0,cnv.width,cnv.height);
  for (i = 0; i < numLights ; i++) {
       renderLight(i, "",true);
  }
}

function isOdd(n) {
  if (Math.floor(n/2)*2 != n) return true;
  return false;
}

function renderLight(light, col, stroke) {
  line=Math.floor(light / lightsPerLine);
    if (isOdd(line)){
      reverse=lightsPerLine*lightSize-lightSize;
      drawDirection=-1;
    } else {
    reverse=0;
    drawDirection=1;
  }
  x = reverse+drawDirection*((light %% lightsPerLine) * lightSize+0.5*drawDirection);
  y = line * lightSize+0.5;
  if (!stroke){
    ctx.fillStyle = col; 
    ctx.fillRect(x+0.5, y+0.5, lightSize-1, lightSize-1); //create the square representing the light (left, top, width, height)
  } else {
    ctx.strokeStyle = "#333333";
    ctx.lineWidth = 1;
    ctx.strokeRect(x, y, lightSize, lightSize); //create the stroke (left, top, width, height)
  }
}


function errorDrawLights(err){
  pcm_loadJSON(drawLights,errorDrawLights); //get actual strip state AGAIN
}

function drawLights(strip) {
  try {
    if (strip === null) throw "empty json";
  }
  catch(err) {
    delete strip;  // local variable to the GC
    errorDrawLights();
    //console.log(err);
    return "";
  }
  if ( numLights != strip.lc) {
      numLights = strip.lc;  
      pcm_resizeCanvas(false);
  }
  else if( prev_leds==strip.leds && pl==strip.pl && fx==strip.fx && mpi==strip.mpi) {
    mi=0; //millis since last frame
  } else {
    fx=strip.fx;  // effect mode index
    pal=strip.pal;  // palette index
    mpi=strip.mpi //multipartIndex
    mi=strip.mi; // millis since last frame
    prev_leds=strip.leds; //update previous strip state with actual state
    mpl=strip.leds.length; // multipart length 
        for (i = 0; i < mpl; i++) {
       renderLight(i+mpi*multipartSize, strip.leds[i],false);
    }
  }
  delete strip;  // local variable to the GC
  pcm_loadJSON(drawLights,errorDrawLights);  //request new  strip state
}

function getWindowSize(){
  windowWidth = (window.innerWidth || document.documentElement.clientWidth || document.body.clientWidth)-30;
  windowHeight = window.innerHeight || document.documentElement.clientHeight || document.body.clientHeight;
}

function windowResize(){
   getWindowSize();
   pcm_resizeCanvas(false);  
 }  

function pcm_setup() {
  canvas = document.getElementById("liveCanvas");  
  ctx = canvas.getContext("2d");
  window.onresize = windowResize;
  getWindowSize();
  pcm_resizeCanvas(false);
  pcm_loadJSON(drawLights,errorDrawLights); //get actual strip state
}

pcm_setup();
</script>
</body>
</html>)=====";
