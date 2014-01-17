#ifndef SRC_CORE_COMPONENTS_NAMECOMPONENT_H
#define SRC_CORE_COMPONENTS_NAMECOMPONENT_H

#include <cstring>
#include <logger/Logger.hpp>
#include <string>

namespace Core
{
    /*!
        Component giving an entity a human readable name, can be 
        used to quickly identifying objects externally
    */
    struct NameComponent
    {
        static const int NAME_LENGTH = 40;
        char name[NAME_LENGTH];

        inline static void SetName( NameComponent& comp, const char * srcName )
        {
            int len = strlen( srcName );
            if( len  >= NAME_LENGTH )
            {
                LOG_WARNING << "NameComponent, Truncating name: " << srcName;
                len = NAME_LENGTH-1;
            }
            memset( comp.name, '\0', NAME_LENGTH * sizeof( char ) );
            memcpy( comp.name, srcName, len * sizeof( char ) );
        };

        inline static std::string GetTrunc( const char * srcName )
        {
            int len = strlen( srcName );

            if( len  >= NAME_LENGTH )
            {
                char tmpName[NAME_LENGTH];
                memset( tmpName, '\0', NAME_LENGTH * sizeof( char ) );
                memcpy( tmpName, srcName, (NAME_LENGTH-1) * sizeof( char ) );

                return std::string( tmpName );
            }
            else
            {
                return std::string( srcName );
            }
        }

        inline static const char* GetName()
        {
            return "NameComponent";
        }
    };

         
}
#endif
