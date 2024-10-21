#include "2DGEBody.h"


Body::~Body() {

}

Body::Body(Shape shape, float radius, BodyColor color, float mass, FlatVector mass_center, int body_id) :
	body_id_(body_id), shape_(shape), radius_(radius),    color_(color), mass_(mass), mass_center_(mass_center) {
		this->inverse_mass_ = 1/this->mass_;//���������������ĵ�����inverseMass���ø�ֵ
}

Body::Body(Shape shape, std::vector<FlatVector> vertices, BodyColor color, float mass, FlatVector mass_center, int body_id):
	body_id_(body_id), shape_(shape),color_(color), vertices_(vertices), mass_(mass), mass_center_(mass_center){
		this->inverse_mass_ = 1 / this->mass_;
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
	/*�������������ϲ���ֱ�����֣�ֱ�ӽ���תΪ���ٶ�*/
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
		this->body_list_.push_back(b1);
		return true;
	}
	else {
		std::cout << "�������������Ϊ0" << std::endl;
		return false;
	}
	
}

bool BodyManager::CreateBody(std::vector<FlatVector> vertices, BodyColor color,float mass) {
	//�������β������body_list_
	if (mass > 0) {
		this->id_count++;
		Body b1(POLTGON, vertices, color, mass, GetMassCenter(vertices), id_count);
		this->body_list_.push_back(b1);
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



void Body::GetAABB()
{
	if (shape_ == 0) {
		const float center_x = this->mass_center_.x;
		const float center_y = this->mass_center_.y;
		const float radius = radius_;
		this->vertices_aabb_[0] = { center_x - radius , center_y + radius };
		this->vertices_aabb_[1] = { center_x + radius , center_y + radius };
		this->vertices_aabb_[2] = { center_x + radius , center_y - radius };
		this->vertices_aabb_[3] = { center_x - radius , center_y - radius };
		this->bounding_box_ = AABBBOX;
	}
	else if (shape_ == 1) {
		const std::vector<FlatVector> vertices = this->vertices_;
		float max_x = -std::numeric_limits<float>::max();
		float max_y = -std::numeric_limits<float>::max();
		float min_x = std::numeric_limits<float>::max();
		float min_y = std::numeric_limits<float>::max();
		for (int i = 0; i < vertices.size(); ++i) {
			if (vertices[i].x > max_x) {
				max_x = vertices[i].x;
			}
			if (vertices[i].x < min_x) {
				min_x = vertices[i].x;
			}
			if (vertices[i].y > max_y) {
				max_y = vertices[i].y;
			}
			if (vertices[i].y < min_y) {
				min_y = vertices[i].y;
			}
		}
		this->vertices_aabb_[0] = { min_x , max_y };
		this->vertices_aabb_[1] = { max_x , max_y };
		this->vertices_aabb_[2] = { max_x , min_y };
		this->vertices_aabb_[3] = { min_x , min_y };
		this->bounding_box_ = AABBBOX;
	}


}












bool BodyManager::DestroyBody(const int body_id) {
	//����bodyid���ҵ�ɾ������true��û�ҵ�����false
	std::vector<Body>::iterator it = BodyManager::FindBody(body_id);
	int befor_body_list_size = this->body_list_.size();
	if (it != this->body_list_.end()) {
		this->body_list_.erase(it);
		this->id_count--;
		return true;
	}
	else {
		return false;
	}
}

std::vector<Body>::iterator BodyManager::FindBody(const int body_id) {
	//��������������Bodyid���ҵ����ش�Ԫ�ض�Ӧ��������û�ҵ�����begin����һ�����󣩵�����
	for (std::vector<Body>::iterator it = (this->body_list_).begin(); it != this->body_list_.end();++it) {
		if (it->body_id_ == body_id) {
			return it;
		}
	}
	return this->body_list_.begin();
}

void BodyManager::RenderBody(Brush& brush)
{
	//��Ⱦ����
	for (Body& body:this->body_list_) { 
		if (body.shape_ == 0) {
			brush.DrawCircle(body.mass_center_.x, body.mass_center_.y,body.radius_,body.color_.r,body.color_.g, body.color_.b, body.color_.a);
		}
		else if(body.shape_ == 1){
			
			brush.DrawPolygon(body.vertices_, body.color_.r, body.color_.g, body.color_.b, body.color_.a);
		}
	}
}

void BodyManager::RenderAABB(Brush& brush)
{
	for (auto& body : this->body_list_) {
		if (body.bounding_box_ != AABBBOX) {}
		else {
			//std::cout << body.body_id_ << ":" << body.color_box_.g << std::endl;
			brush.DrawPolygon(body.vertices_aabb_, body.color_box_.r, body.color_box_.g, body.color_box_.b, body.color_box_.a);
		}
	}
}

void BodyManager::CoutBodyList() {
	//��ӡbodylist
	std::cout << "Shape=====id=====<<<<<<<<<<<<<eigenvalue>>>>>>>>>>>>>==========mass_center==========������"<<this->body_list_.size() << std::endl;
	for (std::vector<Body>::iterator it = (this->body_list_).begin(); it != this->body_list_.end(); ++it) {
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
			FlatVector v0 = body.vertices_[indices[0]];
			FlatVector v1 = body.vertices_[indices[1]];
			FlatVector v2 = body.vertices_[indices[2]];

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

FlatVector GetMassCenter(std::vector<FlatVector> points)
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
			FlatVector v0 = points[indices[0]];
			FlatVector v1 = points[indices[1]];
			FlatVector v2 = points[indices[2]];

			float A = 0.5f * (v0.x * (v1.y - v2.y) + v1.x * (v2.y - v0.y) + v2.x * (v0.y - v1.y)); // ���������  
			total_area += A;

			cx += (v0.x + v1.x + v2.x) * A; // ����x�����ۼ�  
			cy += (v0.y + v1.y + v2.y) * A; // ����y�����ۼ�  
		}

		cx /= (3.0f * total_area);
		cy /= (3.0f * total_area);

		return FlatVector(cx, cy);
}












