#ifndef CORE_CONTENT_MANAGEMENT_CONTENT_MANAGER_DEFINES_HPP
#define CORE_CONTENT_MANAGEMENT_CONTENT_MANAGER_DEFINES_HPP

#include <future>
#include <atomic>
#include <functional>
#include <vector>
#include <tuple>

namespace Core
{
    class BaseAssetLoader;

    enum AssetStatus
    {
        CACHED,
        UNCACHED,
        LOADING
    };

    typedef void* AssetHandle;
    //numberReferences, loaderId, assetHash, assetHandle, assetStatus
    typedef std::vector<std::tuple<short, unsigned int, unsigned int, Core::AssetHandle, Core::AssetStatus>> AssetVector;
    //loaderId, assetHash, assetPromise, loadingDone
    typedef std::vector<std::tuple<unsigned int, unsigned int, std::promise<Core::AssetHandle>*, std::atomic_bool*>> LoadingVector;
    //loaderId, assetSharedFuture, finisherFunction
    typedef std::vector<std::tuple<unsigned int, std::future<AssetHandle>*, std::function<void(Core::BaseAssetLoader*, Core::AssetHandle)>>> AsyncVector;
    //loader, assetHandle, finisherFunction
    typedef std::vector<std::tuple<Core::BaseAssetLoader*, Core::AssetHandle, std::function<void(Core::BaseAssetLoader*, Core::AssetHandle)>>> FinisherVector;
}

#endif
