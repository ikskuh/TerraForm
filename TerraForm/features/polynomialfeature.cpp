#include "polynomialfeature.hpp"
#include "terrain.hpp"
#include <glm/glm.hpp>

PolynomialFeature::PolynomialFeature()
{

}

void PolynomialFeature::apply(Terrain &terrain) const
{
    glm::vec2 const dir {
        glm::sin(glm::radians(direction)),
        glm::cos(glm::radians(direction)),
    };

    glm::vec2 const origin {
        this->x,
        this->y,
    };

    for(size_t y = 0; y < terrain.height; y++)
    {
        for(size_t x = 0; x < terrain.width; x++)
        {
            auto const delta = glm::vec2(x, y) - origin;

            auto const dist = 0.01f * ((mode == Centroid) ? glm::length(delta) : glm::dot(delta, dir));

            terrain(x, y) += constant;
            terrain(x, y) += linear * dist;
            terrain(x, y) += quadratic * dist * dist;
            terrain(x, y) += cubic * dist * dist * dist;
        }
    }
}

REGISTER_TERRAIN_FEATURE("Abstract", "Polynomial", ":/icons/polynomial.svg", {
    auto * ptr = new PolynomialFeature;
    ptr->x = x;
    ptr->y = y;
    return ptr;
});
