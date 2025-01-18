//shadow
#include "show_image.h"
#include <iostream>
#include <vector>
#include <math.h>
const int INF = 0x3f3f3f3f;
const double EPSILON = 1e-6;

//ͼ���Ⱥ͸߶������ż��
//C��ʾCanvas, V��ʾviewport
const int Cw = 600; // ���
const int Ch = 600; // �߶�
const int Vw = 1; // ���
const int Vh = 1; // �߶�
const double d = 1.0;//�ӿںͻ���֮��ľ���
const Color BACKGROUND_COLOR(0, 0, 0);
std::vector<unsigned char> image(Cw* Ch * 3);

Vec3 canvas2viewport(int x, int y) {
	Vec3 D(1.0 * x * Vw / Cw, 1.0 * y * Vh / Ch, d);
	return D;
}

Vec3 reflectRay(Vec3 L, Vec3 N) {
	Vec3 R = 2 * N * dot(N, L) - L;
	return R;
}

class Sphere {
public:
	Vec3 center; double radius; Color color; double specular, reflective;
	Sphere(double _x, double _y, double _z, double _radius, double _r, double _g, double _b) :center(_x, _y, _z), radius(_radius), color(_r, _g, _b), specular(-1), reflective(-1) {}
	Sphere(double _x, double _y, double _z, double _radius, double _r, double _g, double _b, double _specular, double _reflective) :center(_x, _y, _z), radius(_radius), color(_r, _g, _b), specular(_specular), reflective(_reflective) {}
	Sphere() :center(0., 0., 0.), radius(1.), color(255, 255, 255), specular(-1), reflective(-1) {}
};

void intersectSphere(Vec3 O, Vec3 e, Sphere sphere, double& t1, double& t2) {//���߷���ΪO+te,e�Ǳ�ʾ���߷����������δ���ǵ�λ������
	double r = sphere.radius;
	Vec3 CO = O - sphere.center;//CO = center -> O
	double a = dot(e, e);
	double b = 2 * dot(CO, e);
	double c = dot(CO, CO) - r * r;

	double delta = b * b - 4 * a * c;//���η����б�ʽ
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

bool closestSphereIntersect(Vec3 O, Vec3 e, double t_min, double t_max, std::vector<Sphere> spheres, double& closest_t, Sphere& closest_sphere) {
	closest_t = INF;
	closest_sphere = Sphere();
	bool findCrossSphere = false;//�����Ƿ񴩹�����
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
	return findCrossSphere;
}

double computeLighting(Vec3 P, Vec3 N, Vec3 V, double s, std::vector<Sphere> spheres, std::vector<Light> lights) {//P����O�㷢��������������Ľ��㣬N��P��ķ��ߵ�λ����
	double intensity = 0.; double maxI = -INF;
	Vec3 L;//L�ǹ��ߵķ������������Բ��ǵ�λ������
	for (int i = 0; i < lights.size(); ++i) {
		if (lights[i].type == "ambient") {
			intensity += lights[i].intensity;
		}
		else {
			if (lights[i].type == "point") {
				L = lights[i].position - P;
			}
			if (lights[i].type == "directional") {
				L = lights[i].position;
			}
			//��Ӱ���
			double shadow_t; Sphere shadow_sphere;
			bool findShadowSphere = closestSphereIntersect(P, L, EPSILON, INF, spheres, shadow_t, shadow_sphere);
			if (findShadowSphere)
				continue;
			//������
			double N_dot_L = dot(N, L);
			if (N_dot_L > 0) {
				intensity += lights[i].intensity * N_dot_L / (N.length() * L.length());
			}
			//���淴��
			if (s != -1) {
				Vec3 R = reflectRay(L, N);//���㱻����������R
				double R_dot_V = dot(R, V);
				
				if (R_dot_V > 0) {
					//double mirrorI = lights[i].intensity * (1 - exp(-pow(R_dot_V, s)));
					double mirrorI = lights[i].intensity* pow(R_dot_V / (R.length() * V.length()), s);
					mirrorI = mirrorI / (2.0 + mirrorI);//ƽ������
					intensity = intensity + mirrorI;
				}
			}
		}
	}
	if (intensity <= 1)//����˾��淴����ܵĹ�ǿintensity���ܴ���1��������Ϊ���������㲻̫��ȷ������Ҫ��intensity������1���ڣ����intensity����1�������color����ֺ�ɫ
		return intensity;
	else
	{
		return 1;
	}
}

Color traceRay(Vec3 O, Vec3 e, double t_min, double t_max, int recur_depth, std::vector<Sphere>& spheres, std::vector<Light>& lights) {
	double closest_t; Sphere closest_sphere;
	bool findCrossSphere = closestSphereIntersect(O, e, t_min, t_max, spheres, closest_t, closest_sphere);
	if (findCrossSphere) {
		Vec3 P = O + closest_t * e;
		Vec3 N = P - closest_sphere.center;//���P�㴦����ķ���
		N = N / N.length();//��������һ��
		Color local_color = closest_sphere.color * computeLighting(P, N, -e, closest_sphere.specular, spheres, lights);
		if (recur_depth <= 0 || closest_sphere.reflective<=0) {
			return local_color;
		}
		else {
			Vec3 R = reflectRay(-e, N);//��������������������Ľ���P�����ĵ������������-e����������N����������R
			Color reflected_color = traceRay(P, R, EPSILON, INF, recur_depth - 1, spheres, lights);
			return local_color * (1 - closest_sphere.reflective) + reflected_color * closest_sphere.reflective;
		}
	}
	else
	{
		return BACKGROUND_COLOR;
	}
}

int main() {
	Vec3 O(0, 0, 0);
	Sphere sphere1(0, -1, 3, 1, 255, 0, 0, 500, 0.2); // ��ɫ��
	Sphere sphere2(2, 0, 4, 1, 0, 0, 255, 500, 0.3); // ��ɫ��
	Sphere sphere3(-2, 0, 4, 1, 0, 255, 0, 10, 0.4); // ��ɫ��
	Sphere sphere4(0, -5001, 0, 5000, 255, 255, 0, 1000, 0.5);//��ɫ��
	std::vector<Sphere> spheres = { sphere1, sphere2, sphere3, sphere4 };
	Light light1("ambient", 0.2), light2("point", 0.6, 2, 1, 0), light3("directional", 0.2, 1, 4, 4);
	std::vector<Light> lights = { light1, light2, light3 };
	for (int x = -Cw / 2; x < Cw / 2; ++x) {//x, y����ͼ��(canvas��������������꣬����������int
		for (int y = -Ch / 2; y < Ch / 2; ++y) {
			Vec3 D = canvas2viewport(x, y);
			Color color = traceRay(O, D, 1, INF, 3, spheres, lights);
			putPixel(x + Cw / 2, y + Ch / 2, color, image, Cw, Ch);
		}
	}
	createPNG("shadow.png", image, Cw, Ch);
	return 0;
}