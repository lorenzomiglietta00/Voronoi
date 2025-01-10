#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// #define OLIVEC_IMPLEMENTATION
// #include "olive.c"


#define WIDTH 900
#define HEIGHT 600
#define FILE_NAME "image.ppm"

#define COLOR_RED 0xFF0000FF

void fill_color(uint32_t pixels[][WIDTH], uint32_t color){
    for (size_t y = 0; y < HEIGHT; y++)
    {
        for (size_t x = 0; x < WIDTH; x++)
        {
            // pixels[y][x] = color;
        }
    }  

}

void init_image(FILE *fp){
    if (fp == NULL) {
        fprintf(stderr, "Error: It was not possible to write on the File %s\n", FILE_NAME);
        exit(1);
    }
    fprintf(fp, "P6 %d %d 255\n", WIDTH, HEIGHT);
}



void update_image(FILE *fp, uint32_t  pixels[][WIDTH]){
    uint32_t pixel;
    for (size_t y = 0; y < HEIGHT; y++)
    {
        for (size_t x = 0; x < WIDTH; x++)
        {
            pixel = pixels[y][x];
            fwrite(&pixel, 1, sizeof(uint32_t), fp);
        }
    }  
}


int main(int argc, char const *argv[])
{
    FILE *fp = fopen("image.ppm", "wb");
    uint32_t pixels[HEIGHT][WIDTH];
   
    init_image(fp);

    fill_color(pixels, COLOR_RED);



    update_image(fp, pixels);

    fclose(fp);

    return 0;
}

