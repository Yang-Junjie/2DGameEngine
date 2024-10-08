#include <SDL.h>
#include <iostream>
#include "2DGEDraw.h"
#include "2DGEBody.h"
int main(int argc, char* argv[])
{
    
    // 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // 创建窗口
    SDL_Window* window = SDL_CreateWindow("2DGameEngine",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        800,
        600,
        SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // 创建渲染器
    SDL_Renderer* renderer = SDL_CreateRenderer(window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }





























    BodyManager BodyManager1;
    BodyColor color = { 255, 100, 100, 255 };
    FlatVector v1 = { 100,100 };
    FlatVector v2 = { 200,200 };
    //BodyManager1.CreateBody(v1, 100, color);

    // 清除屏幕背景色
    Brush brush(renderer);
    brush.Clear(255, 245, 238, 255);





   

    // 运行主循环直到用户关闭窗口
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } 
        } 
        



        brush.DrawLine(v1,v2,color.r, color.g, color.b, color.a);
        brush.show();




















    }

    // 清理
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


    return 0;
}
