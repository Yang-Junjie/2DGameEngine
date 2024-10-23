#include "2DGEForceGen.h"




//重力的构造函数
Gravity::Gravity(const FlatVector gravity_acceleration):gravity_acceleration_(gravity_acceleration)
{
}

//更新重力
void Gravity::UpdateForce(std::vector<Body>::iterator body, float duration)
{
	body->AddForce(this->gravity_acceleration_ * body->mass_);
}

//固定点的弹簧产生的力
FixedPointElasticity::FixedPointElasticity(FlatVector* anchor, float spring_constant, float rest_length):
	anchor_(anchor),spring_constant_(spring_constant),rest_length_(rest_length)
{
}

//更新弹力
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


//阻力
Resistance::Resistance(float k1, float k2):k1_(k1),k2_(k2)
{

}

//更新阻力
void Resistance::UpdateForce(std::vector<Body>::iterator body, float duration)
{

	FlatVector force = body->velocity_;
	float resistance_coeff = force.len();
	resistance_coeff = this->k1_ * resistance_coeff + this->k2_ * resistance_coeff * resistance_coeff;
	force.normalize();
	force = force * (-resistance_coeff);
	body->AddForce(force);
	
}

//有质弹簧
FakeSpring::FakeSpring(FlatVector* anchor, float spring_constant, float damping):
	anchor_(anchor),spring_constant_(spring_constant),damping_(damping)
{
}

//更新有质弹簧
void FakeSpring::UpdateForce(std::vector<Body>::iterator body, float duration)
{
	FlatVector position = body->mass_center_;
	
	position = position - (*this->anchor_);
	//std::cout << position << std::endl;
	float gamma = 0.5f * sqrt(4 * this->spring_constant_ - this->damping_ * this->damping_);
	if (gamma == 0)return;
	FlatVector c = position * (this->damping_ / (2.0f * gamma)) + body->velocity_ * (1.0f / gamma);

	FlatVector target = position * cos(gamma * duration) + c * sin(gamma * duration);
	
	target = target * exp(-0.5f * duration * damping_);
	FlatVector accel = (target - position) * (1.0f / duration * duration) - body->velocity_ * duration;
	
	body->AddForce(accel * body->mass_);
	
}
