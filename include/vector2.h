#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>

#include "SDL.h"

using namespace std;

class Vector2 {
    public:
        float x, y;

        Vector2();
        Vector2(float x, float y);

        bool operator==(const Vector2 &other) const;
        bool operator!=(const Vector2 &other) const;

        Vector2 operator+() const;
        Vector2 operator-() const;

        Vector2 operator+(const Vector2 &other) const;
        Vector2 operator-(const Vector2 &other) const;
        Vector2 operator*(const float &scalar) const;
        Vector2 operator/(const float &scalar) const;

        Vector2 &operator+=(const Vector2 &other);
        Vector2 &operator-=(const Vector2 &other);
        Vector2 &operator*=(const float &scalar);
        Vector2 &operator/=(const float &scalar);

        float DotProduct(const Vector2 &other) const;
        float Norm() const;
        float Angle() const;

        Vector2 ScaleToLength(const float length) const;
        Vector2 Normalize() const;

        Vector2 RotateToAngle(const float angle) const;

        float DistanceTo(const Vector2 point) const;
        Vector2 MoveTowards(const Vector2 point, const float length) const;
        Vector2 MoveTowards(const float angle, const float length) const;

        std::string toString() const;
};