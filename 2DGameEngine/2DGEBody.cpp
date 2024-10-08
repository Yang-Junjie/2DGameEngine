#include "2DGEBody.h"


Body::Body() {

}

Body::~Body() {
}

Body::Body(Shape shape, float radius, BodyColor color, float mass, FlatVector mass_center, int body_id) :
	body_id_(body_id), shape_(shape), radius_(radius),    color_(color), mass_(mass),mass_center_(mass_center) {
	
}

Body::Body(Shape shape, std::vector<SDL_Point> vertices, BodyColor color, float mass, FlatVector mass_center, int body_id):
	body_id_(body_id), shape_(shape),color_(color), vertices_(vertices), mass_(mass),mass_center_(mass_center){
	
}







BodyManager::BodyManager()
{
}

BodyManager::~BodyManager()
{
}

bool BodyManager::CreateBody(float radius, BodyColor color, float mass, FlatVector mass_center)
{
	//创造圆形并添加至body_list_
	this->id_count++;
	Body b1(CIRCLE, radius, color, mass, mass_center,id_count);
	this->body_lists_.push_back(b1);
	b1.~Body();
	return false;
}

bool BodyManager::CreateBody(std::vector<SDL_Point> vertices, BodyColor color,float mass) {
	//创造多边形并添加至body_list_
	this->id_count++;
	Body b1(POLTGON, vertices, color,mass, GetMassCenter(vertices), id_count);
	this->body_lists_.push_back(b1);
	b1.~Body();
	return false;
}

bool BodyManager::DestroyBody(int body_id) {
	//查找bodyid，找到删除返回true，没找到返回false
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
	//迭代器便利查找Bodyid，找到返回此元素对应迭代器，没找到返回end迭代器
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
			//圆形
			brush.DrawCircle(body.mass_center_.x, body.mass_center_.y,body.radius_,body.color_.r,body.color_.g, body.color_.b, body.color_.a);
		}
		else if(body.shape_ == 1){
			
			brush.DrawPolygon(body.vertices_, body.color_.r, body.color_.g, body.color_.b, body.color_.a);
		}
	}
}

void BodyManager::CoutBodyList() {
	//打印bodylist
	std::cout << "Shape=====id=====总数："<<this->body_lists_.size() << std::endl;
	for (std::vector<Body>::iterator it = (this->body_lists_).begin(); it != this->body_lists_.end(); ++it) {
		std::cout <<it->shape_<<"         " << it->body_id_ << "     " << std::endl;
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
			// 至少需要3个顶点来形成一个三角形  
			return FlatVector(0, 0);
		}

		std::vector<std::vector<int>> triangular_index(num_vertices - 2, std::vector<int>(3, 0));

		// 生成三角形索引（这里假设是简单的凸多边形，按顺序连接顶点形成三角形）  
		for (int i = 0; i < num_vertices - 2; ++i) {
			triangular_index[i][0] = 0;
			triangular_index[i][1] = i + 1;
			triangular_index[i][2] = i + 2;
		}

		float total_area = 0.0f;
		float cx = 0.0f, cy = 0.0f;

		for (const auto& indices : triangular_index) {
			SDL_Point v0 = body.vertices_[indices[0]];
			SDL_Point v1 = body.vertices_[indices[1]];
			SDL_Point v2 = body.vertices_[indices[2]];

			float A = 0.5f * (v0.x * (v1.y - v2.y) + v1.x * (v2.y - v0.y) + v2.x * (v0.y - v1.y)); // 三角形面积  
			total_area += A;

			cx += (v0.x + v1.x + v2.x) * A; // 质心x坐标累加  
			cy += (v0.y + v1.y + v2.y) * A; // 质心y坐标累加  
		}

		cx /= (3.0f * total_area);
		cy /= (3.0f * total_area);

		return FlatVector(cx, cy);
	}
	return FlatVector(0, 0);
}

FlatVector GetMassCenter(std::vector<SDL_Point> points)
{
	
	
		int num_vertices = points.size();
		if (num_vertices < 3) {
			// 至少需要3个顶点来形成一个三角形  
			return FlatVector(0, 0);
		}

		std::vector<std::vector<int>> triangular_index(num_vertices - 2, std::vector<int>(3, 0));

		// 生成三角形索引（这里假设是简单的凸多边形，按顺序连接顶点形成三角形）  
		for (int i = 0; i < num_vertices - 2; ++i) {
			triangular_index[i][0] = 0;
			triangular_index[i][1] = i + 1;
			triangular_index[i][2] = i + 2;
		}

		float total_area = 0.0f;
		float cx = 0.0f, cy = 0.0f;

		for (const auto& indices : triangular_index) {
			SDL_Point v0 = points[indices[0]];
			SDL_Point v1 = points[indices[1]];
			SDL_Point v2 = points[indices[2]];

			float A = 0.5f * (v0.x * (v1.y - v2.y) + v1.x * (v2.y - v0.y) + v2.x * (v0.y - v1.y)); // 三角形面积  
			total_area += A;

			cx += (v0.x + v1.x + v2.x) * A; // 质心x坐标累加  
			cy += (v0.y + v1.y + v2.y) * A; // 质心y坐标累加  
		}

		cx /= (3.0f * total_area);
		cy /= (3.0f * total_area);

		return FlatVector(cx, cy);
}
