#include <SDL.h>
#include <ctime>
#include <iostream>
#include "2DGEDraw.h"
#include "2DGEBody.h"
#include "2DGEWorld.h"
#include "2DGEForceGen.h"
#include "2DGEBoundVol.h"
int main(int argc, char* argv[])
{
    
    // ��ʼ��SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // ��������
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

    // ������Ⱦ��
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
    AABB aabb;
    Gravity Gravity_Gen = FlatVector(0.0f, 9.8f);
    BodyColor color = { 255, 100, 100, 255 };
    // �����Ļ����ɫ
    Brush brush(renderer);
    brush.Clear(0, 0, 0, 0);
    std::vector<SDL_FPoint> points = {{100.0f, 100.0f},
                                      {200.0f, 100.0f},
                                      {250.0f, 200.0f},
                                      {150.0f, 285.0f},
                                      {50.0f , 200.0f}};
    BodyManager1.CreateBody(points, color, 20.0f);


    //BodyManager1.CreateBody(30.0f, color, 30.0f, FlatVector(100, 100));
    std::vector<Body>::iterator body = BodyManager1.FindBody(1);
    //body->MoveTo(FlatVector(50,100));
    
    body->SetVelocity(FlatVector(1, 0));
    
   
    //BodyManager1.CoutBodyList();
    bool quit = false;
    SDL_Event e;
    int x = 0;
    int y = 0;
    float  time = 1.0f;
    // ������ѭ��ֱ���û��رմ���
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
      
        body->Rotation(1);
      
       
       // Gravity_Gen.ClearPreviousForce();
       
        
        //Gravity_Gen.UpdateForce(body, 0.0f);
      // std::cout << body->acceleration_ << std::endl;








        aabb.GetAABB(body);
        aabb.RenderAABB(brush);
        brush.show();
        brush.Clear(0, 0, 0, 0);
        BodyManager1.RenderBody(brush);
        world.Interation(BodyManager1.body_lists_, time);
        float end = clock();
        time = (end - start);
       //std::cout << time<< std::endl;
        //std::cout <<body->mass_center_<< std::endl;
    }

    // ����
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


    return 0;
}
