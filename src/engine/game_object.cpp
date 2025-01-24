#include "./game_object.hpp"

void GameObject::addComponent(std::unique_ptr<Component> component)
{
    component->gameObject = this;
    component->transform = &transform;
    components.push_back(std::move(component));
}

void GameObject::removeComponent(std::unique_ptr<Component> component)
{
}

void GameObject::start()
{
    for (auto &component : components)
        component->start();
}

void GameObject::update()
{
    for (auto &component : components)
        component->update();
}

void GameObject::destroy()
{
    for (auto &component : components)
        component->destroy();
    components.clear();
}

IdPool GameObject::idPool;
