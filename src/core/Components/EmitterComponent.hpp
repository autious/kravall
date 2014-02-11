#ifndef CORE_COMPONENTS_EMITTER_COMPONENT_HPP
#define CORE_COMPONENTS_EMITTER_COMPONENT_HPP

namespace Core
{
    typedef void* ParticleHandle;

    enum EmitterType
    {
        CONE_EMITTER,
        SURFACE_EMITTER
    };

    struct ConeEmitter
    {
        float direction[3];
        float angle;
        float angleVariance;
    };

    struct SurfaceEmitter
    {
        float directionOne[3];
        float directionTwo[3];
    };

    struct EmitterComponent
    {
        float rate;
        float offset[3]; 
        float life;
        float lifeVariance;
        float lifeReduction;
        float lifeReductionVariance;
        float velocity[3];
        float velocityVariance[3];       
        float acceleration[3];

        Core::EmitterType type;
        Core::ParticleHandle handle;

        union
        {
            ConeEmitter cone;
            SurfaceEmitter surface;
        };

        inline static const char* GetName()
        {
            return "EmitterComponent";
        }
    };   
}


#endif
