#pragma once
#include "2DGEmath.h"
#include "2DGEBody.h"
#include "2DGECollision.h"
#include <utility>
class World {
public:
	float pass_time_ = 0;
	size_t interation_ = 10;
	size_t copy_interation_ = 10;
	size_t max_number_body_ = 100;
	std::vector<std::pair<Body, Body>> contact_body_;

	FlatVector gravity_acceleration_ = { 0.0f,9.8f };

	World();
	~World();

	void SetInteration(size_t interation);
	void PauseTime();
	void OpenTime();
	void Interation(std::vector<Body>& body_lists , float time) ;
	
private:
	void Timepiece();
	void BroadPhase(std::vector<Body>& body_lists);
	void NarrowPhase();
	
};