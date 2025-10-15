#include "../entityManager.h"
#include <future>
#include <queue>

Application* EntityManager::mApp = nullptr;

uint64_t EntityManager::entityCount = 0;
std::unordered_map<size_t, std::unique_ptr<IComponentPool>> EntityManager::mComponentPools =
    std::unordered_map<size_t, std::unique_ptr<IComponentPool>>();
std::unordered_set<uint64_t> EntityManager::mDestroyBuffer = std::unordered_set<uint64_t>();
std::unordered_map<uint64_t, uint64_t> EntityManager::mEntityParentMap = std::unordered_map<uint64_t, uint64_t>();
std::unordered_map<uint64_t, std::set<uint64_t>> EntityManager::mEntityChildMap = std::unordered_map<uint64_t, std::set<uint64_t>>();

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

uint64_t EntityManager::CreateEntity() {
    return entityCount++;
}

uint64_t EntityManager::CreateEntity(uint64_t parentEntityId) {
    uint64_t newEntityId = entityCount++;
    
    mEntityParentMap[newEntityId] = parentEntityId;
    mEntityChildMap[parentEntityId].insert(newEntityId);

    return newEntityId;
}

void EntityManager::DestroyEntity(uint64_t entityId) {
    std::queue<uint64_t> deletionQueue;
    deletionQueue.push(entityId);

    while (!deletionQueue.empty()) {
        uint64_t currentId = deletionQueue.front();
        deletionQueue.pop();

        if (mDestroyBuffer.find(currentId) != mDestroyBuffer.end()) {
            continue;
        }

        mDestroyBuffer.insert(currentId);
        auto it = mEntityChildMap.find(currentId);
        if (it != mEntityChildMap.end()) {
            for (uint64_t childId : it->second) {
                deletionQueue.push(childId);
            }
        }
    }
}

std::optional<uint64_t> EntityManager::GetParentEntity(uint64_t entityId) {
    auto it = mEntityParentMap.find(entityId);
    if (it == mEntityParentMap.end()) { return std::nullopt; }

    return std::optional<uint64_t>(it->second);
}

std::set<uint64_t>* EntityManager::GetChildrenEntities(uint64_t entityId) {
    auto it = mEntityChildMap.find(entityId);
    if (it == mEntityChildMap.end()) { return nullptr; }

    return &it->second;
}

void EntityManager::ClearBuffer() {
    for (uint64_t entityId : mDestroyBuffer) {
        for (auto& [typeHash, componentPool] : mComponentPools) {
            componentPool->Remove(entityId);
        }

        std::optional<uint64_t> parentId = GetParentEntity(entityId);
        if (parentId.has_value()) {
            GetChildrenEntities(parentId.value())->erase(entityId);
            mEntityParentMap.erase(entityId);
        }
        mEntityChildMap.erase(entityId);
    }

    mDestroyBuffer.clear();
}