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
	for (size_t i = 0; i < body_lists.size(); ++i) {
		Body& body_a = body_lists[i];
		body_a.GetAABB();  // 确保AABB是最新的  
		for (size_t j = i + 1; j < body_lists.size(); ++j) {
			Body& body_b = body_lists[j];
			body_b.GetAABB();  // 确保AABB是最新的  
			bool intersect = IntersectAABB(body_a, body_b);

			// 设置颜色  
			if (intersect) {
				// 白色  
				body_a.color_box_ = { 255, 255, 255, 255 };
				body_b.color_box_ = { 255, 255, 255, 255 };
				
				//std::cout << body_a.body_id_ << "," << body_b.body_id_ << " intersect" << std::endl;
			}
			else {
				//红色
				body_a.color_box_ = { 255, 100, 100, 255 };
				body_b.color_box_ = { 255, 100, 100, 255 };
			}
		}
	}
}





//void World::BroadPhase(std::vector<Body>& body_lists)
//{
//
//	for (auto& body_a : body_lists) {
//		body_a.GetAABB();
//		for (auto& body_b : body_lists) {
//			if (body_a.body_id_ == body_b.body_id_) {
//				continue;
//			}
//			body_b.GetAABB();
//			bool intersect = IntersectAABB(body_a, body_b);
//			//std::cout << intersect << std::endl;
//			if (!intersect) {
//				body_b.color_box_.a = 255;
//				body_b.color_box_.r = 255;
//				body_b.color_box_.g = 100;
//				body_b.color_box_.b = 100;
//
//				body_a.color_box_.a = 255;
//				body_a.color_box_.r = 255;
//				body_a.color_box_.g = 100;
//				body_a.color_box_.b = 100;
//				continue;
//
//			}
//			//std::cout << intersect << std::endl;
//			if (intersect) {
//				std::cout << body_a.body_id_ << "," << body_b.body_id_ << std::endl;
//
//				body_b.color_box_.a = 255;
//				body_b.color_box_.r = 255;
//				body_b.color_box_.g = 255;
//				body_b.color_box_.b = 255;
//
//				body_a.color_box_.a = 255;
//				body_a.color_box_.r = 255;
//				body_a.color_box_.g = 255;
//				body_a.color_box_.b = 255;
//
//
//			}
//		}
//
//	}
//}

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
		//std::cout << 1 << std::endl;
	}
	
}
