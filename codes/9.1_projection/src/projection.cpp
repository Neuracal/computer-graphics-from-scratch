//projection
#include<iostream>
#include "show_image.h"
#include<vector>
#include<math.h>
const int Cw = 800; // 画布宽度
const int Ch = 800; // 画布高度
const int Vw = 6;   // 视口宽度
const int Vh = 6;   // 视口高度
const double d = 4;
const Color BACKGROUND_COLOR(255, 255, 255);
std::vector<unsigned char> image(Cw* Ch * 3);

Vec3 viewport2canvas(double x, double y) {
	int canvasX = std::round((x + Vw / 2) * Cw / Vw);
	int canvasY = std::round((y + Vh / 2) * Ch / Vh);
	canvasX = std::max(0, std::min(Cw - 1, canvasX));
	canvasY = std::max(0, std::min(Ch - 1, canvasY));
	return Vec3(canvasX, canvasY, 0);
}

Vec3 projectVertex(Vec3 v) {
	return viewport2canvas(v.x * d / v.z, v.y * d / v.z);
}

int main() {
	Vec3 vAf(-1, 1, 5), vBf(1, 1, 5), vCf(1, -1, 5), vDf(-1, -1, 5);
	Vec3 vAb(-1, 1, 6), vBb(1, 1, 6), vCb(1, -1, 6), vDb(-1, -1, 6);
	for (int x = 0; x < Cw; ++x) { // 设置背景颜色
		for (int y = 0; y < Ch; ++y) {
			putPixel(x, y, BACKGROUND_COLOR, image, Cw, Ch);
		}
	}
	// 绘制前面正方形的边
	drawLine(projectVertex(vAf), projectVertex(vBf), BLUE, image, Cw, Ch);
	drawLine(projectVertex(vBf), projectVertex(vCf), BLUE, image, Cw, Ch);
	drawLine(projectVertex(vCf), projectVertex(vDf), BLUE, image, Cw, Ch);
	drawLine(projectVertex(vDf), projectVertex(vAf), BLUE, image, Cw, Ch);

	// 绘制后面正方形的边
	drawLine(projectVertex(vAb), projectVertex(vBb), GREEN, image, Cw, Ch);
	drawLine(projectVertex(vBb), projectVertex(vCb), GREEN, image, Cw, Ch);
	drawLine(projectVertex(vCb), projectVertex(vDb), GREEN, image, Cw, Ch);
	drawLine(projectVertex(vDb), projectVertex(vAb), GREEN, image, Cw, Ch);

	// 绘制连接前后正方形的边
	drawLine(projectVertex(vAf), projectVertex(vAb), RED, image, Cw, Ch);
	drawLine(projectVertex(vBf), projectVertex(vBb), RED, image, Cw, Ch);
	drawLine(projectVertex(vCf), projectVertex(vCb), RED, image, Cw, Ch);
	drawLine(projectVertex(vDf), projectVertex(vDb), RED, image, Cw, Ch);

	createPNG("projection.png", image, Cw, Ch);
	return 0;
}
