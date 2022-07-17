#pragma once

namespace picam {
template <typename T> struct Vector2D {
  T x, y;

  Vector2D(T x, T y) : x(x), y(y) {}

  Vector2D operator+(Vector2D o) const { return {x + o.x, y + o.y}; }

  Vector2D operator-(Vector2D o) const { return {x - o.x, y - o.y}; }

  Vector2D operator+=(const Vector2D &o) {
    x += o.x;
    y += o.y;
    return *this;
  }

  Vector2D operator-=(const Vector2D &o) {
    x -= o.x;
    y -= o.y;
    return *this;
  }
};
} // namespace picam
