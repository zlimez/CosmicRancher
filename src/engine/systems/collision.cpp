#include "collision.hpp"

namespace engine
{
    void Collision::projectPoly(const std::vector<vec2> &poly, const vec2 &axis, float &minProj, float &maxProj)
    {
        minProj = glm::dot(poly[0], axis);
        maxProj = minProj;
        for (size_t i = 1; i < poly.size(); i++)
        {
            float proj = glm::dot(poly[i], axis);
            minProj = std::min(minProj, proj);
            maxProj = std::max(maxProj, proj);
        }
    }

    Contact Collision::findSATCol(const std::vector<vec2> &poly1, const std::vector<vec2> &poly2)
    {
        Contact contact;
        contact.depth = std::numeric_limits<float>::max();
        contact.normal = {0.0f, 0.0f};

        for (size_t i = 0; i < poly1.size() + poly2.size(); i++)
        {
            vec2 edge = i < poly1.size() ? poly1[(i + 1) % poly1.size()] - poly1[i] : poly2[(i + 1) % poly2.size()] - poly2[i];
            vec2 axis = glm::normalize(vec2(-edge.y, edge.x));
            float minProj1, maxProj1, minProj2, maxProj2;
            projectPoly(poly1, axis, minProj1, maxProj1);
            projectPoly(poly2, axis, minProj2, maxProj2);
            float overlap = getOverlap(minProj1, maxProj1, minProj2, maxProj2);
            if (overlap <= 0)
                return contact;
            if (overlap < contact.depth)
            {
                contact.depth = overlap;
                contact.normal = axis;
            }
        }

        return contact;
    }

    void Collision::init(World &world)
    {
    }

    void Collision::update(World &world)
    {
    }

    void Collision::cleanup()
    {
    }
}