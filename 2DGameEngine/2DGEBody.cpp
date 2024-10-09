#include "2DGEBody.h"


Body::Body() {

}
Body::~Body() {
}

Body::Body(Shape shape, float radius, BodyColor color, float mass, FlatVector mass_center, int body_id) :
	body_id_(body_id), shape_(shape), radius_(radius),    color_(color), mass_(mass),mass_center_(mass_center) {
	
}

Body::Body(Shape shape, SDL_Point* vertices, int vertices_num, BodyColor color, float mass, FlatVector mass_center, int body_id):
	body_id_(body_id), shape_(shape),color_(color),vertices_num_(vertices_num), vertices_(vertices), mass_(mass),mass_center_(mass_center){
	
}







BodyManager::BodyManager()
{
}

BodyManager::~BodyManager()
{
}

bool BodyManager::CreateBody(float radius, BodyColor& color, float mass, FlatVector& mass_center)
{
	//����Բ�β������body_list_
	this->id_count++;
	Body b1(CIRCLE, radius, color, mass, mass_center,id_count);
	this->body_lists_.push_back(b1);
	b1.~Body();
	return false;
}

bool BodyManager::CreateBody(SDL_Point* vertices,int vertices_num, BodyColor& color,float mass, FlatVector& mass_center) {
	//�������β������body_list_
	this->id_count++;
	Body b1(POLTGON, vertices, vertices_num,color,mass, mass_center, id_count);
	this->body_lists_.push_back(b1);
	b1.~Body();
	return false;
}


bool BodyManager::DestroyBody(int body_id) {
	//����bodyid���ҵ�ɾ������true��û�ҵ�����false
	std::vector<Body>::iterator it = BodyManager::FindBody(body_id);
	int befor_body_list_size = this->body_lists_.size();
	if (it != this->body_lists_.end()) {
		this->body_lists_.erase(it);
		this->id_count--;
		return true;
	}
	else {
		return false;
	}
}

std::vector<Body>::iterator BodyManager::FindBody(int body_id) {
	//��������������Bodyid���ҵ����ش�Ԫ�ض�Ӧ��������û�ҵ�����end������
	for (std::vector<Body>::iterator it = (this->body_lists_).begin(); it != this->body_lists_.end();++it) {
		if (it->body_id_ == body_id) {
			return it;
		}
	}
	return this->body_lists_.end();
}

void BodyManager::RenderBody(Brush& brush)
{
	for (Body& body:this->body_lists_) { 
		if (body.shape_ == 0) {
			//Բ��
			brush.DrawCircle(body.mass_center_.x, body.mass_center_.y,body.radius_,body.color_.r,body.color_.g, body.color_.b, body.color_.a);
		}
		//else if(body.shape_ == 1){
		//	
		//	//brush.DrawFillPolygon()
		//}
	}
}

void BodyManager::CoutBodyList() {
	//��ӡbodylist
	std::cout << "Shape=====id=====������"<<this->body_lists_.size() << std::endl;
	for (std::vector<Body>::iterator it = (this->body_lists_).begin(); it != this->body_lists_.end(); ++it) {
		std::cout <<it->shape_<<"         " << it->body_id_ << "     " << std::endl;
	}
}