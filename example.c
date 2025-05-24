#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include "roygl.c"

#define WIDTH 800
#define HEIGHT 600




static uint32_t pixels[WIDTH*HEIGHT];

#define COLS (8*2)
#define ROWS (6*2)
#define CELL_WIDTH (WIDTH/COLS)
#define CELL_HEIGHT (HEIGHT/ROWS)

#define BACKGROUND_COLOR 0x808080
#define FOREGROUND_COLOR 0X008000

void swap_int(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}



bool checker_example(void) {
    royglc_fill(WIDTH, HEIGHT, BACKGROUND_COLOR);

    for (int y = 0; y < ROWS; ++y) {
        for(int x = 0; x < COLS; ++x) {
            uint32_t color = BACKGROUND_COLOR;
            if ((x + y)%2 == 0) {
                color = 0x0000FF;
            }
            royglc_fill_rect(WIDTH, HEIGHT, x*CELL_WIDTH, y*CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT, color);
        }
    }
    

    const char *file_path = "checker.ppm";
    Errno err = royglc_save_to_ppm_file(WIDTH, HEIGHT, file_path);
    if (err) {
        fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
        return false;
    }
    return true;
}

float lerpf(float a, float b, float t) {
    return a + (b - a)*t;
}

bool circle_example(void) {
    royglc_fill(WIDTH, HEIGHT, BACKGROUND_COLOR);

    for (int y = 0; y < ROWS; ++y) {
        for (int x = 0; x < COLS; ++x) {
            float u = (float)x/COLS;
            float v = (float)y/ROWS;
            float t = (u + v)/2;

            
            size_t radius = CELL_WIDTH;
            if (CELL_HEIGHT < radius) radius = CELL_HEIGHT;
            royglc_fill_circle(WIDTH, HEIGHT, x*CELL_WIDTH + CELL_WIDTH/2, y*CELL_HEIGHT + CELL_HEIGHT/2, (size_t) lerpf(radius/8, radius/2, t), FOREGROUND_COLOR);
        }
    }
    

    const char *file_path = "circle.ppm";
    Errno err = royglc_save_to_ppm_file(WIDTH, HEIGHT, file_path);
    if (err) {
        fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
        return false;
    }
    return true;
}

bool lines_example(void) {
    royglc_fill(WIDTH, HEIGHT, BACKGROUND_COLOR);

    royglc_draw_line(WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, FOREGROUND_COLOR);
    royglc_draw_line(WIDTH, HEIGHT, WIDTH, 0, 0, HEIGHT, FOREGROUND_COLOR);

    const char *file_path = "lines.ppm";
    Errno err = royglc_save_to_ppm_file(WIDTH, HEIGHT, file_path);
    if (err) {
        fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
        return false;
    }
    return true;
}

int main(void) {
    if (!checker_example()) return -1;
    if (!circle_example()) return -1;
    if (!lines_example()) return -1;
    return 0;
}