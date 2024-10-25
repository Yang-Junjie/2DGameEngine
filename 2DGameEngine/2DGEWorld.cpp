#include "2DGEWorld.h"
#include <unordered_map>

World::World()
{

}

World::~World()
{
}

void World::SetInteration(size_t interation)
{
	this->interation_ = interation;
	this->copy_interation_ = interation;
}

void World::PauseTime()
{
	World::SetInteration(0);
}

void World::OpenTime()
{
	World::SetInteration(this->copy_interation_);
}

void World::Timepiece()
{

}


//分离物体
void World::SepareteBodies(Body& body_a, Body& body_b, FlatVector& separation_vector)
{
	if (body_a.stationary_) {
		body_b.Move(separation_vector / 2);
	}
	else if (body_b.stationary_) {
		body_a.Move(-separation_vector / 2);
	}
	else {
		body_a.Move(-separation_vector / 2);
		body_b.Move(separation_vector / 2);
	}
}


void World::BroadPhase(std::vector<Body>& body_lists) {
	
	//对需要对aabb上色的物体进行标记
	std::unordered_map<int, bool> colored;

	//开始判断两个物体的aabb是否相交
	for (auto& body_a : body_lists) {
		body_a.GetAABB();
		for (auto& body_b : body_lists) {
			
			//如果两个物体是同一个物体则跳过这次检测
			if (body_a.body_id_ == body_b.body_id_) {
				continue;
			}
			body_b.GetAABB();

			//检测
			bool intersect = Intersect::IntersectAABB(body_a, body_b);
			
			//如果没有相交则继续下一个物体的检测
			if (!intersect) {
				continue;
			}
			else {
				//如果相交则进行标记
				colored[body_a.body_id_] = true;
				colored[body_b.body_id_] = true;

				//并将其放入即将接触物体列表中
				std::pair<Body&, Body&> pair_body(body_a, body_b);
				this->contact_body_.push_back(pair_body);

			}
			
		}
	}

	//再对标记过的物体的aabb进行上色
	for (auto& body : body_lists) {
		if (colored[body.body_id_]) {
			body.color_box_ = { 255, 255, 255, 255 };
		}
		else {
			body.color_box_ = { 255, 100, 100, 255 };
		}
	}
}


void World::NarrowPhase() 
{
	//遍历AABB相交的物体
	for (auto& pair: this->contact_body_) {

		//对这两个物体使用碰撞检测，判断这两个物体是否发生碰撞
		IntersectData intersect_data = Collide(pair.first, pair.second);
		
		//如果发生碰撞，则计算分离向量，并分离这两个物体
		if (intersect_data.Collision) {
			FlatVector separation_vector = intersect_data.normal * intersect_data.depth;
			SepareteBodies(pair.first, pair.second, separation_vector);

			//拿到接触点并绘制
			std::vector<FlatVector> contact_points = FindContactPoints(pair.first, pair.second);
			for (auto& contact_point : contact_points) {
				brush.DrawPoint(contact_point, 255, 255, 255, 25);
			}
		}

	}
}




void World::Interation(std::vector<Body>& body_lists,float time) 

{
	//开始interation_次迭代
	for (int i = 0; i < this->interation_; ++i) {

		//每次迭代前清除aabb相交的物体
		this->contact_body_.clear();

		//从body_lists中拿出一个物体对其进行物理操作
		for (auto& body : body_lists) {

			if (interation_ != 0) {
				
				time /= this->interation_;
			}
			FlatVector displacement = body.velocity_ * time + body.acceleration_ * time * time * 0.5;
			body.velocity_ = body.velocity_ + body.acceleration_ * time;
			body.ClearAcceleration();
			body.Move(displacement);
		}

		//物理操作完后进入碰撞管线
		BroadPhase(body_lists);
		NarrowPhase();
	}
	
}
