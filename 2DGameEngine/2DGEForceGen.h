#pragma once
#include "2DGEBody.h"
#include "2DGEmath.h"
//������������ ���ڼ������ɸ����� �縡�������������ȵ�.

class ForceGenerator {

	std::vector<Body>::iterator body;
	float duration = 0.0f;
public:
	virtual void UpdateForce(std::vector<Body>::iterator body,float duration) = 0;
	virtual void ClearPreviousForce() = 0;
};




//����
class Resistance : public ForceGenerator {
	float k1_;
	float k2_;
	std::vector<Body>::iterator body;
	float duration = 0.0f;
public:

	Resistance(float k1,float k2);

	virtual void UpdateForce(std::vector<Body>::iterator body, float duration);
};



//����
class Gravity :public ForceGenerator {
	FlatVector gravity_acceleration_ = {0,0};
	std::vector<Body>::iterator body_;
	float duration_ = 0.0f;
	FlatVector last_force = { 0,0 };

public:
	Gravity(const FlatVector gravity_acceleration);
	~Gravity();

	virtual void UpdateForce(std::vector<Body>::iterator body, float duration);
	virtual void ClearPreviousForce();
};




//�̶��㵯��
class FixedPointElasticity : public ForceGenerator {
	FlatVector* anchor_ = nullptr;
	float spring_constant_ = 0.0f;
	float rest_length_ = 0.0f;
	std::vector<Body>::iterator body;
	float duration = 0.0f;

public:
	FixedPointElasticity(FlatVector *anchor,float spring_constant,float rest_length);
	virtual void UpdateForce(std::vector<Body>::iterator body, float duration);
};

//���ʵ���
class FakeSpring :public ForceGenerator {
	FlatVector* anchor_ = nullptr;
	float spring_constant_ = 0.0f;
	float damping_ = 0.0f;
	std::vector<Body>::iterator body;
	float duration = 0.0f;

public:
	FakeSpring(FlatVector* anchor, float spring_constant, float damping);
	virtual void UpdateForce(std::vector<Body>::iterator body, float duration);
};