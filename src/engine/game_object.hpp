#pragma once

#include "transform.hpp"
#include "id_pool.hpp"
#include <vector>
#include <memory>

class GameObject;
class Component;

class Component
{
public:
    GameObject *gameObject;
    Transform *transform;

    virtual ~Component() {}
    virtual void start();
    virtual void update();
    virtual void destroy();
};

class GameObject
{
private:
    static IdPool idPool;
    uint32_t id;

public:
    Transform transform;
    std::vector<std::unique_ptr<Component>> components;

    GameObject()
    {
        id = idPool.acquireId();
    }

    ~GameObject()
    {
        idPool.releaseId(id);
    }

    void addComponent(std::unique_ptr<Component> component);
    void removeComponent(std::unique_ptr<Component> component);

    void start();
    void update();
    void destroy();
};