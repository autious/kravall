#include "LuaParticleBridge.hpp"

#include <Lua/LuaUtility.hpp>
#include <Lua/LuaMetatableTypes.hpp>
#include <Lua/Bridges/ContentLoading/ContentHandle.hpp>

#include <World.hpp>
#include <Systems/EngineSupport/ParticleSystem.hpp>

extern "C"
{
    static int LuaParticleEq(lua_State* L)  
    {
        LuaParticleDefinition* rhs = luau_checkparticledefinition(L, 1);
        LuaParticleDefinition* lhs = luau_checkparticledefinition(L, 2);
        
        lua_pushboolean(L, rhs->handle == lhs->handle);
        return 1;
    }

    static int LuaEmitterEq(lua_State* L)
    {
        Core::EmitterType* rhs = luau_checkemittertype(L, 1);
        Core::EmitterType* lhs = luau_checkemittertype(L, 2);

        lua_pushboolean(L, (*rhs) == (*lhs));
        return 1;
    }

    static int LuaCreateParticleDefinition(lua_State* L)
    {
        Core::ParticleSystem* particleSystem = Core::world.m_systemHandler.GetSystem<Core::ParticleSystem>();
        if(lua_gettop(L) == 2)
        {
            int numberOfParticles = static_cast<int>(luaL_checkint(L, 1));
            unsigned long long int materialId = static_cast<unsigned int>(luaL_checkint(L, 2));

            LuaParticleDefinition* luaParticleDef = Core::LuaUNewParticleDefinition(L);
            luaParticleDef->handle = particleSystem->CreateParticle(numberOfParticles, materialId);
            return 1;
        }
        else
        {
            return luaL_error(L, "createParticleDefinition(numberOfParticles, [materialId]) expects 2 parameters");
        }
    }
}

namespace Core
{
    LuaParticleBridge::LuaParticleBridge(lua_State* L)
    {
        int sanity = lua_gettop(L);

        luaL_newmetatable(L, PARTICLE_DEFINITION_META_TYPE);
            luau_setfunction(L, "__eq", LuaParticleEq);
        lua_pop(L, 1);

        luaL_newmetatable(L, EMITTER_TYPE_META_TYPE);
            luau_setfunction(L, "__eq", LuaEmitterEq);
        lua_pop(L, 1);


        lua_getglobal(L, "core");
            lua_getfield(L, -1, "system");
                lua_newtable(L);
                    luau_setfunction(L, "createParticleDefinition", LuaCreateParticleDefinition);                    
                        lua_newtable(L);
                        Core::EmitterType* type = LuaUNewEmitterType(L);
                        (*type) = Core::EmitterType::CONE_EMITTER;
                        lua_setfield(L, -2, "Cone");
                        type = LuaUNewEmitterType(L);
                        (*type) = Core::EmitterType::SURFACE_EMITTER;
                        lua_setfield(L, -2, "Surface");
                    lua_setfield(L, -2, "emitters");
                lua_setfield(L, -2, "particle");
        lua_pop(L, 2);

        assert(sanity == lua_gettop(L));
    }
}
