#pragma once
// My Includes
#include "../Component.h"
#include "../../Entities/Entity.h"

// STL
#include <vector>

class FollowPathComponent : public Component
{
public:
    FollowPathComponent(void);
    ~FollowPathComponent(void);

    void FamilyID(componentId_t& out) const { out = "PathComponent"; }
    void ComponentID(componentId_t& out) const { out = "BasicPathComponent"; }

    void Update(float time);

    /**
     * Add the Entity 'newNode' to the next position in the path.
     */
    void AddNode(Entity const* newNode);

    /**
     * Clears all nodes currently on the path.
     */
    void ClearNodes();

private:
    int GetNextNode();

    glm::vec3 CalculateBezierPoint(float t, const glm::vec3& p0, const glm::vec3& p1, 
                                   const glm::vec3& p2, const glm::vec3& p3) const;

private:
    std::vector<Entity const*> m_nodes;
    int                        m_nextNode;
    float                      m_speed;
    float                      m_t;
};

