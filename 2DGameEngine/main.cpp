#include <SDL.h>
#include <ctime>
#include <iostream>
#include "2DGEDraw.h"
#include "2DGEBody.h"
#include "2DGEWorld.h"
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




























    World world;
    BodyManager BodyManager1;
    BodyColor color = { 255, 100, 100, 255 };
    FlatVector v1 = { 100,100 };
    FlatVector v2 = { 200,200 };
    // 清除屏幕背景色
    Brush brush(renderer);
    brush.Clear(0, 0, 0, 0);



    BodyManager1.CreateBody(30,color,20,v1);
    BodyManager1.FindBody(1)->SetVelocity(FlatVector(2,0));
    BodyManager1.FindBody(1)->SetAcceleration(FlatVector(0, 9.8));
    /*BodyManager1.CreateBody(40, color, 20, v1);
    BodyManager1.CreateBody(50, color, 20, v1);
    BodyManager1.CreateBody(60, color, 20, v1);*/

    

    
    //// 定义多边形的顶点（例如，一个四边形）  

    //std::vector<SDL_FPoint> points = { {0, 0},
    //                                  {100, 0},
    //                                  {100, 100},
    //                                  {0, 100}};

    //BodyManager1.CreateBody(points,color,20);
    //BodyManager1.CreateBody(points, color, 20);
    //BodyManager1.FindBody(5)->MoveTo(FlatVector(100,100));
   
       // 运行主循环直到用户关闭窗口
    bool quit = false;
    SDL_Event e;
    int x = 0;
    int y = 0;
    float  time = 0;
    while (!quit) {
        float start = clock();
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } 
            else if (e.type == SDL_MOUSEMOTION) {
                x = e.motion.x;
                y = e.motion.y;
               // printf("Mouse Position: (%d, %d)\n", x, y);
              
            }
        } 
        



       // BodyManager1.FindBody(5)->MoveTo(FlatVector(x, y));
       //BodyManager1.FindBody(5)->Rotation(1);
       




        brush.Clear(0, 0, 0, 0);
       
       // BodyManager1.CoutBodyList();
        BodyManager1.RenderBody(brush);

















        world.Interation(BodyManager1.body_lists_, time);
        brush.show();
        float end = clock();
        time = (end - start);
        std::cout << time << std::endl;
    }

    // 清理
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


    return 0;
}
