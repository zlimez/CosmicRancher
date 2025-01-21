#include "./game_object.hpp"

template <typename T>
void GameObject::addComponent(std::shared_ptr<T> component)
{
    static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
    component->gameObject = shared_from_this();
    component->transform = &transform;
    components.push_back(component);
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