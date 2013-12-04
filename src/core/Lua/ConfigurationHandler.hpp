#ifndef SRC_LUA_CONFIGURATIONHANDLER_H
#define SRC_LUA_CONFIGURATIONHANDLER_H

#include <glm/glm.hpp>
#include <string>

namespace Core
{
    class LuaState;
    /*!
        ConfigurationHandler. Handles getting and setting config values in the lua state under the
        core.config table. All values are also accesible from the lua runtime environment.
    */
    class ConfigurationHandler
    {
    private: 
        LuaState* m_luaState;
    public:
        ConfigurationHandler( LuaState* state );

        std::string GetString( const char *name, const char *fallback );
        double      GetDouble( const char *name, double fallback );
        int         GetInt( const char *name, int fallback );
        bool        GetBool( const char *name, bool fallback );
        glm::vec2   GetVec2( const char *name, glm::vec2 fallback );
        glm::vec3   GetVec3( const char *name, glm::vec3 fallback );
        glm::vec4   GetVec4( const char *name, glm::vec4 fallback );
         

        void        SetString( const char *name, const char *value );
        void        SetDouble( const char *name, double& value );
        void        SetInt( const char *name, int& value );
        void        SetBool( const char *name, bool& value );
        void        SetVec2( const char *name, glm::vec2& value );
        void        SetVec3( const char *name, glm::vec3& value );
        void        SetVec4( const char *name, glm::vec4& value );

    private: 
        void GetVecX( const char *name, float *val, int count );
        void PushConfigTable();
        void PopConfigTable();
    };
}
#endif
