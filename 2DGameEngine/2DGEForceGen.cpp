#include "2DGEForceGen.h"





Gravity::Gravity(const FlatVector gravity_acceleration):gravity_acceleration_(gravity_acceleration)
{
}

void Gravity::UpdateForce(std::vector<Body>::iterator body, float duration)
{
	body->AddForce(this->gravity_acceleration_ * body->mass_);
}








FixedPointElasticity::FixedPointElasticity(FlatVector* anchor, float spring_constant, float rest_length):
	anchor_(anchor),spring_constant_(spring_constant),rest_length_(rest_length)
{
}

void FixedPointElasticity::UpdateForce(std::vector<Body>::iterator body, float duration)
{
	FlatVector force = body->mass_center_;
	force = force - *(this->anchor_);

	float magnitude = force.len();
	magnitude = abs(magnitude - this->rest_length_);
	magnitude *= this->spring_constant_;

	force.normalize();
	force = force * ( - magnitude);
	body->AddForce(force);
}




Resistance::Resistance(float k1, float k2):k1_(k1),k2_(k2)
{

}

void Resistance::UpdateForce(std::vector<Body>::iterator body, float duration)
{

	FlatVector force = body->velocity_;
	float resistance_coeff = force.len();
	resistance_coeff = this->k1_ * resistance_coeff + this->k2_ * resistance_coeff * resistance_coeff;
	force.normalize();
	force = force * (-resistance_coeff);
	body->AddForce(force);
	
}

FakeSpring::FakeSpring(FlatVector* anchor, float spring_constant, float damping):
	anchor_(anchor),spring_constant_(spring_constant),damping_(damping)
{
}

void FakeSpring::UpdateForce(std::vector<Body>::iterator body, float duration)
{
	FlatVector position = body->mass_center_;
	std::cout << position;
	position = position - (*this->anchor_);
	
	float gamma = 0.5f * sqrt(4 * this->spring_constant_ - this->damping_ * this->damping_);
	if (gamma == 0)return;
	FlatVector c = position * (this->damping_ / (2.0f * gamma)) + body->velocity_ * (1.0f / gamma);

	FlatVector target = position * cos(gamma * duration) + c * sin(gamma * duration);
	
	target = target * exp(-0.5f * duration * damping_);
	FlatVector accel = (target - position) * (1.0f / duration * duration) - body->velocity_ * duration;
	
	body->AddForce(accel * body->mass_);
	
}
