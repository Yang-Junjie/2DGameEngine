#pragma once
#include "2DGEmath.h"
#include "2DGEBody.h"
class World {
public:
	float pass_time_ = 0;
	size_t interation_ = 10;
	size_t copy_interation_ = 10;
	size_t max_number_body_ = 100;

	FlatVector gravity_acceleration_ = { 0.0f,9.8 };

	World();
	~World();

	void SetInteration(size_t interation);
	void PauseTime();
	void OpenTime();
	void Interation(std::vector<Body>& body_lists , float time) const;

private:
	void Timepiece();
	
};