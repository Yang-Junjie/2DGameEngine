#include "2DGEWorld.h"

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
void World::BroadPhase(std::vector<Body>& body_lists) {
	for (auto& body_a:body_lists) {//拿出一个物体a
		body_a.GetAABB();  //得到物体a的AABB包围盒
		for (auto& body_b : body_lists) {//拿出一个物体b
			if (body_a.body_id_ == body_b.body_id_) {//如果物体a和物体b是同一个物体则跳过
				continue;
			}
			body_b.GetAABB();  //得到物体b的AABB包围盒
			if (!IntersectAABB(body_a, body_b)) {//如果两个物体AABB盒不相交则将物体a和物体b的包围盒颜色设为红色并跳过
				body_a.color_box_ = { 255,100,100,255 };
				body_b.color_box_ = { 255,100,100,255 };
				continue;
			}
			std::cout << "intersect" << std::endl;//如果两个物体AABB盒相交则将物体a和物体b的包围盒颜色设为白色
			body_a.color_box_ = { 255,255,255,255 };
			body_b.color_box_ = { 255,255,255,255 };
		
		}
	}
}
	






void World::Interation(std::vector<Body>& body_lists,float time) 

{
	
	for (int i = 0; i < this->interation_; ++i) {
		for (auto& body : body_lists) {
			if (interation_ != 0) {
				
				time /= this->interation_;
			}
			FlatVector displacement = body.velocity_ * time + body.acceleration_ * time * time * 0.5;
			body.velocity_ = body.velocity_ + body.acceleration_ * time;
			body.ClearAcceleration();
			body.Move(displacement);
		}
		BroadPhase(body_lists);
		
	}
	
}
