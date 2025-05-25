#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define WIDTH 800
#define HEIGHT 600

#include "roygl.c" // Include drawing logic directly

void test_fill_rect(void) {
    royglc_fill(WIDTH, HEIGHT, 0xFFFFFF);
    royglc_fill_rect(WIDTH, HEIGHT, 200, 150, 400, 300, 0x0000FF);
    royglc_save_to_ppm_file(WIDTH, HEIGHT, "tests/test_fill_rect.ppm");
}

void test_fill_circle(void) {
    royglc_fill(WIDTH, HEIGHT, 0xFFFFFF);
    royglc_fill_circle(WIDTH, HEIGHT, 400, 300, 150, 0x00FF00);
    royglc_save_to_ppm_file(WIDTH, HEIGHT, "tests/test_fill_circle.ppm");
}

void test_draw_line(void) {
    royglc_fill(WIDTH, HEIGHT, 0xFFFFFF);
    royglc_draw_line(WIDTH, HEIGHT, 0, 0, WIDTH - 1, HEIGHT - 1, 0xFF0000);
    royglc_draw_line(WIDTH, HEIGHT, WIDTH - 1, 0, 0, HEIGHT - 1, 0xFF0000);
    royglc_save_to_ppm_file(WIDTH, HEIGHT, "tests/test_draw_line.ppm");
}

void test_fill_triangle(void) {
    royglc_fill(WIDTH, HEIGHT, 0x000000);
    royglc_fill_triangle(WIDTH, HEIGHT, 100, 100, 400, 200, 150, 500, 0xFF0000);
    royglc_save_to_ppm_file(WIDTH, HEIGHT, "tests/test_fill_triangle.ppm");
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <subcommand>\n", argv[0]);
        return 1;
    }

    const char *subcommand = argv[1];

    if (strcmp(subcommand, "record") == 0) {
        test_fill_rect();
        test_fill_circle();
        test_draw_line();
        test_fill_triangle();
        printf("All tests completed. Output saved to 'tests/'.\n");
        return 0;
    }

    printf("Unknown subcommand: %s\n", subcommand);
    return 1;
}
