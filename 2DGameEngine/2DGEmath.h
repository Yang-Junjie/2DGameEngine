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

typedef struct FlatVector
{
    float x;
    float y;
    FlatVector() :x(0.0f), y(0.0f) {};
    FlatVector(float x, float y) : x(x), y(y) {}

    friend std::ostream& operator<<(std::ostream& os, const FlatVector& p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os; // 返回输出流以便链式调用  
    }
    // Addition
    FlatVector operator+(const FlatVector& other) const
    {
        return { this->x + other.x, this->y + other.y };
    }

    // Subtraction
    FlatVector operator-(const FlatVector& other) const
    {
        return { this->x - other.x, this->y - other.y };
    }

    // Unary Negation
    FlatVector operator-() const
    {
        return { -this->x, -this->y };
    }

    // Multiplication with scalar
    FlatVector operator*(float scalar) const
    {
        return { this->x * scalar, this->y * scalar };
    }

    // Multiplication with scalar (commutative version)
    friend FlatVector operator*(float scalar, const FlatVector& vec)
    {
        return vec * scalar;
    }

    // Division with scalar
    FlatVector operator/(float scalar) const
    {
        return { this->x / scalar, this->y / scalar };
    }

    // Transformation operation
    static void Transform(float &x,float &y,const float angle)
    {
        float cos_angle = std::cos(angle * (M_PI / 180.0));
        float sin_angle = std::sin(angle * (M_PI / 180.0));
        float temp_x = cos_angle * x - sin_angle * y;
        y = sin_angle * x + cos_angle * y;
        x = temp_x;
    }

    // Equality check
    bool operator==(const FlatVector& other) const
    {
        return this->x == other.x && this->y == other.y;
    }

    // Inequality check
    bool operator!=(const FlatVector& other) const
    {
        return !(*this == other);
    }

    // Equality with object
    bool equals(const FlatVector& other) const
    {
        return *this == other;
    }

    float len() const {
        return sqrt(this->x * this->x + this->y * this->y);
    }

    void normalize() {
        float length = this->len();
        this->x = this->x / length;
        this->y = this->y / length;
    }

};