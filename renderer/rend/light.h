/*
 * light.h
 *
 *      Author: flamingo
 *      E-mail: epiforce57@gmail.com
 */

#ifndef LIGHT_H
#define LIGHT_H

#include "../math/vec3.h"
#include "color.h"
#include "renderlist.h"
#include "node.h"

namespace rend
{

DECLARE_EXCEPTION(LightException)

class Light : public Node
{
public:
    enum LightType
    {
        LT_AMBIENT_LIGHT,
        LT_DIRECTIONAL_LIGHT,
        LT_POINT_LIGHT,
        LT_SPOT_LIGHT
    };

    static const size_t MAX_LIGHTS;

private:
    static size_t NumLights;
    size_t m_lightId;

protected:
    bool m_isEnabled;   // on\off
    Color3 m_intensity;

    virtual Color3 shader(const sptr(Material) material, const math::vec3 &normal, const math::vec3 &pt) const = 0;

    Light(const Color3 &intensity);
    virtual ~Light();

public:
    void turnon() { m_isEnabled = true; }
    void turnoff() { m_isEnabled = false; }

    int getId() const { return m_lightId; }

    virtual void illuminate(RenderList *renderlist) const;
};

//! Ambient light
class AmbientLight : public Light
{
protected:
    virtual Color3 shader(const sptr(Material) material, const math::vec3 &normal, const math::vec3 &pt) const;

public:
    AmbientLight(const Color3 &intensity);
};

//! Directional light
class DirectionalLight : public Light
{
    math::vec3 m_dir;

    virtual Color3 shader(const sptr(Material) material, const math::vec3 &normal, const math::vec3 &pt) const;

public:
    DirectionalLight(const Color3 &intensity, const math::vec3 &dir);
};

//! Point light
class PointLight : public Light
{
    float m_kc, m_kl, m_kq;

    virtual Color3 shader(const sptr(Material) material, const math::vec3 &normal, const math::vec3 &pt) const;

public:
    PointLight(const Color3 &intensity, const math::vec3 &pos,
               float kc, float kl, float kq);
};
/*
//! Spot light
class SpotLight : public Light
{
    math::vec3 m_pos;
    math::vec3 m_dir;

    float m_innerAngle;    // spot inner angle
    float m_outerAngle;    // spot outer angle
    float m_falloff;

    virtual Color3 shader(const sptr(Material) material, const math::vec3 &normal) const;
    virtual Color3 getMaterialColor(sptr(Material) material) const;

public:
    SpotLight(const Color3 &intensity, const math::vec3 &pos, const math::vec3 &dir,
              float umbra, float penumbra, float falloff);
};
*/
}

#endif // LIGHT_H
