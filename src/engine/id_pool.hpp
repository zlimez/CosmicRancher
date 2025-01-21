#pragma once

#include <queue>

class IdPool
{
private:
    std::queue<uint32_t> availableIds;
    uint32_t nextId = 0;

public:
    uint32_t acquireId();
    void releaseId(uint32_t id);
};
