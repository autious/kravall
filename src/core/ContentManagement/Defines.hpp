#ifndef CORE_CONTENT_MANAGEMENT_CONTENT_MANAGER_DEFINES_HPP
#define CORE_CONTENT_MANAGEMENT_CONTENT_MANAGER_DEFINES_HPP

#include <future>
#include <functional>
#include <vector>
#include <tuple>

namespace Core
{
    class BaseAssetLoader;

    typedef void* AssetHandle;
    typedef std::vector<std::tuple<short, unsigned int, unsigned int, AssetHandle>> AssetVector;
    typedef std::vector<std::tuple<unsigned int, std::future<AssetHandle>, std::function<void(Core::BaseAssetLoader*, AssetHandle)>>> AsyncVector;
    typedef std::vector<std::tuple<Core::BaseAssetLoader*, AssetHandle, std::function<void(Core::BaseAssetLoader*, AssetHandle)>>> FinisherVector;
}

#endif
