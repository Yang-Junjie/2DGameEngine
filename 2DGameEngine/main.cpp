#include <SDL.h>
#include <ctime>
#include <iostream>
#include <SDL_opengl.h>
#include "2DGEDraw.h"
#include "2DGEBody.h"
#include "2DGEWorld.h"
#include "2DGEForceGen.h"
#include "2DGECollision.h"

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
    
    BodyColor color = { 255, 100, 100, 255 };
    // 清除屏幕背景色
    Brush brush(renderer);
    brush.Clear(0, 0, 0, 0);
    std::vector<FlatVector> points = {{100.0f, 100.0f},
                                      {200.0f, 100.0f},
                                      {250.0f, 200.0f},
                                      {150.0f, 285.0f},
                                      {50.0f , 200.0f}};
    BodyManager1.CreateBody(points, color, 20.0f);

    BodyManager1.CreateBody(points, color, 10.0f);
    BodyManager1.CreateBody(points, color, 10.0f);
   // BodyManager1.CreateBody(100.0f, color, 20.0f, FlatVector(400, 300));
    //BodyManager1.CreateBody(30.0f, color, 30.0f, FlatVector(100, 100));
    std::vector<Body>::iterator body1 = BodyManager1.FindBody(1);
    std::vector<Body>::iterator body2 = BodyManager1.FindBody(2);
    body2->MoveTo(FlatVector(500, 200));
    //body->MoveTo(FlatVector(50,100));
    
    
    
   














    bool show_demo_window = true;
    bool button_pressed = false;


    BodyManager1.CoutBodyList();
    bool quit = false;
    SDL_Event e;
    int x = 0;
    int y = 0;
    float  time = 1.0f;
    // 运行主循环直到用户关闭窗口
    while (!quit) {
        float start = clock();
        while (SDL_PollEvent(&e)) {
             // ImGui_ImplSDL2_ProcessEvent(&e);
            if (e.type == SDL_QUIT) {
              
                quit = true;
            } 
            else if (e.type == SDL_MOUSEMOTION) {
                x = e.motion.x;
                y = e.motion.y;
                //printf("Mouse Position: (%d, %d)\n", x, y);
                body1->MoveTo(FlatVector(x, y));
            }
        } 
       // brush.DrawPoint(FlatVector(400.0f, 300.0f), color.r, color.g, color.b, color.a);;
      
        body1->Rotation(1);
      
       
       // Gravity_Gen.ClearPreviousForce();
       
        
        //Gravity_Gen.UpdateForce(body, 0.0f);
      // std::cout << body->acceleration_ << std::endl;




        
        brush.show();
        brush.Clear(0, 0, 0, 0);
       // BodyManager1.RenderAABB(brush);
        BodyManager1.RenderBody(brush);
        
        world.Interation(BodyManager1.body_list_, time);
        float end = clock();
        time = (end - start);
       
    }

            // 清理
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

    return 0;

   
}

