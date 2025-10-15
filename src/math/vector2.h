#pragma once

namespace Math {
    struct Vector2 {
        float x;
        float y;

        Vector2(float x, float y) : x(x), y(y) {}

        static const Vector2 Zero;

        void operator=(const Vector2& other);
        Vector2 operator+(const Vector2& other) const;
        Vector2 operator-(const Vector2& other) const;
        bool operator==(const Vector2& other) const;
        bool operator!=(const Vector2& other) const;

        Vector2 operator*(const int& scalar) const;
        Vector2 operator*(const float& scalar) const;
        Vector2 operator/(const int& scalar) const;
        Vector2 operator/(const float& scalar) const;

        void operator+=(const Vector2& other);
        void operator-=(const Vector2& other);
        void operator*=(const int& scalar);
        void operator*=(const float& scalar);
        void operator/=(const int& scalar);
        void operator/=(const float& scalar);
    };

    float DotProduct(const Vector2& vector1, const Vector2& vector2);
    float CrossProduct(const Vector2& vector1, const Vector2& vector2);
    float LengthSqr(const Vector2& vector);
    float Length(const Vector2& vector);
    Vector2 Normalize(const Vector2& vector);
    Vector2 GetForward(const float& angle);
    float GetAngle(const Vector2& vector);
    float GetAngle(const Vector2& vector1, const Vector2& vector2);
}