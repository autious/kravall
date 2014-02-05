#ifndef SRC_CORE_SYSTEMS_HOVERTEXTSYSTEM_H
#define SRC_CORE_SYSTEMS_HOVERTEXTSYSTEM_H

#include <ComponentFramework/BaseSystem.hpp>

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
            void SetString( int id, const char * string );
            void FreeString( int id );
    };
}

#endif
