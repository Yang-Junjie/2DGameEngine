#pragma once
#include <vector>
#include "2DGEmath.h"
#include <SDL.h>
#include <iostream>
#include "2DGEDraw.h"
enum Shape {
	NONE = -1,
	CIRCLE = 0,
	POLTGON = 1
};
struct BodyColor
{
	int r;
	int g;
	int b;
	int a;
};
struct FVertices {
	FlatVector position;
};
class Body {
public:
	int body_id_ = 0;
	Shape shape_ = NONE;
	float radius_ = 0;
	float mass_ = 0;
	BodyColor color_ = { 0,0,0,0 };
	FlatVector mass_center_ = { 0,0 };
	std::vector<SDL_Point> vertices_ = {};

	Body();
	Body(Shape shape, float radius                   , BodyColor color, float mass, FlatVector mass_center, int body_id);
	Body(Shape shape, std::vector<SDL_Point> vertices, BodyColor color, float mass, FlatVector mass_center, int body_id);
	~Body();
};

class BodyManager {
public:
	BodyManager();
	~BodyManager();

	std::vector<Body> body_lists_;
	int id_count = 0;
	bool CreateBody(float radius                   , BodyColor color, float mass, FlatVector mass_center);
	bool CreateBody(std::vector<SDL_Point> vertices, BodyColor color, float mass, FlatVector mass_center);
	
	bool DestroyBody(int body_id);
	std::vector<Body>::iterator FindBody(int body_id);
	void RenderBody(Brush& brush);
	void CoutBodyList();
	
};


FlatVector GetMassCenter(Body& body);
