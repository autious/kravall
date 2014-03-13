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

			Core::PoliceStance returnStance = Core::PoliceStance::Passive;

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
                return luaL_error(L, "argument 1 of attackGroup is not a table");
            }


			if( lua_isnumber( L, 2 ) )
			{
				targetGroup = static_cast<int>(luaL_checknumber( L, 2 ));
			}
			else
			{
				return luaL_error(L, "argument 2 of attackGroup is not a number");
			}

			Core::SquadSystem* squadSystem = Core::world.m_systemHandler.GetSystem<Core::SquadSystem>();
			if( targetGroup < 0 )
			{
				targetGroup = std::numeric_limits<int>::max();
				for( int i = 0; i < nSquads; i++ )
					squadSystem->SetSquadTargetGroup( squads[i], targetGroup );

				returnStance = squadSystem->RevertSquadStanceFromAgressive( squads, nSquads );

				Core::PoliceStance* stance = (Core::PoliceStance*)lua_newuserdata( L, sizeof( Core::PoliceStance ) );
				*stance = returnStance;        
				luaL_newmetatable( L, POLICE_STANCE_META_TYPE );
				lua_setmetatable( L, -2 );
    
				return 1;
			}
			else
			{
				for( int i = 0; i < nSquads; i++ )
					squadSystem->SetSquadTargetGroup( squads[i], targetGroup );

				squadSystem->SetSquadStance( squads, nSquads, Core::PoliceStance::Aggressive );
				return 0;
			}
        }
        else
        {
            return luaL_error(L, "attackGroup( targetGroup, [SelectedGroups] )  expects 2 parameters");
        }
		return 0;
    }

	static int LuaHaltOrder(lua_State * L)
    {
		if(lua_gettop(L) == 1)
        {
			int nSquads = 0;
            int* squads = nullptr;

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
                return luaL_error(L, "argument 1 of haltGroup is not a table");
            }


			Core::SquadSystem* squadSystem = Core::world.m_systemHandler.GetSystem<Core::SquadSystem>();

			int targetGroup = std::numeric_limits<int>::max();
			for( int i = 0; i < nSquads; i++ )
				squadSystem->SetSquadTargetGroup( squads[i], targetGroup );
			
			squadSystem->StopGroup( squads, nSquads );


			return 0;
        }
        else
        {
            return luaL_error(L, "haltGroup( [groupsToHalt] ) expects 1 parameters");
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
				luau_setfunction( L, "haltGroup", LuaHaltOrder );

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
