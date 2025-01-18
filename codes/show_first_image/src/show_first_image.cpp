#include "show_image.h"
#include <iostream>
#include <cmath>
#include <vector>

// 图像宽度和高度
const int WIDTH = 600;
const int HEIGHT = 600;

void drawLine() {
    std::vector<unsigned char> image(WIDTH * HEIGHT * 3);
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            Color color = { 255, 0, 0 };
            if (y==2*x){
                color = {0, 255, 0};
            }
            putPixel(x, y, color, image, WIDTH, HEIGHT);
        }
    }
    createPNG("line.png", image, WIDTH, HEIGHT);
    return;
}

int main() {
    drawLine();
    return 0;
}
