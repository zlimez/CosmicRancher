#include <queue>
#include "id_pool.hpp"

uint32_t IdPool::acquireId()
{
    if (!availableIds.empty())
    {
        uint32_t id = availableIds.front();
        availableIds.pop();
        return id;
    }
    return nextId++;
}

void IdPool::releaseId(uint32_t id)
{
    availableIds.push(id);
}
