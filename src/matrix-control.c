#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "matrix-control.h"
#include "ws2811.h"

#define TARGET_FREQ             WS2811_TARGET_FREQ
#define GPIO_PIN                18
#define DMA                     10
#define STRIP_TYPE              WS2811_STRIP_GBR

static ws2811_led_t colors[] = {
//  0x??BBGGRR
    0x00000020,  // red
    0x00000720,  // orange
    0x00001720,  // yellow
    0x00002010,  // yellow green
    0x00002000,  // green
    0x00052000,  // 
    0x00171700,  // cyan
    0x00200700,  // 
    0x00200000,  // blue
    0x00200007,  // violet
    0x00170017,  // pink
    0x00070020,  // 
};

static void serializeMatrix(ws2811_t* ledInterface, ws2811_led_t** matrix, uint32_t height, uint32_t width) {
    for (uint32_t x = 0; x < width; x++) {
        for (uint32_t y = 0; y < height; y++) {
            ledInterface->channel[0].leds[y + x * height] = matrix[x % 2 ? y : height - y - 1][width - x - 1];
        }
    }
}

// static void clearMatrix(ws2811_led_t** matrix, uint32_t height, uint32_t width) {
//     for (uint32_t y = 0; y < height; y++) {
//         for (uint32_t x = 0; x < width; x++) {
//             matrix[y][x] = 0;
//         }
//     }
// }

int A1[32] = { 0, 0, 7, 7, 0, 0, 7, 0, 0, 0, 0, 0, 0, 7, 0, 0, 7, 7, 7, 7, 0, 0, 7, 7, 7, 7, 0, 0, 0, 0, 0, 0 };
int A2[32] = { 0, 0, 7, 7, 0, 0, 7, 0, 0, 0, 0, 0, 0, 7, 0, 0, 7, 7, 7, 7, 0, 0, 7, 7, 7, 7, 0, 0, 0, 0, 0, 0 };
int A3[32] = { 0, 0, 7, 7, 0, 0, 7, 0, 0, 7, 7, 7, 7, 7, 0, 0, 7, 7, 7, 7, 0, 0, 7, 7, 7, 7, 0, 0, 7, 7, 0, 0 };
int A4[32] = { 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 7, 7, 7, 0, 0, 7, 7, 7, 7, 0, 0, 7, 7, 7, 7, 0, 0, 7, 7, 0, 0 };
int A5[32] = { 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 7, 7, 7, 0, 0, 7, 7, 7, 7, 0, 0, 7, 7, 7, 7, 0, 0, 7, 7, 0, 0 };
int A6[32] = { 0, 0, 7, 7, 0, 0, 7, 0, 0, 7, 7, 7, 7, 7, 0, 0, 7, 7, 7, 7, 0, 0, 7, 7, 7, 7, 0, 0, 7, 7, 0, 0 };
int A7[32] = { 0, 0, 7, 7, 0, 0, 7, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0 };
int A8[32] = { 0, 0, 7, 7, 0, 0, 7, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0 };

int* testColors[8] = {A1, A2, A3, A4, A5, A6, A7, A8};

static void insertColors(ws2811_led_t** matrix, int** colorMap, uint32_t height, uint32_t width) {
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            matrix[y][x] = colors[colorMap[y][x]];
        }
    }
}

// static void printMatrix(int** matrix, uint32_t height, uint32_t width) {
//     for (uint32_t rowIndex = 0; rowIndex < height; rowIndex++) {
//         for (uint32_t cellIndex = 0; cellIndex < width; cellIndex++) {
//             printf("%d ", matrix[rowIndex][cellIndex]);
//         }
//         printf("\n");
//     }
// }

int drawStill(uint32_t height, uint32_t width, uint8_t brightness, int** colorIndexes) {
    ws2811_led_t** matrix;
    matrix = (ws2811_led_t**) malloc(sizeof(*matrix) * height);
    for (uint32_t rowIndex = 0; rowIndex < height; rowIndex++) {
        matrix[rowIndex] = (ws2811_led_t*) malloc(sizeof(*matrix[rowIndex]) * width);
    }
    
    ws2811_t ledInterface = {
        .freq = TARGET_FREQ,
        .dmanum = DMA,
        .channel = {
            [0] = {
                .gpionum = GPIO_PIN,
                .count = height * width,
                .invert = 0,
                .brightness = brightness,
                .strip_type = STRIP_TYPE,
            },
            [1] = {
                .gpionum = 0,
                .count = 0,
                .invert = 0,
                .brightness = 0,
            },
        },
    };
    
    ws2811_return_t initResult;
    if ((initResult = ws2811_init(&ledInterface)) != WS2811_SUCCESS) {
        fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(initResult));
    } else {
        insertColors(matrix, colorIndexes, height, width);
        serializeMatrix(&ledInterface, matrix, height, width);
        ws2811_render(&ledInterface);
        ws2811_fini(&ledInterface);
    }
    
    return initResult;
}