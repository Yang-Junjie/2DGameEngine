#pragma once

#include <vector>
#include "SDL.h"
#include "2DGEBody.h"


/*�������
Collision���Ƿ���ײ���ཻ��
depth:��ײ���
normal����������
*/
struct IntersectData {
    bool Collision = false;
    float depth = 0.0f;
    FlatVector normal;
};

//����������ĳ�����ϵ�ͶӰ
static FlatVector ProjectVertices(const FlatVector axis,const std::vector<FlatVector> vertices) {
    FlatVector interval;//��������ʾ��������x��ʾ�������ȷ�磬y��ʾ�������ȷ��
    interval.x = std::numeric_limits<float>::max();
    interval.y = std::numeric_limits<float>::min();
    for (FlatVector vertiex : vertices) {
        float project = FlatVector::dot(vertiex, axis);//��������������ϵ�ͶӰ
        if (project < interval.x) {//����ͶӰ����ķ�ΧΪͶӰ��Χ
            interval.x = project;
        }
        if (project > interval.y) {
            interval.y = project;
        }
    }
    return interval;
}

//����Բ��ĳ�����ϵ�ͶӰ
static FlatVector ProjectCircle(const FlatVector center,const float radius,const FlatVector axis) {
    FlatVector interval;
    
    FlatVector direction_vector = FlatVector::normalize(axis);
    FlatVector direction_and_radius = direction_vector * radius;

    FlatVector p1 = center + direction_and_radius;
    FlatVector p2 = center - direction_and_radius;

    interval.x = FlatVector::dot(p1, axis);
    interval.y = FlatVector::dot(p2, axis);

    if (interval.x > interval.y) {
        float temp;
        temp = interval.x;
        interval.x = interval.y;
        interval.y = temp;
    }
    return interval;
}

//�ҵ�����ξ��������Ķ���
static size_t FindVertexClosestPoint(const FlatVector point, const std::vector<FlatVector> vertices) {
    size_t result = 0;
    float min_distance = std::numeric_limits<float>::max();
    for (size_t i = 0; i < vertices.size(); ++i) {
        float distance = FlatVector::Distance(vertices[i], point);
        if (distance <= min_distance) {
            min_distance = distance;
            result = i;
        }
    }
    return result;
}




//�������AABB�⽫������㵽AABB�ľ����ƽ��
//�������AABB�ϻ�����AABB�ڽ������0
static float DistSqdPointAABB(FlatVector& p, std::vector<FlatVector>& vertices_aabb) {
    float sqDist = 0.0f;
    for (int i = 0; i < 2; i++) {
        float v, min, max;
        if (i == 0)
        {
            v = p.x;
            min = vertices_aabb[3].x;
            max = vertices_aabb[1].x;
            /*��iΪ0ʱ��Ӧx
            v����Ϊ�� p �� x ���ꡣ
            min ����Ϊ�߽�����Ͻǵ�� x ���꣨vertices_aabb[3].x����
            max ����Ϊ�߽�����½ǵ�� x ���꣨vertices_aabb[1].x����*/
        }
        if (i == 1)
        {
            v = p.y;
            min = vertices_aabb[3].y;
            max = vertices_aabb[1].y;
            /*��iΪ1ʱ��Ӧy
            v����Ϊ�� p �� y ���ꡣ
            min ����Ϊ�߽�����Ͻǵ�� y ���꣨vertices_aabb[3].y����
            max ����Ϊ�߽�����½ǵ�� y ���꣨vertices_aabb[1].y����y*/
        }
        if (v >= min && v <= max) return 0.0f;//�����AABB����ֱ�ӷ���0��
        if (v < min) sqDist += (min - v) * (min - v);
        if (v > max) sqDist += (v - max) * (v - max);
    }
    return sqDist; // ����ƽ����  
}


//�������ֱ�ߵĽӴ���
//��ԭ���Ǽ��������߶��ĸ��˵����ô�Ǹ��˵㼫�п����ǽӴ���
static FlatVector GetSegmentContactPoint(FlatVector& point, FlatVector& segment_start, FlatVector& segment_end) {
    FlatVector segment_vector = segment_end - segment_start;//�����߶�����
    FlatVector point_to_segment_start_vector = point - segment_start;//������߶����ָ��������

    float projection_length = FlatVector::dot(point_to_segment_start_vector, segment_vector);
    float segment_length_squared = FlatVector::LengthSquared(segment_vector);
    float projection_parameter = projection_length / segment_length_squared;//����ͶӰ���� proj_param = (|AP||AB| * cos(��)) / |AB|^2
    
    if (projection_parameter <= 0) {
        return segment_start;
    }
    if (projection_parameter >= 1) {
        return segment_end;
    }
    else {
        return segment_start + (segment_end - segment_start) * projection_parameter;
    }
}

//�������ֱ�ߵĽӴ��㷵�ؽӴ���
inline FlatVector SegmentNearestEndpoint(const float projection_parameter, const FlatVector& segment_start, const FlatVector& segment_end) {
    if (projection_parameter <= 0) {
        return segment_start;
    }
    if (projection_parameter >= 1) {
        return segment_end;
    }
    else {
        return segment_start + (segment_end - segment_start) * projection_parameter;
    }
}

//���������߶�����Ķ˵�ľ����ƽ��
static float DisSqdPointSegmentNearestEndpoint(FlatVector& point, FlatVector& segment_start, FlatVector& segment_end) {
    FlatVector segment_vector = segment_end - segment_start;//�����߶�����
    FlatVector point_to_segment_start_vector = point - segment_start;//������߶����ָ��������

    float projection_length = FlatVector::dot(point_to_segment_start_vector, segment_vector);
    float segment_length_squared = FlatVector::LengthSquared(segment_vector);
    float projection_parameter = projection_length / segment_length_squared;//����ͶӰ���� proj_param = (|AP||AB| * cos(��)) / |AB|^2

    FlatVector closest_point = SegmentNearestEndpoint(projection_parameter,  segment_start, segment_end);
    float distance_squared = FlatVector::DistanceSquared(point, closest_point);

    return distance_squared;
}


//�������ռ������Ѱ������������ĽӴ���ĺ���
//�����������ռ���Ϊ�˴���ģ�黯��ʹ����ṹ������
namespace FindContactPoint {

    /*�ҵ�Բ��Բ��ײʱ�ĽӴ���*/
    static FlatVector FindCirclesContactPoint(FlatVector& center_a, float radius_a, FlatVector& center_b) {
        FlatVector v_ab = center_b - center_a;
        v_ab.normalize();
        FlatVector contact_point = center_a + v_ab * radius_a;
        return contact_point;
    }


    /*�ҵ�Բ��������ײʱ�ĽӴ���*/
    static FlatVector FindCirclePolygonContactPoint(FlatVector& circle_center, std::vector<FlatVector>& polygon_vertices) {
        float min_distance_Sq = std::numeric_limits<float>::max();
        FlatVector contact_point;
        for (int i = 0; i < static_cast<int>(polygon_vertices.size()); i++) {
            FlatVector v_a = polygon_vertices[i];
            FlatVector v_b = polygon_vertices[(i + 1) % polygon_vertices.size()];
            float distance_sq = DisSqdPointSegmentNearestEndpoint(circle_center,v_a,v_b);
            if (distance_sq < min_distance_Sq) {
                min_distance_Sq = distance_sq;
                contact_point = GetSegmentContactPoint(circle_center, v_a, v_b);
            }
        }
        return contact_point;
    }


};


//�������ռ�����˼��������������Ƿ��ཻ�ĺ���
//�����������ռ���Ϊ�˴���ģ�黯��ʹ����ṹ������
namespace Intersect {

    /*�˺��������ж�2�������AABB�Ƿ��ཻ*/
    static bool IntersectAABB(Body& bodyA, Body& bodyB)
    {
        if (bodyA.vertices_aabb_[1].x <= bodyB.vertices_aabb_[0].x ||
            bodyB.vertices_aabb_[1].x <= bodyA.vertices_aabb_[0].x ||
            bodyA.vertices_aabb_[0].y <= bodyB.vertices_aabb_[2].y ||
            bodyB.vertices_aabb_[0].y <= bodyA.vertices_aabb_[2].y) {
            return false;
        }
        return true;
    }


    /*Բ��Բ��ײ���*/
    static IntersectData IntersectCircles(FlatVector& center_a, float radius_a, FlatVector& center_b, float radius_b) {
        IntersectData data;
        float both_distance = FlatVector::Distance(center_a,center_b);
        float sum_of_radii = radius_a + radius_b;

        if (both_distance >= sum_of_radii) {
            return data;
        }

        FlatVector unit_direction_vector_ab = center_b - center_a;
        unit_direction_vector_ab.normalize();
        data.Collision = true;
        data.normal = unit_direction_vector_ab;
        data.depth = sum_of_radii - both_distance;

        return data;
    }

};


//�ҵ�������ײ����ĽӴ���
static std::vector<FlatVector> FindContactPoints(Body& body_a,Body& body_b) {
    std::vector<FlatVector> contact_points;
    Shape shape_type_a = body_a.shape_;
    Shape shape_type_b = body_b.shape_;

    if (shape_type_a == POLTGON) {
        if (shape_type_b == POLTGON) {
            return contact_points;
        }
        else if (shape_type_b == CIRCLE) {
            //FindCirclePolygonContactPoint
            return contact_points;
        }
    }
    else if (shape_type_a == CIRCLE) {
        if (shape_type_b == CIRCLE) {
            contact_points.push_back(FindContactPoint::FindCirclesContactPoint(body_a.mass_center_, body_a.radius_, body_b.mass_center_));
            return contact_points;
        }
        else if (shape_type_b == POLTGON) {
            contact_points.push_back(FindContactPoint::FindCirclePolygonContactPoint(body_a.mass_center_,body_b.vertices_));
            return contact_points;
        }
        
    }
    return contact_points;
}


//�ж����������Ƿ���ײ�������������ײboolֵ����ײ���,��ײ����
static IntersectData  Collide(Body& body_a, Body& body_b) {
    IntersectData intersect_data;
    Shape shape_type_a = body_a.shape_;
    Shape shape_type_b = body_b.shape_;

    if (shape_type_a == POLTGON) {
        if (shape_type_b == POLTGON) {
        
        }
        else if (shape_type_b == CIRCLE) {
        
        }
    }
    else if (shape_type_a == CIRCLE) {
        if (shape_type_b == CIRCLE) {
            intersect_data = Intersect::IntersectCircles(body_a.mass_center_, body_a.radius_, body_b.mass_center_, body_b.radius_);
           
            return intersect_data;
        }
        else if (shape_type_b == POLTGON) {
        
        }
    }

    return intersect_data;
}

