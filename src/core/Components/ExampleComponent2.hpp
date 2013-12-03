#ifndef SRC_CORE_COMPONENTS_EXAMPLECOMPONENT2_H
#define SRC_CORE_COMPONENTS_EXAMPLECOMPONENT2_H

namespace Core
{
    struct ExampleComponent2
    {
        float v;

    
        static ExampleComponent2 D2() 
        { 
            ExampleComponent2 v; 
            v.v = 1; 
            return v;
        }
    };
}

#endif
