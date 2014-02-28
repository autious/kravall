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

    const char * prep_config = "core = core or {}; core.config = {}";
        

    TEST( LuaConfigurationHandler, GetString )
    {
        LuaState state;
        state.OpenLibs();
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
        state.OpenLibs();
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
        state.OpenLibs();
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
        state.OpenLibs();
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
        state.OpenLibs();
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
        state.OpenLibs();
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
        state.OpenLibs();
        ConfigurationHandler handler(&state);

        state.DoBlock( incorrect_config );

        glm::vec4 fallback = handler.GetVec4( "m_vec4", glm::vec4(10,20,30,40) );

        EXPECT_EQ( glm::vec4(10,20,30,40), fallback );

        state.DoBlock( correct_config );

        glm::vec4 correct = handler.GetVec4( "m_vec4", glm::vec4(10,20,30,40) );

        EXPECT_EQ( glm::vec4(1,2,3,4), correct );
    }

    TEST( LuaConfigurationHandler, SetString )
    {
        LuaState state;
        state.OpenLibs();
        ConfigurationHandler handler(&state);

        state.DoBlock( prep_config );

        handler.SetString( "val", "the" );

        std::string str = handler.GetString( "val", "WRONG" );

        EXPECT_STREQ( "the", str.c_str() );
    }

    TEST( LuaConfigurationHandler, SetDouble )
    {
        LuaState state;
        state.OpenLibs();
        ConfigurationHandler handler(&state);

        state.DoBlock( prep_config );

        double in = 1.5;
        handler.SetDouble( "val", in );

        double val = handler.GetDouble( "val", 5 );

        EXPECT_EQ( in, val );
    }

    TEST( LuaConfigurationHandler, SetInt )
    {
        LuaState state;
        state.OpenLibs();
        ConfigurationHandler handler(&state);

        state.DoBlock( prep_config );

        int in = 1;
        handler.SetInt( "val", in );

        int val = handler.GetInt( "val", 2 );

        EXPECT_EQ( in, val );
    }

    TEST( LuaConfigurationHandler, SetBool )
    {
        LuaState state;
        state.OpenLibs();
        ConfigurationHandler handler(&state);

        state.DoBlock( prep_config );

        bool in = true;
        handler.SetBool( "val", in );

        bool val = handler.GetBool( "val", false );

        EXPECT_EQ( in, val );
    }

    TEST( LuaConfigurationHandler, SetVec2 )
    {
        LuaState state;
        state.OpenLibs();
        ConfigurationHandler handler(&state);

        state.DoBlock( prep_config );

        glm::vec2 in(1,2);
        handler.SetVec2( "val", in );

        glm::vec2 val = handler.GetVec2( "val", glm::vec2(10,20) );

        EXPECT_EQ( in, val );
    }

    TEST( LuaConfigurationHandler, SetVec3 )
    {
        LuaState state;
        state.OpenLibs();
        ConfigurationHandler handler(&state);

        state.DoBlock( prep_config );

        glm::vec3 in(1,2,3);
        handler.SetVec3( "val", in );

        glm::vec3 val = handler.GetVec3( "val", glm::vec3(10,20,30) );

        EXPECT_EQ( in, val );
    }

    TEST( LuaConfigurationHandler, SetVec4 )
    {
        LuaState state;
        state.OpenLibs();
        ConfigurationHandler handler(&state);

        state.DoBlock( prep_config );

        glm::vec4 in(1,2,3,4);
        handler.SetVec4( "val", in );

        glm::vec4 val = handler.GetVec4( "val", glm::vec4(10,20,30,40) );

        EXPECT_EQ( in, val );
    }
}

#endif
