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

enum BoundindBox {
	NONEBOX = -1,
	CIRCLEBOX = 0,
	AABBBOX = 1,
	OBBBOX = 2,
};



class Body {
public:

	//特征值
	int body_id_ = 0;
	Shape shape_ = NONE;
	float radius_ = 0.0f;
	BodyColor color_ = { 0,0,0,0 };
	std::vector<SDL_FPoint> vertices_ = {};
	std::vector<SDL_FPoint> vertices_aabb = { {0.0f, 0.0f},
										 {0.0f, 0.0f},
										 {0.0f, 0.0f},
										 {0.0f, 0.0f} };
	BoundindBox bounding_box_ = NONEBOX;
	BodyColor color_box_ = { 255, 100, 100, 255 };

	//物理属性值
	float restitusion = 0.0f;
	float angle = 0.0f;
	float angular_velocity = 0.0f;//对于二维情况下角速度的方向是垂直于屏幕的所以使用float
	float density = 0.0f;
	float mass_ = 0.0f;
	float inverse_mass_ = mass_>0 ? 1/mass_: 0 ;
	FlatVector mass_center_ = { 0.0f,0.0f };	//position
	FlatVector velocity_ = { 0.0f,0.0f };
	FlatVector acceleration_ = { 0.0f,0.0f };

	~Body();
	Body(Shape shape, float radius, BodyColor color, float mass, FlatVector mass_center, int body_id);
	Body(Shape shape, std::vector<SDL_FPoint> vertices, BodyColor color, float mass, FlatVector mass_center, int body_id);
	void SetVelocity(const FlatVector v1);
	void SetAcceleration(const FlatVector v1);
	void Move(const FlatVector v1);
	void MoveTo(const FlatVector v1);
	void Rotation(const float angle);
	void AddForce(const FlatVector v1);
	void ClearAcceleration();
	
	void GetAABB();

};




typedef struct OBB {
	//先不实现
	std::vector<SDL_FPoint> vertices_ = { {0.0f, 0.0f},
										 {0.0f, 0.0f},
										 {0.0f, 0.0f},
										 {0.0f, 0.0f} };

	void GetOBB(const Body body);
	void RenderOBB(Brush& brush);
};


class BodyManager {
public:
	
	BodyManager();
	~BodyManager();
	std::vector<Body> body_lists_;
	int id_count = 0;
	bool CreateBody(float radius                   , BodyColor color, float mass, FlatVector mass_center);
	bool CreateBody(std::vector<SDL_FPoint> vertices, BodyColor color, float mass);

	std::vector<Body>::iterator FindBody(const int body_id);
	bool DestroyBody(const int body_id);
	void RenderBody(Brush& brush);
	void RenderAABB(Brush& brush);
	void CoutBodyList();
	
};


FlatVector GetMassCenter(Body& body);
FlatVector GetMassCenter(std::vector<SDL_FPoint> points);
