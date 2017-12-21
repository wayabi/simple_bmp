#include <stdio.h>
#include <math.h>
#include "a.h"

int main()
{
	sm a(400, 400);
	int len = 100000000;
	for(int i=0;i<360;++i){
		int x = len*cos(2*M_PI*i/360);
		int y = len*sin(2*M_PI*i/360);
		a.sla(-x, -y, x, y, 255, 255, 255);
	}
	//a.sla(-25, -25, 400, 800, 255, 255, 255);
	a.go("a.bmp");
	return 0;
}
