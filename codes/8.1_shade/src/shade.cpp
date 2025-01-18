//shade
#include<iostream>
#include "show_image.h"
#include<vector>
#include<math.h>
const int Cw = 600; // 宽度
const int Ch = 600; // 高度
const Color BACKGROUND_COLOR(255, 255, 255);
std::vector<unsigned char> image(Cw* Ch * 3);

void drawShadedTriangle(Vec3 P0, Vec3 P1, Vec3 P2, double h0, double h1, double h2, Color color, std::vector<unsigned char>& image) {
	if (P1.y < P0.y)
		std::swap(P1, P0);
	if (P2.y < P0.y)
		std::swap(P2, P0);
	if (P2.y < P1.y)
		std::swap(P2, P1);
	std::vector<double> x01 = interpolate(P0.y, P0.x, P1.y, P1.x);
	std::vector<double> h01 = interpolate(P0.y, h0, P1.y, h1);

	std::vector<double> x12 = interpolate(P1.y, P1.x, P2.y, P2.x);
	std::vector<double> h12 = interpolate(P1.y, h1, P2.y, h2);

	std::vector<double> x02 = interpolate(P0.y, P0.x, P2.y, P2.x);
	std::vector<double> h02 = interpolate(P0.y, h0, P2.y, h2);

	x01.pop_back(); h01.pop_back();
	std::vector<double> x012; std::vector<double> h012;
	x012.insert(x012.end(), x01.begin(), x01.end()); x012.insert(x012.end(), x12.begin(), x12.end());
	h012.insert(h012.end(), h01.begin(), h01.end()); h012.insert(h012.end(), h12.begin(), h12.end());

	int m = floor(x012.size() / 2);
	std::vector<double> x_left, x_right, h_left, h_right;
	if (x02[m] < x012[m]) {
		x_left = x02; x_right = x012;
		h_left = h02; h_right = h012;
	}
	else {
		x_left = x012; x_right = x02;
		h_left = h012; h_right = h02;
	}
	for (int y = (int)P0.y; y <= (int)P2.y; ++y) {
		double x_l = x_left[adjustIndex(y - P0.y, x_left)], x_r = x_right[adjustIndex(y - P0.y, x_right)];
		std::vector<double> h_segment = interpolate(x_l, h_left[adjustIndex(y - P0.y, h_left)], x_r, h_right[adjustIndex(y - P0.y, h_right)]);
		for (int x = x_l; x <= x_r; ++x) {
			Color shadedColor = color * h_segment[x - x_l];
			putPixel(x, y, shadedColor, image, Cw, Ch);
		}
	}
}

int main() {
	for (int x = 0; x < Cw; ++x) { // 设置背景颜色
		for (int y = 0; y < Ch; ++y) {
			putPixel(x, y, BACKGROUND_COLOR, image, Cw, Ch);
		}
	}
	Color triangleColor(255, 0, 0), lineColor(0, 0, 0);
	Vec3 P0(50, 50, 0), P1(500, 200, 0), P2(100, 300, 0); int h0 = 0.2, h1 = 0.7, h2 = 1;
	drawShadedTriangle(P0, P1, P2, h0, h1, h2, triangleColor, image);
	drawLine(P0, P1, lineColor, image, Cw, Ch); drawLine(P0, P2, lineColor, image, Cw, Ch); drawLine(P1, P2, lineColor, image, Cw, Ch);
	createPNG("shadedTriangle.png", image, Cw, Ch);
	return 0;
}
