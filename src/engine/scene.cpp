#include "scene.hpp"

void Scene::load()
{
    for (auto &gameObject : gameObjects)
        gameObject.start();
}

void Scene::unload()
{
    for (auto &gameObject : gameObjects)
        gameObject.destroy();
}
