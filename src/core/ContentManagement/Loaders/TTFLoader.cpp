#include "TTFLoader.hpp"


namespace Core
{
    TTFLoader::TTFLoader()
    {

    }

    TTFLoader::~TTFLoader()
    {

    }

    void* TTFLoader::Load(const char* assetName)
    {
        return nullptr;
    }

    void* TTFLoader::LoadAsync(const char* assetName, Core::FinisherVector& finisherList, std::mutex& finisherMutex)
    {
        return nullptr;
    }

    void TTFLoader::Destroy(const Core::AssetHandle assetHandle)
    {

    }
}
