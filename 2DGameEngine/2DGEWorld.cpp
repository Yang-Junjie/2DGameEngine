#include "2DGEWorld.h"
#include <unordered_map>

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


//��������
void World::SepareteBodies(Body& body_a, Body& body_b, FlatVector& separation_vector)
{
	if (body_a.stationary_) {
		body_b.Move(separation_vector / 2);
		//std::cout << separation_vector << std::endl;
	}
	else if (body_b.stationary_) {
		body_a.Move(-separation_vector / 2);
		
	}
	else {
		body_a.Move(-separation_vector / 2);
		body_b.Move(separation_vector / 2);
	}
}


void World::BroadPhase(std::vector<Body>& body_lists) {
	
	//����Ҫ��aabb��ɫ��������б��
	std::unordered_map<int, bool> colored;

	//��ʼ�ж����������aabb�Ƿ��ཻ
	for (auto& body_a : body_lists) {
		body_a.GetAABB();
		for (auto& body_b : body_lists) {
			
			//�������������ͬһ��������������μ��
			if (body_a.body_id_ == body_b.body_id_) {
				continue;
			}
			body_b.GetAABB();
			if (body_a.stationary_ && body_b.stationary_) {
				continue;
			}
			//���
			bool intersect = Intersect::IntersectAABB(body_a, body_b);
			
			//���û���ཻ�������һ������ļ��
			if (!intersect) {
				continue;
			}
			else {
				//����ཻ����б��
				colored[body_a.body_id_] = true;
				colored[body_b.body_id_] = true;

				//��������뼴���Ӵ������б���
				std::pair<Body&, Body&> pair_body(body_a, body_b);
				this->contact_body_.push_back(pair_body);

			}
			
		}
	}

	//�ٶԱ�ǹ��������aabb������ɫ
	for (auto& body : body_lists) {
		if (colored[body.body_id_]) {
			body.color_box_ = { 255, 255, 255, 255 };
		}
		else {
			body.color_box_ = { 255, 100, 100, 255 };
		}
	}
}


void World::NarrowPhase() 
{
	//����AABB�ཻ������
	for (auto& pair: this->contact_body_) {

		//������������ʹ����ײ��⣬�ж������������Ƿ�����ײ
		IntersectData intersect_data = Collide(pair.first, pair.second);
		
		//���������ײ��������������������������������
		if (intersect_data.Collision) {
			FlatVector separation_vector = intersect_data.normal * intersect_data.depth;
			SepareteBodies(pair.first, pair.second, separation_vector);

			//�õ��Ӵ��㲢����
			std::vector<FlatVector> contact_points = FindContactPoints(pair.first, pair.second);
			for (auto& contact_point : contact_points) {
				brush.DrawPoint(contact_point, 255, 255, 255, 25);
			}
			
			//���½Ӵ�����
			ContactData contact_data;
			contact_data.body_a_ = &(pair.first);
			contact_data.body_b_ = &(pair.second);
			contact_data.normal_ = intersect_data.normal;
			contact_data.contact_points_ = contact_points;
			ResolveCollisionWithRotationAndFriction(contact_data);

		}

	}
}




void World::Interation(std::vector<Body>& body_lists,float time) 

{
	//��ʼinteration_�ε���
	for (int i = 0; i < this->interation_; ++i) {

		//ÿ�ε���ǰ���aabb�ཻ������
		this->contact_body_.clear();

		//��body_lists���ó�һ�������������������
		for (auto& body : body_lists) {
			if (!body.stationary_) {
				body.velocity_ = body.velocity_ + body.acceleration_ * time;
				FlatVector displacement = body.velocity_ * time + body.acceleration_ * time * time * 0.5;
				float angle = body.angular_velocity_ * time;
				body.ClearAcceleration();
				body.Rotation(angle);
				body.Move(displacement);
			}
			
			
		}

		//���������������ײ����
		BroadPhase(body_lists);
		NarrowPhase();
	}
	
}


void World::ResolveCollisionWithRotationAndFriction(ContactData& contact) {
	
	Body* body_a = contact.body_a_;
	Body* body_b = contact.body_b_;
	FlatVector collision_normal = contact.normal_;
	FlatVector contact_point1;
	
	FlatVector contact_point2;
	int number_of_contacts = contact.contact_points_.size();
	

	if (number_of_contacts == 1) {
		contact_point1 = contact.contact_points_[0];
	}
	else {
		contact_point1 = contact.contact_points_[0];
		contact_point2 = contact.contact_points_[1];
	}
	
	float restitution_coefficient = std::min(body_a->restitution_, body_b->restitution_);  // ��ײ�ָ�ϵ��
	
	float static_friction_coefficient = (body_a->inherent_static_friction_ + body_b->inherent_static_friction_) * 0.5f;  // ��Ħ��ϵ��
	
	float dynamic_friction_coefficient = (body_a->inherent_dynamic_friction_ + body_b->inherent_dynamic_friction_) * 0.5f;  // ��Ħ��ϵ��
	

	std::vector<FlatVector> contact_points = { contact_point1, contact_point2 };  // ��ײ���б�
	std::vector<FlatVector> friction_impulses(number_of_contacts);  // Ħ�������б�
	std::vector<float> normal_impulses(number_of_contacts);  // ��������б�
	std::vector<FlatVector> impulses(number_of_contacts);  // �����б�
	std::vector<FlatVector> position_to_center_a(number_of_contacts);  // ��ײ�㵽����A���ĵ������б�
	std::vector<FlatVector> position_to_center_b(number_of_contacts);  // ��ײ�㵽����B���ĵ������б�

	// ���㷨�����
	for (int i = 0; i < number_of_contacts; ++i) {
		
		FlatVector position_from_a_to_contact = contact_points[i] - body_a->mass_center_;  // ��ײ�㵽����A���ĵ�����
		
		
		FlatVector position_from_b_to_contact = contact_points[i] - body_b->mass_center_;  // ��ײ�㵽����B���ĵ�����
		
		position_to_center_a[i] = position_from_a_to_contact;

		position_to_center_b[i] = position_from_b_to_contact;

		FlatVector perpendicular_to_position_a(-position_from_a_to_contact.y, position_from_a_to_contact.x);  // ra�����ķ�����
		FlatVector perpendicular_to_position_b(-position_from_b_to_contact.y, position_from_b_to_contact.x);  // rb�����ķ�����


		FlatVector angular_linear_velocity_a = perpendicular_to_position_a * body_a->angular_velocity_;  // ����A�Ľ��ٶȶ�raPerp�Ĺ���
	//	std::cout << angular_linear_velocity_a << std::endl;

		FlatVector angular_linear_velocity_b = perpendicular_to_position_b * body_b->angular_velocity_;  // ����B�Ľ��ٶȶ�rbPerp�Ĺ���

		FlatVector relative_velocity = (body_b->velocity_ + angular_linear_velocity_b) - (body_a->velocity_ + angular_linear_velocity_a);  // ����ٶ�
		
		float contact_velocity_magnitude = relative_velocity.dot(collision_normal);  // ����ٶ��ڷ��߷����ϵķ���
		
		if (contact_velocity_magnitude > 0) {
			continue;
		}

		float perpendicular_dot_normal_a = perpendicular_to_position_a.dot(collision_normal);  // raPerp�����뷨�������ĵ��
		float perpendicular_dot_normal_b = perpendicular_to_position_b.dot(collision_normal);  // rbPerp�����뷨�������ĵ��
		
		float denominator = body_a->inverse_mass_ + body_b->inverse_mass_ +
			(perpendicular_dot_normal_a * perpendicular_dot_normal_a) * body_a->inverse_rotational_inertia_ +
			(perpendicular_dot_normal_b * perpendicular_dot_normal_b) * body_b->inverse_rotational_inertia_;  // ��������ķ�ĸ
		
		float normal_impulse = -(1 + restitution_coefficient) * contact_velocity_magnitude / denominator / number_of_contacts;  // ������С
		
		normal_impulses[i] = normal_impulse;
		
		FlatVector impulse = normal_impulse * collision_normal;  // ��������
		
		impulses[i] = impulse;
	}

	// Ӧ�÷������
	for (int i = 0; i < number_of_contacts; ++i) {
		FlatVector impulse = impulses[i];
		FlatVector position_from_a_to_contact = position_to_center_a[i];
		FlatVector position_from_b_to_contact = position_to_center_b[i];

		body_a->velocity_ = body_a->velocity_ + ( - impulse * body_a->inverse_mass_);  // ����A���ٶ�����
		body_a->angular_velocity_ += -position_from_a_to_contact.cross(impulse) * body_a->inverse_rotational_inertia_;  // ����A���ٶ�����
		body_b->velocity_ = body_b->velocity_ + impulse * body_b->inverse_mass_;  // ����B���ٶ�����
		body_b->angular_velocity_ += position_from_b_to_contact.cross(impulse) * body_b->inverse_rotational_inertia_;  // ����B���ٶ�����
	}

	// ����Ħ������
	for (int i = 0; i < number_of_contacts; ++i) {
		
		FlatVector position_from_a_to_contact = contact_points[i] - body_a->mass_center_;  // ��ײ�㵽����A���ĵ�����
		FlatVector position_from_b_to_contact = contact_points[i] - body_b->mass_center_;  // ��ײ�㵽����B���ĵ�����

		position_to_center_a[i] = position_from_a_to_contact;
		position_to_center_b[i] = position_from_b_to_contact;

		FlatVector perpendicular_to_position_a(-position_from_a_to_contact.y, position_from_a_to_contact.x);  // ra�����ķ�����
		FlatVector perpendicular_to_position_b(-position_from_b_to_contact.y, position_from_b_to_contact.x);  // rb�����ķ�����

		FlatVector angular_linear_velocity_a = perpendicular_to_position_a * body_a->angular_velocity_;  // ����A�Ľ��ٶȶ�raPerp�Ĺ���
		FlatVector angular_linear_velocity_b = perpendicular_to_position_b * body_b->angular_velocity_;  // ����B�Ľ��ٶȶ�rbPerp�Ĺ���

		FlatVector relative_velocity = (body_b->velocity_ + angular_linear_velocity_b) - (body_a->velocity_ + angular_linear_velocity_a);  // ����ٶ�

		FlatVector tangent_direction = relative_velocity - relative_velocity.dot(collision_normal) * collision_normal;  // ������
		if (FlatVector::NearlyEqualFv(tangent_direction, FlatVector(0.0f, 0.0f))) {
			continue;
		}
		else {
			tangent_direction = FlatVector::normalize(tangent_direction);  // ��һ����������
		}

		float perpendicular_dot_tangent_a = perpendicular_to_position_a.dot(tangent_direction);  // raPerp���������������ĵ��
		float perpendicular_dot_tangent_b = perpendicular_to_position_b.dot(tangent_direction);  // rbPerp���������������ĵ��

		float denominator = body_a->inverse_mass_ + body_b->inverse_mass_ +
			(perpendicular_dot_tangent_a * perpendicular_dot_tangent_a) * body_a->inverse_rotational_inertia_ +
			(perpendicular_dot_tangent_b * perpendicular_dot_tangent_b) * body_b->inverse_rotational_inertia_;  // ��������ķ�ĸ

		float tangential_impulse = -relative_velocity.dot(tangent_direction) / denominator / number_of_contacts;  // ���������С
		float normal_impulse = normal_impulses[i];  // ���������С
		//std::cout << normal_impulse * static_friction_coefficient << std::endl;
		if (std::abs(tangential_impulse) <= normal_impulse * static_friction_coefficient) {  // ���ݾ�Ħ��ϵ���ж��Ƿ����㾲Ħ������
			FlatVector friction_impulse = tangential_impulse * tangent_direction;  // ��Ħ������
			
			friction_impulses[i] = friction_impulse;
		}
		else {
			FlatVector friction_impulse = -normal_impulse * tangent_direction * dynamic_friction_coefficient;  // ��Ħ������
			
			friction_impulses[i] = friction_impulse;
		}
	}

	// Ӧ��Ħ������
	for (int i = 0; i < number_of_contacts; ++i) {
		FlatVector friction_impulse = friction_impulses[i];
		FlatVector position_from_a_to_contact = position_to_center_a[i];
		FlatVector position_from_b_to_contact = position_to_center_b[i];
		
		body_a->velocity_ = body_a->velocity_+ ( - friction_impulse * body_a->inverse_mass_);  // ����A���ٶ�����
		//std::cout << position_from_b_to_contact << std::endl;

		body_a->angular_velocity_ += -position_from_a_to_contact.cross(friction_impulse) * body_a->inverse_rotational_inertia_;  // ����A���ٶ�����
		body_b->velocity_ = body_b->velocity_+ friction_impulse * body_b->inverse_mass_;  // ����B���ٶ�����
		body_b->angular_velocity_ += position_from_b_to_contact.cross(friction_impulse) * body_b->inverse_rotational_inertia_;  // ����B���ٶ�����
	}

	
}