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
    
   
    Gravity Gravity_Gen = FlatVector(0.0f, 3.0f);
    
    BodyColor color = { 255, 100, 100, 255 };
    // 清除屏幕背景色
    Brush brush(renderer);
    world.brush = brush;
    brush.Clear(0, 0, 0, 0);
    std::vector<FlatVector> points = {{100.0f, 100.0f},
                                      {200.0f, 100.0f},
                                      {250.0f, 200.0f},
                                      {150.0f, 285.0f},
                                      {50.0f , 200.0f}};

    std::vector<FlatVector> points1 = { {10.0f,500.0f},
                                        { 790.0f,500.0f },
                                        { 790.0f,590.0f },
                                        { 10.0f,590.0f },
};

    std::vector<FlatVector> points2 = { {0.0f,0.0f},
                                       { 100.0f,0.0f },
                                       { 100.0f,100.0f },
                                       { 0.0f,100.0f },
    };
  BodyManager1.CreateBody(30.0f, color, 10.0f, FlatVector(100, 350),false,0.8);
   // BodyManager1.CreateBody(points2, color, 1.0f, false, 0.4);

  BodyManager1.CreateBody(points2, color, 10.0f, false, 0.4);
  BodyManager1.CreateBody(points1, color, 1.0f, true, 1);

    /*for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            BodyManager1.CreateBody(10.0f, color, 1.0f, FlatVector(450+j*20, 250+i*20), false, 0.5);
        }
       
    }*/
   
    //BodyManager1.CreateBody(points1, color, 10.0f, true, 0.5);
   

    std::vector<Body>::iterator body1 = BodyManager1.FindBody(1);

   // body1->MoveTo(FlatVector(0, 350));
    std::vector<Body>::iterator body2 = BodyManager1.FindBody(2);
    
  //  std::vector<Body>::iterator body3 = BodyManager1.FindBody(3);
    body1->MoveTo(FlatVector(400, 300));
    
    
    
   

  //  body1->velocity_ = FlatVector(0,10.0f);








    



    

    
    BodyManager1.CoutBodyList();
    BodyManager1.CoutBodyMass();
    bool quit = false;
    SDL_Event e;
    float x = 0.0f;
    float y = 0.0f;
    float  time = 1.0f;
    // 运行主循环直到用户关闭窗口
    while (!quit) {
        float start = static_cast<float>(clock());
        
        while (SDL_PollEvent(&e)) {
             // ImGui_ImplSDL2_ProcessEvent(&e);
            if (e.type == SDL_QUIT) {
              
                quit = true;
            } 
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_UP:
                    body1->SetVelocity(FlatVector(0.0f, -10.0f));
                  //  printf("up");
                    break;
                case SDLK_DOWN:
                    body1->SetVelocity(FlatVector(0.0f, 10.0f));
                   // printf("down");
                    break;
                case SDLK_LEFT:
                    body1->SetVelocity(FlatVector(-10.0f, 0.0f));
                    //printf("left");
                    break;
                case SDLK_RIGHT:
                    body1->SetVelocity(FlatVector(10.0f, 0.0f));
                    //printf("right");
                    break;
                case SDLK_q:
                    body1->angular_velocity_ = -1;
                    break;

                case SDLK_e:
                    body1->angular_velocity_ = 1;
                    std::cout << "E键被按下" << std::endl;
                    break;
                }
            }
        } 
      
   //     std::cout << body1->velocity_ << std::endl;
       
       
       
       
      

       // body1->angular_velocity_ += 0.01;




        
        brush.show();
        brush.Clear(0, 0, 0, 0);
        world.Interation(BodyManager1.body_list_, time);
       // BodyManager1.RenderAABB(brush);
        BodyManager1.RenderBody(brush);
        
        float end = static_cast<float>(clock());
        time = (end - start) / CLOCKS_PER_SEC;
       
       
    }

            // 清理
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

    return 0;

   
}

