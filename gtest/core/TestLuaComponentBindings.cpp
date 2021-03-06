#ifdef RUN_GTEST
#include <gtest/gtest.h>
#include <Lua/LuaState.hpp>
#include <World.hpp>
#include <Lua/Bridges/LuaGFXBridge.hpp>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

TEST( LuaBindingDataRetention, GraphicsComponentBinding )
{
    const char * lua_set = "test_entity = core.entity.create( core.componentType.GraphicsComponent )\n"
                           "test_entity:set( core.componentType.GraphicsComponent, { mesh = 10, material = 9, type = core.gfx.objectTypes.OpaqueGeometry, render = true, outlineColor = {1,1,1,1}, layer = core.gfx.layerTypes.MeshLayer } )\n";


    const char * lua_get = "local function elems( table )\n"
                           "    local i = 0\n"
                           "    for k,v in pairs( table ) do\n"
                           "        i = i + 1\n"
                           "    end\n"
                           "    return i\n"
                           "end\n"
                           "local data = test_entity:get( core.componentType.GraphicsComponent )\n"
                           "test_entity:destroy()\n"
                           "test_entity = nil\n"
                           "return data.layer, data.outlineColor, data.render, data.type, data.material, data.mesh, elems(data)\n";

    Core::world.m_luaState.DoBlock( lua_set );

    lua_State * L  = Core::world.m_luaState.GetState();

    int values = Core::world.m_luaState.DoBlock( lua_get, 0, 7 );
    ASSERT_LE( 0, values );

    int count = lua_tointeger( L , -1 );
    EXPECT_EQ( 6, count ); 
    int data = lua_tointeger( L, -2 );
    EXPECT_EQ( data, 10 );
    data = lua_tointeger( L, -3 );
    EXPECT_EQ( data, 9 );
    EXPECT_TRUE( lua_isuserdata(  L, -4 ) );
    bool render = lua_toboolean( L, -5 );
    EXPECT_EQ( true, render );
    EXPECT_TRUE( lua_istable(L, -6) );
    EXPECT_TRUE( lua_isuserdata(L, -7));
    
    lua_pop( Core::world.m_luaState.GetState(), values );
}

TEST( LuaBindingDataRetention, RotationComponentBinding  )
{
    const char * lua_set = "test_entity = core.entity.create( core.componentType.RotationComponent )\n"
                           "test_entity:set( core.componentType.RotationComponent, { rotation = {1,2,3,4} } )\n";


    const char * lua_get = "local function elems( table )\n"
                           "    local i = 0\n"
                           "    for k,v in pairs( table ) do\n"
                           "        i = i + 1\n"
                           "    end\n"
                           "    return i\n"
                           "end\n"
                           "local data = test_entity:get( core.componentType.RotationComponent )\n"
                           "test_entity:destroy()\n"
                           "test_entity = nil\n"
                           "return data.rotation[1], data.rotation[2], data.rotation[3], data.rotation[4], elems( data )\n";

    Core::world.m_luaState.DoBlock( lua_set );

    int values = Core::world.m_luaState.DoBlock( lua_get, 0, 5 );
    ASSERT_LE( 0, values );

    int count = lua_tointeger(Core::world.m_luaState.GetState(), -1 );
    EXPECT_EQ( count, 1 );

    int data = lua_tointeger(Core::world.m_luaState.GetState(), -2 );
    EXPECT_EQ( data, 4 );

    data = lua_tointeger(Core::world.m_luaState.GetState(), -3 );
    EXPECT_EQ( data, 3 );

    data = lua_tointeger(Core::world.m_luaState.GetState(), -4 );
    EXPECT_EQ( data, 2 );

    data = lua_tointeger(Core::world.m_luaState.GetState(), -5 );
    EXPECT_EQ( data, 1 );

    lua_pop( Core::world.m_luaState.GetState(), values );
}

TEST( LuaBindingDataRetention, ScaleComponentBinding )
{
    const char * lua_set = "test_entity = core.entity.create( core.componentType.ScaleComponent )\n"
                           "test_entity:set( core.componentType.ScaleComponent, { scale = 2 } )\n";


    const char * lua_get = "local function elems( table )\n"
                           "    local i = 0\n"
                           "    for k,v in pairs( table ) do\n"
                           "        i = i + 1\n"
                           "    end\n"
                           "    return i\n"
                           "end\n"
                           "local data = test_entity:get( core.componentType.ScaleComponent )\n"
                           "test_entity:destroy()\n"
                           "test_entity = nil\n"
                           "return data.scale[1], data.scale[2], data.scale[3], elems( data )\n";

    Core::world.m_luaState.DoBlock( lua_set );

    int values = Core::world.m_luaState.DoBlock( lua_get, 0, 4 );

    ASSERT_LE( 0, values );
    int count = lua_tointeger( Core::world.m_luaState.GetState(), -1 );
    EXPECT_EQ( 1, count ); 
    int data = lua_tonumber(Core::world.m_luaState.GetState(), -2 );
    EXPECT_EQ( data, 2 );
    data = lua_tonumber(Core::world.m_luaState.GetState(), -3 );
    EXPECT_EQ( data, 2 );
    data = lua_tonumber(Core::world.m_luaState.GetState(), -4 );
    EXPECT_EQ( data, 2 );
    lua_pop( Core::world.m_luaState.GetState(), values );
}

TEST( LuaBindingDataRetention, WorldPositionComponentBinding )
{
    const char * lua_set = "test_entity = core.entity.create( core.componentType.WorldPositionComponent )\n"
                           "test_entity:set( core.componentType.WorldPositionComponent, { position = { 3,2,1 } } )\n";


    const char * lua_get = "local function elems( table )\n"
                           "    local i = 0\n"
                           "    for k,v in pairs( table ) do\n"
                           "        i = i + 1\n"
                           "    end\n"
                           "    return i\n"
                           "end\n"
                           "local data = test_entity:get( core.componentType.WorldPositionComponent )\n"
                           "test_entity:destroy()\n"
                           "test_entity = nil\n"
                           "return data.position[1],data.position[2],data.position[3], elems( data )\n";

    Core::world.m_luaState.DoBlock( lua_set );

    int values = Core::world.m_luaState.DoBlock( lua_get, 0, 4 );

    ASSERT_LE( 0, values );
    int count = lua_tointeger( Core::world.m_luaState.GetState(), -1 );
    EXPECT_EQ( 1, count ); 

    int data = lua_tointeger(Core::world.m_luaState.GetState(), -2 );
    EXPECT_EQ( data,1);
    data = lua_tointeger(Core::world.m_luaState.GetState(), -3 );
    EXPECT_EQ( data,2);
    data = lua_tointeger(Core::world.m_luaState.GetState(), -4 );
    EXPECT_EQ( data,3);

    lua_pop( Core::world.m_luaState.GetState(), values );
}

#endif
