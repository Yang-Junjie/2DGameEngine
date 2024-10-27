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
        return os; // ����������Ա���ʽ����  
    }
    // �������
    FlatVector operator+(const FlatVector& other) const
    {
        return { this->x + other.x, this->y + other.y };
    }

    // �������
    FlatVector operator-(const FlatVector& other) const
    {
        return { this->x - other.x, this->y - other.y };
    }

    // ����ȡ����
    FlatVector operator-() const
    {
        return { -this->x, -this->y };
    }

    // ����������
    FlatVector operator*(float scalar) const
    {
        return { this->x * scalar, this->y * scalar };
    }

    // ����������
    friend FlatVector operator*(float scalar, const FlatVector& vec)
    {
        return vec * scalar;
    }

    // ����������
    FlatVector operator/(float scalar) const
    {
        return { this->x / scalar, this->y / scalar };
    }

    FlatVector& operator+=(const FlatVector& other) {
        this->x += other.x; // ����һ�������ֵ�ӵ���ǰ������
        this->y += other.y;
        return *this; // ���ص�ǰ���������
    }

    FlatVector& operator-=(const FlatVector& other) {
        this->x -= other.x; // ����һ�������ֵ�ӵ���ǰ������
        this->y -= other.y;
        return *this; // ���ص�ǰ���������
    }
    FlatVector& operator*=(const FlatVector& other) {
        this->x *= other.x; // ����һ�������ֵ�ӵ���ǰ������
        this->y *= other.y;
        return *this; // ���ص�ǰ���������
    }

    FlatVector& operator/=(const FlatVector& other) {
        this->x /= other.x; // ����һ�������ֵ�ӵ���ǰ������
        this->y /= other.y;
        return *this; // ���ص�ǰ���������
    }

    // ������ת
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
    // �ж����������Ƿ����
    bool operator==(const FlatVector& other) const
    {
        return this->x == other.x && this->y == other.y;
    }

    // �ж����������Ƿ����
    bool operator!=(const FlatVector& other) const
    {
        return !(*this == other);
    }

    // �ж����������Ƿ����
    bool equals(const FlatVector& other) const
    {
        return *this == other;
    }

    //����ȡģ
    float len() const {
        return sqrt(this->x * this->x + this->y * this->y);
    }

    //����ȡģ
    static float len(FlatVector& v)  {
        return sqrt(v.x * v.x + v.y * v.y);
    }

    //������һ��
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

    //������һ��
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
    
    //�����ĵ��
    float dot(const FlatVector& v) const {
        return this->x * v.x + this->y * v.y;
    }

    //�����ĵ��
    static float dot(const FlatVector& v1, const FlatVector& v2) {
        return v1.x * v2.x + v1.y * v2.y;
    }

    //�����Ĳ��
    float cross(const FlatVector& v) const {
        return this->x * v.y - this->y * v.x;
    }
    
    static float cross(const FlatVector& v1, const FlatVector& v2) {
        return v1.x * v2.y - v1.y * v2.x;
    }

    //������ģ��ƽ��
    float LengthSquared() const {
        return this->x * this->x + this->y * this->y;
    }

    //������ģ��ƽ��
    static float LengthSquared(const FlatVector& v) {
        return v.x*v.x+v.y*v.y;
    }

    //���������ľ���
    static float Distance(const FlatVector v1, const FlatVector v2) {
        float dx = v1.x - v2.x;
        float dy = v1.y - v2.y;
        return sqrt(dx * dx + dy * dy);
    }

    //�������������ƽ��
    static float DistanceSquared(const FlatVector v1, const FlatVector v2) {
        return (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y);
    }

    //�ǳ�С�ж�
    static bool NearlyEqualFv(const FlatVector a, const FlatVector b) {
        return FlatVector::DistanceSquared(a, b) < VerySmallAmount * VerySmallAmount;
    }

    //�ǳ�С�ж�
    static bool NearlyEqual(const float a, const float b) {
        return std::abs(a - b) < VerySmallAmount * VerySmallAmount;
    }
};