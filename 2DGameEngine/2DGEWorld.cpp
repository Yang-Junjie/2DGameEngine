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

void World::BroadPhase(std::vector<Body>& body_lists) {
	std::unordered_map<int, bool> colored;
	for (auto& body_a : body_lists) {
		body_a.GetAABB();  
		for (auto& body_b : body_lists) {
			if (body_a.body_id_ == body_b.body_id_) {
				continue;
			}
			body_b.GetAABB(); 
			bool intersect = Intersect::IntersectAABB(body_a, body_b);
			if (!intersect) {
				continue;
			}
			else {
				colored[body_a.body_id_] = true;
				colored[body_b.body_id_] = true;
			}
			std::pair<Body&, Body&> pair_body(body_a, body_b);
			this->contact_body_.push_back(pair_body);
			//std::cout << contact_body_[0].first.body_id_<<"£¬" << contact_body_[0].second.body_id_ << std::endl;
		}
	}

	for (auto& body : body_lists) {
		if (colored[body.body_id_]) {
			body.color_box_ = { 255, 255, 255, 255 };
		}
		else {
			body.color_box_ = { 255, 100, 100, 255 };
		}
	}
}

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

void World::NarrowPhase() 
{
	for (auto& pair: this->contact_body_) {
		IntersectData intersect_data = Collide(pair.first, pair.second);
		if (intersect_data.Collision) {
			FlatVector separation_vector = intersect_data.normal * intersect_data.depth;
			SepareteBodies(pair.first, pair.second, separation_vector);
			FlatVector contact_point;
			contact_point = FindContactPoints(pair.first, pair.second)[0];

			brush.DrawPoint(contact_point, 255, 255, 255, 25);
		}

	}
}




void World::Interation(std::vector<Body>& body_lists,float time) 

{
	
	for (int i = 0; i < this->interation_; ++i) {
		this->contact_body_.clear();
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
		NarrowPhase();
	}
	
}
