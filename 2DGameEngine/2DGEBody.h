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

enum BoundingBox {
	NONEBOX = -1,
	CIRCLEBOX = 0,
	AABBBOX = 1,
	OBBBOX = 2,
};




//物体类
//功能：是本物理引擎最基本的元素， 所有的物体都是Body类，通常不能直接调用Body的构造函数创建物体，需要通过BodyManager的CreateBody函数来创建物体
//能创建多边形，圆形两种形状。创建的物体包含了基本的物理属性
class Body {
public:

	//以下是物体的特征值
	//物体的id
	int body_id_ = 0;

	//物体的形状
	Shape shape_ = NONE;

	//圆形物体专属属性半径
	float radius_ = 0.0f;

	//物体的颜色分别对应，rgba
	BodyColor color_ = { 0,0,0,0 };

	//polygon形状专属属性
	std::vector<FlatVector> vertices_ = {};

	//aabb包围盒的顶点
	std::vector<FlatVector> vertices_aabb_ = {  {0.0f, 0.0f},
												{0.0f, 0.0f},
												{0.0f, 0.0f},
												{0.0f, 0.0f} };

	//使用哪种包围盒
	BoundingBox bounding_box_ = NONEBOX;

	//包围盒的颜色
	BodyColor color_box_ = { 255, 100, 100, 255 };

	//物理属性值
	//物体是否静止，false->非静止，true->静止
	bool stationary_ = false;

	//物体的恢复系数
	float restitution_ = 0.0f;

	//物体旋转的角度
	float angle = 0.0f;
	
	//物体的角速度,对于二维情况下角速度的方向是垂直于屏幕的所以使用float
	float angular_velocity_ = 0.0f;

	//物体的密度
	float density_ = 0.0f;

	//物体的质量
	float mass_ = 0.0f;

	//物体质量的倒数
	float inverse_mass_ = 0.0f;

	//position物体的位置
	FlatVector mass_center_ = { 0.0f,0.0f };

	//物体的线速度
	FlatVector velocity_ = { 0.0f,0.0f };

	//物体的加速度
	FlatVector acceleration_ = { 0.0f,0.0f };

	//固定静摩擦力
	float inherent_static_friction_ = 1.0f;

	//固定动摩擦力
	float inherent_dynamic_friction_ = 0.8f;

	//转动惯量
	float rotational_inertia_ = 0.0f;

	//转动惯量的倒数
	float inverse_rotational_inertia_ = 0.0f;

	//物体的析构函数
	~Body();

	//Circle构造函数
	//依次传入参数：shape,radius，color,mass>0,mass_center,body_id（可选，但不能使用重复id可通过body_list查看所有物体的id）
	Body(Shape shape, float radius, BodyColor color, float mass, FlatVector mass_center, int body_id, bool stationary, float restitution);

	//Polygon构造函数，重载函数
	//依次传入参数：shape,vertices，color,mass>0,mass_center,body_id（可选，但不能使用重复id可通过body_list查看所有物体的id）
	Body(Shape shape, std::vector<FlatVector> vertices, BodyColor color, float mass, FlatVector mass_center, int body_id, bool stationary, float restitution);

	//传入二维向量v1，设定物体现在的速度为v1
	void SetVelocity(const FlatVector v1);

	//传入二维向量v1，设定物体现在的加速度为v1
	void SetAcceleration(const FlatVector v1);

	//传入二维向量v1，将物体在现在的位置上位移v1
	void Move(const FlatVector& v1);

	//传入二维向量v1，将物体直接传送到v1处
	void MoveTo(const FlatVector v1);

	//传入角度制angle，将物体在原来的角度上旋转angle度
	void Rotation(const float angle);

	//传入二维向量v1，在物体上施加一个力
	void AddForce(const FlatVector v1);

	//清除物体的加速度，常用于清除重新解算加速度时使用
	void ClearAcceleration();
	
	//得到物体的AABB包围盒
	void GetAABB();
};





//BodyManager用于创建和销毁、管理Body
//其中的body_list_的成员变量存储了你所创建的所有Body
class BodyManager {
public:

	//存储所有的Body
	std::vector<Body> body_list_;

	//用于统计Body的id数
	int id_count = 0;

	//默认构造函数
	BodyManager();

	//析构函数
	~BodyManager();
	
	//用于创建Shape为Circle的Body，需要传入mass_center以表示圆心
	bool CreateBody(float radius                   , BodyColor color, float mass, FlatVector mass_center,bool stationary,float restitution);

	//用于创建Shape为Polygon的Body，不需要传入位置，应为顶点确定后其位置也确定了。
	bool CreateBody(std::vector<FlatVector> vertices, BodyColor color, float mass, bool stationary, float restitution);

	//使用body的id寻找Body，返回对应id的Body的迭代器
	std::vector<Body>::iterator FindBody(const int body_id);

	//使用bodyid销毁物体
	bool DestroyBody(const int body_id);

	//渲染物体
	void RenderBody(Brush& brush);

	//渲染AABB
	void RenderAABB(Brush& brush);

	//打印Bodylist
	void CoutBodyList();
	
	//打印Bodylist_mass
	void CoutBodyMass();
};

//全局函数获得物体的质心
FlatVector GetMassCenter(Body& body);
FlatVector GetMassCenter(std::vector<FlatVector> points);

//计算多边形的转动惯量
static float momentOfInertia(const std::vector<FlatVector>& points) {
	float inertia = 0.0f;
	int n = points.size();
	for (int i = 0; i < n; ++i) {
		int j = (i + 1) % n;
		inertia += (points[i].x * points[i].x + points[i].x * points[j].x + points[j].x * points[j].x +
			points[i].y * points[i].y + points[i].y * points[j].y + points[j].y * points[j].y) *
			(points[i].x * points[j].y - points[j].x * points[i].y);
	}
	return inertia / 12.0;
}
