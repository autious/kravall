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

            if( htc->hoverTextId != -1 && htc->render )
            {
                GFX::FontData * font = localFontData;

                if( htc->font != nullptr )
                    font = htc->font;
                if( htc->box ) 
                {
	                GFX::RenderTextbox(font, glm::vec4( wpc->position[0], -wpc->position[1], htc->dim[0], htc->dim[1] ), 0, htc->size, HoverTextComponent::GetColor( *htc ), m_strings[htc->hoverTextId].c_str() );
                }
                else
                {
                    GFX::RenderText( font, glm::vec2( wpc->position[0], -wpc->position[1] ), htc->size, HoverTextComponent::GetColor( *htc ), m_strings[htc->hoverTextId].c_str() );
                }


                if( CONF.GetBool( "debugRenderHoverTextBounding", true ) )
                { 
                    glm::vec2 pos(wpc->position[0], -wpc->position[1]);
                    glm::vec2 dim = GetStringDimensions( htc->hoverTextId );
                    pos.y -= dim.y;
                    

                    GFX::Debug::DrawRectangle(pos,dim,false,Colors::Black );
                }
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
        return glm::vec2(m_strings[id].size() * 10, 14);
    }

    glm::vec2 HoverTextSystem::GetTextboxDimensions( GFX::FontData* font, float width, float size, int id )
    {
		glm::vec2 result;
		GFX::GetActualTextboxSize(font, width, size, m_strings[id].c_str(), result.x, result.y);
        return result;
    }
}
