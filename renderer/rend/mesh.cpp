#include "mesh.h"

#include "m33.h"

namespace rend
{

Mesh::Mesh(const vector<math::vec3> &vertices,
           const vector<size_t> &indices,
           const vector<Material> &materials,
           const MeshType type)
    : m_vertices(vertices),
      m_indices(indices),
      m_materials(materials),
      m_type(type)
{
    m_boundingSphere.calculate(vertices);
}

Mesh::Mesh(const vector<math::vec3> &vertices,
           const MeshType type)
    : m_vertices(vertices),
      m_type(type)
{
}

Mesh::~Mesh()
{
}

size_t Mesh::numVertices() const
{
    return m_vertices.size();
}

size_t Mesh::numIndices() const
{
    return m_indices.size();
}

const vector<math::vec3> &Mesh::vertices() const
{
    return m_vertices;
}

const vector<size_t> &Mesh::indices() const
{
    return m_indices;
}

const vector<Material> &Mesh::materials() const
{
    return m_materials;
}

Mesh::MeshType Mesh::type() const
{
    return m_type;
}

void Mesh::setPosition(const math::vec3 &pos)
{
    m_worldTransformation.setv(pos);
    setTransformation(m_worldTransformation);
}

void Mesh::setRotation(const math::vec3 &angles)
{
    math::M33 resM = math::M33::getRotateYawPitchRollMatrix(angles.y,
                                                            angles.x,
                                                            angles.z);
    m_worldTransformation.setm(resM);
    setTransformation(m_worldTransformation);
}

void Mesh::setTransformation(const math::AffineTransform &tr)
{
    for (unsigned i = 0; i < m_vertices.size(); i++)
        tr.transformPoint(m_vertices[i]);
}

}
