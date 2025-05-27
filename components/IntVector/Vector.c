#include "Vector.h"

#pragma region Vector2
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

int32_t Vector2L1Distance(Vector2 this)
{
    return (this.x > 0 ? this.x : -this.x) + (this.y > 0 ? this.y : -this.y);
}

int32_t Vector2Distance(Vector2 this)
{
    return (int32_t)sqrt(this.x * this.x + this.y * this.y);
}

#pragma endregion

#pragma region Vector3
Vector3 Vector3Ctor(int32_t x, int32_t y, int32_t z)
{
    Vector3 vec = {x, y, z};
    return vec;
}

Vector3 Vector3Add(Vector3 left, Vector3 right)
{
    return Vector3Ctor(left.x + right.x, left.y + right.y, left.z + right.z);
}

Vector3 Vector3Sub(Vector3 left, Vector3 right)
{
    return Vector3Ctor(left.x - right.x, left.y - right.y, left.z - right.z);
}

Vector3 Vector3Mul(Vector3 left, int32_t right)
{
    return Vector3Ctor(left.x * right, left.y * right, left.z * right);
}

Vector3 Vector3Div(Vector3 left, int32_t right)
{
    return Vector3Ctor(left.x / right, left.y / right, left.z / right);
}

bool Vector3Equals(Vector3 left, Vector3 right)
{
    return left.x == right.x && left.y == right.y && left.z == right.z;
}

int32_t Vector3L1Distance(Vector3 this)
{
    return (this.x > 0 ? this.x : -this.x) +
           (this.y > 0 ? this.y : -this.y) +
           (this.z > 0 ? this.z : -this.z);
}

int32_t Vector3Distance(Vector3 this)
{
    return (int32_t)sqrt(this.x * this.x + this.y * this.y + this.z * this.z);
}

#pragma endregion
