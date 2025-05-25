#include <stdio.h>
#include <stdint.h>
#include <errno.h>

#define WIDTH 800
#define HEIGHT 600

static uint32_t pixels[WIDTH * HEIGHT];

void royglc_fill(size_t width, size_t height, uint32_t color) {
    for (size_t i = 0; i < width * height; ++i) {
        pixels[i] = color;
    }
}

#define return_defer(value) do { result = (value); goto defer; } while (0)

typedef int Errno;

Errno royglc_save_to_ppm_file(size_t width, size_t height, const char *file_path) {
    int result = 0;
    FILE *f = NULL;
    {
        f = fopen(file_path, "wb");
        if (f == NULL) return_defer(errno);

        fprintf(f, "P6\n%zu %zu\n255\n", width, height);

        if (ferror(f)) return_defer(errno);

        for (size_t i = 0; i < width * height; ++i) {
            uint32_t pixel = pixels[i];
            uint8_t bytes[3] = {
                (pixel >> (8 * 2)) & 0xFF,  // Red (highest byte)
                (pixel >> (8 * 1)) & 0xFF,  // Green
                (pixel >> (8 * 0)) & 0xFF,  // Blue (lowest byte)
            };
            fwrite(bytes, sizeof(bytes), 1, f);
            if (ferror(f)) return_defer(errno);
        }
    }
defer:
    if (f) fclose(f);
    return result;
}

void royglc_fill_rect(size_t pixels_width, size_t pixels_height, int x0, int y0, size_t w, size_t h, uint32_t color) {
    for (int dy = 0; dy < (int)h; ++dy) {
        int y = y0 + dy;
        if (0 <= y && y < (int)pixels_height) {
            for (int dx = 0; dx < (int)w; ++dx) {
                int x = x0 + dx;
                if (0 <= x && x < (int)pixels_width) {
                    pixels[y * pixels_width + x] = color;
                }
            }
        }
    }
}

void royglc_fill_circle(size_t pixels_width, size_t pixels_height, int cx, int cy, int r, uint32_t color) {
    int x1 = cx - r;
    int y1 = cy - r;
    int x2 = cx + r;
    int y2 = cy + r;
    for (int y = y1; y <= y2; ++y) {
        if (0 <= y && y < (int)pixels_height) {
            for (int x = x1; x <= x2; ++x) {
                if (0 <= x && x < (int)pixels_width) {
                    int dx = x - cx;
                    int dy = y - cy;
                    if (dx * dx + dy * dy <= r * r) {
                        pixels[y * pixels_width + x] = color;
                    }
                }
            }
        }
    }
}

static void swap_int(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void royglc_draw_line(size_t pixels_width, size_t pixels_height, int x1, int y1, int x2, int y2, uint32_t color) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    if (dx != 0) {
        // line is not vertical
        if (x1 > x2) swap_int(&x1, &x2);
        for (int x = x1; x <= x2; ++x) {
            int y = dy * (x - x1) / dx + y1;
            if (0 <= y && y < (int)pixels_height) {
                pixels[y * pixels_width + x] = color;
            }
        }
    } else {
        // vertical line
        int x = x1;
        if (0 <= x && x < (int)pixels_width) {
            if (y1 > y2) swap_int(&y1, &y2);
            for (int y = y1; y <= y2; ++y) {
                if (0 <= y && y < (int)pixels_height) {
                    pixels[y * pixels_width + x] = color;
                }
            }
        }
    }
}
void royglc_fill_triangle(size_t pixels_width, size_t pixels_height,
                         int x0, int y0,
                         int x1, int y1,
                         int x2, int y2,
                         uint32_t color) {
    // Compute bounding box of the triangle
    int min_x = x0 < x1 ? (x0 < x2 ? x0 : x2) : (x1 < x2 ? x1 : x2);
    int max_x = x0 > x1 ? (x0 > x2 ? x0 : x2) : (x1 > x2 ? x1 : x2);
    int min_y = y0 < y1 ? (y0 < y2 ? y0 : y2) : (y1 < y2 ? y1 : y2);
    int max_y = y0 > y1 ? (y0 > y2 ? y0 : y2) : (y1 > y2 ? y1 : y2);

    // Clamp to pixel buffer bounds
    if (min_x < 0) min_x = 0;
    if (min_y < 0) min_y = 0;
    if (max_x >= (int)pixels_width) max_x = (int)pixels_width - 1;
    if (max_y >= (int)pixels_height) max_y = (int)pixels_height - 1;

    // Compute edge functions coefficients
    int dx01 = x1 - x0, dy01 = y1 - y0;
    int dx12 = x2 - x1, dy12 = y2 - y1;
    int dx20 = x0 - x2, dy20 = y0 - y2;

    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            // Compute barycentric coordinates
            int c0 = (x - x0)*dy01 - (y - y0)*dx01;
            int c1 = (x - x1)*dy12 - (y - y1)*dx12;
            int c2 = (x - x2)*dy20 - (y - y2)*dx20;

            // Check if point (x,y) is inside or on edge of triangle
            if ((c0 >= 0 && c1 >= 0 && c2 >= 0) || (c0 <= 0 && c1 <= 0 && c2 <= 0)) {
                pixels[y * pixels_width + x] = color;
            }
        }
    }
}
