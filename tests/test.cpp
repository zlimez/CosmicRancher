#include <iostream>

using namespace std;

struct Component
{
    static int GetNewID()
    {
        static int lastID = 0;
        return lastID++;
    }
};

struct PositionComponent : public Component
{
};
struct VelocityComponent : public Component
{
};

template <typename T>
int GetComponentID()
{
    static int id = Component::GetNewID();
    return id;
}
