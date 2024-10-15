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


    FlatVector anchor = { 400,300 };
    FixedPointElasticity Elasticity(&anchor,0.05f,60.0f);
        
    Resistance fr(1,1);



    BodyColor color = { 255, 100, 100, 255 };
    FlatVector v1 = { 400,150 };
    FlatVector v2 = { 200,200 };
    // 清除屏幕背景色
    Brush brush(renderer);
    brush.Clear(0, 0, 0, 0);
    


   //BodyManager1.CreateBody(30,color,20,v1);

    std::vector<SDL_FPoint> points = { {0, 0},
                                      {100, 0},
                                      {100, 100},
                                      {0, 100} };

    BodyManager1.CreateBody(points, color, 20);
    std::vector<Body>::iterator body = BodyManager1.FindBody(1);
   // body->SetVelocity(FlatVector(10,0));
    body->MoveTo(FlatVector(400, 310));

    //Elasticity.UpdateForce(body, 0.0f);
    //Gravity_Gen.UpdateForce(body, 0.0f);
    
  
   
    
   
    // 运行主循环直到用户关闭窗口
   
    //BodyManager1.CoutBodyList();
   

  

    




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
               // body->MoveTo(FlatVector(x, y));
            }
        } 
        brush.DrawPoint(FlatVector(400, 300), color.r, color.g, color.b, color.a);;
      
      // BodyManager1.FindBody(1)->Rotation(1);
      
        Elasticity.UpdateForce(body, 0.0f);
        fr.UpdateForce(body, 0.0f);






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
