#include "../entityManager.h"
#include <future>
#include <queue>

Application* EntityManager::mApp = nullptr;

uint32_t EntityManager::mEntityCount = 0;
std::unordered_map<size_t, std::unique_ptr<IComponentPool>> EntityManager::mComponentPools =
    std::unordered_map<size_t, std::unique_ptr<IComponentPool>>();
std::unordered_set<uint32_t> EntityManager::mDestroyBuffer = std::unordered_set<uint32_t>();
std::unordered_map<uint32_t, uint32_t> EntityManager::mEntityParentMap = std::unordered_map<uint32_t, uint32_t>();
std::unordered_map<uint32_t, std::set<uint32_t>> EntityManager::mEntityChildMap = std::unordered_map<uint32_t, std::set<uint32_t>>();

void EntityManager::Initialize(Application* app) {
    mApp = app;

    RegisterComponentType<Transform>();
    RegisterComponentType<Sprite>();
    RegisterComponentType<Controller>();
}

void EntityManager::Update(float deltaTime) {
    std::vector<std::future<void>> transformDependencyStage;

    ComponentPool<Controller>* controllers = GetPool<Controller>();
    if (controllers) {
        controllers->UpdateContents(Controller::Update, deltaTime);
    }

    ComponentPool<Transform>* transforms = GetPool<Transform>();
    if (transforms) {
        transforms->UpdateContents(Transform::Update, deltaTime);
    }

    transformDependencyStage.push_back(std::async(std::launch::async, [deltaTime]() {
        ComponentPool<Sprite>* sprites = GetPool<Sprite>();
        if (sprites) {
            sprites->UpdateContents(Sprite::Update, deltaTime);
        }
    }));

    for (auto& future : transformDependencyStage) {
        future.get();
    }

    ClearBuffer();
}

void EntityManager::Shutdown() {
    mApp = nullptr;
}

uint32_t EntityManager::CreateEntity() {
    return mEntityCount++;
}

uint32_t EntityManager::CreateEntity(uint32_t parentEntityId) {
    uint32_t newEntityId = mEntityCount++;
    
    mEntityParentMap[newEntityId] = parentEntityId;
    mEntityChildMap[parentEntityId].insert(newEntityId);

    return newEntityId;
}

void EntityManager::DestroyEntity(uint32_t entityId) {
    std::queue<uint64_t> deletionQueue;
    deletionQueue.push(entityId);

    while (!deletionQueue.empty()) {
        uint32_t currentId = deletionQueue.front();
        deletionQueue.pop();

        if (mDestroyBuffer.find(currentId) != mDestroyBuffer.end()) {
            continue;
        }

        mDestroyBuffer.insert(currentId);
        auto it = mEntityChildMap.find(currentId);
        if (it != mEntityChildMap.end()) {
            for (uint32_t childId : it->second) {
                deletionQueue.push(childId);
            }
        }
    }
}

std::optional<uint32_t> EntityManager::GetParentEntity(uint32_t entityId) {
    auto it = mEntityParentMap.find(entityId);
    if (it == mEntityParentMap.end()) { return std::nullopt; }

    return std::optional<uint32_t>(it->second);
}

std::set<uint32_t>* EntityManager::GetChildrenEntities(uint32_t entityId) {
    auto it = mEntityChildMap.find(entityId);
    if (it == mEntityChildMap.end()) { return nullptr; }

    return &it->second;
}

void EntityManager::ClearBuffer() {
    for (uint32_t entityId : mDestroyBuffer) {
        for (auto& [typeHash, componentPool] : mComponentPools) {
            componentPool->Remove(entityId);
        }

        std::optional<uint32_t> parentId = GetParentEntity(entityId);
        if (parentId.has_value()) {
            GetChildrenEntities(parentId.value())->erase(entityId);
            mEntityParentMap.erase(entityId);
        }
        mEntityChildMap.erase(entityId);
    }

    mDestroyBuffer.clear();
}