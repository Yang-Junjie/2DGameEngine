#pragma once

#include <vector>
#include "SDL.h"
#include "2DGEBody.h"


//���º�����ʹ��const ���ü���copy�ڴ濪��

/*�������
Collision���Ƿ���ײ���ཻ��
depth:��ײ���
normal����������
*/
struct IntersectData {
    bool Collision = false;
    float depth = std::numeric_limits<float>::max();
    FlatVector normal;
};

//�������������ϵ�ͶӰ
static  FlatVector ProjectVertices(const FlatVector& axis, const std::vector<FlatVector>& vertices) {
    float min = std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::lowest();

    for (const auto& vertex : vertices) {
        float projection = axis.dot(vertex);
        min = std::min(min, projection);
        max = std::max(max, projection);
    }

    return FlatVector(min, max);
}

//����Բ��ĳ�����ϵ�ͶӰ
static FlatVector ProjectCircle(const FlatVector& center,const float& radius,const FlatVector& axis) {
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
static size_t FindVertexClosestPoint(const FlatVector& point, const std::vector<FlatVector>& vertices) {
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
static float DistSqdPointAABB(const FlatVector& p, const std::vector<FlatVector>& vertices_aabb) {
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
static FlatVector GetSegmentContactPoint(const FlatVector& point, const FlatVector& segment_start, const FlatVector& segment_end) {
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

//�������ֱ�ߵ�����㣨���ܵĽӴ��㣩���������(���ܵĽӴ���)
inline FlatVector SegmentNearestEndpoint(const float& projection_parameter, const FlatVector& segment_start, const FlatVector& segment_end) {
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
static float DisSqdPointSegmentNearestEndpoint(const FlatVector& point, const FlatVector& segment_start, const FlatVector& segment_end) {
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
    static FlatVector FindCirclesContactPoint(const FlatVector& center_a, const float& radius_a, const FlatVector& center_b) {
        FlatVector v_ab = center_b - center_a;
        v_ab.normalize();
        FlatVector contact_point = center_a + v_ab * radius_a;
        return contact_point;
    }


    /*�ҵ�Բ��������ײʱ�ĽӴ���*/
    static FlatVector FindCirclePolygonContactPoint(const FlatVector& circle_center, const std::vector<FlatVector>& polygon_vertices) {
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

    //�ҵ�����������εĽӴ���
    static std::vector<FlatVector> FindPolygonsContactPoints(const std::vector<FlatVector>& vertices_a, const std::vector<FlatVector>& vertices_b) {
        //��ʼ���Ӵ���
        std::vector<FlatVector> contact_points;
        FlatVector contact1;//��һ���Ӵ���
        FlatVector contact2;//�ڶ����Ӵ��㣬���û�еڶ����Ӵ��㣬�򱣳�Ϊԭ�� 
        size_t contact_count = 0;
        
        //��ʼ����С�����ƽ��Ϊ��������ں����Ƚ�
        float min_distancesq = std::numeric_limits<float>::max();

        //������һ������ε�ÿ�����㣬���������ڶ��������ÿ���ߵľ���  
        for (FlatVector point:vertices_a) {
        
            for (size_t j = 0; j < vertices_b.size(); j++){
                FlatVector v_a = vertices_b[j];//�ڶ�������εĵ�ǰ�ߵ����
                FlatVector v_b = vertices_b[(j + 1) % vertices_b.size()];//�ڶ�������εĵ�ǰ�ߵ��յ㣬ʹ��ģ���㴦��պ϶����  
                float distancesq = DisSqdPointSegmentNearestEndpoint(point,v_a,v_b);
                FlatVector cloesest_point = GetSegmentContactPoint(point, v_a, v_b);

                //�����ǰ�����ƽ������������С�����ƽ���������½Ӵ��㲻���ڵ�һ���Ӵ���  
                if (FlatVector::NearlyEqual(distancesq, min_distancesq)) {
                    if (!FlatVector::NearlyEqualFv(cloesest_point, contact1)) {
                        contact2 = cloesest_point;
                        contact_count = 2;
                    }
                }
                else if (distancesq < min_distancesq) {
                    min_distancesq = distancesq;
                    contact_count = 1;
                    contact1 = cloesest_point; 
                }
            }
        }
        // �Եڶ���������ظ��������̣��Դ���������ڶ���θ�����״����©�ĽӴ���
        for (FlatVector point : vertices_b) {

            for (size_t j = 0; j < vertices_a.size(); j++) {
                FlatVector v_a = vertices_a[j];//�ڶ�������εĵ�ǰ�ߵ����
                FlatVector v_b = vertices_a[(j + 1) % vertices_a.size()];//�ڶ�������εĵ�ǰ�ߵ��յ㣬ʹ��ģ���㴦��պ϶����  
                float distancesq = DisSqdPointSegmentNearestEndpoint(point, v_a, v_b);
                FlatVector cloesest_point = GetSegmentContactPoint(point, v_a, v_b);


                //�����ǰ�����ƽ������������С�����ƽ���������½Ӵ��㲻���ڵ�һ���Ӵ���
                if (FlatVector::NearlyEqual(distancesq, min_distancesq)) {
                    if (!FlatVector::NearlyEqualFv(cloesest_point, contact1)) {
                        contact2 = cloesest_point;
                        contact_count = 2;
                    }
                }
                else if (distancesq < min_distancesq) {
                    min_distancesq = distancesq;
                    contact_count = 1;
                    contact1 = cloesest_point;
                    
                }
            }
        }

        //�жϽӴ������
        if (contact_count == 1) {
            contact_points.push_back(contact1);
        }
        else {
            contact_points.push_back(contact1);
            contact_points.push_back(contact2);
        }
        
        
        return contact_points;
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
    static IntersectData IntersectCircles(const FlatVector& center_a, const float& radius_a, const FlatVector& center_b, const float& radius_b) {
        IntersectData data;
        float both_distance = FlatVector::Distance(center_a,center_b);
        float sum_of_radii = radius_a + radius_b;

        if (both_distance >= sum_of_radii) {
            return data;
        }

        //���㵥λ��������
        FlatVector unit_direction_vector_ab = center_b - center_a;
        unit_direction_vector_ab.normalize();

        //����data
        data.Collision = true;
        data.normal = unit_direction_vector_ab;
        data.depth = sum_of_radii - both_distance;

        return data;
    }

    //Բ�����ε���ײ��⣬�������㷨
    static IntersectData IntersectCirclePolygon(const FlatVector& circle_center,
                                                const float& circle_radius,
                                                const FlatVector& polygon_mass_center,
                                                const std::vector<FlatVector>& polygon_vertices) {
        IntersectData data;
        data.depth = std::numeric_limits<float>::max();

        //��������ε�ÿ����
        for (int i = 0; i < polygon_vertices.size(); i++) {
            FlatVector v_a = polygon_vertices[i];
            FlatVector v_b = polygon_vertices[int((i + 1) % polygon_vertices.size())];

            //����ߵķ�����
            FlatVector edge_vector = v_b - v_a;
            FlatVector axis = FlatVector(-(edge_vector.y), (edge_vector.x));
            axis.normalize();

            //�������κ�Բ�ڸ����ϵ�ͶӰ
            FlatVector project_polygon = ProjectVertices(axis, polygon_vertices);
            FlatVector project_circlr = ProjectCircle(circle_center,circle_radius,axis);

            //�ж�ͶӰ�Ƿ��ص���������ص�˵��û������ײ����False
            if (project_polygon.x >= project_circlr.y || project_circlr.x >= project_polygon.y) {
                return data;
            }

            //�����ײ������ײ
            //����ͶӰ���ص����
            float axis_depth = std::min(project_circlr.y - project_polygon.x, project_polygon.y - project_circlr.x);

            //������С�ڵ�ǰ��С��ȣ�����·����������
            if (axis_depth < data.depth) {
                data.depth = axis_depth;
                data.normal = axis;
            }
        }
        //�ȵ�����ξ���Բ������ĵ�
        size_t closest_point_on_polygon_index = FindVertexClosestPoint(circle_center, polygon_vertices);
        FlatVector closest_point_on_polygon = polygon_vertices[closest_point_on_polygon_index];

        //����õ㵽Բ�ĵķ�������
        FlatVector axis = closest_point_on_polygon - circle_center;
        axis.normalize();

        //�������κ�Բ�ڸ����ϵ�ͶӰ
        FlatVector project_polygon = ProjectVertices(axis, polygon_vertices);
        FlatVector project_circlr = ProjectCircle(circle_center, circle_radius, axis);

        //�ж�ͶӰ�Ƿ��ص���������ص�˵��û������ײ����False
        if (project_polygon.x >= project_circlr.y || project_circlr.x >= project_polygon.y) {
            return data;
        }

        //�����ײ������ײ
        //����ͶӰ���ص����
        float axis_depth = std::min(project_circlr.y - project_polygon.x, project_polygon.y - project_circlr.x);

        //������С�ڵ�ǰ��С��ȣ�����·����������
        if (axis_depth < data.depth) {
            data.depth = axis_depth;
            data.normal = axis;
        }

        //�������ĵ�Բ�ĵķ�������
        FlatVector direction_vector = polygon_mass_center - circle_center;
        
        //������������뷨�����ĵ��Ϊ������ȡ�������ķ�����
        if (FlatVector::dot(direction_vector, data.normal) < 0) {
            data.normal = -data.normal;
        }

        //������ײ���
        data.Collision = true;

        return data;
    }


    static IntersectData IntersectPolygons(const FlatVector& mass_center_a, const std::vector<FlatVector>& vertices_a,
        const FlatVector& mass_center_b, const std::vector<FlatVector>& vertices_b) {
        IntersectData data;
        data.depth = std::numeric_limits<float>::max();

        auto CheckAxis = [&](const FlatVector& axis, const std::vector<FlatVector>& vertices_a, const std::vector<FlatVector>& vertices_b) {
            // ���������ڸ����ϵ�ͶӰ
            FlatVector project_polygon_a = ProjectVertices(axis, vertices_a);
            FlatVector project_polygon_b = ProjectVertices(axis, vertices_b);

            // �ж�ͶӰ�Ƿ��ص�
            if (project_polygon_a.y < project_polygon_b.x || project_polygon_b.y < project_polygon_a.x) {
                
                return false;
            }

            // ����ͶӰ���ص����
            float axis_depth = std::min(project_polygon_b.y - project_polygon_a.x, project_polygon_a.y - project_polygon_b.x);     

            // ������С�ڵ�ǰ��С��ȣ�����·����������
            if (axis_depth < data.depth) {
                data.depth = axis_depth;
                data.normal = axis;
            }
            return true;
            };

        // ���������A��ÿ����
        for (int i = 0; i < vertices_a.size(); i++) {
            FlatVector v_a = vertices_a[i];
            FlatVector v_b = vertices_a[(i + 1) % vertices_a.size()];

            // ����ߵķ�����
            FlatVector edge_vector = v_b - v_a;
            FlatVector axis;

            //���ַ�������ָ��ͼ������
            if (edge_vector.x * edge_vector.y > 0) {
                axis = FlatVector(edge_vector.y, -edge_vector.x);
            }
            else if (edge_vector.x * edge_vector.y < 0) {
                axis = FlatVector(-edge_vector.y, edge_vector.x);
            }
            else {
                axis = FlatVector(edge_vector.y, -edge_vector.x);
            }
            axis.normalize();

            if (!CheckAxis(axis, vertices_a, vertices_b)) {
                
                return data; // û���ص�������
            }
        }

        // ���������B��ÿ����
        for (int i = 0; i < vertices_b.size(); i++) {
            FlatVector v_a = vertices_b[i];
            FlatVector v_b = vertices_b[(i + 1) % vertices_b.size()];

            // ����ߵķ�����
            FlatVector edge_vector = v_b - v_a;
            FlatVector axis;

            //���ַ�������ָ��ͼ������
            if (edge_vector.x * edge_vector.y >0) {
                 axis = FlatVector(edge_vector.y, -edge_vector.x);
            }
            else if (edge_vector.x * edge_vector.y < 0) {
                 axis = FlatVector(-edge_vector.y, edge_vector.x);
            }
            else {
                axis = FlatVector(edge_vector.y, -edge_vector.x);
            }
            
            axis.normalize();

            if (!CheckAxis(axis, vertices_b, vertices_a)) {
               
                return data; // û���ص�������
            }
        }

        // ����������϶����ص���������ײ���
        data.Collision = true;

        // �������ĵ�Բ�ĵķ�������
        FlatVector direction_vector = mass_center_b - mass_center_a;

        // ������������뷨�����ĵ��Ϊ������ȡ�������ķ�����
        if (FlatVector::dot(direction_vector, data.normal) < 0) {
            data.normal = -data.normal;
        }
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
            contact_points  = FindContactPoint::FindPolygonsContactPoints(body_a.vertices_, body_b.vertices_);
            return contact_points;
        }
        else if (shape_type_b == CIRCLE) {
            contact_points.push_back(FindContactPoint::FindCirclePolygonContactPoint(body_b.mass_center_, body_a.vertices_));
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
            intersect_data = Intersect::IntersectPolygons(body_a.mass_center_, body_a.vertices_, body_b.mass_center_, body_b.vertices_);
            return intersect_data;
        }
        else if (shape_type_b == CIRCLE) {
            intersect_data = Intersect::IntersectCirclePolygon(body_b.mass_center_,body_b.radius_,body_a.mass_center_,body_a.vertices_);
            return intersect_data;
        }
    }
    else if (shape_type_a == CIRCLE) {
        if (shape_type_b == CIRCLE) {
            intersect_data = Intersect::IntersectCircles(body_a.mass_center_, body_a.radius_, body_b.mass_center_, body_b.radius_);
            return intersect_data;
        }
        else if (shape_type_b == POLTGON) {
            intersect_data = Intersect::IntersectCirclePolygon(body_a.mass_center_, body_a.radius_, body_b.mass_center_, body_b.vertices_);
            return intersect_data;
        }
    }

    return intersect_data;
}

