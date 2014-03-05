#ifndef CORE_SYSTEMS_RIOTERANIMATION_SYSTEM_HPP
#define CORE_SYSTEMS_RIOTERANIMATION_SYSTEM_HPP

#include <ComponentFramework/BaseSystem.hpp>

#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <map>

#define GRACE_THRESHOLD 0.3f
#define STILL_THRESHOLD 0.35f

namespace Core 
{
    /*!
        Game logic system that calculates various data regarding groups of entities.
    */
    class MovementAnimationSystem : public  Core::BaseSystem
    {
    public:
        MovementAnimationSystem();

        virtual void Update( float delta ) override;

        virtual const char* GetHumanName() { return "MovementAnimationSystem"; }

    private:

    };
}

#endif
