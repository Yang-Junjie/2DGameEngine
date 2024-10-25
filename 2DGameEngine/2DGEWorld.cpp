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


//分离物体
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
	
	//对需要对aabb上色的物体进行标记
	std::unordered_map<int, bool> colored;

	//开始判断两个物体的aabb是否相交
	for (auto& body_a : body_lists) {
		body_a.GetAABB();
		for (auto& body_b : body_lists) {
			
			//如果两个物体是同一个物体则跳过这次检测
			if (body_a.body_id_ == body_b.body_id_) {
				continue;
			}
			body_b.GetAABB();
			if (body_a.stationary_ && body_b.stationary_) {
				continue;
			}
			//检测
			bool intersect = Intersect::IntersectAABB(body_a, body_b);
			
			//如果没有相交则继续下一个物体的检测
			if (!intersect) {
				continue;
			}
			else {
				//如果相交则进行标记
				colored[body_a.body_id_] = true;
				colored[body_b.body_id_] = true;

				//并将其放入即将接触物体列表中
				std::pair<Body&, Body&> pair_body(body_a, body_b);
				this->contact_body_.push_back(pair_body);

			}
			
		}
	}

	//再对标记过的物体的aabb进行上色
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
	//遍历AABB相交的物体
	for (auto& pair: this->contact_body_) {

		//对这两个物体使用碰撞检测，判断这两个物体是否发生碰撞
		IntersectData intersect_data = Collide(pair.first, pair.second);
		
		//如果发生碰撞，则计算分离向量，并分离这两个物体
		if (intersect_data.Collision) {
			FlatVector separation_vector = intersect_data.normal * intersect_data.depth;
			SepareteBodies(pair.first, pair.second, separation_vector);

			//拿到接触点并绘制
			std::vector<FlatVector> contact_points = FindContactPoints(pair.first, pair.second);
			for (auto& contact_point : contact_points) {
				brush.DrawPoint(contact_point, 255, 255, 255, 25);
			}
			
			//更新接触数据
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
	//开始interation_次迭代
	for (int i = 0; i < this->interation_; ++i) {

		//每次迭代前清除aabb相交的物体
		this->contact_body_.clear();

		//从body_lists中拿出一个物体对其进行物理操作
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

		//物理操作完后进入碰撞管线
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
	
	float restitution_coefficient = std::min(body_a->restitution_, body_b->restitution_);  // 碰撞恢复系数
	
	float static_friction_coefficient = (body_a->inherent_static_friction_ + body_b->inherent_static_friction_) * 0.5f;  // 静摩擦系数
	
	float dynamic_friction_coefficient = (body_a->inherent_dynamic_friction_ + body_b->inherent_dynamic_friction_) * 0.5f;  // 动摩擦系数
	

	std::vector<FlatVector> contact_points = { contact_point1, contact_point2 };  // 碰撞点列表
	std::vector<FlatVector> friction_impulses(number_of_contacts);  // 摩擦冲量列表
	std::vector<float> normal_impulses(number_of_contacts);  // 法向冲量列表
	std::vector<FlatVector> impulses(number_of_contacts);  // 冲量列表
	std::vector<FlatVector> position_to_center_a(number_of_contacts);  // 碰撞点到物体A质心的向量列表
	std::vector<FlatVector> position_to_center_b(number_of_contacts);  // 碰撞点到物体B质心的向量列表

	// 计算法向冲量
	for (int i = 0; i < number_of_contacts; ++i) {
		
		FlatVector position_from_a_to_contact = contact_points[i] - body_a->mass_center_;  // 碰撞点到物体A质心的向量
		
		
		FlatVector position_from_b_to_contact = contact_points[i] - body_b->mass_center_;  // 碰撞点到物体B质心的向量
		
		position_to_center_a[i] = position_from_a_to_contact;

		position_to_center_b[i] = position_from_b_to_contact;

		FlatVector perpendicular_to_position_a(-position_from_a_to_contact.y, position_from_a_to_contact.x);  // ra向量的法向量
		FlatVector perpendicular_to_position_b(-position_from_b_to_contact.y, position_from_b_to_contact.x);  // rb向量的法向量


		FlatVector angular_linear_velocity_a = perpendicular_to_position_a * body_a->angular_velocity_;  // 物体A的角速度对raPerp的贡献
	//	std::cout << angular_linear_velocity_a << std::endl;

		FlatVector angular_linear_velocity_b = perpendicular_to_position_b * body_b->angular_velocity_;  // 物体B的角速度对rbPerp的贡献

		FlatVector relative_velocity = (body_b->velocity_ + angular_linear_velocity_b) - (body_a->velocity_ + angular_linear_velocity_a);  // 相对速度
		
		float contact_velocity_magnitude = relative_velocity.dot(collision_normal);  // 相对速度在法线方向上的分量
		
		if (contact_velocity_magnitude > 0) {
			continue;
		}

		float perpendicular_dot_normal_a = perpendicular_to_position_a.dot(collision_normal);  // raPerp向量与法线向量的点积
		float perpendicular_dot_normal_b = perpendicular_to_position_b.dot(collision_normal);  // rbPerp向量与法线向量的点积
		
		float denominator = body_a->inverse_mass_ + body_b->inverse_mass_ +
			(perpendicular_dot_normal_a * perpendicular_dot_normal_a) * body_a->inverse_rotational_inertia_ +
			(perpendicular_dot_normal_b * perpendicular_dot_normal_b) * body_b->inverse_rotational_inertia_;  // 冲量计算的分母
		
		float normal_impulse = -(1 + restitution_coefficient) * contact_velocity_magnitude / denominator / number_of_contacts;  // 冲量大小
		
		normal_impulses[i] = normal_impulse;
		
		FlatVector impulse = normal_impulse * collision_normal;  // 冲量向量
		
		impulses[i] = impulse;
	}

	// 应用法向冲量
	for (int i = 0; i < number_of_contacts; ++i) {
		FlatVector impulse = impulses[i];
		FlatVector position_from_a_to_contact = position_to_center_a[i];
		FlatVector position_from_b_to_contact = position_to_center_b[i];

		body_a->velocity_ = body_a->velocity_ + ( - impulse * body_a->inverse_mass_);  // 物体A线速度增量
		body_a->angular_velocity_ += -position_from_a_to_contact.cross(impulse) * body_a->inverse_rotational_inertia_;  // 物体A角速度增量
		body_b->velocity_ = body_b->velocity_ + impulse * body_b->inverse_mass_;  // 物体B线速度增量
		body_b->angular_velocity_ += position_from_b_to_contact.cross(impulse) * body_b->inverse_rotational_inertia_;  // 物体B角速度增量
	}

	// 计算摩擦冲量
	for (int i = 0; i < number_of_contacts; ++i) {
		
		FlatVector position_from_a_to_contact = contact_points[i] - body_a->mass_center_;  // 碰撞点到物体A质心的向量
		FlatVector position_from_b_to_contact = contact_points[i] - body_b->mass_center_;  // 碰撞点到物体B质心的向量

		position_to_center_a[i] = position_from_a_to_contact;
		position_to_center_b[i] = position_from_b_to_contact;

		FlatVector perpendicular_to_position_a(-position_from_a_to_contact.y, position_from_a_to_contact.x);  // ra向量的法向量
		FlatVector perpendicular_to_position_b(-position_from_b_to_contact.y, position_from_b_to_contact.x);  // rb向量的法向量

		FlatVector angular_linear_velocity_a = perpendicular_to_position_a * body_a->angular_velocity_;  // 物体A的角速度对raPerp的贡献
		FlatVector angular_linear_velocity_b = perpendicular_to_position_b * body_b->angular_velocity_;  // 物体B的角速度对rbPerp的贡献

		FlatVector relative_velocity = (body_b->velocity_ + angular_linear_velocity_b) - (body_a->velocity_ + angular_linear_velocity_a);  // 相对速度

		FlatVector tangent_direction = relative_velocity - relative_velocity.dot(collision_normal) * collision_normal;  // 切向方向
		if (FlatVector::NearlyEqualFv(tangent_direction, FlatVector(0.0f, 0.0f))) {
			continue;
		}
		else {
			tangent_direction = FlatVector::normalize(tangent_direction);  // 归一化切向向量
		}

		float perpendicular_dot_tangent_a = perpendicular_to_position_a.dot(tangent_direction);  // raPerp向量与切向向量的点积
		float perpendicular_dot_tangent_b = perpendicular_to_position_b.dot(tangent_direction);  // rbPerp向量与切向向量的点积

		float denominator = body_a->inverse_mass_ + body_b->inverse_mass_ +
			(perpendicular_dot_tangent_a * perpendicular_dot_tangent_a) * body_a->inverse_rotational_inertia_ +
			(perpendicular_dot_tangent_b * perpendicular_dot_tangent_b) * body_b->inverse_rotational_inertia_;  // 冲量计算的分母

		float tangential_impulse = -relative_velocity.dot(tangent_direction) / denominator / number_of_contacts;  // 切向冲量大小
		float normal_impulse = normal_impulses[i];  // 法向冲量大小
		//std::cout << normal_impulse * static_friction_coefficient << std::endl;
		if (std::abs(tangential_impulse) <= normal_impulse * static_friction_coefficient) {  // 根据静摩擦系数判断是否满足静摩擦条件
			FlatVector friction_impulse = tangential_impulse * tangent_direction;  // 静摩擦冲量
			
			friction_impulses[i] = friction_impulse;
		}
		else {
			FlatVector friction_impulse = -normal_impulse * tangent_direction * dynamic_friction_coefficient;  // 动摩擦冲量
			
			friction_impulses[i] = friction_impulse;
		}
	}

	// 应用摩擦冲量
	for (int i = 0; i < number_of_contacts; ++i) {
		FlatVector friction_impulse = friction_impulses[i];
		FlatVector position_from_a_to_contact = position_to_center_a[i];
		FlatVector position_from_b_to_contact = position_to_center_b[i];
		
		body_a->velocity_ = body_a->velocity_+ ( - friction_impulse * body_a->inverse_mass_);  // 物体A线速度增量
		//std::cout << position_from_b_to_contact << std::endl;

		body_a->angular_velocity_ += -position_from_a_to_contact.cross(friction_impulse) * body_a->inverse_rotational_inertia_;  // 物体A角速度增量
		body_b->velocity_ = body_b->velocity_+ friction_impulse * body_b->inverse_mass_;  // 物体B线速度增量
		body_b->angular_velocity_ += position_from_b_to_contact.cross(friction_impulse) * body_b->inverse_rotational_inertia_;  // 物体B角速度增量
	}

	
}