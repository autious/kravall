#ifndef CORE_SYSTEMS_ANIMATION_SYSTEM_HPP
#define CORE_SYSTEMS_ANIMATION_SYSTEM_HPP

#include <ComponentFramework/BaseSystem.hpp>

#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <map>

namespace Core 
{
    /*!
        Game logic system that calculates various data regarding groups of entities.
    */
    class AnimationSystem : public  Core::BaseSystem
    {
    public:
        AnimationSystem();

        virtual void Update( float delta ) override;

        virtual const char* GetHumanName() { return "AnimationSystem"; }

    private:

		int GetAnimationIDFromName(std::string animationName);

		std::map<std::string, int> m_animations;
    };
}

#endif
