/*
 * common_math.h
 *
 *      Author: flamingo
 *      E-mail: epiforce57@gmail.com
 */

#ifndef COMMON_MATH_H
#define COMMON_MATH_H

#ifndef M_PI
#define M_PI 3.1415926535897932f
#endif

namespace math
{

//! Some kind of machine epsilons.
const float EPSILON_E1 = 1E-1f;
const float EPSILON_E3 = 1E-3f;
const float EPSILON_E4 = 1E-4f;
const float EPSILON_E5 = 1E-5f;
const float EPSILON_E6 = 1E-6f;
const float EPSILON_E12 = 1E-12f;

//! Float point comparsion of two values.
/*!
  * \returns true if x == y with some imprecision.
  */
inline bool DCMP(float x, float y, float pres = EPSILON_E12)
{
    return (fabs(x - y) < pres);
}

//! Degrees to radians conversion function.
inline float DegToRad(float ang) { return ang * (M_PI / 180.0f); }
//! Radians to degrees conversion function.
inline float RadToDeg(float rad) { return rad * (180.0f / M_PI); }

}

#endif // COMMON_MATH_H
