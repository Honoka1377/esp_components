#ifndef VECTOR2_H
#define VECTOR2_H

#include <stdint.h>
#include <stdbool.h>
typedef struct Vector2
{
    int32_t x, y;
} Vector2;

Vector2 Vector2Ctor(int32_t x, int32_t y);

Vector2 Vector2Add(Vector2 left, Vector2 right);
Vector2 Vector2Sub(Vector2 left, Vector2 right);
Vector2 Vector2Mul(Vector2 left, int32_t right);
Vector2 Vector2Div(Vector2 left, int32_t right);
bool Vector2Equals(Vector2 left, Vector2 right);

int32_t Vector2ManhattanDistance(Vector2 this);

#endif