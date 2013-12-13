#ifndef SRC_CORE_COMPONENTS_EXAMPLECOMPONENT1_HPP
#define SRC_CORE_COMPONENTS_EXAMPLECOMPONENT1_HPP

namespace Core
{
    struct ExampleComponent1
    {
        float v;

        static ExampleComponent1 D1() { ExampleComponent1 v; v.v = 1; return v;}
    };
}

#endif
