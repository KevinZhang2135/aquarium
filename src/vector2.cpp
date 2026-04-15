#include "vector2.h"

Vector2::Vector2() : x{0}, y{0} {}
Vector2::Vector2(float x, float y) : x{x}, y{y} {}

/// @brief Compares whether the components of two vectors are equal
/// @param other The other vector to be compared
/// @return True if the vectors are equal; false otherwise
bool Vector2::operator==(const Vector2 &other) const { return (x == other.x && y == other.y); }

/// @brief Compares whether the components of two vectors are not equal
/// @param other The other vector to be compared
/// @return True if the vectors are not equal; false otherwise
bool Vector2::operator!=(const Vector2 &other) const { return !operator==(other); }

/// @brief Unary plus operator that returns the identity of the vector
/// @return A copy of the vector
Vector2 Vector2::operator+() const { return Vector2(x, y); }

/// @brief Unary minus negates the vector by rotating π radians around the
/// origin
/// @return A negated vector
Vector2 Vector2::operator-() const { return Vector2(-x, -y); }

/// @brief Binary addition that performs vector addition
/// @param other The vector to be added
/// @return The sum of the vectors
Vector2 Vector2::operator+(const Vector2 &other) const { return Vector2(x + other.x, y + other.y); }

/// @brief Binary subtraction that performs vector subtraction
/// @param other The vector that this is subtracted by
/// @return The difference of the vectors
Vector2 Vector2::operator-(const Vector2 &other) const { return Vector2(x - other.x, y - other.y); }

/// @brief Binary multiplication that performs vector scaling
/// @param scalar The scalar to be applied to the vector
/// @return A copy of the vector scaled by the specified scalar
Vector2 Vector2::operator*(const float &scalar) const { return Vector2(x * scalar, y * scalar); }

/// @brief Binary division that performs vector scaling
/// @param scalar The inverse of the scalar to be applied to the vector
/// @return A copy of the vector scaled by the inverse of the specified scalar
Vector2 Vector2::operator/(const float &scalar) const {
    if (scalar == 0.0f) {
        throw std::runtime_error("Zero Division Error: attempted to divide by zero");
    }

    return Vector2(x / scalar, y / scalar);
}

/// @brief Compound assignment operator that performs in-place vector addition
/// @param other The vector to be added
/// @return The address of the sum of the vectors
Vector2 &Vector2::operator+=(const Vector2 &other) {
    x += other.x;
    y += other.y;
    return *this;
}

/// @brief Compound assignment operator that performs in-place vector
/// subtraction
/// @param other The vector that this is subtracted by
/// @return The address of the difference of the vectors
Vector2 &Vector2::operator-=(const Vector2 &other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

/// @brief Binary multiplication that performs in-place vector scaling
/// @param scalar The scalar to be applied to the vector
/// @return The address of the vector after scaling
Vector2 &Vector2::operator*=(const float &scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

/// @brief Binary division that performs vector scaling
/// @param scalar The inverse of the scalar to be applied to the vector
/// @return The address of the vector after scaling
Vector2 &Vector2::operator/=(const float &scalar) {
    if (scalar == 0.0f) throw std::runtime_error("Zero Division Error: attempted to divide by zero");

    x /= scalar;
    y /= scalar;
    return *this;
}

/// @brief Dot product or inner product of this vector and `other` vector
/// @param other The specified vector to find the dot product with this vector
/// @return The dot product of this vector and `other` vector
float Vector2::DotProduct(const Vector2 &other) const { return x * other.x + y * other.y; }

/// @brief Returns the Euclidean length of the vector from the origin
/// @return The Euclidean length of the vector from the origin
float Vector2::Norm() const { return SDL_sqrtf(DotProduct(*this)); }

/// @brief Returns the angle of the vector in radians
/// @return The angle of the vector in radians
float Vector2::Angle() const { return !(x || y) ? 0.0f : SDL_atan2f(y, x); }

/// @brief Returns a new vector scaled to `length`. A zero vector
/// is scaled to a zero vector
/// @param length The specified length of the new vector
/// @return A new vector with `length`
Vector2 Vector2::ScaleToLength(const float length) const {
    if (Norm() == 0.0f) {
        return Vector2();
    }

    float scalar = length / Norm();
    return *this * scalar;
}

/// @brief Returns a normalized vector with length 1. The zero vector is
/// normalized as the zero vector
/// @return A normalized vector
Vector2 Vector2::Normalize() const { return ScaleToLength(1); }

/// @brief Returns a new vector rotated by the specified angle
/// @param angle The angle to turn the vector by
/// @return A new vector rotated by the specified angle
Vector2 Vector2::Rotate(const float angle) const {
    return RotateToAngle(Angle() + angle);
}

/// @brief Returns a new vector rotated to `angle`
/// @param angle The angle to rotate the vector to
/// @return A new vector rotated to `angle`
Vector2 Vector2::RotateToAngle(const float angle) const {
    Vector2 new_vector(SDL_cosf(angle) * Norm(), SDL_sinf(angle) * Norm());

    return new_vector;
}

/// @brief Returns the Euclidean distance to the point
/// @param point The point to compare the distance
/// @return The distance to the specified point
float Vector2::DistanceTo(const Vector2 point) const { return (point - *this).Norm(); }

/// @brief Returns a new vector of length moved towards the specified point
/// @param point The point to move towards
/// @param length The length of the displacement vector
/// @return A new vector of length moved towards the point
Vector2 Vector2::MoveTowards(const Vector2 point, const float length) const {
    return (point - *this).ScaleToLength(length);
}

/// @brief Returns a vector moved length at `angle`
/// @param angle The angle to move to
/// @param length The length of the displacement vector
/// @return A new vector of length moved at the angle
Vector2 Vector2::MoveTowards(const float angle, const float length) const {
    Vector2 displacement(length, 0);
    displacement = displacement.RotateToAngle(angle);

    return *this + displacement;
}

std::string Vector2::toString() const { return std::to_string(x) + " " + std::to_string(y); }
