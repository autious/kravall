#include "HoverTextSystem.hpp"
#include <SystemDef.hpp>
#include <World.hpp>
#include <main.hpp>
#include <gfx/GFXInterface.hpp>

namespace Core
{
    
    HoverTextSystem::HoverTextSystem()
    : BaseSystem( EntityHandler::GenerateAspect<HoverTextComponent,WorldPositionComponent>(), 0ULL )
    {
        nextId = -1; 
    }

    void HoverTextSystem::Update( float delta )
    {
        for(std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
        {
            HoverTextComponent * htc = WGETC<HoverTextComponent>(*it);
            WorldPositionComponent * wpc = WGETC<WorldPositionComponent>(*it);

            if( htc->hoverTextId != -1 )
            {
                GFX::RenderText( localFontData, glm::vec2( wpc->position[0], -wpc->position[1] ), htc->size, HoverTextComponent::GetColor( *htc ), m_strings[htc->hoverTextId].c_str() );
            }
        }
    }

    int HoverTextSystem::AllocateString( const char * string )
    {
        nextId++;

        m_strings[nextId] = std::string( string );

        return nextId;
    }

    int HoverTextSystem::AllocateString( const char * string, glm::vec2 offset, glm::vec2 dimensions )
    {
        return AllocateString( string );
    }

    void HoverTextSystem::SetString( int id, const char * string )
    {
        m_strings[id] = std::string( string );
    }

    void HoverTextSystem::SetString( int id, const char * string, glm::vec2 offset, glm::vec2 dimensions )
    {
        SetString( id, string );
    }

    void HoverTextSystem::FreeString( int id )
    {
        m_strings.erase( id ); 
    }

    glm::vec2 HoverTextSystem::GetStringDimensions( int id )
    {
        return glm::vec2(m_strings[id].size() * 6, 14);
    }
}
