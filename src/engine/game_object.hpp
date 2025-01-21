#pragma once

#include "transform.hpp"
#include "component.hpp"
#include "id_pool.hpp"
#include <vector>
#include <memory>

class GameObject
{
private:
    static IdPool idPool;
    uint32_t id;

public:
    Transform transform;
    std::vector<std::shared_ptr<Component>> components;

    GameObject()
    {
        id = idPool.acquireId();
    }

    ~GameObject()
    {
        idPool.releaseId(id);
    }

    template <typename T>
    void addComponent(std::shared_ptr<T> component);

    void start();
    void update();
    void destroy();
};

IdPool GameObject::idPool;