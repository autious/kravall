#ifndef SRC_CORE_COMPONENTS_EXAMPLECOMPONENT1_H
#define SRC_CORE_COMPONENTS_EXAMPLECOMPONENT1_H

namespace Core
{
    struct ExampleComponent1
    {
        float v;

        static ExampleComponent1 D1() { ExampleComponent1 v; v.v = 1; return v;}
    };
}

#endif
