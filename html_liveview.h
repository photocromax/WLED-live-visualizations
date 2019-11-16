/*
   LiveView html
*/

// Live View
const char PAGE_liveView[] PROGMEM = R"=====(<!DOCTYPE html>
<html><head><meta name=viewport content="width=100, initial-scale=1" charset=utf-8>
<meta http-equiv=refresh content=3600>
<style>html,body{margin:0;padding:0}canvas{display:block}</style>
<title>Live View</title>
<body>
<canvas id=liveCanvas width=5 height=5 style=display:block;position:absolute;top:0px;left:0px;margin:0px></canvas>
<script>var windowWidth=500;var windowHeight=500;var bgColor="#222";var lightSize=%LS%;var lines=0;var multipartSize=%MPS%;var lightPerLines=1;var numLights=0;var canvas;var ctx;var fx=-1;var pal=-1;var mi=-1;var mpi=0;var hidden,state,visibilityChange;var play=true;prev_leds=(["#000000"]);host="";function pcm_loadJSON(cbk,cbkError){var httpReq=new XMLHttpRequest();httpReq.overrideMimeType("application/json");httpReq.addEventListener("error",cbkError,false);httpReq.open("GET",host+"/json/live",true);httpReq.responseType="json";httpReq.onload=function(e){var json=httpReq.response;cbk(json);}
httpReq.onerror=function(e){cbkError(httpReq.error);}
httpReq.send();}
function pcm_resizeCanvas(noRedraw){if(!noRedraw)pcm_clearCanvas(canvas);lightsPerLine=Math.floor((windowWidth-1)/lightSize);if(numLights<lightsPerLine&&numLights>0)lightsPerLine=numLights;lines=Math.ceil(numLights/lightsPerLine)
if(lines==0)lines=1;w=lightsPerLine*lightSize+1;h=lines*lightSize+1;canvas.width=w;canvas.height=h;canvas.style.width=w+"px";canvas.style.height=h+"px";canvas.style.left=Math.floor((windowWidth-w)/2)+"px";var iFrame=window.parent.document.getElementById("lvf");if(iFrame!=null){iFrame.style.height=h+"px";iFrame.height=h+"px";}}
function pcm_clearCanvas(cnv){ctx.clearRect(0,0,cnv.width,cnv.height);for(i=0;i<numLights;i++){renderLight(i,"",true);}}
function isOdd(n){if(Math.floor(n/2)*2!=n)return true;return false;}
function renderLight(light,col,stroke){gamma=2.2;r=parseInt(Math.pow(parseInt("0x"+col.substring(1,3))/255,gamma)*255);r="0"+r.toString(16);r=r.substring(r.length-2);g=parseInt(Math.pow(parseInt("0x"+col.substring(3,5))/255,gamma)*255);g="0"+g.toString(16);g=g.substring(g.length-2);b=parseInt(Math.pow(parseInt("0x"+col.substring(5,7))/255,gamma)*255);b="0"+b.toString(16);b=b.substring(b.length-2);colGamma="#"+r+g+b;line=Math.floor(light/lightsPerLine);if(isOdd(line)){reverse=lightsPerLine*lightSize-lightSize;drawDirection=-1;}else{reverse=0;drawDirection=1;}
x=reverse+drawDirection*((light%%lightsPerLine)*lightSize+0.5*drawDirection);y=line*lightSize+0.5;if(!stroke){ctx.fillStyle=colGamma;ctx.fillRect(x+0.5,y+0.5,lightSize-1,lightSize-1);}else{ctx.strokeStyle=bgColor;ctx.lineWidth=1;ctx.strokeRect(x,y,lightSize,lightSize);}}
function errorDrawLights(err){pcm_loadJSON(drawLights,errorDrawLights);}
function drawLights(strip){if(strip!=null){if(numLights!=strip.lc){numLights=strip.lc;pcm_resizeCanvas(false);}
else if(prev_leds==strip.leds&&pal==strip.pal&&fx==strip.fx&&mpi==strip.mpi){mi=0;}else{fx=strip.fx;pal=strip.pal;mpi=strip.mpi
mi=strip.mi;prev_leds=strip.leds;mpl=strip.leds.length;for(i=0;i<mpl;i++){renderLight(i+mpi*multipartSize,strip.leds[i],false);}}}
delete strip;if(play){pcm_loadJSON(drawLights,errorDrawLights);}}
function getWindowSize(){windowWidth=window.innerWidth||document.documentElement.clientWidth||document.body.clientWidth;windowHeight=window.innerHeight||document.documentElement.clientHeight||document.body.clientHeight;}
function windowResize(){getWindowSize();pcm_resizeCanvas(false);}
function visibilityHelper(){if(typeof document.hidden!=="undefined"){hidden="hidden";visibilityChange="visibilitychange";state="visibilityState";}else if(typeof document.mozHidden!=="undefined"){hidden="mozHidden";visibilityChange="mozvisibilitychange";state="mozVisibilityState";}else if(typeof document.msHidden!=="undefined"){hidden="msHidden";visibilityChange="msvisibilitychange";state="msVisibilityState";}else if(typeof document.webkitHidden!=="undefined"){hidden="webkitHidden";visibilityChange="webkitvisibilitychange";state="webkitVisibilityState";}}
function pcm_setup(){canvas=document.getElementById("liveCanvas");ctx=canvas.getContext("2d");window.onresize=windowResize;windowResize();visibilityHelper();document.addEventListener(visibilityChange,function(){if(document[state]=="hidden"){play=false;}else{play=true;pcm_loadJSON(drawLights,errorDrawLights);}});pcm_loadJSON(drawLights,errorDrawLights);}
pcm_setup();</script>
</body>
</html>)=====";
