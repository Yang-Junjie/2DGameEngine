#pragma once
#include "2DGEBody.h"
#include "2DGEmath.h"
//������������ ���ڼ������ɸ����� �縡�������������ȵ�.
class ForceGenerator {

public:
	virtual void UpdateForce(std::vector<Body>::iterator body,float duration) = 0;
};
class Resistance : public ForceGenerator {
	float k1_;
	float k2_;

public:

	Resistance(float k1,float k2);
	virtual void UpdateForce(std::vector<Body>::iterator body, float duration);
};


class Gravity :public ForceGenerator {
	FlatVector gravity_acceleration_ = {0,0};

public:
	Gravity(const FlatVector gravity_acceleration);

	virtual void UpdateForce(std::vector<Body>::iterator body, float duration);
};

class FixedPointElasticity : public ForceGenerator {
	FlatVector* anchor_ = nullptr;
	float spring_constant_ = 0;
	float rest_length_ = 0;
public:
	FixedPointElasticity(FlatVector *anchor,float spring_constant,float rest_length);
	virtual void UpdateForce(std::vector<Body>::iterator body, float duration);
};