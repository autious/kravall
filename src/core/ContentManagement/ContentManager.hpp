#ifndef CORE_CONTENT_MANAGEMENT_CONTENT_MANAGER_HPP
#define CORE_CONTENT_MANAGEMENT_CONTENT_MANAGER_HPP

#include <ContentManagement/ContentManagerTemplate.hpp>

namespace Core
{
    typedef Core::ContentManagerTemplate<Core::TTFLoader, Core::GnomeLoader, Core::NavigationMeshLoader> ContentManager;
}

#endif
