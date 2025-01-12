#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>


#define WIDTH 900
#define HEIGHT 600
#define FILE_NAME "image.ppm"

#define COLOR_CADET_GREY 0xFF717C89
#define COLOR_SPACE_CADET 0xFF2C2A4A

void BGR_to_RGB(uint32_t bgr);
static uint8_t color[3];

typedef struct{
    uint8_t R;
    uint8_t G;
    uint8_t B;
}RGB;

typedef struct{
    uint16_t x;
    uint16_t y;
    RGB color;
}Seed;




RGB random_RGB_color(){
    RGB color;
    color.R = rand() % 256;
    color.G = rand() % 256;
    color.B = rand() % 256;
    return color;
}

void fill_color(uint32_t pixels[][WIDTH], uint32_t color){
    for (size_t y = 0; y < HEIGHT; y++)
    {
        for (size_t x = 0; x < WIDTH; x++)
        {
            pixels[y][x] = color;
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
            BGR_to_RGB(pixels[y][x]);
            fwrite(color, 1, sizeof(color), fp);
        }
    }  
}

void BGR_to_RGB(uint32_t bgr){
    // b g r -> r g b
    // R = COLORE & 0x0000FF >> 8 * 0
    // G = COLORE & 0x00FF00 >> 8 * 1
    // B = COLORE & 0xFF0000 >> 8 * 2
    color[0] = (bgr & 0x0000FF) >> 8 * 0;
    color[1] = (bgr & 0x00FF00) >> 8 * 1;
    color[2] = (bgr & 0xFF0000) >> 8 * 2;
}

void put_dot(uint32_t pixels[][WIDTH], uint32_t x,uint32_t y, uint32_t bgr)
{
    pixels[y][x] = bgr;
}

int put_circle(uint32_t pixels[][WIDTH], uint32_t w, uint32_t h, 
                uint32_t x, uint32_t y, uint32_t radious, uint32_t bgr)
{
    // define square in which the cirle is inscribed
    int x0, x1, y0, y1;
    x0 = x - radious;
    x1 = x + radious;
    y0 = y - radious;
    y1 = y + radious;
    if (x0 < 0 || x1 >= w || y0 <0 || y1 >= h) return 0; // failed to put circle
    for (int r = y0; r <= y1; r++)
    {
        for (int c = x0; c <= x1; c++)
        {
            if (((c - x)*(c - x) + (r - y)*(r - y)) <= radious*radious){
                put_dot(pixels, c, r, bgr);
            }
        }
    }
    return 1;        
}

void seeds_generation(Seed* seeds, uint8_t dim, uint32_t w, uint32_t h){
    for (size_t i = 0; i < dim; i++)
    {
        seeds[i].x = rand() % w;
        seeds[i].y = rand() % h;
        seeds[i].color = random_RGB_color()
    }
    
}

void voronoi_generation(uint32_t pixels, size_t seed_n){
    Seed* seeds;
    seeds_generation(seeds, 10, WIDTH, HEIGHT);
    
    for (size_t i = 0; i < seed_n; i++)
    {
        
        if(!put_circle(pixels,WIDTH, HEIGHT, seeds[i].x, seeds[i].y, 5, seeds[i].color) {
            i--;
        }
    }
}

int main(int argc, char const *argv[])
{
    FILE *fp = fopen("image.ppm", "wb");
    uint32_t pixels[HEIGHT][WIDTH];
   
    init_image(fp);

    fill_color(pixels, COLOR_CADET_GREY);

    // put_dot(pixels, 100, 200, COLOR_SPACE_CADET);


    srand(time(0));
    


    update_image(fp, pixels);

    fclose(fp);

    return 0;
}

