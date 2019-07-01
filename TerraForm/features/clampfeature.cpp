#include "clampfeature.hpp"
#include "terrain.hpp"
#include <glm/glm.hpp>

// polynomial smooth min (k = 0.1);
static float smin( float a, float b, float k )
{
    float h = glm::clamp( 0.5+0.5*(b-a)/k, 0.0, 1.0 );
    return glm::mix( b, a, h ) - k*h*(1.0-h);
}

void ClampFeature::apply(Terrain & terrain) const
{
    for(size_t y = 0; y < terrain.height; y++)
    {
        for(size_t x = 0; x < terrain.width; x++)
        {
            if(use_smooth)
            {
                if(clamp_lower)
                    terrain(x, y) = -smin(-terrain(x, y) , -lower_bound, smooth_radius);
                if(clamp_upper)
                    terrain(x, y) = smin(terrain(x, y) , upper_bound, smooth_radius);
            }
            else
            {
                if(clamp_lower)
                    terrain(x, y) = glm::max<float>(terrain(x, y) , lower_bound);
                if(clamp_upper)
                    terrain(x, y) = glm::min<float>(terrain(x, y) , upper_bound);
            }
        }
    }
}

REGISTER_TERRAIN_FEATURE("Abstract", "Clamp", ":/icons/clamp.svg", {
    auto * ptr = new ClampFeature;
    ptr->x = x;
    ptr->y = y;
    return ptr;
});
