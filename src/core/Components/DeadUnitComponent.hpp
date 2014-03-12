#ifndef SRC_CORE_COMPONENTS_DEADUNITCOMPONENT_H
#define SRC_CORE_COMPONENTS_DEADUNITCOMPONENT_H

namespace Core
{
    struct DeadUnitComponent
    {

        DeadUnitComponent()
        {
            this->groupID = -1;
        }

        DeadUnitComponent( int groupID)
        {
            this->groupID = groupID;
        }

        int groupID;

        inline static const char* GetName()
        {
            return "DeadUnitComponent";
        }
    };
}

#endif
