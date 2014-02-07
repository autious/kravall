#ifndef SRC_CORE_SYSTEMS_HOVERTEXTSYSTEM_H
#define SRC_CORE_SYSTEMS_HOVERTEXTSYSTEM_H

#include <ComponentFramework/BaseSystem.hpp>

#include <glm/glm.hpp>

#include <string>
#include <map>

namespace Core
{
    class HoverTextSystem : public BaseSystem
    {
        private:
            int nextId;
            std::map< int,std::string > m_strings;
            
        public:
            HoverTextSystem(); 

            virtual void Update( float delta ) override;

		    virtual const char* GetHumanName(){ return "HoverTextSystem"; }


            int AllocateString( const char * string );
            /*!
                Generate a new string object, 
                the offset is a hint to the rasterizer if the text block should be
                offset within it's bounderies (e.g. scrolling text blocks)
                dimension is specification for the maximum width and height if the text block
            */
            int AllocateString( const char * string, glm::vec2 offset, glm::vec2 dimensions );

            void SetString( int id, const char * string );
            void SetString( int id, const char * string, glm::vec2 offset, glm::vec2 dimensions );
            void FreeString( int id );
            glm::vec2 GetStringDimensions( int id );
    };
}

#endif
