#pragma once
#include <utility> // For std::pair
#include <cmath>   // For std::cos and std::sin
#include <iostream>
#include <SDL_stdinc.h>

const float VerySmallAmount = 0.00005f;

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

    FlatVector& operator+=(const FlatVector& other) {
        this->x += other.x; // 将另一个对象的值加到当前对象上
        this->y += other.y;
        return *this; // 返回当前对象的引用
    }

    FlatVector& operator-=(const FlatVector& other) {
        this->x -= other.x; // 将另一个对象的值加到当前对象上
        this->y -= other.y;
        return *this; // 返回当前对象的引用
    }
    FlatVector& operator*=(const FlatVector& other) {
        this->x *= other.x; // 将另一个对象的值加到当前对象上
        this->y *= other.y;
        return *this; // 返回当前对象的引用
    }

    FlatVector& operator/=(const FlatVector& other) {
        this->x /= other.x; // 将另一个对象的值加到当前对象上
        this->y /= other.y;
        return *this; // 返回当前对象的引用
    }

    // 向量旋转
    static void Transform(float &x,float &y,const float radian)
    {
        float cos_angle = static_cast<float>(std::cos(radian ));
        float sin_angle = static_cast<float>(std::sin(radian ));
        float temp_x = cos_angle * x - sin_angle * y;
        y = sin_angle * x + cos_angle * y;
        x = temp_x;
    }
    static float AngleToRadian(float angle) {
        return (float)angle * M_PI / 180.0;
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
        if (length > VerySmallAmount) {
            this->x /= length;
            this->y /= length;
        }
        else {
            this->x = 0.0f;
            this->y = 0.0f;
        }
    }

    //向量归一化
    static FlatVector normalize(FlatVector v) {
        float length = FlatVector::len(v);
        if (length > VerySmallAmount) {
            v.x /= length;
            v.y /= length;
        }
        else {
            v.x = 0.0f;
            v.y = 0.0f;
        }
        return v;
    }
    
    //向量的点积
    float dot(const FlatVector& v) const {
        return this->x * v.x + this->y * v.y;
    }

    //向量的点积
    static float dot(const FlatVector& v1, const FlatVector& v2) {
        return v1.x * v2.x + v1.y * v2.y;
    }

    //向量的叉积
    float cross(const FlatVector& v) const {
        return this->x * v.y - this->y * v.x;
    }
    
    static float cross(const FlatVector& v1, const FlatVector& v2) {
        return v1.x * v2.y - v1.y * v2.x;
    }

    //向量的模的平方
    float LengthSquared() const {
        return this->x * this->x + this->y * this->y;
    }

    //向量的模的平方
    static float LengthSquared(const FlatVector& v) {
        return v.x*v.x+v.y*v.y;
    }

    //两个向量的距离
    static float Distance(const FlatVector v1, const FlatVector v2) {
        float dx = v1.x - v2.x;
        float dy = v1.y - v2.y;
        return sqrt(dx * dx + dy * dy);
    }

    //两个向量距离的平方
    static float DistanceSquared(const FlatVector v1, const FlatVector v2) {
        return (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y);
    }

    //非常小判断
    static bool NearlyEqualFv(const FlatVector a, const FlatVector b) {
        return FlatVector::DistanceSquared(a, b) < VerySmallAmount * VerySmallAmount;
    }

    //非常小判断
    static bool NearlyEqual(const float a, const float b) {
        return std::abs(a - b) < VerySmallAmount * VerySmallAmount;
    }
};