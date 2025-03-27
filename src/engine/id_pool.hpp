#pragma once
#include <queue>

class IdPool
{
private:
    std::queue<uint32_t> availableIds_;
    uint32_t nextId_ = 0;

public:
    uint32_t acquireId();
    void releaseId(uint32_t id);
};
