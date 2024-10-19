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
void World::BroadPhase(std::vector<Body>& body_lists) {
	for (auto& body_a:body_lists) {//�ó�һ������a
		body_a.GetAABB();  //�õ�����a��AABB��Χ��
		for (auto& body_b : body_lists) {//�ó�һ������b
			if (body_a.body_id_ == body_b.body_id_) {//�������a������b��ͬһ������������
				continue;
			}
			body_b.GetAABB();  //�õ�����b��AABB��Χ��
			if (!IntersectAABB(body_a, body_b)) {//�����������AABB�в��ཻ������a������b�İ�Χ����ɫ��Ϊ��ɫ������
				body_a.color_box_ = { 255,100,100,255 };
				body_b.color_box_ = { 255,100,100,255 };
				continue;
			}
			std::cout << "intersect" << std::endl;//�����������AABB���ཻ������a������b�İ�Χ����ɫ��Ϊ��ɫ
			body_a.color_box_ = { 255,255,255,255 };
			body_b.color_box_ = { 255,255,255,255 };
		
		}
	}
}
	






void World::Interation(std::vector<Body>& body_lists,float time) 

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
		BroadPhase(body_lists);
		
	}
	
}
