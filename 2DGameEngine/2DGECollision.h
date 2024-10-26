#pragma once

#include <vector>
#include "SDL.h"
#include "2DGEBody.h"


//以下函数都使用const 引用减少copy内存开销

/*检测数据
Collision：是否碰撞（相交）
depth:碰撞深度
normal：分离向量
*/
struct IntersectData {
    bool Collision = false;
    float depth = std::numeric_limits<float>::max();
    FlatVector normal;
};

//计算多边形在轴上的投影
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

//计算圆在某个轴上的投影
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

//找到多边形距离点最近的顶点
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




//如果点在AABB外将会给出点到AABB的距离的平方
//如果点在AABB上或者在AABB内将会给出0
static float DistSqdPointAABB(const FlatVector& p, const std::vector<FlatVector>& vertices_aabb) {
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
        if (v >= min && v <= max) return 0.0f;//如果在AABB上内直接返回0；
        if (v < min) sqDist += (min - v) * (min - v);
        if (v > max) sqDist += (v - max) * (v - max);
    }
    return sqDist; // 计算平方根  
}


//计算点与直线的接触点
//其原理是计算点距离线段哪个端点近那么那个端点极有可能是接触点
static FlatVector GetSegmentContactPoint(const FlatVector& point, const FlatVector& segment_start, const FlatVector& segment_end) {
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

//计算点与直线的最近点（可能的接触点）返回最近点(可能的接触点)
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

//计算点距离线段最近的端点的距离的平方
static float DisSqdPointSegmentNearestEndpoint(const FlatVector& point, const FlatVector& segment_start, const FlatVector& segment_end) {
    FlatVector segment_vector = segment_end - segment_start;//计算线段向量
    FlatVector point_to_segment_start_vector = point - segment_start;//计算从线段起点指向点的向量

    float projection_length = FlatVector::dot(point_to_segment_start_vector, segment_vector);
    float segment_length_squared = FlatVector::LengthSquared(segment_vector);
    float projection_parameter = projection_length / segment_length_squared;//计算投影参数 proj_param = (|AP||AB| * cos(θ)) / |AB|^2

    FlatVector closest_point = SegmentNearestEndpoint(projection_parameter,  segment_start, segment_end);
    float distance_squared = FlatVector::DistanceSquared(point, closest_point);

    return distance_squared;
}


//此命名空间包含了寻找物体与物体的接触点的函数
//构建此命名空间是为了代码模块化，使代码结构更清晰
namespace FindContactPoint {

    /*找到圆与圆碰撞时的接触点*/
    static FlatVector FindCirclesContactPoint(const FlatVector& center_a, const float& radius_a, const FlatVector& center_b) {
        FlatVector v_ab = center_b - center_a;
        v_ab.normalize();
        FlatVector contact_point = center_a + v_ab * radius_a;
        return contact_point;
    }


    /*找到圆与多边形碰撞时的接触点*/
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

    //找到多边形与多边形的接触点
    static std::vector<FlatVector> FindPolygonsContactPoints(const std::vector<FlatVector>& vertices_a, const std::vector<FlatVector>& vertices_b) {
        //初始化接触点
        std::vector<FlatVector> contact_points;
        FlatVector contact1;//第一个接触点
        FlatVector contact2;//第二个接触点，如果没有第二个接触点，则保持为原点 
        size_t contact_count = 0;
        
        //初始化最小距离的平方为无穷大，用于后续比较
        float min_distancesq = std::numeric_limits<float>::max();

        //遍历第一个多边形的每个顶点，并检查它与第二个多边形每条边的距离  
        for (FlatVector point:vertices_a) {
        
            for (size_t j = 0; j < vertices_b.size(); j++){
                FlatVector v_a = vertices_b[j];//第二个多边形的当前边的起点
                FlatVector v_b = vertices_b[(j + 1) % vertices_b.size()];//第二个多边形的当前边的终点，使用模运算处理闭合多边形  
                float distancesq = DisSqdPointSegmentNearestEndpoint(point,v_a,v_b);
                FlatVector cloesest_point = GetSegmentContactPoint(point, v_a, v_b);

                //如果当前距离的平方几乎等于最小距离的平方，并且新接触点不等于第一个接触点  
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
        // 对第二个多边形重复上述过程，以处理可能由于多边形复杂形状而遗漏的接触点
        for (FlatVector point : vertices_b) {

            for (size_t j = 0; j < vertices_a.size(); j++) {
                FlatVector v_a = vertices_a[j];//第二个多边形的当前边的起点
                FlatVector v_b = vertices_a[(j + 1) % vertices_a.size()];//第二个多边形的当前边的终点，使用模运算处理闭合多边形  
                float distancesq = DisSqdPointSegmentNearestEndpoint(point, v_a, v_b);
                FlatVector cloesest_point = GetSegmentContactPoint(point, v_a, v_b);


                //如果当前距离的平方几乎等于最小距离的平方，并且新接触点不等于第一个接触点
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

        //判断接触点个数
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


//此命名空间包含了检测物体与物体的是否相交的函数
//构建此命名空间是为了代码模块化，使代码结构更清晰
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
    static IntersectData IntersectCircles(const FlatVector& center_a, const float& radius_a, const FlatVector& center_b, const float& radius_b) {
        IntersectData data;
        float both_distance = FlatVector::Distance(center_a,center_b);
        float sum_of_radii = radius_a + radius_b;

        if (both_distance >= sum_of_radii) {
            return data;
        }

        //计算单位方向向量
        FlatVector unit_direction_vector_ab = center_b - center_a;
        unit_direction_vector_ab.normalize();

        //更新data
        data.Collision = true;
        data.normal = unit_direction_vector_ab;
        data.depth = sum_of_radii - both_distance;

        return data;
    }

    //圆与多边形的碰撞检测，分离轴算法
    static IntersectData IntersectCirclePolygon(const FlatVector& circle_center,
                                                const float& circle_radius,
                                                const FlatVector& polygon_mass_center,
                                                const std::vector<FlatVector>& polygon_vertices) {
        IntersectData data;
        data.depth = std::numeric_limits<float>::max();

        //遍历多边形的每条边
        for (int i = 0; i < polygon_vertices.size(); i++) {
            FlatVector v_a = polygon_vertices[i];
            FlatVector v_b = polygon_vertices[int((i + 1) % polygon_vertices.size())];

            //计算边的法向量
            FlatVector edge_vector = v_b - v_a;
            FlatVector axis = FlatVector(-(edge_vector.y), (edge_vector.x));
            axis.normalize();

            //计算多边形和圆在该轴上的投影
            FlatVector project_polygon = ProjectVertices(axis, polygon_vertices);
            FlatVector project_circlr = ProjectCircle(circle_center,circle_radius,axis);

            //判断投影是否重叠，如果不重叠说明没发生碰撞返回False
            if (project_polygon.x >= project_circlr.y || project_circlr.x >= project_polygon.y) {
                return data;
            }

            //如果碰撞处理碰撞
            //计算投影的重叠深度
            float axis_depth = std::min(project_circlr.y - project_polygon.x, project_polygon.y - project_circlr.x);

            //如果深度小于当前最小深度，则更新法向量和深度
            if (axis_depth < data.depth) {
                data.depth = axis_depth;
                data.normal = axis;
            }
        }
        //先到多边形距离圆心最近的点
        size_t closest_point_on_polygon_index = FindVertexClosestPoint(circle_center, polygon_vertices);
        FlatVector closest_point_on_polygon = polygon_vertices[closest_point_on_polygon_index];

        //计算该点到圆心的方向向量
        FlatVector axis = closest_point_on_polygon - circle_center;
        axis.normalize();

        //计算多边形和圆在该轴上的投影
        FlatVector project_polygon = ProjectVertices(axis, polygon_vertices);
        FlatVector project_circlr = ProjectCircle(circle_center, circle_radius, axis);

        //判断投影是否重叠，如果不重叠说明没发生碰撞返回False
        if (project_polygon.x >= project_circlr.y || project_circlr.x >= project_polygon.y) {
            return data;
        }

        //如果碰撞处理碰撞
        //计算投影的重叠深度
        float axis_depth = std::min(project_circlr.y - project_polygon.x, project_polygon.y - project_circlr.x);

        //如果深度小于当前最小深度，则更新法向量和深度
        if (axis_depth < data.depth) {
            data.depth = axis_depth;
            data.normal = axis;
        }

        //计算重心到圆心的方向向量
        FlatVector direction_vector = polygon_mass_center - circle_center;
        
        //如果方向向量与法向量的点积为负，则取法向量的反方向
        if (FlatVector::dot(direction_vector, data.normal) < 0) {
            data.normal = -data.normal;
        }

        //更新碰撞结果
        data.Collision = true;

        return data;
    }


    static IntersectData IntersectPolygons(const FlatVector& mass_center_a, const std::vector<FlatVector>& vertices_a,
        const FlatVector& mass_center_b, const std::vector<FlatVector>& vertices_b) {
        IntersectData data;
        data.depth = std::numeric_limits<float>::max();

        auto CheckAxis = [&](const FlatVector& axis, const std::vector<FlatVector>& vertices_a, const std::vector<FlatVector>& vertices_b) {
            // 计算多边形在该轴上的投影
            FlatVector project_polygon_a = ProjectVertices(axis, vertices_a);
            FlatVector project_polygon_b = ProjectVertices(axis, vertices_b);

            // 判断投影是否重叠
            if (project_polygon_a.y < project_polygon_b.x || project_polygon_b.y < project_polygon_a.x) {
                
                return false;
            }

            // 计算投影的重叠深度
            float axis_depth = std::min(project_polygon_b.y - project_polygon_a.x, project_polygon_a.y - project_polygon_b.x);     

            // 如果深度小于当前最小深度，则更新法向量和深度
            if (axis_depth < data.depth) {
                data.depth = axis_depth;
                data.normal = axis;
            }
            return true;
            };

        // 遍历多边形A的每条边
        for (int i = 0; i < vertices_a.size(); i++) {
            FlatVector v_a = vertices_a[i];
            FlatVector v_b = vertices_a[(i + 1) % vertices_a.size()];

            // 计算边的法向量
            FlatVector edge_vector = v_b - v_a;
            FlatVector axis;

            //保持法向量都指向图形外面
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
                
                return data; // 没有重叠，返回
            }
        }

        // 遍历多边形B的每条边
        for (int i = 0; i < vertices_b.size(); i++) {
            FlatVector v_a = vertices_b[i];
            FlatVector v_b = vertices_b[(i + 1) % vertices_b.size()];

            // 计算边的法向量
            FlatVector edge_vector = v_b - v_a;
            FlatVector axis;

            //保持法向量都指向图形外面
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
               
                return data; // 没有重叠，返回
            }
        }

        // 如果所有轴上都有重叠，设置碰撞结果
        data.Collision = true;

        // 计算重心到圆心的方向向量
        FlatVector direction_vector = mass_center_b - mass_center_a;

        // 如果方向向量与法向量的点积为负，则取法向量的反方向
        if (FlatVector::dot(direction_vector, data.normal) < 0) {
            data.normal = -data.normal;
        }
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


//判断两个物体是否碰撞，返回物体的碰撞bool值，碰撞深度,碰撞向量
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

