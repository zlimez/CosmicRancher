#pragma once

#include "game_object.hpp"
#include <vector>
#include <string>

class Scene
{
private:
    std::string sceneName;
    std::vector<GameObject> gameObjects;

public:
    void load();
    void unload();
};