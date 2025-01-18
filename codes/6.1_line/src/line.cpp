//line
#include<iostream>
#include "show_image.h"
#include<vector>
#include<math.h>
const int Cw = 600; // 宽度
const int Ch = 600; // 高度
const Color BACKGROUND_COLOR(255, 255, 255);
std::vector<unsigned char> image(Cw* Ch * 3);

std::vector<double> interpolate(double i0, double d0, double i1, double d1) {
	std::vector<double> values;
	double a = (d1 - d0) / (i1 - i0);
	double d = d0;
	for (int i = i0; i <= i1; ++i) {
		values.push_back(d);
		d += a;
	}
	return values;
}

template <typename T>
int adjustIndex(int j, const std::vector<T>& v) {
	if (j < 0) return 0;
	if (j >= static_cast<int>(v.size())) return v.size() - 1;
	return j;
}


void drawLine(Vec3 P0, Vec3 P1, Color color, std::vector<unsigned char>& image) {
	
	if (abs(P1.x - P0.x) > abs(P1.y - P0.y)) {
		if (P0.x > P1.x) {
			std::swap(P0, P1);
		}
		std::vector<double> ys = interpolate(P0.x, P0.y, P1.x, P1.y);
		int j = 0;
		for (int x = (int)P0.x; x <= (int)P1.x; ++x) {
			putPixel(x, ys[adjustIndex(j++, ys)], color, image, Cw, Ch);
		}
	}
	else {
		if (P0.y > P1.y) {
			std::swap(P0, P1);
		}
		std::vector<double> xs = interpolate(P0.y, P0.x, P1.y, P1.x);
		int j = 0;
		for (int y = (int)P0.y; y <= (int)P1.y; ++y) {
			putPixel(xs[adjustIndex(j, xs)], y, color, image, Cw, Ch);
		}
	}
	return;
}

int main() {
	for (int x = 0; x < Cw; ++x) { // 设置背景颜色
		for (int y = 0; y < Ch; ++y) {
			putPixel(x, y, BACKGROUND_COLOR, image, Cw, Ch);
		}
	}
	Vec3 P0(50, 50, 0), P1(500, 200, 0);
	Color lineColor(0, 0, 0); // 黑色直线
	drawLine(P0, P1, lineColor, image);
	createPNG("line.png", image, Cw, Ch);
	return 0;
}
