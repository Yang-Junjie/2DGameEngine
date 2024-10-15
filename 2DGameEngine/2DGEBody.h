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


class Body {
public:

	//特征值
	int body_id_ = 0;
	Shape shape_ = NONE;
	float radius_ = 0.0f;
	BodyColor color_ = { 0,0,0,0 };
	std::vector<SDL_FPoint> vertices_ = {};

	//物理属性值
	/*由于力在物体上不会直接体现，直接将力转为加速度*/
	float mass_ = 0.0f;
	float inverse_mass_ = 0.0f;
	FlatVector mass_center_ = { 0.0f,0.0f };	//position
	FlatVector velocity_ = { 0.0f,0.0f };
	FlatVector acceleration_ = { 0.0f,0.0f };



	Body();
	~Body();
	Body(Shape shape, float radius, BodyColor color, float mass, FlatVector mass_center, int body_id);
	Body(Shape shape, std::vector<SDL_FPoint> vertices, BodyColor color, float mass, FlatVector mass_center, int body_id);

	void SetVelocity(const FlatVector v1);
	void SetAcceleration(const FlatVector v1);

	void Move(const FlatVector v1);
	void MoveTo(const FlatVector v1);

	void Rotation(const float angle);

	void AddForce(const FlatVector v1);
};

class BodyManager {
public:
	BodyManager();
	~BodyManager();

	std::vector<Body> body_lists_;
	int id_count = 0;
	bool CreateBody(float radius                   , BodyColor color, float mass, FlatVector mass_center);
	bool CreateBody(std::vector<SDL_FPoint> vertices, BodyColor color, float mass);
	
	bool DestroyBody(const int body_id);
	std::vector<Body>::iterator FindBody(const int body_id);
	void RenderBody(Brush& brush);
	void CoutBodyList();
	
};


FlatVector GetMassCenter(Body& body);
FlatVector GetMassCenter(std::vector<SDL_FPoint> points);
