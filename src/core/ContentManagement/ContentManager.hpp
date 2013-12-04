#ifndef CORE_CONTENT_MANAGEMENT_CONTENT_MANAGER_HPP
#define CORE_CONTENT_MANAGEMENT_CONTENT_MANAGER_HPP

#include <ContentManagement/ContentManagerTemplate.hpp>

#include <ContentManagerment/Loaders/TTFLoader.hpp>

namespace Core
{
    typedef Core::ContentManagerTemplate<Core::TTFLoader> ContentManager;
}

#endif
