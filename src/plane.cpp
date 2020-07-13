#include "pch.h"
#pragma hdrstop
#include "plane.h"

using namespace quadric;

Plane::Plane(float width, float length, bool twoSided,
    std::shared_ptr<magma::CommandBuffer> cmdBuffer):
    Quadric(4, twoSided ? 4 : 2, cmdBuffer->getDevice()),
    width(width),
    length(length)
{
    if ((width < 0.f) || (length < 0.f))
        throw std::invalid_argument("invalid plane dimension");

    const float w = width * .5f;
    const float l = length * .5f;
    constexpr rapid::float3 normal(0.f, 1.f, 0.f);

    Vertex *verts = vertices->getMemory()->map<Vertex>();
    verts[0].pos = {-w, 0.f, -l};
    verts[0].normal = normal;
    verts[0].tex = {0.f, 0.f};
    verts[1].pos = {-w, 0.f, l};
    verts[1].normal = normal;
    verts[1].tex = {0.f, 1.f};
    verts[2].pos = {w, 0.f, -l};
    verts[2].normal = normal;
    verts[2].tex = {1.f, 0.f};
    verts[3].pos = {w, 0.f, l};
    verts[3].normal = normal;
    verts[3].tex = {1.f, 1.f};

    constexpr uint16_t front[] = {
        0, 1, 2, 2, 1, 3
    };
    constexpr uint16_t frontAndBack[] = {
        0, 1, 2, 2, 1, 3,
        0, 2, 1, 1, 2, 3
    };
    memcpy(indices->getMemory()->map(),
        twoSided ? frontAndBack : front,
        numFaces * 3 * sizeof(uint16_t));
    upload(std::move(cmdBuffer));
}
