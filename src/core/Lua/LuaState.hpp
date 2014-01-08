#ifndef SRC_CORE_LUA_MAINLUA_H
#define SRC_CORE_LUA_MAINLUA_H

#include <lua.h>

#include <chrono>


namespace Core
{

    struct LuaStateBindings;
    class ConfigurationHandler;
    /*! 
        lua_State wrapper, to make the lua interface slightly more object oriented.
    */
    class LuaState
    {
        private:
            friend class ConfigurationHandler;
            lua_State *m_state;
            std::chrono::microseconds m_lastFrameTime;
            int m_coreUpdateFunctionReg;
            bool m_activeUpdate;

            void VerifyUpdateFunction();
            
        public:
            LuaState();
            ~LuaState();

            bool Execute( const char *filename ); 
            bool DoBlock( const char *block );
            int DoBlock( const char * block, int args, int rargs );

            void Update( float delta );

            std::chrono::microseconds GetUpdateTiming();
            int GetMemoryUse();

            lua_State *GetState();

        
            LuaStateBindings *bindings;

    };
}
#endif
