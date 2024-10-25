#pragma once
#include "2DGEmath.h"
#include "2DGEBody.h"
#include "2DGECollision.h"
#include <utility>

struct ContactData {
	Body* body_a_ = nullptr;
	Body* body_b_ = nullptr;
	FlatVector normal_;
	std::vector<FlatVector> contact_points_;
};

//模拟世界
class World {
public:

	//用于统计过了多长时间
	float pass_time_ = 0;

	//世界迭代器，数字越大碰撞检测、物理模拟进行的越精确，不过计算时间会变长
	size_t interation_ = 16;
	size_t copy_interation_ = 16;

	//世界中最多可存在的物体数
	size_t max_number_body_ = 100;

	//世界中相接触的物体列表
	std::vector<std::pair<Body&, Body&>> contact_body_;

	//测试用
	Brush brush;

	//世界中的重力加速度
	FlatVector gravity_acceleration_ = { 0.0f,9.8f };

	World();
	~World();


	//设置世界的Interation
	void SetInteration(size_t interation);

	//暂停迭代
	void PauseTime();

	//继续迭代
	void OpenTime();

	//开始迭代
	void Interation(std::vector<Body>& body_lists , float time);

	//处理碰撞
	void SepareteBodies(Body& body_a, Body& body_b, FlatVector& separation_vector);
	
private:
	//计时器
	void Timepiece();

	//碰撞处理中的BroadPhase
	void BroadPhase(std::vector<Body>& body_lists);

	void NarrowPhase();
	
	static void ResolveCollisionWithRotationAndFriction(ContactData &contact);
};