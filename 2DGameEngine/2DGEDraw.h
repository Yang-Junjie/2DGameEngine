#pragma once
#include <vector>
#include <SDL.h>
#include "2DGEmath.h"



class Brush {
private:
	SDL_Renderer* renderer_ = nullptr;
public:
	Brush(SDL_Renderer* renderer);
	~Brush();

	void Clear(int r,int g,int b,int a);
	void show();

	void DrawPiex(float x, float y, int r, int g, int b, int a);
	void DrawPiex(const FlatVector v, int r, int g, int b, int a);


	void DrawLine(float x1, float y1, float x2, float y2, int r, int g, int b, int a);
	void DrawLine(const FlatVector v1, const FlatVector v2, int r, int g, int b, int a);

	void DrawCircle(float x, float y, float radius, int r, int g, int b, int a);
	void DrawCircle(const FlatVector v, float radius, int r, int g, int b, int a);

	void DrawFillCircle(float cx, float cy, float radius, int r, int g, int b, int a);
	void DrawFillCircle(const FlatVector v, float radius, int r, int g, int b, int a);

	void DrawPoint(float cx, float cy, int r, int g, int b, int a);
	void DrawPoint(const FlatVector v, int r, int g, int b, int a);

	void DrawPolygon(std::vector<FlatVector>& points, int r, int g, int b, int a);

	

	/*void DrawHollowCirlce(float x1, float y1, float x2, float y2, int r, int g, int b, int a);
	void DrawHollowPolygon(const SDL_Vertex* vertices, int num_vertices, const int* indices, int num_indices);*/
}; 