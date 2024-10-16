#include <SDL.h>
#include <ctime>
#include <iostream>
#include "2DGEDraw.h"
#include "2DGEBody.h"
#include "2DGEWorld.h"
#include "2DGEForceGen.h"
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


    Gravity Gravity_Gen = FlatVector(0.0f, 9.8f);
    FlatVector choar = { 50,50 };
    


    BodyColor color = { 255, 100, 100, 255 };
    // 清除屏幕背景色
    Brush brush(renderer);
    brush.Clear(0, 0, 0, 0);
    


   //BodyManager1.CreateBody(30,color,20,v1);

    std::vector<SDL_FPoint> points = { {0.0f, 0.0f},
                                      {100.0f, 0.0f},
                                      {100.0f, 100.0f},
                                      {0.0f, 100.0f} };

    BodyManager1.CreateBody(points, color, 20.0f);
    std::vector<Body>::iterator body = BodyManager1.FindBody(1);
    body->MoveTo(FlatVector(50,100));
  
  
    
    
   
    // 运行主循环直到用户关闭窗口
   
    //BodyManager1.CoutBodyList();
   

  

    
    Gravity_Gen.UpdateForce(body, 0.0f);

    
    


    bool quit = false;
    SDL_Event e;
    int x = 0;
    int y = 0;
    float  time = 1.0f;
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
               // body->MoveTo(FlatVector(x, y));
            }
        } 
        brush.DrawPoint(FlatVector(400.0f, 300.0f), color.r, color.g, color.b, color.a);;
      
      // BodyManager1.FindBody(1)->Rotation(1);
      
       
        Gravity_Gen.ClearPreviousForce();
        Gravity_Gen.UpdateForce(body, 0.0f);
        
       
       std::cout << body->acceleration_ << std::endl;




        brush.show();
        brush.Clear(0, 0, 0, 0);
        BodyManager1.RenderBody(brush);
        world.Interation(BodyManager1.body_lists_, time);
        float end = clock();
        time = (end - start);
       //std::cout << time<< std::endl;
        //std::cout <<body->mass_center_<< std::endl;
    }

    // 清理
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


    return 0;
}
