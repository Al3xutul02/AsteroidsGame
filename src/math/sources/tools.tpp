#pragma once
#include "../tools.h"

namespace Math {
    template <typename T>
    T Lerp(const T& a, const T& b, float t) {
        return a + t * (b - a);
    }

    template <typename T>
    float InvLerp(const T& a, const T& b, const T& v) {
        return ((float)(v - a)) / ((float)(b - a));
    }
}