/*
 * m22.h
 *
 *      Author: flamingo
 *      E-mail: epiforce57@gmail.com
 */

#ifndef M22_H
#define M22_H

#include "vec2.h"

namespace math
{

//! Row-column 2x2 Matrix.
/*!
  * (a00, a01)
  * (a10, a11)
  */
struct M22
{
    float x[4];

    //! Default ctor.
    /*! Default identity matrix. */
    M22();
    //! Array ctor.
    M22(const float (&src)[4]);
    //! Component ctor.
    M22(float a00, float a01, float a10, float a11);

    //! Set elements with array.
    void set(const float (&src)[4]);
    //! Set elements.
    void set(float a00, float a01, float a10, float a11);
    //! Reset to identity matrix.
    void reset();

    //! Matrix addition.
    M22 &operator+= (const M22 &a);
    //! Matrix subtration.
    M22 &operator-= (const M22 &a);
    //! Matrix multiplication.
    M22 &operator*= (const M22 &a);
    //! Scalar multiplication.
    M22 &operator*= (float s);
    //! Scalar multiplication.
    M22 &operator/= (float s);

    //! Equality check.
    bool operator== (const M22 &a) const;

    //! Matrix inversion.
    M22 &invert();
    //! Matrix transposition.
    M22 &transpose();
    //! Determinant computing.
    float determinant() const;
    //! Returns matrix memory address.
    float *getPointer() { return x; }

    //! Addition of two matrices.
    friend M22 operator+ (const M22 &a, const M22 &b);
    //! Subtraction of tho matrices.
    friend M22 operator- (const M22 &a, const M22 &b);
    //! Multiplication of two matrices.
    friend M22 operator* (const M22 &a, const M22 &b);
    //! Scalar multiplication.
    friend M22 operator* (const M22 &a, const float s);
    //! Scalar multiplication.
    friend M22 operator* (float s, const M22 &b);

    //! 2-vector and 2x2 matrix multiplication.
    /*!
      * (1 2) * |1 2|
      *         |3 4|
      */
    friend vec2 operator* (const vec2 &v, const M22 &m);
};

inline M22::M22()
{
    reset();
}

inline M22::M22(float a00, float a01, float a10, float a11)
{
    set(a00, a01, a10, a11);
}

inline M22::M22(const float (&src)[4])
{
    set(src);
}

inline void M22::set(const float (&src)[4])
{
    memcpy(x, src, 4 * sizeof(float));
}

inline void M22::set(float a00, float a01, float a10, float a11)
{
    x[0] = a00;
    x[1] = a01;
    x[2] = a10;
    x[3] = a11;
}

inline void M22::reset()
{
    x[0] = x[3] = 1.0;
    x[1] = x[2] = 0.0;
}

inline M22 &M22::operator+= (const M22 &a)
{
    x[0] += a.x[0];
    x[1] += a.x[1];
    x[2] += a.x[2];
    x[3] += a.x[3];
    return *this;
}

inline M22 &M22::operator-= (const M22 &a)
{
    x[0] -= a.x[0];
    x[1] -= a.x[1];
    x[2] -= a.x[2];
    x[3] -= a.x[3];
    return *this;
}

inline M22 &M22::operator*= (const M22 &a)
{
    M22 temp = *this;

    x[0] = temp.x[0] * a.x[0] + temp.x[1] * a.x[2];
    x[1] = temp.x[0] * a.x[1] + temp.x[1] * a.x[3];
    x[2] = temp.x[2] * a.x[0] + temp.x[3] * a.x[2];
    x[3] = temp.x[2] * a.x[1] + temp.x[3] * a.x[3];
    return *this;
}

inline M22 &M22::operator*= (float s)
{
    std::for_each(x, x + 4, [s](float &el) { el *= s; });

    return *this;
}

inline M22 &M22::operator/= (float s)
{
    assert(!DCMP(s, 0.0));

    std::for_each(x, x + 4, [s](float &el) { el /= s; });

return *this;
}

inline bool M22::operator== (const M22 &a) const
{
    return DCMP(x[0], a.x[0], EPSILON_E3) && DCMP(x[1], a.x[1], EPSILON_E3)
            && DCMP(x[2], a.x[2], EPSILON_E3) && DCMP(x[3], a.x[3], EPSILON_E3);
}

inline M22 &M22::invert()
{
    float d = determinant();
    assert(!DCMP(d, 0.0));
    M22 temp;

    temp.x[0] = x[3] / d;
    temp.x[1] = -x[1] / d;
    temp.x[2] = -x[2] / d;
    temp.x[3] = x[0] / d;
    return *this = temp;
}

inline M22 &M22::transpose()
{
    M22 temp;
    temp.x[0] = x[0];
    temp.x[1] = x[2];
    temp.x[2] = x[1];
    temp.x[3] = x[3];
    return *this = temp;
}

inline float M22::determinant() const
{
    return (x[0] * x[3] - x[1] * x[2]);
}

inline M22 operator+ (const M22 &a, const M22 &b)
{
    M22 res;
    res.x[0] = a.x[0] + b.x[0];
    res.x[1] = a.x[1] + b.x[1];
    res.x[2] = a.x[2] + b.x[2];
    res.x[3] = a.x[3] + b.x[3];
    return res;
}

inline M22 operator- (const M22 &a, const M22 &b)
{
    M22 res;
    res.x[0] = a.x[0] - b.x[0];
    res.x[1] = a.x[1] - b.x[1];
    res.x[2] = a.x[2] - b.x[2];
    res.x[3] = a.x[3] - b.x[3];
    return res;
}

inline M22 operator* (const M22 &a, const M22 &b)
{
    M22 res;
    res.x[0] = a.x[0] * b.x[0] + a.x[1] * b.x[2];
    res.x[1] = a.x[0] * b.x[1] + a.x[1] * b.x[3];
    res.x[2] = a.x[2] * b.x[0] + a.x[3] * b.x[2];
    res.x[3] = a.x[2] * b.x[1] + a.x[3] * b.x[3];
    return res;
}

inline M22 operator* (const M22 &a, float s)
{
    M22 res(a);
    return res *= s;
}

inline M22 operator* (float s, const M22 &b)
{
    M22 res(b);
    return res *= s;
}

inline vec2 operator* (const vec2 &v, const M22 &m)
{
    return vec2(v.x * m.x[0] + v.y * m.x[2],
                v.x * m.x[1] + v.y * m.x[3]);
}

}

#endif // M22_H
