#include "2DGEBoundVol.h"

void AABB::GetAABB(const std::vector<Body>::iterator body)
{
	if (body->shape_ == 0) {
		const float center_x = body->mass_center_.x;
		const float center_y = body->mass_center_.y;
		const float radius = body->radius_;
		this->vertices_[0] = { center_x - radius , center_y + radius };
		this->vertices_[1] = { center_x + radius , center_y + radius };
		this->vertices_[2] = { center_x + radius , center_y - radius };
		this->vertices_[3] = { center_x - radius , center_y - radius };
	}
	else if (body->shape_ == 1) {
		const std::vector<SDL_FPoint> vertices = body->vertices_;
		float max_x = -std::numeric_limits<float>::max();
		float max_y = -std::numeric_limits<float>::max();
		float min_x =  std::numeric_limits<float>::max();
		float min_y =  std::numeric_limits<float>::max();	
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
		this->vertices_[0] = { min_x , max_y };
		this->vertices_[1] = { max_x , max_y };
		this->vertices_[2] = { max_x , min_y };
		this->vertices_[3] = { min_x , min_y };
	}


}

void AABB::RenderAABB(Brush& brush)
{
	brush.DrawPolygon(this->vertices_, 255, 100, 100, 255);
}
