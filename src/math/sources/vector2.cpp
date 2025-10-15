#include "../vector2.h"
#include <cmath>
#include <numbers>
#include <algorithm>

namespace Math {
    const Vector2 Vector2::Zero = Vector2(0.0f, 0.0f);

    void Vector2::operator=(const Vector2& other) {
        this->x = other.x;
        this->y = other.y;
    }

    Vector2 Vector2::operator+(const Vector2& other) const {
        return Vector2 (
            this->x + other.x,
            this->y + other.y
        );
    }

    Vector2 Vector2::operator-(const Vector2& other) const {
        return Vector2(
            this->x - other.x,
            this->y - other.y
        );
    }

    bool Vector2::operator==(const Vector2& other) const {
        return this->x == other.x && this->y == other.y;
    }
    bool Vector2::operator!=(const Vector2& other) const {
        return !((*this) == other);
    }

    Vector2 Vector2::operator*(const int& scalar) const {
        return Vector2(
            this->x * scalar,
            this->y * scalar
        );
    }

    Vector2 Vector2::operator*(const float& scalar) const {
        return Vector2(
            this->x * scalar,
            this->y * scalar
        );
    }

    Vector2 Vector2::operator/(const int& scalar) const {
        if (scalar == 0) {
            return Vector2(NAN, NAN);
        }

        return Vector2(
            this->x / scalar,
            this->y / scalar
        );
    }

    Vector2 Vector2::operator/(const float& scalar) const {
        if (scalar == 0.0f) {
            return Vector2(NAN, NAN);
        }

        return Vector2(
            this->x / scalar,
            this->y / scalar
        );
    }

    void Vector2::operator+=(const Vector2& other) {
        this->x += other.x;
        this->y += other.y;
    }

    void Vector2::operator-=(const Vector2& other) {
        this->x -= other.x;
        this->y -= other.y;
    }

    void Vector2::operator*=(const int& scalar) {
        this->x *= scalar;
        this->y *= scalar;
    }

    void Vector2::operator*=(const float& scalar) {
        this->x *= scalar;
        this->y *= scalar;
    }

    void Vector2::operator/=(const int& scalar) {
        this->x /= scalar;
        this->y /= scalar;
    }

    void Vector2::operator/=(const float& scalar) {
        this->x /= scalar;
        this->y /= scalar;
    }

    float DotProduct(const Vector2& vector1, const Vector2& vector2) {
        return vector1.x * vector2.x + vector1.y + vector2.y;
    }

    float CrossProduct(const Vector2& vector1, const Vector2& vector2) {
        return vector1.x * vector2.y - vector1.y * vector2.x;
    }

    float LengthSqr(const Vector2& vector) {
        return std::pow(vector.x, 2) + std::pow(vector.y, 2);
    }

    float Length(const Vector2& vector) {
        return std::sqrt(LengthSqr(vector));
    }

    Vector2 Normalize(const Vector2& vector) {
        float length = Length(vector);
        if (length == 0.0f) {
            return Vector2(NAN, NAN);
        }

        return Vector2(
            vector.x / length,
            vector.y / length
        );
    }

    Vector2 GetForward(const float& angle) {
        return Vector2(
            std::cosf(angle),
            std::sinf(angle)
        );
    }

    float GetAngle(const Vector2& vector) {
        float radians = std::atan2f(vector.y, vector.x);
        if (radians < 0.0f) {
            radians += 2.0f * std::numbers::pi_v<float>;
        }

        return radians * (180.0f / std::numbers::pi_v<float>); 
    }

    float GetAngle(const Vector2& vector1, const Vector2& vector2) {
        float mag1 = Length(vector1);
        float mag2 = Length(vector2);
        if (mag1 == 0.0f || mag2 == 0.0f) {
            return NAN;
        }

        float dot_normalized = DotProduct(vector1, vector2) / (mag1 * mag2);
        dot_normalized = std::clamp(dot_normalized, -1.0f, 1.0f);

        float radians = std::acosf(dot_normalized);
        return radians * (180.0f / std::numbers::pi_v<float>); 
    }
}