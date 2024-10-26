#pragma once
#include <vector>
#include "2DGEmath.h"
#include <SDL.h>
#include <iostream>
#include "2DGEDraw.h"


enum Shape {
	NONE = -1,
	CIRCLE = 0,
	POLTGON = 1
};


struct BodyColor
{
	int r;
	int g;
	int b;
	int a;
};

enum BoundingBox {
	NONEBOX = -1,
	CIRCLEBOX = 0,
	AABBBOX = 1,
	OBBBOX = 2,
};




//������
//���ܣ��Ǳ����������������Ԫ�أ� ���е����嶼��Body�࣬ͨ������ֱ�ӵ���Body�Ĺ��캯���������壬��Ҫͨ��BodyManager��CreateBody��������������
//�ܴ�������Σ�Բ��������״����������������˻�������������
class Body {
public:

	//���������������ֵ
	//�����id
	int body_id_ = 0;

	//�������״
	Shape shape_ = NONE;

	//Բ������ר�����԰뾶
	float radius_ = 0.0f;

	//�������ɫ�ֱ��Ӧ��rgba
	BodyColor color_ = { 0,0,0,0 };

	//polygon��״ר������
	std::vector<FlatVector> vertices_ = {};

	//aabb��Χ�еĶ���
	std::vector<FlatVector> vertices_aabb_ = {  {0.0f, 0.0f},
												{0.0f, 0.0f},
												{0.0f, 0.0f},
												{0.0f, 0.0f} };

	//ʹ�����ְ�Χ��
	BoundingBox bounding_box_ = NONEBOX;

	//��Χ�е���ɫ
	BodyColor color_box_ = { 255, 100, 100, 255 };

	//��������ֵ
	//�����Ƿ�ֹ��false->�Ǿ�ֹ��true->��ֹ
	bool stationary_ = false;

	//����Ļָ�ϵ��
	float restitution_ = 0.0f;

	//������ת�ĽǶ�
	float angle = 0.0f;
	
	//����Ľ��ٶ�,���ڶ�ά����½��ٶȵķ����Ǵ�ֱ����Ļ������ʹ��float
	float angular_velocity_ = 0.0f;

	//������ܶ�
	float density_ = 0.0f;

	//���������
	float mass_ = 0.0f;

	//���������ĵ���
	float inverse_mass_ = 0.0f;

	//position�����λ��
	FlatVector mass_center_ = { 0.0f,0.0f };

	//��������ٶ�
	FlatVector velocity_ = { 0.0f,0.0f };

	//����ļ��ٶ�
	FlatVector acceleration_ = { 0.0f,0.0f };

	//�̶���Ħ����
	float inherent_static_friction_ = 1.0f;

	//�̶���Ħ����
	float inherent_dynamic_friction_ = 0.8f;

	//ת������
	float rotational_inertia_ = 0.0f;

	//ת�������ĵ���
	float inverse_rotational_inertia_ = 0.0f;

	//�������������
	~Body();

	//Circle���캯��
	//���δ��������shape,radius��color,mass>0,mass_center,body_id����ѡ��������ʹ���ظ�id��ͨ��body_list�鿴���������id��
	Body(Shape shape, float radius, BodyColor color, float mass, FlatVector mass_center, int body_id, bool stationary, float restitution);

	//Polygon���캯�������غ���
	//���δ��������shape,vertices��color,mass>0,mass_center,body_id����ѡ��������ʹ���ظ�id��ͨ��body_list�鿴���������id��
	Body(Shape shape, std::vector<FlatVector> vertices, BodyColor color, float mass, FlatVector mass_center, int body_id, bool stationary, float restitution);

	//�����ά����v1���趨�������ڵ��ٶ�Ϊv1
	void SetVelocity(const FlatVector v1);

	//�����ά����v1���趨�������ڵļ��ٶ�Ϊv1
	void SetAcceleration(const FlatVector v1);

	//�����ά����v1�������������ڵ�λ����λ��v1
	void Move(const FlatVector& v1);

	//�����ά����v1��������ֱ�Ӵ��͵�v1��
	void MoveTo(const FlatVector v1);

	//����Ƕ���angle����������ԭ���ĽǶ�����תangle��
	void Rotation(const float angle);

	//�����ά����v1����������ʩ��һ����
	void AddForce(const FlatVector v1);

	//�������ļ��ٶȣ�������������½�����ٶ�ʱʹ��
	void ClearAcceleration();
	
	//�õ������AABB��Χ��
	void GetAABB();
};





//BodyManager���ڴ��������١�����Body
//���е�body_list_�ĳ�Ա�����洢����������������Body
class BodyManager {
public:

	//�洢���е�Body
	std::vector<Body> body_list_;

	//����ͳ��Body��id��
	int id_count = 0;

	//Ĭ�Ϲ��캯��
	BodyManager();

	//��������
	~BodyManager();
	
	//���ڴ���ShapeΪCircle��Body����Ҫ����mass_center�Ա�ʾԲ��
	bool CreateBody(float radius                   , BodyColor color, float mass, FlatVector mass_center,bool stationary,float restitution);

	//���ڴ���ShapeΪPolygon��Body������Ҫ����λ�ã�ӦΪ����ȷ������λ��Ҳȷ���ˡ�
	bool CreateBody(std::vector<FlatVector> vertices, BodyColor color, float mass, bool stationary, float restitution);

	//ʹ��body��idѰ��Body�����ض�Ӧid��Body�ĵ�����
	std::vector<Body>::iterator FindBody(const int body_id);

	//ʹ��bodyid��������
	bool DestroyBody(const int body_id);

	//��Ⱦ����
	void RenderBody(Brush& brush);

	//��ȾAABB
	void RenderAABB(Brush& brush);

	//��ӡBodylist
	void CoutBodyList();
	
	//��ӡBodylist_mass
	void CoutBodyMass();
};

//ȫ�ֺ���������������
FlatVector GetMassCenter(Body& body);
FlatVector GetMassCenter(std::vector<FlatVector> points);

//�������ε�ת������
static float momentOfInertia(const std::vector<FlatVector>& points) {
	float inertia = 0.0f;
	int n = points.size();
	for (int i = 0; i < n; ++i) {
		int j = (i + 1) % n;
		inertia += (points[i].x * points[i].x + points[i].x * points[j].x + points[j].x * points[j].x +
			points[i].y * points[i].y + points[i].y * points[j].y + points[j].y * points[j].y) *
			(points[i].x * points[j].y - points[j].x * points[i].y);
	}
	return inertia / 12.0;
}
