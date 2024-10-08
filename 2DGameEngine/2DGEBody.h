#pragma once
#include <vector>
#include "2DGEmath.h"
#include <SDL.h>
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

class Body {
public:
	Shape shape_ = NONE;
	float radius_ = 0;
	float mass_ = 0;
	BodyColor color_ = { 0,0,0,0 };
	FlatVector mass_center_ = { 0,0 };
	SDL_Vertex* vertices_ = {};

	Body();
	Body(Shape shape, float radius        , BodyColor color, float mass, FlatVector mass_center);
	Body(Shape shape, SDL_Vertex* vertices, BodyColor color, float mass, FlatVector mass_center);
	~Body();
};

class BodyManager {
public:
	BodyManager();
	~BodyManager();

	std::vector<Body> body_lists;

	bool CreateBody(float radius        , BodyColor& color, float mass, FlatVector& mass_center);
	bool CreateBody(SDL_Vertex* vertices, BodyColor& color, float mass, FlatVector& mass_center);
	bool DestroyBody();
	int FindBody();

};

