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

    // ������ת
    static void Transform(float &x,float &y,const float angle)
    {
        float cos_angle = static_cast<float>(std::cos(angle * (M_PI / 180.0)));
        float sin_angle = static_cast<float>(std::sin(angle * (M_PI / 180.0)));
        float temp_x = cos_angle * x - sin_angle * y;
        y = sin_angle * x + cos_angle * y;
        x = temp_x;
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
        this->x = this->x / length;
        this->y = this->y / length;
    }

    //������һ��
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
    
    //�����ĵ��
    float dot(const FlatVector& v) const {
        return this->x * v.x + this->y * v.y;
    }

    //�����ĵ��
    static float dot(const FlatVector& v1, const FlatVector& v2) {
        return v1.x * v2.x + v1.y * v2.y;
    }

    // ���������Ƕ�ά���Բ�˺�����������޷��ĵ������������Ƿ��ز�˺��ģ
    float cross(const FlatVector& v) const {
        return this->x * v.y - this->y * v.x;
    }

    //������ģ��ƽ��
    float LengthSquared() const {
        return this->x * this->x + this->y * this->y;
    }

    //������ģ��ƽ��
    static float LengthSquared(const FlatVector& v) {
        return v.x*v.x+v.y*v.y;
    }

    //���������˵�ƽ�Ƶ�һ����յ���ɵ�������ģ��
    static float Distance(const FlatVector& v1, const FlatVector& v2){
        float dx = v1.x - v2.x;
        float dy = v1.y - v2.y;
        return sqrt(dx*dx + dy*dy);
    }
    //���������˵�ƽ�Ƶ�һ����յ���ɵ�������ģ����ƽ��
    static float DistanceSquared(const FlatVector& v1, const FlatVector& v2) {
        return (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y);
    }
};