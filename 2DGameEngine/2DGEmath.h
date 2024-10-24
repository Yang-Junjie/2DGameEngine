#pragma once
#include <utility> // For std::pair
#include <cmath>   // For std::cos and std::sin
#include <iostream>
#include <SDL_stdinc.h>



struct FlatTransform
{
    float cos;          // Cosine of the rotation angle
    float sin;          // Sine of the rotation angle
    float positionX;    // Translation along the X-axis
    float positionY;    // Translation along the Y-axis
};

struct FlatVector
{
    float x;
    float y;
    FlatVector() :x(0.0f), y(0.0f) {};
    FlatVector(float x, float y) : x(x), y(y) {}

    friend std::ostream& operator<<(std::ostream& os, const FlatVector& p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os; // 返回输出流以便链式调用  
    }
    // 向量相加
    FlatVector operator+(const FlatVector& other) const
    {
        return { this->x + other.x, this->y + other.y };
    }

    // 向量相减
    FlatVector operator-(const FlatVector& other) const
    {
        return { this->x - other.x, this->y - other.y };
    }

    // 向量取负数
    FlatVector operator-() const
    {
        return { -this->x, -this->y };
    }

    // 向量的数乘
    FlatVector operator*(float scalar) const
    {
        return { this->x * scalar, this->y * scalar };
    }

    // 向量的数乘
    friend FlatVector operator*(float scalar, const FlatVector& vec)
    {
        return vec * scalar;
    }

    // 向量的数除
    FlatVector operator/(float scalar) const
    {
        return { this->x / scalar, this->y / scalar };
    }

    // 向量旋转
    static void Transform(float &x,float &y,const float angle)
    {
        float cos_angle = static_cast<float>(std::cos(angle * (M_PI / 180.0)));
        float sin_angle = static_cast<float>(std::sin(angle * (M_PI / 180.0)));
        float temp_x = cos_angle * x - sin_angle * y;
        y = sin_angle * x + cos_angle * y;
        x = temp_x;
    }

    // 判断两个向量是否相等
    bool operator==(const FlatVector& other) const
    {
        return this->x == other.x && this->y == other.y;
    }

    // 判断两个向量是否不相等
    bool operator!=(const FlatVector& other) const
    {
        return !(*this == other);
    }

    // 判断两个向量是否相等
    bool equals(const FlatVector& other) const
    {
        return *this == other;
    }

    //向量取模
    float len() const {
        return sqrt(this->x * this->x + this->y * this->y);
    }

    //向量取模
    static float len(FlatVector& v)  {
        return sqrt(v.x * v.x + v.y * v.y);
    }

    //向量归一化
    void normalize() {
        float length = sqrt(this->x * this->x + this->y * this->y);
        this->x = this->x / length;
        this->y = this->y / length;
    }

    //向量归一化
    static FlatVector normalize(FlatVector v) {
        float length = FlatVector::len(v);
        if (length != 0.0f) {
            v.x = v.x / length;
            v.y = v.y / length;
            return v;
        }
        else {
            return FlatVector(0,0);
        }
    }
    
    //向量的点积
    float dot(const FlatVector& v) const {
        return this->x * v.x + this->y * v.y;
    }

    //向量的点积
    static float dot(const FlatVector& v1, const FlatVector& v2) {
        return v1.x * v2.x + v1.y * v2.y;
    }

    // 由于我们是二维所以叉乘后的向量我们无法的到方向，所以我们返回叉乘后的模
    float cross(const FlatVector& v) const {
        return this->x * v.y - this->y * v.x;
    }

    //向量的模的平方
    float LengthSquared() const {
        return this->x * this->x + this->y * this->y;
    }

    //向量的模的平方
    static float LengthSquared(const FlatVector& v) {
        return v.x*v.x+v.y*v.y;
    }

    //两个向量端点平移到一起后终点组成的向量的模长
    static float Distance(const FlatVector& v1, const FlatVector& v2){
        float dx = v1.x - v2.x;
        float dy = v1.y - v2.y;
        return sqrt(dx*dx + dy*dy);
    }
    //两个向量端点平移到一起后终点组成的向量的模长的平方
    static float DistanceSquared(const FlatVector& v1, const FlatVector& v2) {
        return (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y);
    }
};