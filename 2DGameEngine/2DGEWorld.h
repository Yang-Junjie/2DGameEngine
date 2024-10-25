#pragma once
#include "2DGEmath.h"
#include "2DGEBody.h"
#include "2DGECollision.h"
#include <utility>

struct ContactData {
	Body* body_a_ = nullptr;
	Body* body_b_ = nullptr;
	FlatVector normal_;
	std::vector<FlatVector> contact_points_;
};

//ģ������
class World {
public:

	//����ͳ�ƹ��˶೤ʱ��
	float pass_time_ = 0;

	//���������������Խ����ײ��⡢����ģ����е�Խ��ȷ����������ʱ���䳤
	size_t interation_ = 16;
	size_t copy_interation_ = 16;

	//���������ɴ��ڵ�������
	size_t max_number_body_ = 100;

	//��������Ӵ��������б�
	std::vector<std::pair<Body&, Body&>> contact_body_;

	//������
	Brush brush;

	//�����е��������ٶ�
	FlatVector gravity_acceleration_ = { 0.0f,9.8f };

	World();
	~World();


	//���������Interation
	void SetInteration(size_t interation);

	//��ͣ����
	void PauseTime();

	//��������
	void OpenTime();

	//��ʼ����
	void Interation(std::vector<Body>& body_lists , float time);

	//������ײ
	void SepareteBodies(Body& body_a, Body& body_b, FlatVector& separation_vector);
	
private:
	//��ʱ��
	void Timepiece();

	//��ײ�����е�BroadPhase
	void BroadPhase(std::vector<Body>& body_lists);

	void NarrowPhase();
	
	static void ResolveCollisionWithRotationAndFriction(ContactData &contact);
};