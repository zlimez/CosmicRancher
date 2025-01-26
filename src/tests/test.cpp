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

// Usage
int main()
{
    int positionID = GetComponentID<PositionComponent>();
    int velocityID = GetComponentID<VelocityComponent>();
    cout << positionID << '\n';
    cout << velocityID << '\n';
    positionID = GetComponentID<PositionComponent>();
    cout << positionID << '\n';
}
