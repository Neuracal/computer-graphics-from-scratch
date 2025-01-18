#include "show_image.h"
#include <iostream>
#include <vector>
#include <math.h>
const int INF = 0x3f3f3f3f;

//图像宽度和高度最好是偶数
//C表示Canvas, V表示viewport
const int Cw = 600; // 宽度
const int Ch = 600; // 高度
const int Vw = 1; // 宽度
const int Vh = 1; // 高度
const double d = 1.0;//视口和画布之间的距离
const Color BACKGROUND_COLOR(0, 0, 0);
std::vector<unsigned char> image(Cw* Ch * 3);

Vec3 canvas2viewport(int x, int y) {
	Vec3 D(1.0 * x * Vw / Cw, 1.0 * y * Vh / Ch, d);
	return D;
}

class Sphere {
public:
	Vec3 center; double radius; Color color;
	Sphere(double _x, double _y, double _z, double _radius, double _r, double _g, double _b):center(_x,_y,_z), radius(_radius), color(_r,_g,_b){}
	Sphere() :center(0., 0., 0.), radius(1.), color(255,255,255) {}
};

void intersectSphere(Vec3 O, Vec3 e, Sphere sphere, double& t1, double& t2) {//射线方向为O+te,e是表示射线方向的向量（未必是单位向量）
	double r = sphere.radius;
	Vec3 CO = O - sphere.center;//CO = center -> O
	double a = dot(e, e);
	double b = 2 * dot(CO, e);
	double c = dot(CO, CO) - r * r;
	
	double delta = b * b - 4 * a * c;//二次方程判别式
	if (delta < 0) {
		t1 = INF; t2 = INF; return;
	}
	else
	{
		t1 = (-b + sqrt(delta)) / (2 * a);
		t2 = (-b - sqrt(delta)) / (2 * a);
		return;
	}
}

Color traceRay(Vec3 O, Vec3 e, double t_min, double t_max, std::vector<Sphere> spheres) {
	double closest_t = INF;
	Sphere closest_sphere;
	bool findCrossSphere = false;//射线是否穿过球体
	for (int i = 0; i < spheres.size(); ++i) {
		Sphere sphere = spheres[i];
		double t1 = 0., t2 = 0.;
		intersectSphere(O, e, sphere, t1, t2);
		if (t_min < t1 && t1 < t_max && t1 < closest_t) {
			closest_t = t1;
			closest_sphere = sphere;
			findCrossSphere = true;
		}
		if (t_min < t2 && t2 < t_max && t2 < closest_t) {
			closest_t = t2;
			closest_sphere = sphere;
			findCrossSphere = true;
		}
	}
	if (findCrossSphere)
		return closest_sphere.color;
	else
	{
		return BACKGROUND_COLOR;
	}
}

int main() {
	Vec3 O(0,0,0);
	Sphere sphere1(0, -1, 3, 1, 255, 0, 0); // 红色球
	Sphere sphere2(2, 0, 4, 1, 0, 0, 255); // 蓝色球
	Sphere sphere3(-2, 0, 4, 1, 0, 255, 0); // 绿色球
	std::vector<Sphere> spheres = { sphere1, sphere2, sphere3 };
	for (int x = -Cw / 2; x < Cw / 2; ++x) {//x, y都是图像(canvas）上面的像素坐标，必须是整数int
		for (int y = -Ch / 2; y < Ch / 2; ++y) {
			Vec3 D = canvas2viewport(x, y);
			Color color = traceRay(O, D, 1, INF, spheres);
			putPixel(x + Cw / 2, y + Ch / 2, color, image, Cw, Ch);
		}
	}
	createPNG("first_spheres.png", image, Cw, Ch);
	return 0;
}