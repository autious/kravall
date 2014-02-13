#ifndef CORE_COMPONENTS_EMITTER_COMPONENT_HPP
#define CORE_COMPONENTS_EMITTER_COMPONENT_HPP


namespace Core
{
    class ParticleContainer;
}

namespace Core
{
    typedef ParticleContainer* ParticleHandle;

    enum EmitterType
    {
        CONE_EMITTER,
        SURFACE_EMITTER
    };

    struct ConeEmitter
    {
        float coneDirection[3];        
        float coneAngle;
        float coneAngleVariance;
    };

    struct SurfaceEmitter
    {
        float surfaceDirectionOne[3];
        float surfaceDirectionTwo[3];
    };

    struct EmitterComponent
    {
        int rate;
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
