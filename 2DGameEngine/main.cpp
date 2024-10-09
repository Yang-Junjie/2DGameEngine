#include <SDL.h>
#include <iostream>
#include "2DGEDraw.h"
#include "2DGEBody.h"
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





























    BodyManager BodyManager1;
    BodyColor color = { 255, 100, 100, 255 };
    FlatVector v1 = { 100,100 };
    FlatVector v2 = { 200,200 };
    // �����Ļ����ɫ
    Brush brush(renderer);
    brush.Clear(255, 245, 238, 255);



    BodyManager1.CreateBody(30,color,20,v1);
    BodyManager1.CreateBody(40, color, 20, v1);
    BodyManager1.CreateBody(50, color, 20, v1);
    BodyManager1.CreateBody(60, color, 20, v1);

    

    
    // �������εĶ��㣨���磬һ���ı��Σ�  
    SDL_Point points[] = { {100, 100}, {200, 100}, {200, 200}, {100, 200} };
    int numPoints = sizeof(points) / sizeof(points[0]);

    BodyManager1.CreateBody(points,numPoints,color,20,v1);
    


   
   


    BodyManager1.CoutBodyList();
    BodyManager1.RenderBody(brush);
       // ������ѭ��ֱ���û��رմ���
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } 
        } 
        



        
        


















        brush.show();

    }

    // ����
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


    return 0;
}
