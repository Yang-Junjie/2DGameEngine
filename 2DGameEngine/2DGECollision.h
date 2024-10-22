#pragma once

#include <vector>
#include "SDL.h"
#include "2DGEBody.h"


/*检测数据
Collision：是否碰撞（相交）
depth:碰撞深度
normal：分离向量
*/
struct IntersectData {
    bool Collision = false;
    float depth = 0.0f;
    FlatVector normal;
};

/*此函数将会给出点到AABB的距离的平方*/
static float DistSqrtPointAABB(FlatVector& p, std::vector<FlatVector>& vertices_aabb) {
    float sqDist = 0.0f;
    for (int i = 0; i < 2; i++) {
        float v, min, max;
        if (i == 0)
        {
            v = p.x;
            min = vertices_aabb[3].x;
            max = vertices_aabb[1].x;
            /*当i为0时对应x
            v设置为点 p 的 x 坐标。
            min 设置为边界框左上角点的 x 坐标（vertices_aabb[3].x）。
            max 设置为边界框右下角点的 x 坐标（vertices_aabb[1].x）。*/
        }
        if (i == 1)
        {
            v = p.y;
            min = vertices_aabb[3].y;
            max = vertices_aabb[1].y;
            /*当i为1时对应y
            v设置为点 p 的 y 坐标。
            min 设置为边界框左上角点的 y 坐标（vertices_aabb[3].y）。
            max 设置为边界框右下角点的 y 坐标（vertices_aabb[1].y）。y*/
        }
        if (v >= min && v <= max) return 0.0f;//如果在AABB上直接返回0；
        if (v < min) sqDist += (min - v) * (min - v);
        if (v > max) sqDist += (v - max) * (v - max);
    }
    return sqDist; // 计算平方根  
}


//计算接触点
static FlatVector GetSegmentPoint(FlatVector& point, FlatVector& segment_start, FlatVector& segment_end) {
    FlatVector segment_vector = segment_end - segment_start;//计算线段向量
    FlatVector point_to_segment_start_vector = point - segment_start;//计算从线段起点指向点的向量

    float projection_length = FlatVector::dot(point_to_segment_start_vector, segment_vector);
    float segment_length_squared = FlatVector::LengthSquared(segment_vector);
    float projection_parameter = projection_length / segment_length_squared;//计算投影参数 proj_param = (|AP||AB| * cos(θ)) / |AB|^2
    
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

// The distance from a point to the nearest end of a line segment
static float DisSqdPointSegmentNearestEndpoint(FlatVector& point, FlatVector& segment_start, FlatVector& segment_end) {
    FlatVector segment_vector = segment_end - segment_start;//计算线段向量
    FlatVector point_to_segment_start_vector = point - segment_start;//计算从线段起点指向点的向量

    float projection_length = FlatVector::dot(point_to_segment_start_vector, segment_vector);
    float segment_length_squared = FlatVector::LengthSquared(segment_vector);
    float projection_parameter = projection_length / segment_length_squared;//计算投影参数 proj_param = (|AP||AB| * cos(θ)) / |AB|^2

    FlatVector closest_point = SegmentNearestEndpoint(projection_parameter,  segment_start, segment_end);
    float distance_squared = FlatVector::DistanceSquared(point, closest_point);

    return distance_squared;
}

namespace FindContactPoint {

    /*找到圆与圆碰撞时的接触点*/
    static FlatVector FindCirclesContactPoint(FlatVector& center_a, float radius_a, FlatVector& center_b) {
        FlatVector v_ab = center_b - center_a;
       
        v_ab.normalize();

        FlatVector contact_point = center_a + v_ab * radius_a;
       
        return contact_point;
    }
    /*找到圆与多边形碰撞时的接触点*/
    static FlatVector FindCirclePolygonContactPoint(FlatVector& circle_center, std::vector<FlatVector>& polygon_vertices) {
        float min_distance_Sq = std::numeric_limits<float>::max();
        FlatVector contact_point;
        for (int i = 0; i < static_cast<int>(polygon_vertices.size()); i++) {
            FlatVector v_a = polygon_vertices[i];
            FlatVector v_b = polygon_vertices[(i + 1) % polygon_vertices.size()];
            float distance_sq = DisSqdPointSegmentNearestEndpoint(circle_center,v_a,v_b);
            if (distance_sq < min_distance_Sq) {
                min_distance_Sq = distance_sq;
                contact_point = GetSegmentPoint(circle_center, v_a, v_b);
            }
        }
        return contact_point;
    }
};



//包含检测碰撞的函数
namespace Intersect {

    /*此函数将会判断2个物体的AABB是否相交*/
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


    /*圆与圆碰撞检测*/
    static IntersectData IntersectCircles(FlatVector& center_a, float radius_a, FlatVector& center_b, float radius_b) {
        IntersectData data;
        float both_distance = FlatVector::Distance(center_a,center_b);
        float sum_of_radii = radius_a + radius_b;

        if (both_distance >= sum_of_radii) {
          //  std::cout << 1;
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


//找到两个碰撞物体的接触点
static std::vector<FlatVector> FindContactPoints(Body& body_a,Body& body_b) {
    std::vector<FlatVector> contact_points;
    Shape shape_type_a = body_a.shape_;
    Shape shape_type_b = body_b.shape_;

    if (shape_type_a == POLTGON) {
        if (shape_type_b == POLTGON) {
            return contact_points;
        }
        else if (shape_type_b == CIRCLE) {
            return contact_points;
        }
    }
    else if (shape_type_a == CIRCLE) {
        if (shape_type_b == CIRCLE) {
            contact_points.push_back(FindContactPoint::FindCirclesContactPoint(body_a.mass_center_, body_a.radius_, body_b.mass_center_));
          //  std::cout << contact_points[0] << std::endl;
            return contact_points;
        }
        else if (shape_type_b == POLTGON) {
            return contact_points;
        }
    }
}


//判断两个物体是否碰撞，返回物体的碰撞bool值，碰撞深度,碰撞向量
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

