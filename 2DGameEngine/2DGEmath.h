#pragma once
#include <utility> // For std::pair
#include <cmath>   // For std::cos and std::sin

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

    FlatVector(float x, float y) : x(x), y(y) {}

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
    static FlatVector Transform(const FlatVector& v, const FlatTransform& transform)
    {
        return { transform.cos * v.x - transform.sin * v.y + transform.positionX,
                transform.sin * v.x + transform.cos * v.y + transform.positionY };
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

};