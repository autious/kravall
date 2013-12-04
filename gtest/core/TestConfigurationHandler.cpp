#ifdef RUN_GTEST
#include <gtest/gtest.h>


#include <Lua/LuaState.hpp>
#include <Lua/ConfigurationHandler.hpp>

#include <string>

namespace Core
{
    const char * incorrect_config =
    "core = core or {}\n"
    "core.config = {\n"
    "m_string = {},"
    "m_double = 'ss',"
    "m_int = 'ss',"
    "m_bool = false,"
    "m_vec2 = 1,"
    "m_vec3 = 1,"
    "m_vec4 = 1}";

    const char * correct_config = 
    "core = core or {}\n"
    "core.config = {\n"
    "m_string = 'TestString',"
    "m_double = 12,"
    "m_int = 12,"
    "m_bool = true,"
    "m_vec2 = {1,2},"
    "m_vec3 = {1,2,3},"
    "m_vec4 = {1,2,3,4}}";
        

    TEST( LuaConfigurationHandler, GetString )
    {
        LuaState state;
        ConfigurationHandler handler(&state);

        state.DoBlock( incorrect_config );

        std::string fallback = handler.GetString( "m_string", "WRONG" );

        EXPECT_STREQ( "WRONG", fallback.c_str() );

        state.DoBlock( correct_config );

        std::string correct = handler.GetString( "m_string", "WRONG" );

        EXPECT_STREQ( "TestString", correct.c_str() );
    }

    TEST( LuaConfigurationHandler, GetDouble )
    {
        LuaState state;
        ConfigurationHandler handler(&state);

        state.DoBlock( incorrect_config );

        double fallback = handler.GetDouble( "m_double", 20.0 );

        EXPECT_EQ( 20.0, fallback );

        state.DoBlock( correct_config );

        double correct = handler.GetDouble( "m_double", 20.0 );

        EXPECT_EQ( 12.0, correct );
    }

    TEST( LuaConfigurationHandler, GetInt )
    {
        LuaState state;
        ConfigurationHandler handler(&state);

        state.DoBlock( incorrect_config );

        int fallback = handler.GetInt( "m_int", 20 );

        EXPECT_EQ( 20, fallback );

        state.DoBlock( correct_config );

        int correct = handler.GetInt( "m_int", 20 );

        EXPECT_EQ( 12, correct );
    }

    TEST( LuaConfigurationHandler, GetBool )
    {
        LuaState state;
        ConfigurationHandler handler(&state);

        state.DoBlock( incorrect_config );

        bool fallback = handler.GetBool( "m_bool", false );

        EXPECT_FALSE( fallback );

        state.DoBlock( correct_config );

        bool correct = handler.GetBool( "m_bool", false );

        EXPECT_TRUE( correct );
    }

    TEST( LuaConfigurationHandler, GetVec2 )
    {
        LuaState state;
        ConfigurationHandler handler(&state);

        state.DoBlock( incorrect_config );

        glm::vec2 fallback = handler.GetVec2( "m_vec2", glm::vec2(10,20) );

        EXPECT_EQ( glm::vec2(10,20), fallback );

        state.DoBlock( correct_config );

        glm::vec2 correct = handler.GetVec2( "m_vec2", glm::vec2(10,20) );

        EXPECT_EQ( glm::vec2(1,2), correct );
    }

    TEST( LuaConfigurationHandler, GetVec3 )
    {
        LuaState state;
        ConfigurationHandler handler(&state);

        state.DoBlock( incorrect_config );

        glm::vec3 fallback = handler.GetVec3( "m_vec3", glm::vec3(10,20,30) );

        EXPECT_EQ( glm::vec3(10,20,30), fallback );

        state.DoBlock( correct_config );

        glm::vec3 correct = handler.GetVec3( "m_vec3", glm::vec3(10,20,30) );

        EXPECT_EQ( glm::vec3(1,2,3), correct );
    }

    TEST( LuaConfigurationHandler, GetVec4 )
    {
        LuaState state;
        ConfigurationHandler handler(&state);

        state.DoBlock( incorrect_config );

        glm::vec4 fallback = handler.GetVec4( "m_vec4", glm::vec4(10,20,30,40) );

        EXPECT_EQ( glm::vec4(10,20,30,40), fallback );

        state.DoBlock( correct_config );

        glm::vec4 correct = handler.GetVec4( "m_vec4", glm::vec4(10,20,30,40) );

        EXPECT_EQ( glm::vec4(1,2,3,4), correct );
    }
}

#endif
