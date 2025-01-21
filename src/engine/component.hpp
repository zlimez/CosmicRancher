#pragma once
#include <memory>

class Component
{
private:
    std::weak_ptr<GameObject> gameObject;
    std::weak_ptr<Transform> transform;

public:
    virtual void start();
    virtual void update();
    virtual void destroy();
};