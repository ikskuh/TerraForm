#include "sinewavefeature.hpp"
#include "terrain.hpp"
#include <cmath>
#include <glm/glm.hpp>

SineWaveFeature::SineWaveFeature()
{

}

void SineWaveFeature::apply(Terrain & terrain) const
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

            auto const dist = (mode == Centroid) ? glm::length(delta) : glm::dot(delta, dir);

            terrain(x, y) += amplitude * cos(glm::radians(phase_shift) + (M_PI * frequency / 500.0) * dist);
        }
    }
}

REGISTER_TERRAIN_FEATURE("Abstract", "Sine Wave", ":/icons/sine-wave.svg", {
    auto * ptr = new SineWaveFeature;
    ptr->x = x;
    ptr->y = y;
    return ptr;
});
