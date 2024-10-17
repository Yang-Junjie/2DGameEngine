#include "2DGEBody.h"


Body::Body() {

}

Body::~Body() {

}

Body::Body(Shape shape, float radius, BodyColor color, float mass, FlatVector mass_center, int body_id) :
	body_id_(body_id), shape_(shape), radius_(radius),    color_(color), mass_(mass), mass_center_(mass_center) {
		this->inverse_mass_ == 1/this->mass_;//���������������ĵ�����inverseMass���ø�ֵ
}

Body::Body(Shape shape, std::vector<SDL_FPoint> vertices, BodyColor color, float mass, FlatVector mass_center, int body_id):
	body_id_(body_id), shape_(shape),color_(color), vertices_(vertices), mass_(mass), mass_center_(mass_center){
		this->inverse_mass_ == 1 / this->mass_;
}

void Body::SetVelocity(const FlatVector v1)
{
	this->velocity_ = v1;
}

void Body::SetAcceleration(const FlatVector v1)
{
	this->acceleration_ = v1;
}

void Body::Move(const FlatVector v1) {
	//��ԭ��λ�û�����λ��v1
	if (this->shape_ == 0) {

		this->mass_center_.y += v1.y;
		this->mass_center_.x += v1.x;
	}
	else if (this->shape_ == 1) {
		int vertices_num = this->vertices_.size();
		for (int i = 0; i < vertices_num; ++i) {
			this->vertices_[i].x += v1.x;
			this->vertices_[i].y += v1.y;
		}
		this->mass_center_ = GetMassCenter(this->vertices_);
	}
}

void Body::MoveTo(const FlatVector v1) {
	if (this->shape_ == 0) {
		this->mass_center_ = v1;
	}
	else if(this->shape_ == 1){
		//�Ƚ�����ε�����ƽ����ԭ�㣬�ڽ�����ƽ��ֵĿ��λ��

		int vertices_num = this->vertices_.size();
		FlatVector origin = this->mass_center_;
		for (auto& vertex : vertices_) {
			vertex.x -= origin.x;
			vertex.y -= origin.y;
		}
		// ƽ�ƻ��µ�λ��
			for (auto& vertex : vertices_) {
				vertex.x += v1.x;
				vertex.y += v1.y;
			}

		this->mass_center_ = GetMassCenter(this->vertices_);
	}
}

void Body::Rotation(const float angle)
{
	if (this->shape_ == 0) {
	}
	else if (this->shape_ == 1) {
		int num_vertices = this->vertices_.size();
		// ƽ�����ж��㵽ԭ��  
		FlatVector origin = this->mass_center_;
		for (auto& vertex : vertices_) {
			vertex.x -= origin.x;
			vertex.y -= origin.y;
		}

		// ��ת���ж���  
		for (auto& vertex : vertices_) {
			FlatVector::Transform(vertex.x,vertex.y, angle);
		}

		// ƽ�ƻ�ԭ����λ��  
		for (auto& vertex : vertices_) {
			vertex.x += origin.x;
			vertex.y += origin.y;
		}

		this->mass_center_ = GetMassCenter(this->vertices_);
	}
}

void Body::AddForce(const FlatVector F)
{
	//ֱ�ӽ���תΪ���ٶ�
	this->acceleration_ = this->acceleration_ + (F / this->mass_);

}





BodyManager::BodyManager()
{
}

BodyManager::~BodyManager()
{
}

bool BodyManager::CreateBody(float radius, BodyColor color, float mass, FlatVector mass_center)
{
	//����Բ�β������body_list_
	if (mass > 0) {
		this->id_count++;
		Body b1(CIRCLE, radius, color, mass, mass_center, id_count);
		this->body_lists_.push_back(b1);
		return false;
		b1.~Body();
	}
	else {
		std::cout << "�������������Ϊ0" << std::endl;
		return false;
	}
	
}

bool BodyManager::CreateBody(std::vector<SDL_FPoint> vertices, BodyColor color,float mass) {
	//�������β������body_list_
	if (mass > 0) {
		this->id_count++;
		Body b1(POLTGON, vertices, color, mass, GetMassCenter(vertices), id_count);
		this->body_lists_.push_back(b1);
		b1.~Body();
		return true;
	}
	else {
		std::cout << "�������������Ϊ0" << std::endl;
		return false;
	}
}

void Body::ClearAcceleration()
{
	this->acceleration_ = FlatVector(0, 0);
}

bool BodyManager::DestroyBody(const int body_id) {
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

std::vector<Body>::iterator BodyManager::FindBody(const int body_id) {
	//��������������Bodyid���ҵ����ش�Ԫ�ض�Ӧ��������û�ҵ�����begin����һ�����󣩵�����
	for (std::vector<Body>::iterator it = (this->body_lists_).begin(); it != this->body_lists_.end();++it) {
		if (it->body_id_ == body_id) {
			return it;
		}
	}
	return this->body_lists_.begin();
}

void BodyManager::RenderBody(Brush& brush)
{
	//��Ⱦ����
	for (Body& body:this->body_lists_) { 
		if (body.shape_ == 0) {
			brush.DrawCircle(body.mass_center_.x, body.mass_center_.y,body.radius_,body.color_.r,body.color_.g, body.color_.b, body.color_.a);
		}
		else if(body.shape_ == 1){
			
			brush.DrawPolygon(body.vertices_, body.color_.r, body.color_.g, body.color_.b, body.color_.a);
		}
	}
}

void BodyManager::CoutBodyList() {
	//��ӡbodylist
	std::cout << "Shape=====id=====<<<<<<<<<<<<<eigenvalue>>>>>>>>>>>>>==========mass_center==========������"<<this->body_lists_.size() << std::endl;
	for (std::vector<Body>::iterator it = (this->body_lists_).begin(); it != this->body_lists_.end(); ++it) {
		//std::cout <<it->shape_<<"         " << it->body_id_ << "     " <<std::endl;
		if (it->shape_ == 0) {
			std::cout <<it->shape_<<"         " << it->body_id_ << "           " <<it->radius_<<"                                       " << it->mass_center_ << std::endl;
		}
		else if (it->shape_ == 1) {
			int num_vertices = it->vertices_.size();
			std::cout << it->shape_ << "         " << it->body_id_ << "     ";
			for (int i = 0; i < num_vertices; ++i) {
				std::cout << "(" << it->vertices_[i].x << "," << it->vertices_[i].y << ")";
			}
			std::cout <<"                  " << it->mass_center_ << std::endl;
		}
	}
}

FlatVector GetMassCenter(Body& body)
{
	if (body.shape_ == 0) {
		return body.mass_center_;
	}
	else if (body.shape_ == 1) {
		int num_vertices = body.vertices_.size();
		if (num_vertices < 3) {
			// ������Ҫ3���������γ�һ��������  
			return FlatVector(0, 0);
		}

		std::vector<std::vector<int>> triangular_index(num_vertices - 2, std::vector<int>(3, 0));

		// ������������������������Ǽ򵥵�͹����Σ���˳�����Ӷ����γ������Σ�  
		for (int i = 0; i < num_vertices - 2; ++i) {
			triangular_index[i][0] = 0;
			triangular_index[i][1] = i + 1;
			triangular_index[i][2] = i + 2;
		}

		float total_area = 0.0f;
		float cx = 0.0f, cy = 0.0f;

		for (const auto& indices : triangular_index) {
			SDL_FPoint v0 = body.vertices_[indices[0]];
			SDL_FPoint v1 = body.vertices_[indices[1]];
			SDL_FPoint v2 = body.vertices_[indices[2]];

			float A = 0.5f * (v0.x * (v1.y - v2.y) + v1.x * (v2.y - v0.y) + v2.x * (v0.y - v1.y)); // ���������  
			total_area += A;

			cx += (v0.x + v1.x + v2.x) * A; // ����x�����ۼ�  
			cy += (v0.y + v1.y + v2.y) * A; // ����y�����ۼ�  
		}

		cx /= (3.0f * total_area);
		cy /= (3.0f * total_area);

		return FlatVector(cx, cy);
	}
	return FlatVector(0, 0);
}

FlatVector GetMassCenter(std::vector<SDL_FPoint> points)
{
	
	
		int num_vertices = points.size();
		if (num_vertices < 3) {
			// ������Ҫ3���������γ�һ��������  
			return FlatVector(0, 0);
		}

		std::vector<std::vector<int>> triangular_index(num_vertices - 2, std::vector<int>(3, 0));

		// ������������������������Ǽ򵥵�͹����Σ���˳�����Ӷ����γ������Σ�  
		for (int i = 0; i < num_vertices - 2; ++i) {
			triangular_index[i][0] = 0;
			triangular_index[i][1] = i + 1;
			triangular_index[i][2] = i + 2;
		}

		float total_area = 0.0f;
		float cx = 0.0f, cy = 0.0f;

		for (const auto& indices : triangular_index) {
			SDL_FPoint v0 = points[indices[0]];
			SDL_FPoint v1 = points[indices[1]];
			SDL_FPoint v2 = points[indices[2]];

			float A = 0.5f * (v0.x * (v1.y - v2.y) + v1.x * (v2.y - v0.y) + v2.x * (v0.y - v1.y)); // ���������  
			total_area += A;

			cx += (v0.x + v1.x + v2.x) * A; // ����x�����ۼ�  
			cy += (v0.y + v1.y + v2.y) * A; // ����y�����ۼ�  
		}

		cx /= (3.0f * total_area);
		cy /= (3.0f * total_area);

		return FlatVector(cx, cy);
}
