#include "2DGEBody.h"

Body::Body() {

}
Body::Body(Shape shape, float radius, BodyColor color, float mass, FlatVector mass_center):shape_(shape),radius_(radius),color_(color),mass_center_(mass_center){

}

Body::Body(Shape shape, SDL_Vertex* vertices, BodyColor color, float mass, FlatVector mass_center):shape_(shape),vertices_(vertices),color_(color),mass_(mass),mass_center_(mass_center){

}

Body::~Body() {

}
BodyManager::BodyManager()
{
}

BodyManager::~BodyManager()
{
}

bool BodyManager::CreateBody(float radius, BodyColor& color, float mass, FlatVector& mass_center)
{
	Body b1(CIRCLE, radius, color, mass, mass_center);
	this->body_lists.push_back(b1);
	b1.~Body();
	return false;
}
bool BodyManager::CreateBody(SDL_Vertex* vertices, BodyColor& color, float mass, FlatVector& mass_center) {
	Body b1(CIRCLE, vertices, color, mass, mass_center);
	this->body_lists.push_back(b1);
	b1.~Body();
	return false;
}




bool BodyManager::DestroyBody() {
	return false;
}

int BodyManager::FindBody() {
	return -1;
}

