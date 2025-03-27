#include "id_pool.hpp"

uint32_t IdPool::acquireId()
{
    if (!availableIds_.empty())
    {
        uint32_t id = availableIds_.front();
        availableIds_.pop();
        return id;
    }
    return nextId_++;
}

void IdPool::releaseId(uint32_t id)
{
    availableIds_.push(id);
}
