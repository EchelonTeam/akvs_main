var n=100;
var dx=101;
var dy=1;
function getX()
{
var x=[[1,1],[2,2],[39,50],[30,75],[75,76],[75,77],[72,71],[79,78]];
for(var i=0;i<x.length;i++)
{
	x[i][0]+=dx;
	x[i][1]+=dy;
}
return x;
}