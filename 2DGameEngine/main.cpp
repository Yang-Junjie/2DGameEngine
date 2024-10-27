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
        1000,
        800,
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
    std::vector<FlatVector> points = {{10.0f, 10.0f},
                                      {20.0f, 10.0f},
                                      {25.0f, 20.0f},
                                      {15.0f, 28.0f},
                                      {5.0f , 20.0f}};

    std::vector<FlatVector> points1 = { {10.0f ,790.0f},{ 990.0f,790.0f },
                                        { 990.0f,800.0f },{ 10.0f,800.0f },
};

    std::vector<FlatVector> points2 = { {0.0f,0.0f},
                                       { 100.0f,0.0f },
                                       { 100.0f,100.0f },
                                       { 0.0f,100.0f },
    };

    std::vector<FlatVector> points3 = { {0.0f,0.0f},
                                       { 500.0f,0.0f },
                                       { 500.0f,50.0f },
                                       { 0.0f,50.0f },
    };
   
    //BodyManager1.CreateBody(points2, color, 1.0f, false, 0.4f);
   
    BodyManager1.CreateBody(points1, color, 1.0f, true, 1.f);
    

   // std::vector<Body>::iterator body1 = BodyManager1.FindBody(1);

   // std::vector<Body>::iterator body3 = BodyManager1.FindBody(3);
  

   // body3->MoveTo(FlatVector(100, 300));
    //body3->Rotation(30*(M_PI / 180.0));
    
   

  //  body1->velocity_ = FlatVector(0,10.0f);








    



    

    
   // BodyManager1.CoutBodyList();
   // BodyManager1.CoutBodyMass();
    bool quit = false;
    SDL_Event e;
    float x = 0.0f;
    float y = 0.0f;
    float  time = 1.0f;
    float timecount = 0;
    // 运行主循环直到用户关闭窗口
    while (!quit) {
        
        
        while (SDL_PollEvent(&e)) {
             // ImGui_ImplSDL2_ProcessEvent(&e);
            if (e.type == SDL_QUIT) {
              
                quit = true;
            } 
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                // 检查是否是鼠标左键
                if (e.button.button == SDL_BUTTON_LEFT) {
                    // 获取鼠标点击的位置
                    int x = e.button.x;
                    int y = e.button.y;
                    BodyManager1.CreateBody(points2, color, 1.0f, false, 0.4f);
                    BodyManager1.FindBody(BodyManager1.body_list_.size())->MoveTo(FlatVector(x,y));

                    //printf("Mouse left button clicked at (%d, %d)\n", x, y);
                }
            }
            //else if (e.type == SDL_KEYDOWN) {
            //    switch (e.key.keysym.sym) {
            //    case SDLK_UP:
            //        body1->SetVelocity(FlatVector(0.0f, -10.0f));
            //      //  printf("up");
            //        break;
            //    case SDLK_DOWN:
            //        body1->SetVelocity(FlatVector(0.0f, 10.0f));
            //       // printf("down");
            //        break;
            //    case SDLK_LEFT:
            //        body1->SetVelocity(FlatVector(-10.0f, 0.0f));
            //        //printf("left");
            //        break;
            //    case SDLK_RIGHT:
            //        body1->SetVelocity(FlatVector(10.0f, 0.0f));
            //        //printf("right");
            //        break;
            //    case SDLK_q:
            //        body1->angular_velocity_ = -0.1;
            //        break;

            //    case SDLK_e:
            //        body1->angular_velocity_ = 0.1;
            //        std::cout << "E键被按下" << std::endl;
            //        break;

            //
            //    }
            }
            
        float start = static_cast<float>(clock());


        
        brush.show();
        brush.Clear(0, 0, 0, 0);
        world.Interation(BodyManager1.body_list_, time);
       // BodyManager1.RenderAABB(brush);
        BodyManager1.RenderBody(brush);
        
        float end = static_cast<float>(clock());
        time = (end - start) / CLOCKS_PER_SEC;
        timecount += time;
        if (timecount > 3) {
            std::cout << 1 / time << std::endl;
            std::cout <<"物体数" << BodyManager1.body_list_.size() << std::endl;
            timecount = 0;
        }
       
    }

            // 清理
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

    return 0;

   
}

