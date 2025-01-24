#include "Vector2.h"

Vector2 Vector2Ctor(int32_t x, int32_t y)
{
    Vector2 vec = {x, y};
    return vec;
}

Vector2 Vector2Add(Vector2 left, Vector2 right)
{
    return Vector2Ctor(left.x + right.x, left.y + right.y);
}
Vector2 Vector2Sub(Vector2 left, Vector2 right)
{
    return Vector2Ctor(left.x - right.x, left.y - right.y);
}

Vector2 Vector2Mul(Vector2 left, int32_t right)
{
    return Vector2Ctor(left.x * right, left.y * right);
}
Vector2 Vector2Div(Vector2 left, int32_t right)
{
    return Vector2Ctor(left.x / right, left.y / right);
}

bool Vector2Equals(Vector2 left, Vector2 right)
{
    return left.x == right.x && left.y == right.y;
}

int32_t Vector2ManhattanDistance(Vector2 this)
{
    return (this.x > 0 ? this.x : -this.x) + (this.y > 0 ? this.y : -this.y);
}
