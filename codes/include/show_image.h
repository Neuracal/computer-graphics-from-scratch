#define STB_IMAGE_WRITE_IMPLEMENTATION
#ifndef SHOW_IMAGE_H
#define SHOW_IMAGE_H
#include<stb_image_write.h>
#include<iostream>
#include<string>
#include<vector>
#include<math.h>

class Color {
public:
    double r, g, b;
    Color(double _r, double _g, double _b) :r(_r), g(_g), b(_b) {}
    friend Color operator * (const double&, const Color&);
    friend Color operator * (const Color&, const double&);
    friend Color operator + (const Color&, const Color&);
};

const Color RED(255, 0, 0), GREEN(0, 255, 0), BLUE(0, 0, 255);

Color operator * (const double& t, const Color& C) {
    Color R(t * C.r, t * C.g, t * C.b);
    return R;
}

Color operator * (const Color& C, const double& t) {
    Color R(t * C.r, t * C.g, t * C.b);
    return R;
}

Color operator + (const Color& c1, const Color& c2) {
    Color R(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b);
    return R;
}

class Vec3 {
public:
    double x, y, z;//表示向量的方向和模长
    Vec3(double _x, double _y, double _z) :x(_x), y(_y), z(_z){}
    Vec3() :x(0.), y(0.), z(0.){}

    friend double dot(Vec3&, Vec3&);
    friend Vec3 operator - (const Vec3&, const Vec3&);
    friend Vec3 operator -(const Vec3&);
    friend Vec3 operator + (const Vec3&, const Vec3&);
    friend Vec3 operator * (const double&, const Vec3&);//计算向量的数乘
    friend Vec3 operator * (const Vec3&, const double&);
    friend Vec3 operator * (const Vec3&, const double&);
    double length() {
        return sqrt(x * x + y * y + z * z);
    }
};

double dot(Vec3& v1, Vec3& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vec3 operator - (const Vec3& start, const Vec3& end) {
    Vec3 v(start.x - end.x, start.y - end.y, start.z - end.z);
    return v;
}

Vec3 operator - (const Vec3& D) {
    Vec3 v(-D.x, -D.y, -D.z);
    return v;
}

Vec3 operator + (const Vec3& v1, const Vec3& v2) {
    Vec3 v(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
    return v;
}

Vec3 operator * (const double& t, const Vec3& D) {
    Vec3 V(t * D.x, t * D.y, t * D.z);
    return V;
}

Vec3 operator * (const Vec3& D, const double& t) {
    Vec3 V(t * D.x, t * D.y, t * D.z);
    return V;
}

Vec3 operator / (const Vec3& D, const double& t) {
    Vec3 V(D.x / t, D.y / t, D.z / t);
    return V;
}

class Light {
public:
    std::string type;
    double intensity; Vec3 position, direction;
    Light(std::string _type, double _intensity): type(_type), intensity(_intensity){}
    Light(std::string _type, double _intensity, double _x, double _y, double _z) {
        type = _type; intensity = _intensity; Vec3 _v(_x, _y, _z);
        if (type == "point") {
            position = _v; direction = Vec3();
        }
        else if (type == "directional") {
            position = Vec3(); direction = _v;
        }
    }
};

class Matrix {
public:
    int row, col; std::vector<std::vector<double>> mat;
    Matrix(int _row, int _col) : row(_row), col(_col) {
        mat = std::vector<std::vector<double>>(_row, std::vector<double>(_col, 0.0));
    }
    Vec3 operator*(const Vec3& vec) const {
        if (col != 3) {
            throw std::invalid_argument("Matrix must have 3 columns to multiply with Vec3.");
        }
        if (row != 3) {
            throw std::invalid_argument("Matrix rows and Vec3 size must match for valid multiplication.");
        }
        return Vec3(
            mat[0][0] * vec.x + mat[0][1] * vec.y + mat[0][2] * vec.z,
            mat[1][0] * vec.x + mat[1][1] * vec.y + mat[1][2] * vec.z,
            mat[2][0] * vec.x + mat[2][1] * vec.y + mat[2][2] * vec.z
        );
    }
};

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

void putPixel(int x, int y, Color color, std::vector<unsigned char>& image, int width, int height) {
    int index = ((height-1-y) * width + x) * 3;//(height-1-y)表示(0,0)在图像的左下角，若写成y，则(0,0)在图像左上角
    image[index] = color.r;
    image[index + 1] = color.g;
    image[index + 2] = color.b;
}

template <typename T>
int adjustIndex(int j, const std::vector<T>& v) {
    if (j < 0) return 0;
    if (j >= static_cast<int>(v.size())) return v.size() - 1;
    return j;
}


void drawLine(Vec3 P0, Vec3 P1, Color color, std::vector<unsigned char>& image, int Cw, int Ch) {

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
            putPixel(xs[adjustIndex(j++, xs)], y, color, image, Cw, Ch);
        }
    }
    return;
}

bool createPNG(const char* fileName, std::vector<unsigned char>& image, int width, int height) {
    if (stbi_write_png(fileName, width, height, 3, image.data(), width * 3)) {
        std::cout << "Image successfully generated as " << fileName << std::endl;
        return true;
    }
    else {
        std::cerr << "Failed to write image.\n";
        return false;
    }
}
#endif