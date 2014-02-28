#ifdef RUN_GTEST
#include <gtest/gtest.h>

#include "TestEntityHandlerAux.hpp"

namespace Core
{
    TEST( EntityHandlerTest, VariadicTemplateTest )
    {
        SystemHandler system;
        EntityHandler instance(&system); 
        
        ASSERT_EQ( 0, instance.GetComponentType<Component1>() );
        ASSERT_EQ( 1, instance.GetComponentType<Component2>() );
        ASSERT_EQ( 2, instance.GetComponentType<Component3>() );


        //This should always generate a static assert error.
        //instance.GetComponentType<NotComponent1>();
    }

    TEST( EntityHandlerTest, UnorderedAspectGeneration )
    {
        SystemHandler system;
        EntityHandler instance(&system); 

        Aspect asp1 = instance.GenerateAspect<Component1,Component2,Component3>();
        Aspect asp2 = instance.GenerateAspect<Component2,Component1,Component3>();


        ASSERT_EQ(asp1, asp2);

    }

    TEST( EntityHandlerTest, CorrectlyGeneratedAspect )
    {
        SystemHandler system;
        EntityHandler instance(&system); 

        Aspect asp1 = instance.GenerateAspect<Component1,Component2,Component3>();
        Aspect asp2 = 1ULL << 0 | 1ULL << 1 | 1ULL << 2;

        ASSERT_EQ(asp2, asp1);

        asp1 = instance.GenerateAspect<Component1,Component3>();
        asp2 = 1ULL << 0 |  1ULL << 2;

        ASSERT_EQ(asp2, asp1);

        asp1 = instance.GenerateAspect<Component3,Component1>();
        asp2 = 1ULL << 0 |  1ULL << 2;

        ASSERT_EQ(asp2, asp1);
    }

    TEST( EntityHandlerTest, ComponentRetainData )
    {
        SystemHandler system;
        EntityHandler instance(&system); 
        

        Component1 comp1; comp1.value = 1;
        Component1 comp2; comp2.value = 2;
        Component1 comp3; comp3.value = 3;

        Entity ent1 = instance.CreateEntity<Component1>(comp1);
        Entity ent2 = instance.CreateEntity<Component1>(comp2);
        Entity ent3 = instance.CreateEntity<Component1>(comp3);

        EXPECT_EQ(1,instance.GetComponentTmpPointer<Component1>( ent1 )->value);
        EXPECT_EQ(2,instance.GetComponentTmpPointer<Component1>( ent2 )->value);
        EXPECT_EQ(3,instance.GetComponentTmpPointer<Component1>( ent3 )->value);
    }

    TEST( EntityHandlerTest, CleanRelease )
    {
        SystemHandler system;
        EntityHandler instance(&system); 
        
        Entity ent1 = instance.CreateEntity<Component1,Component3>(Component1(),Component3());
        Entity ent2 = instance.CreateEntity<Component1,Component2>(Component1(),Component2());
        Entity ent3 = instance.CreateEntity<Component1,Component2,Component3>(Component1(),Component2(),Component3());
        Entity ent4 = instance.CreateEntity<Component1>(Component1());

        EXPECT_EQ(4,instance.GetEntityCount());
        EXPECT_EQ(8,instance.GetComponentCount());

        instance.DestroyEntity( ent1 );

        EXPECT_EQ(3,instance.GetEntityCount());
        EXPECT_EQ(6,instance.GetComponentCount());

        instance.DestroyEntity( ent3 );

        EXPECT_EQ(2,instance.GetEntityCount());
        EXPECT_EQ(3,instance.GetComponentCount());

        instance.DestroyEntity( ent2 );

        EXPECT_EQ(1,instance.GetEntityCount());
        EXPECT_EQ(1,instance.GetComponentCount());

        instance.DestroyEntity( ent4 );

        EXPECT_EQ(0,instance.GetEntityCount());
        EXPECT_EQ(0,instance.GetComponentCount());
    }

    TEST( EntityHandlerTest, AddAndRemoveComponentsDynamic )
    {
        SystemHandler system;
        EntityHandler entity( &system );

        Entity ent1 = entity.CreateEntity();
        Entity ent2 = entity.CreateEntity();

        entity.AddComponentsAspect( ent1, 1ULL << 1 | 1ULL << 2 );
        entity.AddComponentsAspect( ent2, 1ULL << 0 | 1ULL << 2 );

        ASSERT_EQ( (Aspect)(entity.GenerateAspect<Component2,Component3>()), entity.GetEntityAspect( ent1 ) );
        ASSERT_EQ( (Aspect)(1ULL << 0 | 1ULL << 2), entity.GetEntityAspect( ent2 ) );

        entity.RemoveComponentsAspect( ent1, 1ULL << 1 );
        entity.RemoveComponentsAspect( ent2, 1ULL << 2 );

        ASSERT_EQ( (Aspect)(entity.GenerateAspect<Component3>()), entity.GetEntityAspect( ent1 ) );
        ASSERT_EQ( (Aspect)(1ULL << 0), entity.GetEntityAspect( ent2 ) );

        entity.RemoveComponentsAspect( ent1, 1ULL << 2 );
        entity.RemoveComponentsAspect( ent2, 1ULL << 0 );

        ASSERT_EQ( (Aspect)(0ULL), entity.GetEntityAspect( ent1 ) );
        ASSERT_EQ( (Aspect)(0ULL), entity.GetEntityAspect( ent2 ) );
    }

    TEST( EntityHandlerTest, AddAndRemoveComponentsTemplate )
    {
        SystemHandler system;
        EntityHandler entity( &system );

        Entity ent1 = entity.CreateEntity();
        Entity ent2 = entity.CreateEntity();

        entity.AddComponents<Component2,Component3>( ent1, Component2(), Component3() );
        entity.AddComponents<Component1,Component3>( ent2, Component1(), Component3() );

        ASSERT_EQ( (Aspect)(entity.GenerateAspect<Component2,Component3>()), entity.GetEntityAspect( ent1 ) );
        ASSERT_EQ( (Aspect)(1ULL << 0 | 1ULL << 2), entity.GetEntityAspect( ent2 ) );

        entity.RemoveComponents<Component2>( ent1 );
        entity.RemoveComponents<Component3>( ent2 );

        ASSERT_EQ( (Aspect)(entity.GenerateAspect<Component3>()), entity.GetEntityAspect( ent1 ) );
        ASSERT_EQ( (Aspect)(1ULL << 0), entity.GetEntityAspect( ent2 ) );

        entity.RemoveComponents<Component3>( ent1 );
        entity.RemoveComponents<Component1>( ent2 );

        ASSERT_EQ( (Aspect)(0ULL), entity.GetEntityAspect( ent1 ) );
        ASSERT_EQ( (Aspect)(0ULL), entity.GetEntityAspect( ent2 ) );
    }
}
#endif
