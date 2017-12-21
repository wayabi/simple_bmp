#include "a.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

using namespace std;

sm::sm(int w, int h)
{
	w_ = w;
	h_ = h;
	
	w *= 3;
	if(w%4 != 0){
		w += 4-(w%4);
	}
	buf_.resize(h_*w);
	memset(&buf_[0], 0, buf_.size());
}

sm::~sm()
{
}

void sm::go(const char* path)
{
	char head[] = {(char)0x42, (char)0x4d, (char)0x36, (char)0x03, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x36, (char)0x00, (char)0x00, (char)0x00, (char)0x28, (char)0x00, (char)0x00, (char)0x00, (char)0x10, (char)0x00, (char)0x00, (char)0x00, (char)0x10, (char)0x00, (char)0x00, (char)0x00, (char)0x01, (char)0x00, (char)0x18, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x03, (char)0x00, (char)0x00, (char)0xc3, (char)0x0e, (char)0x00, (char)0x00, (char)0xc3, (char)0x0e, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00};
	const int size_head = 54;

	FILE* f;
	if((f = fopen(path, "wb")) == NULL){
		printf("file open error:%s\n", path);
		return;
	}
	
	vector<char> head_m;
	head_m.assign(head, head+size_head);
	int size_file = 54+buf_.size();
	memcpy(&head_m[2], &size_file, sizeof(int));
	memcpy(&head_m[18], &w_, sizeof(int));
	memcpy(&head_m[22], &h_, sizeof(int));
	fwrite(&head_m[0], size_head, 1, f);
	fwrite(&buf_[0], buf_.size(), 1, f);
	fclose(f);

}

void sm::set(int x, int y, int a, int b, int c)
{
	if(x<0||x>=w_||y<0||y>=h_) return;
	int w = w_*3;
	if(w%4 != 0){
		w += 4-(w%4);
	}
	int h = h_-y-1;
	buf_.at(h*w+3*x+0) = (unsigned char)c;
	buf_.at(h*w+3*x+1) = (unsigned char)b;
	buf_.at(h*w+3*x+2) = (unsigned char)a;
}

void sm::box(int x1, int y1, int x2, int y2, int a, int b, int c)
{
	if(x1 > x2){
		int t_x = x1;x1 = x2;x2 = t_x;
	}
	if(y1 > y2){
	}
	for(int y=y1;y<=y2;++y){
		if(y<0 || y>=h_) continue;
		for(int x=x1;x<=x2;++x){
			if(x<0 || x>=w_) continue;
			set(x, y, a, b, c);
		}
	}
}

void sm::get_rgb(float a, int& r, int& g, int& b)
{
	float unit = 1.0/6;
	if(a <= 0.0){
		r = 255;
		g = 0;
		b = 0;
	}else if(a < unit){
		r = 255;
		g = 255*a/unit;
		b = 0;
	}else if(a < 2*unit){
		a = a-unit;
		g = 255;
		r = 255-255*a/unit;
		b = 0;
	}else if(a < 3*unit){
		a = a - 2*unit;
		g = 255;
		b = 255*a/unit;
		a = 0;
	}else if(a < 4*unit){
		a = a - 3*unit;
		b = 255;
		g = 255-255*a/unit;
		r = 0;
	}else if(a < 5*unit){
		a = a - 4*unit;
		b = 255;
		r = 255*a/unit;
		g = 0;
	}else if(a < 6*unit){
		a = a - 5*unit;
		r = 255;
		b = 255-255*a/unit;
		g = 0;
	}else{
		r = 255;
		g = 255;
		b = 255;
	}
}

void sm::get_thermo_rgb(float a, int& r, int& g, int& b)
{
	a = 0.7*(1.0-a);
	sm::get_rgb(a, r, g, b);
}

void sm::sla(int x1, int y1, int x2, int y2, int a, int b, int c)
{
	if(x1 > x2){
		swap(x1, x2);
		swap(y1, y2);
	}
	int ax = x2-x1;
	int ay = y2-y1;
	int yy = (ay>=0?1:-1);
	ay *= yy;
	int add_x = 0;
	int add_y = 0;
	int y = 0;
	for(int x=x1;x<=x2;++x){
		set(x, y1+y, a, b, c);
		add_x += ay;
		while(add_y < add_x){
			add_y += ax;
			y += yy;
			if(abs(y) > abs(ay)) break;
			set(x, y1+y, a, b, c);
		}
	}
}

void sm::cir(int x, int y, float r, int a, int b, int c, bool fill)
{
	float rr = r*r;
	int yy = 0;
	for(int xx=(int)r;xx>=0;--xx){
		set(xx+x, yy+y, a, b, c);
		set(xx+x, -yy+y, a, b, c);
		while(yy*yy+xx*xx < rr){
			++yy;
			set(xx+x, yy+y, a, b, c);
			set(xx+x, -yy+y, a, b, c);
		}
		if(fill){
			sla(xx+x, yy+y, xx+x, -yy+y, a, b, c);
		}
	}
	yy = 0;
	for(int xx=(int)-r;xx<=0;++xx){
		set(xx+x, yy+y, a, b, c);
		set(xx+x, -yy+y, a, b, c);
		while(yy*yy+xx*xx < rr){
			++yy;
			set(xx+x, yy+y, a, b, c);
			set(xx+x, -yy+y, a, b, c);
		}
		if(fill){
			sla(xx+x, yy+y, xx+x, -yy+y, a, b, c);
		}
	}
}

void sm::tri(int x1, int y1, int x2, int y2, int x3, int y3, int a, int b, int c)
{
	if(x1 > x2){
		swap(x1, x2);
		swap(y1, y2);
	}
	if(x2 > x3){
		swap(x2, x3);
		swap(y2, y3);
	}
	if(x1 > x2){
		swap(x1, x2);
		swap(y1, y2);
	}

	int y12 = 0;
	int y13 = 0;
	int y23 = 0;
	int a_y12 = abs(y2 - y1);
	int a_y13 = abs(y3 - y1);
	int a_y23 = abs(y3 - y2);
	int a_x12 = abs(x2 - x1);
	int a_x13 = abs(x3 - x1);
	int a_x23 = abs(x3 - x2);
	int d_y12 = (y2-y1>=0?1:-1);
	int d_y13 = (y3-y1>=0?1:-1);
	int d_y23 = (y3-y2>=0?1:-1);
	int sum_x12 = 0;
	int sum_y12 = 0;
	int sum_x13 = 0;
	int sum_y13 = 0;
	int sum_x23 = 0;
	int sum_y23 = 0;
	
	if(a_x12 == 0){
		sla(x1, y1, x2, y2, a, b, c);
	}else{
		for(int x=0;x<a_x12;++x){
			if(x+x1 > w_) break;
			sum_x12 += a_y12;
			sum_x13 += a_y13;
			while(sum_x12 > sum_y12){
				y12 += d_y12;
				sum_y12 += a_x12;
			}
			while(sum_x13 > sum_y13){
				y13 += d_y13;
				sum_y13 += a_x13;
			}
			sla(x+x1, y12+y1, x+x1, y13+y1, a, b, c);
		}
	}
	if(a_x23 == 0){
		sla(x2, y2, x3, y3, a, b, c);
	}else{
		for(int x=0;x<a_x23;++x){
			if(x+x2 > w_) break;
			sum_x13 += a_y13;
			sum_x23 += a_y23;
			while(sum_x13 > sum_y13){
				y13 += d_y13;
				sum_y13 += a_x13;
			}
			while(sum_x23 > sum_y23){
				y23 += d_y23;
				sum_y23 += a_x23;
			}
			sla(x2+x, y13+y1, x2+x, y23+y2, a, b, c);
		}
	}
}
