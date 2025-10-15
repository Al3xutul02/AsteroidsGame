#pragma once

namespace Math {
    template <typename T>
    T Lerp(const T& a, const T& b, float t);

    template <typename T>
    float InvLerp(const T& a, const T& b, const T& v);
}

#include "sources/tools.tpp"