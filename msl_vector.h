#ifndef MSL_VECTOR_H
#define MSL_VECTOR_H

#include <math.h>

typedef struct msl_vector2
{
    float x, y; 
} msl_vector2;

typedef struct msl_vector3
{
    float x, y, z; 
} msl_vector3;


static msl_vector2 msl_vector2_add(msl_vector2 a, msl_vector2 b);
static msl_vector2 msl_vector2_sub(msl_vector2 a, msl_vector2 b);
static msl_vector2 msl_vector2_scalar_product(msl_vector2 a, float f);
static float msl_vector2_dot_product(msl_vector2 a, msl_vector2 b);
static float msl_vector2_magnitude(msl_vector2 a);

static msl_vector3 msl_vector3_add(msl_vector3 a, msl_vector3 b);
static msl_vector3 msl_vector3_sub(msl_vector3 a, msl_vector3 b);
static msl_vector3 msl_vector3_cross_product(msl_vector3 a, msl_vector3 b);
static float msl_vector3_dot_product(msl_vector3 a, msl_vector3 b);
static float msl_vector3_magnitude(msl_vector3 a);

static msl_vector2 msl_vector2_add(msl_vector2 a, msl_vector2 b)
{
    return (msl_vector2){a.x + b.x, a.y + b.y};
}
static msl_vector2 msl_vector2_sub(msl_vector2 a, msl_vector2 b)
{
    return (msl_vector2){a.x - b.x, a.y - b.y};
}
static msl_vector2 msl_vector2_scalar_product(msl_vector2 a, float f)
{
    return (msl_vector2){a.x * f, a.y * f};
}
static float msl_vector2_dot_product(msl_vector2 a, msl_vector2 b)
{
   return a.x * b.x + a.y * b.y;
}
static float msl_vector2_magnitude(msl_vector2 a)
{
    return sqrt(pow(a.x, 2) + pow(a.y, 2));
}


static msl_vector3 msl_vector3_add(msl_vector3 a, msl_vector3 b)
{
    return (msl_vector3){a.x + b.x, a.y + b.y, a.z + b.z};
}
static msl_vector3 msl_vector3_sub(msl_vector3 a, msl_vector3 b)
{
    return (msl_vector3){a.x - b.x, a.y - b.y, a.z - b.z};
}
static msl_vector3 msl_vector3_cross_product(msl_vector3 a, msl_vector3 b)
{
    return (msl_vector3){a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}
static float msl_vector3_dot_product(msl_vector3 a, msl_vector3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
static float msl_vector3_magnitude(msl_vector3 a)
{
    return sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2));
}

#endif
