#include "LuaUnitOrderBridge.hpp"

#include <Lua/LuaUtility.hpp>
#include <Lua/LuaMetatableTypes.hpp>

#include <World.hpp>

extern "C"
{
    static int LuaAttackGroup(lua_State * L)
    {
		if(lua_gettop(L) == 2)
        {
			int nSquads = 0;
            int* squads = nullptr;
			int targetGroup = -1;

			if(lua_istable(L, 1))
            {   
                lua_pushnil(L);
                while(lua_next(L, 1))
                {
                    lua_pop(L, 1);
                    ++nSquads;
                }

                squads = Core::world.m_frameHeap.NewPODArray<int>(nSquads);

                lua_pushnil(L);
                for(int i=0; lua_next(L, 1); ++i)
                {
                    squads[i] = static_cast<int>(luaL_checknumber(L, -1));
                    lua_pop(L, 1);
                }
            }
            else
            {
                return luaL_error(L, "argument 1 of disableOutline is not a table");
            }


			if( lua_isnumber( L, 2 ) )
			{
				targetGroup = static_cast<int>(luaL_checknumber( L, 2 ));
			}
			else
			{
				return luaL_error(L, "argument 2 of LuaAttackGroup is not a number");
			}

			Core::SquadSystem* squadSystem = Core::world.m_systemHandler.GetSystem<Core::SquadSystem>();
			if( targetGroup < 0 )
			{
				targetGroup = std::numeric_limits<int>::max();
				for( int i = 0; i < nSquads; i++ )
					squadSystem->SetSquadTargetGroup( squads[i], targetGroup );

				squadSystem->SetSquadStance( squads, nSquads, Core::PoliceStance::Passive );
			}
			else
			{
				for( int i = 0; i < nSquads; i++ )
					squadSystem->SetSquadTargetGroup( squads[i], targetGroup );
			}

			return 0;
        }
        else
        {
            return luaL_error(L, "LuaAttackGroup( targetGroup, [SelectedGroups] )  expects 2 parameters");
        }
    }

}

namespace Core
{
    LuaUnitOrderBridge::LuaUnitOrderBridge(lua_State* L)
    {
		int stackpos = lua_gettop( L );

		lua_getglobal( L, "core" );
		int coreTableIndex = lua_gettop( L );

		if( lua_isnil( L, -1 ) == false )
		{
			lua_pushstring( L, "orders" );
			lua_newtable( L ); // new table
			int movementDataTable = lua_gettop( L );

				luau_setfunction( L, "attackGroup", LuaAttackGroup );

			lua_settable( L, coreTableIndex );
		}
		else
		{
			LOG_ERROR << "Missing core table" << std::endl; 
		}

		lua_pop( L, 1 );

		assert( stackpos == lua_gettop( L ) );
    }

}
