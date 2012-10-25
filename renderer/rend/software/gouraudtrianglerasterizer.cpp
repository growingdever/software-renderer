/*
 * gouraudtrianglerasterizer.cpp
 *
 *      Author: flamingo
 *      E-mail: epiforce57@gmail.com
 */

#include "gouraudtrianglerasterizer.h"

#include "poly.h"
#include "framebuffer.h"
#include "vertex.h"
#include "vec3.h"
#include "color.h"
#include <smmintrin.h>

namespace rend
{

// test func
inline bool testClip(const math::vertex &v, FrameBuffer *fb)
{
    int xorig = fb->xorig();
    int yorig = fb->yorig();
    int width = fb->width();
    int height = fb->height();

    return v.p.y < yorig || v.p.y >= height || v.p.x < xorig || v.p.x >= width;
}

union Interpolant
{
    struct { float dr, dg, db, dx; } __attribute__((aligned(16)));
    __m128 v __attribute__((aligned(16)));
};

// TODO: SSE stuff for interpolating
void GouraudTriangleRasterizer::drawTriangle(const math::Triangle &t, FrameBuffer *fb)
{
    math::vertex v0 = t.v(0);
    math::vertex v1 = t.v(1);
    math::vertex v2 = t.v(2);

    // if triangle isn't on a screen
    // without complex clipping......
    /*if (testClip(v0, fb) || testClip(v1, fb) || testClip(v2, fb))
        return;*/

    // CW order
    if (v1.p.y < v0.p.y)
        std::swap(v1, v0);
    if (v2.p.y < v0.p.y)
        std::swap(v0, v2);
    if (v1.p.y < v2.p.y)
        std::swap(v1, v2);

    auto material = t.getMaterial();
    int alpha = material->alpha;

    Interpolant leftInt;
    Interpolant rightInt;

    // Interpolating 1/z values.
    leftInt.dx = v2.p.x - v0.p.x;
    leftInt.dr = (float)v2.color[RED] - (float)v0.color[RED];
    leftInt.dg = (float)v2.color[GREEN] - (float)v0.color[GREEN];
    leftInt.db = (float)v2.color[BLUE] - (float)v0.color[BLUE];
    float dzl = 1.0f / v2.p.z - 1.0f / v0.p.z;

    rightInt.dx = v1.p.x - v0.p.x;
    rightInt.dr = (float)v1.color[RED] - (float)v0.color[RED];
    rightInt.dg = (float)v1.color[GREEN] - (float)v0.color[GREEN];
    rightInt.db = (float)v1.color[BLUE] - (float)v0.color[BLUE];
    float dzr = 1.0f / v1.p.z - 1.0f / v0.p.z;

    float dy1 = v2.p.y - v0.p.y;
    float dy2 = v1.p.y - v0.p.y;

    leftInt.v = _mm_div_ps(leftInt.v, _mm_set_ps1(dy1));
    rightInt.v = _mm_div_ps(rightInt.v, _mm_set_ps1(dy2));
    dzl /= dy1;
    dzr /= dy2;

    Interpolant leftIntC;
    Interpolant rightIntC;
    float dzlc, dzrc;

    if (leftInt.dx < rightInt.dx)
    {
        leftIntC = leftInt;
        rightIntC = rightInt;
        dzlc = dzl;
        dzrc = dzr;
    }
    else
    {
        leftIntC = rightInt;
        rightIntC = leftInt;
        dzlc = dzr;
        dzrc = dzl;
    }

    Interpolant start, end;
    start.dx = v0.p.x; start.dr = v0.color[RED]; start.dg = v0.color[GREEN]; start.db = v0.color[BLUE];
    end = start;
    float startz = 1.0f / v0.p.z;
    float endz = 1.0f / v0.p.z;

    Interpolant p, pdelta;
    float z, zdelta;

    int x, y;
    for (y = (int)v0.p.y; y < (int)v2.p.y; y++)
    {
        pdelta.v = _mm_sub_ps(end.v, start.v);
        zdelta = endz - startz;
        pdelta.dx = 0;

        pdelta.v = _mm_div_ps(pdelta.v, _mm_set_ps1(end.dx - start.dx));
        if (!math::DCMP((end.dx - start.dx), 0))
            zdelta /= end.dx - start.dx;

        p = start;
        z = startz;
        for (x = (int)start.dx; x < (int)end.dx; x++)
        {
            fb->wpixel(x, y, Color3(p.dr, p.dg, p.db), z, alpha);

            p.v = _mm_add_ps(p.v, pdelta.v);
            z += zdelta;
            p.dx = 0;
        }

        start.v = _mm_add_ps(start.v, leftIntC.v);
        end.v = _mm_add_ps(end.v, rightIntC.v);
        startz += dzlc;
        endz += dzrc;
    }

    // Now for the bottom of the triangle
    if (leftInt.dx < rightInt.dx)
    {
        leftIntC.dx = v1.p.x - v2.p.x;
        leftIntC.dr = (float)v1.color[RED] - (float)v2.color[RED];
        leftIntC.dg = (float)v1.color[GREEN] - (float)v2.color[GREEN];
        leftIntC.db = (float)v1.color[BLUE] - (float)v2.color[BLUE];
        dzlc = 1.0f / v1.p.z - 1.0f / v2.p.z;

        leftIntC.v = _mm_div_ps(leftIntC.v, _mm_set_ps1(v1.p.y - v2.p.y));
        dzlc /= v1.p.y - v2.p.y;

        start.dx = v2.p.x; start.dr = v2.color[RED]; start.dg = v2.color[GREEN]; start.db = v2.color[BLUE];
        startz = 1.0f / v2.p.z;
    }
    else
    {
        rightIntC.dx = v1.p.x - v2.p.x;
        rightIntC.dr = (float)v1.color[RED] - (float)v2.color[RED];
        rightIntC.dg = (float)v1.color[GREEN] - (float)v2.color[GREEN];
        rightIntC.db = (float)v1.color[BLUE] - (float)v2.color[BLUE];
        dzrc = 1.0f / v1.p.z - 1.0f / v2.p.z;

        rightIntC.v = _mm_div_ps(rightIntC.v, _mm_set_ps1(v1.p.y - v2.p.y));
        dzrc /= v1.p.y - v2.p.y;

        end.dx = v2.p.x; end.dr = v2.color[RED]; end.dg = v2.color[GREEN]; end.db = v2.color[BLUE];
        endz = 1.0f / v2.p.z;
    }

    for (y = (int)v2.p.y; y< (int)v1.p.y; y++)
    {
        pdelta.v = _mm_sub_ps(end.v, start.v);
        zdelta = endz - startz;
        pdelta.dx = 0;

        pdelta.v = _mm_div_ps(pdelta.v, _mm_set_ps1(end.dx - start.dx));
        if (!math::DCMP((end.dx - start.dx), 0))
            zdelta /= end.dx - start.dx;

        p = start;
        z = startz;
        for (x = (int)start.dx; x < (int)end.dx; x++)
        {
            fb->wpixel(x, y, Color3(p.dr, p.dg, p.db), z, alpha);

            p.v = _mm_add_ps(p.v, pdelta.v);
            p.dx = 0;
            z += zdelta;
        }

        start.v = _mm_add_ps(start.v, leftIntC.v);
        end.v = _mm_add_ps(end.v, rightIntC.v);
        startz += dzlc;
        endz += dzrc;
    }
}

}
