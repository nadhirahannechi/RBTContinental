var myColor = ["#39ca74","#e54d42","#c1eefc"];
var myData = [passed,failed,notExecuted];
var myLabel = ["PASSED","FAILED","NOT EXECUTED"];
function getTotal(){
var myTotal = 0;
for (var j = 0; j < myData.length; j++) {
myTotal += (typeof myData[j] == 'number') ? myData[j] : 0;
}
return myTotal;
}
function plotData() {
var canvas;
var ctx;
var lastend = 0;
var myTotal = getTotal();
var doc;
canvas = document.getElementById("canvas");
var x = (canvas.width)/2;
var y = (canvas.height)/2;
var r = 150;
ctx = canvas.getContext("2d");
ctx.clearRect(0, 0, canvas.width, canvas.height);
for (var i = 0; i < myData.length; i++) {
if (myData[i] != 0){ctx.fillStyle = myColor[i];
ctx.beginPath();
ctx.moveTo(x,y);
ctx.arc(x,y,r,lastend,lastend+(Math.PI*2*(myData[i]/myTotal)),false);
ctx.lineTo(x,y);
ctx.fill();
ctx.beginPath();
var start = [];
var end = [];
var last = 0;
var flip = 0;
var textOffset = 0;
var precentage = (myData[i]/myTotal)*100;
start = getPoint(x,y,r-20,(lastend+(Math.PI*2*(myData[i]/myTotal))/2));
end = getPoint(x,y,r+20,(lastend+(Math.PI*2*(myData[i]/myTotal))/2));
if(start[0] <= x)
{
flip = -1;
textOffset = -110;
}
else
{
flip = 1;
textOffset = 10;
}
ctx.moveTo(start[0],start[1]);
ctx.lineTo(end[0],end[1]);
ctx.lineTo(end[0]+120*flip,end[1]);
ctx.strokeStyle = "#bdc3c7";
ctx.lineWidth   = 2;
ctx.stroke();
ctx.font="17px Arial";
ctx.fillText(myLabel[i]+" "+precentage.toFixed(2)+"%",end[0]+textOffset,end[1]-4);
lastend += Math.PI*2*(myData[i]/myTotal);
}
}
}
function getPoint(c1,c2,radius,angle) {
return [c1+Math.cos(angle)*radius,c2+Math.sin(angle)*radius];
}
plotData();
