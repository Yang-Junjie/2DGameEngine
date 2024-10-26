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
	
	// 对需要对aabb上色的物体进行标记
	std::unordered_map<int, bool> colored;

	// 开始判断两个物体的aabb是否相交
	for (size_t i = 0; i < body_lists.size(); ++i) {
		auto& body_a = body_lists[i];
		body_a.GetAABB();

		for (size_t j = i + 1; j < body_lists.size(); ++j) { 
			auto& body_b = body_lists[j];
			body_b.GetAABB();

			if (body_a.stationary_ && body_b.stationary_) {
				continue;
			}

			// 检测
			bool intersect = Intersect::IntersectAABB(body_a, body_b);

			// 如果没有相交则继续下一个物体的检测
			if (!intersect) {
				continue;
			}
			else {
				// 如果相交则进行标记
				colored[body_a.body_id_] = true;
				colored[body_b.body_id_] = true;

				// 并将其放入即将接触物体列表中
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
			contact_data.contact1_ = contact_points[0];
			contact_data.contact_count_ = 1;
			if (contact_points.size() > 1) {
				contact_data.contact2_ = contact_points[1];
				contact_data.contact_count_ = 2;
			}
			//std::cout << "进入函数前的velocity" << pair.first.velocity_ << std::endl;
			ResolveCollisionWithRotationAndFriction(contact_data);
			//std::cout << "进入函数后的velocity" << pair.first.velocity_ << std::endl;
		}

	}
}

//void World::ResolveCollisionWithRotationAndFriction(ContactData& contact)
//{
//    
//        std::vector<FlatVector> contactList;
//        std::vector<FlatVector> impulseList;
//        std::vector<FlatVector> raList;
//        std::vector<FlatVector> rbList;
//        std::vector<FlatVector> frictionImpulseList;
//        std::vector<float> jList;
//
//        Body* bodyA = contact.body_a_;
//        Body* bodyB = contact.body_b_;
//        FlatVector normal = contact.normal_;
//        FlatVector contact1 = contact.contact1_;
//        FlatVector contact2 = contact.contact2_;
//        int contactCount = contact.contact_count_;
//
//        float e = std::min(bodyA->restitution_, bodyB->restitution_);
//
//        float sf = (bodyA->inherent_static_friction_ + bodyB->inherent_static_friction_) * 0.5f;
//        float df = (bodyA->inherent_dynamic_friction_ + bodyB->inherent_dynamic_friction_) * 0.5f;
//
//        contactList.resize(contactCount);
//        impulseList.resize(contactCount);
//        raList.resize(contactCount);
//        rbList.resize(contactCount);
//        frictionImpulseList.resize(contactCount);
//        jList.resize(contactCount);
//
//        contactList[0] = contact1;
//        contactList[1] = contact2;
//
//        for (int i = 0; i < contactCount; ++i) {
//            impulseList[i] = { 0.0f, 0.0f };
//            raList[i] = { 0.0f, 0.0f };
//            rbList[i] = { 0.0f, 0.0f };
//            frictionImpulseList[i] = { 0.0f, 0.0f };
//            jList[i] = 0.0f;
//        }
//
//        for (int i = 0; i < contactCount; ++i) {
//            FlatVector ra = contactList[i] - bodyA->mass_center_;
//            FlatVector rb = contactList[i] - bodyB->mass_center_;
//
//            raList[i] = ra;
//            rbList[i] = rb;
//
//            FlatVector raPerp{ -ra.y, ra.x };
//            FlatVector rbPerp{ -rb.y, rb.x };
//
//            FlatVector angularLinearVelocityA = raPerp * bodyA->angular_velocity_;
//            FlatVector angularLinearVelocityB = rbPerp * bodyB->angular_velocity_;
//
//            FlatVector relativeVelocity =
//                (bodyB->velocity_ + angularLinearVelocityB) -
//                (bodyA->velocity_ + angularLinearVelocityA);
//
//            float contactVelocityMag = FlatVector::dot(relativeVelocity, normal);
//
//            if (contactVelocityMag > 0.0f) {
//                continue;
//            }
//
//            float raPerpDotN = FlatVector::dot(raPerp, normal);
//            float rbPerpDotN = FlatVector::dot(rbPerp, normal);
//
//            float denom = bodyA->inverse_mass_ + bodyB->inverse_mass_ +
//                (raPerpDotN * raPerpDotN) * bodyA->inverse_rotational_inertia_ +
//                (rbPerpDotN * rbPerpDotN) * bodyB->inverse_rotational_inertia_;
//
//            float j = -(1.0f + e) * contactVelocityMag;
//            j /= denom;
//            j /= static_cast<float>(contactCount);
//
//            jList[i] = j;
//
//            FlatVector impulse = j * normal;
//            impulseList[i] = impulse;
//        }
//
//        for (int i = 0; i < contactCount; ++i) {
//            FlatVector impulse = impulseList[i];
//            FlatVector ra = raList[i];
//            FlatVector rb = rbList[i];
//
//            bodyA->velocity_ += -impulse * bodyA->inverse_mass_;
//            bodyA->angular_velocity_ += -FlatVector::cross(ra, impulse) * bodyA->inverse_rotational_inertia_;
//            bodyB->velocity_ += impulse * bodyB->inverse_mass_;
//            bodyB->angular_velocity_ += FlatVector::cross(rb, impulse) * bodyB->inverse_rotational_inertia_;
//        }
//
//        for (int i = 0; i < contactCount; ++i) {
//            FlatVector ra = contactList[i] - bodyA->mass_center_;
//            FlatVector rb = contactList[i] - bodyB->mass_center_;
//
//            raList[i] = ra;
//            rbList[i] = rb;
//
//            FlatVector raPerp{ -ra.y, ra.x };
//            FlatVector rbPerp{ -rb.y, rb.x };
//
//            FlatVector angularLinearVelocityA = raPerp * bodyA->angular_velocity_;
//            FlatVector angularLinearVelocityB = rbPerp * bodyB->angular_velocity_;
//
//            FlatVector relativeVelocity =
//                (bodyB->velocity_ + angularLinearVelocityB) -
//                (bodyA->velocity_ + angularLinearVelocityA);
//
//            FlatVector tangent = relativeVelocity - FlatVector::dot(relativeVelocity, normal) * normal;
//
//            if (FlatVector::NearlyEqualFv(tangent, FlatVector(0, 0))) {
//                continue;
//            }
//            else {
//                tangent = FlatVector::normalize(tangent);
//            }
//
//            float raPerpDotT = FlatVector::dot(raPerp, tangent);
//            float rbPerpDotT = FlatVector::dot(rbPerp, tangent);
//
//            float denom = bodyA->inverse_mass_ + bodyB->inverse_mass_ +
//                (raPerpDotT * raPerpDotT) * bodyA->inverse_rotational_inertia_ +
//                (rbPerpDotT * rbPerpDotT) * bodyB->inverse_rotational_inertia_;
//
//            float jt = -FlatVector::dot(relativeVelocity, tangent);
//            jt /= denom;
//            jt /= static_cast<float>(contactCount);
//
//            FlatVector frictionImpulse;
//            float j = jList[i];
//
//            if (std::fabs(jt) <= j * sf) {
//                frictionImpulse = jt * tangent;
//            }
//            else {
//                frictionImpulse = -j * tangent * df;
//            }
//
//            frictionImpulseList[i] = frictionImpulse;
//        }
//
//        for (int i = 0; i < contactCount; ++i) {
//            FlatVector frictionImpulse = frictionImpulseList[i];
//            FlatVector ra = raList[i];
//            FlatVector rb = rbList[i];
//
//            bodyA->velocity_ += -frictionImpulse * bodyA->inverse_mass_;
//            bodyA->angular_velocity_ += -FlatVector::cross(ra, frictionImpulse) * bodyA->inverse_rotational_inertia_;
//            bodyB->velocity_ += frictionImpulse * bodyB->inverse_mass_;
//            bodyB->angular_velocity_ += FlatVector::cross(rb, frictionImpulse) * bodyB->inverse_rotational_inertia_;
//        }
//}
//





void World::Interation(std::vector<Body>& body_lists,float time) 

{
	//std::cout << "开始迭代的velocity" << body_lists[0].velocity_ << std::endl;
	//开始interation_次迭代
	for (int i = 0; i < this->interation_; ++i) {

		//每次迭代前清除aabb相交的物体
		this->contact_body_.clear();

		//从body_lists中拿出一个物体对其进行物理操作
		for (auto& body : body_lists) {
			if (!body.stationary_) {

				
				FlatVector displacement = body.velocity_ * time;
				float angle = body.angular_velocity_ * time;
				
				body.ClearAcceleration();
				body.Rotation(angle);
				body.Move(displacement);
				
			}
		}
		//物理操作完后进入碰撞管线
		
		BroadPhase(body_lists);
		
		

		//bug
		
		NarrowPhase();
		

		
	}
	
}
/*			FlatBody bodyA = contact.BodyA;
            FlatBody bodyB = contact.BodyB;
            FlatVector normal = contact.Normal;
            FlatVector contact1 = contact.Contact1;
            FlatVector contact2 = contact.Contact2;
            int contactCount = contact.ContactCount;

            float e = MathF.Min(bodyA.Restitution, bodyB.Restitution);

            this.contactList[0] = contact1;
            this.contactList[1] = contact2;

            for(int i = 0; i < contactCount; i++)
            {
                this.impulseList[i] = FlatVector.Zero;
                this.raList[i] = FlatVector.Zero;
                this.rbList[i] = FlatVector.Zero;
            }

            for(int i = 0; i < contactCount; i++)
            {
                FlatVector ra = contactList[i] - bodyA.Position;
                FlatVector rb = contactList[i] - bodyB.Position;

                raList[i] = ra;
                rbList[i] = rb;

                FlatVector raPerp = new FlatVector(-ra.Y, ra.X);
                FlatVector rbPerp = new FlatVector(-rb.Y, rb.X);

                FlatVector angularLinearVelocityA = raPerp * bodyA.AngularVelocity;
                FlatVector angularLinearVelocityB = rbPerp * bodyB.AngularVelocity;

                FlatVector relativeVelocity = 
                    (bodyB.LinearVelocity + angularLinearVelocityB) - 
                    (bodyA.LinearVelocity + angularLinearVelocityA);

                float contactVelocityMag = FlatMath.Dot(relativeVelocity, normal);

                if (contactVelocityMag > 0f)
                {
                    continue;
                }

                float raPerpDotN = FlatMath.Dot(raPerp, normal);
                float rbPerpDotN = FlatMath.Dot(rbPerp, normal);

                float denom = bodyA.InvMass + bodyB.InvMass + 
                    (raPerpDotN * raPerpDotN) * bodyA.InvInertia + 
                    (rbPerpDotN * rbPerpDotN) * bodyB.InvInertia;

                float j = -(1f + e) * contactVelocityMag;
                j /= denom;
                j /= (float)contactCount;

                FlatVector impulse = j * normal;
                impulseList[i] = impulse;
            }

            for(int i = 0; i < contactCount; i++)
            {
                FlatVector impulse = impulseList[i];
                FlatVector ra = raList[i];
                FlatVector rb = rbList[i];

                bodyA.LinearVelocity += -impulse * bodyA.InvMass;
                bodyA.AngularVelocity += -FlatMath.Cross(ra, impulse) * bodyA.InvInertia;
                bodyB.LinearVelocity += impulse * bodyB.InvMass;
                bodyB.AngularVelocity += FlatMath.Cross(rb, impulse) * bodyB.InvInertia;
            }*/

void World::ResolveCollisionWithRotationAndFriction(ContactData& contact) {
	Body* bodyA = contact.body_a_;
	Body* bodyB = contact.body_b_;
	FlatVector normal = contact.normal_;
	
	// 计算相对速度
	FlatVector relativeVelocity = bodyB->velocity_ - bodyA->velocity_;

	// 如果相对速度沿法线方向为正，表示物体已经分离，不需要处理碰撞
	float relativeVelocityDotNormal = FlatVector::dot(relativeVelocity, normal);
	
	if (relativeVelocityDotNormal > 0.f) {
		return;
	}

	// 计算恢复系数
	float e = std::min(bodyA->restitution_, bodyB->restitution_);

	// 计算分母，确保不为零
	float denominator = bodyA->inverse_mass_ + bodyB->inverse_mass_;
	const float epsilon = 1e-10f;
	denominator = std::max(denominator, epsilon);

	// 计算冲量大小
	float j = -(1.0f + e) * relativeVelocityDotNormal / denominator;
	
	// 计算冲量向量
	FlatVector impulse = j * normal;
	
	// 应用冲量
	if (bodyA->inverse_mass_ > 0.0f) {
		bodyA->velocity_ -= impulse * bodyA->inverse_mass_;
	}
	if (bodyB->inverse_mass_ > 0.0f) {
		bodyB->velocity_ += impulse * bodyB->inverse_mass_;
	}
	
	
}

   

