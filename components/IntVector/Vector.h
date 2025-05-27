#ifndef VECTOR2_H
#define VECTOR2_H

#include <stdint.h>
#include <stdbool.h>
#include <math.h> 

// 2D Vector structure and operations
typedef struct Vector2
{
    int32_t x, y;
} Vector2;

// Constructs a new Vector2 with given coordinates
Vector2 Vector2Ctor(int32_t x, int32_t y);

// Vector arithmetic operations
Vector2 Vector2Add(Vector2 left, Vector2 right);
Vector2 Vector2Sub(Vector2 left, Vector2 right);
Vector2 Vector2Mul(Vector2 left, int32_t right);
Vector2 Vector2Div(Vector2 left, int32_t right);

// Returns true if two vectors have identical coordinates
bool Vector2Equals(Vector2 left, Vector2 right);

// Returns the Manhattan distance (L1 norm) from origin
int32_t Vector2L1Distance(Vector2 this);

// Returns the Euclidean distance (L2 norm) from origin
int32_t Vector2Distance(Vector2 this);

// 3D Vector structure and operations
typedef struct Vector3
{
    int32_t x, y, z;
} Vector3;

// Constructs a new Vector3 with given coordinates
Vector3 Vector3Ctor(int32_t x, int32_t y, int32_t z);

// Vector arithmetic operations
Vector3 Vector3Add(Vector3 left, Vector3 right);
Vector3 Vector3Sub(Vector3 left, Vector3 right);
Vector3 Vector3Mul(Vector3 left, int32_t right);
Vector3 Vector3Div(Vector3 left, int32_t right);

// Returns true if two vectors have identical coordinates
bool Vector3Equals(Vector3 left, Vector3 right);

// Returns the Manhattan distance (L1 norm) from origin
int32_t Vector3L1Distance(Vector3 this);

// Returns the Euclidean distance (L2 norm) from origin
int32_t Vector3Distance(Vector3 this);

#endif
