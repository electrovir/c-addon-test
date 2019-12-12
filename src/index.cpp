#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdarg.h>
#include <getopt.h>

#include <napi.h>

#include "greeting.h"
extern "C" {
#include "my-thing.h"
#include "pcm.h"
#include "ws281x-main.h"
#include "clk.h"
#include "gpio.h"
#include "dma.h"
#include "pwm.h"
#include "version.h"
#include "matrix-control.h"
}

#include "ws2811.h"

Napi::String greetHello(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    std::string user = (std::string) info[0].ToString();
    std::string result = helloUser( user );

    return Napi::String::New(env, result);
}

Napi::String test(const Napi::CallbackInfo& info) {
    printf("%d.%d.%d\n", VERSION_MAJOR, VERSION_MINOR, VERSION_MICRO);
    std::string result = std::to_string(VERSION_MAJOR) + "." + std::to_string(VERSION_MINOR) + "." +  std::to_string(VERSION_MICRO) + " clk thing: " + std::to_string(CM_CLK_CTL_SRC_TSTDBG0) + " gpio: " + std::to_string(GPIO_OFFSET) + " dma: " + std::to_string(PAGE_SIZE) + " tgbw: " + std::to_string(SK6812_STRIP_RGBW) + " importd: " + std::to_string(doMyThing(6)) + " lights: " + std::to_string(lightsTest());   
    return Napi::String::New(info.Env(), result);
}

// function drawStill(height: number, width: number, brightness: number, colorIndexMap: number[][]): number {
Napi::Number ledDrawStillImage(const Napi::CallbackInfo& info) {
    
    uint32_t height = (uint32_t) info[0].ToNumber();
    uint32_t width = (uint32_t) info[1].ToNumber();
    std::uint8_t brightness = (std::uint8_t) info[2].ToNumber().Int32Value();
    
    Napi::TypedArray inputColorMap = info[3].As<Napi::TypedArray>();
    
    int** colorMap;
    colorMap = (int**) malloc(sizeof(*colorMap) * height);
    for (uint32_t rowIndex = 0; rowIndex < height; rowIndex++) {
        colorMap[rowIndex] = (int*) malloc(sizeof(*colorMap[rowIndex]) * width);
    }
    
    for (uint32_t rowIndex = 0; rowIndex < height; rowIndex++) {
        for (uint32_t cellIndex = 0; cellIndex < width; cellIndex++) {
            Napi::Value value = inputColorMap[rowIndex * width + cellIndex];
            colorMap[rowIndex][cellIndex] = (int) value.As<Napi::Number>();
        }
    }
    
    int result = drawStill(height, width, brightness, colorMap);
    
    return Napi::Number::New(info.Env(), result);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {

    exports.Set(
        Napi::String::New(env, "greetHello"),
        Napi::Function::New(env, greetHello)
    );
    
    exports.Set(
        Napi::String::New(env, "test"),
        Napi::Function::New(env, test)
    );
    
    exports.Set(
        Napi::String::New(env, "drawStill"),
        Napi::Function::New(env, ledDrawStillImage)
    );

    return exports;
}

NODE_API_MODULE(greet, Init)