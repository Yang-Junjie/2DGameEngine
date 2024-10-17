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

void World::Interation(std::vector<Body>& body_lists,float time) const

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
	}
	
}
