#include "2DGEDraw.h"
Brush::Brush(SDL_Renderer* renderer):renderer_(renderer) {

}

Brush::~Brush() {

}

void Brush::Clear(int r, int g, int b, int a){
	//使用指定颜色清空屏幕
	SDL_SetRenderDrawColor(this->renderer_, r, g, b, a);
	SDL_RenderClear(this->renderer_);
}
void Brush::show() {
	SDL_RenderPresent(this->renderer_);
}

void Brush::DrawPoint(float x, float y, int r, int g, int b, int a) {
	SDL_SetRenderDrawColor(this->renderer_, r, g, b, a);
	SDL_RenderDrawPoint(this->renderer_, x, y);
}


void Brush::DrawLine(float x1, float y1, float x2, float y2, int r, int g, int b, int a) {
	SDL_SetRenderDrawColor(this->renderer_, r, g, b, a);
	SDL_RenderDrawLine(this->renderer_, x1, y1, x2, y2);
}
void Brush::DrawPoint(const FlatVector v, int r, int g, int b, int a) {
     DrawPoint(v.x, v.y, r, g, b, a);
}
void Brush::DrawLine(const FlatVector v1, const FlatVector v2, int r, int g, int b, int a) {
    DrawLine(v1.x, v1.y,v2.x,v2.y, r, g, b, a);
}

void Brush::DrawCircle(float centreX, float centreY, float radius, int r, int g, int b, int a) {
    SDL_SetRenderDrawColor(this->renderer_, r, g, b, a);
    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(this->renderer_, centreX + x, centreY - y);
        SDL_RenderDrawPoint(this->renderer_, centreX + x, centreY + y);
        SDL_RenderDrawPoint(this->renderer_, centreX - x, centreY - y);
        SDL_RenderDrawPoint(this->renderer_, centreX - x, centreY + y);
        SDL_RenderDrawPoint(this->renderer_, centreX + y, centreY - x);
        SDL_RenderDrawPoint(this->renderer_, centreX + y, centreY + x);
        SDL_RenderDrawPoint(this->renderer_, centreX - y, centreY - x);
        SDL_RenderDrawPoint(this->renderer_, centreX - y, centreY + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}
void Brush::DrawPolygon(std::vector<SDL_Point> &points, int r, int g, int b, int a) {
   
    SDL_SetRenderDrawColor(this->renderer_, r, g, b, a);
    for (size_t i = 0; i < points.size(); ++i) {
        SDL_RenderDrawLine(this->renderer_, points[i].x, points[i].y, points[(i+1)%points.size()].x, points[(i + 1) % points.size()].y);
    }
   
}


