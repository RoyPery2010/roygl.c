#include "roygl.c"

#define WIDTH 800
#define HEIGHT 600

float angle = 0;
float sqrtf(float x);
float atan2f(float y, float x);
float cosf(float x);
float sinf(float x);

#define PI 3.14

void rotate_point(int *x, int *y) {
    int dx = *x - WIDTH/2;
    int dy = *y - HEIGHT/2;
    float mag = sqrtf(dx * dx + dy * dy);
    float dir = atan2f(dy, dx);
    *x = cosf(dir)*mag + WIDTH/2;
    *y = sinf(dir)*mag + HEIGHT/2;
}

uint32_t *render(float dt) {
    angle += 0.5f*PI*dt;
    royglc_fill(WIDTH, HEIGHT, 0xFF181818);
    {
        int x1 = WIDTH / 2, y1 = HEIGHT / 8;
        int x2 = WIDTH / 8, y2 = HEIGHT/2;
        int x3 = WIDTH/7*8, y3 = HEIGHT/7*8;
        rotate_point(&x1, &y1);
        rotate_point(&x2, &y2);
        rotate_point(&x3, &y3);
        royglc_fill_triangle(WIDTH, HEIGHT, x1, y1, x2, y2, x3, y3, 0xFF2020AA);
    }
    return pixels;
}