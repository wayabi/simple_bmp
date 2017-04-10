#ifndef __MYC_A__
#define __MYC_A__

#include <stdio.h>
#include <vector>

class sm {
public:
	sm(int w, int h);
	~sm();
	void go(const char* path);
	void set(int x, int y, int a, int b, int c);
	void sla(int x1, int y1, int x2, int y2, int a, int b, int c);
	void box(int x1, int y1, int x2, int y2, int a, int b, int c);
	void cir(int x, int y, float r, int a, int b, int c, bool fill);
	void tri(int x1, int y1, int x2, int y2, int x3, int y3, int a, int b, int c);
	static void get_rgb(float a, int& r, int& g, int& b);
	static void get_thermo_rgb(float a, int& r, int& g, int& b);

public:
	int w_;
	int h_;
	std::vector<unsigned char> buf_;
};

#endif
