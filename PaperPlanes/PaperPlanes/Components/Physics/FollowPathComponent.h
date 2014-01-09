#pragma once
// My Includes
#include "../Component.h"
#include "../../Entities/Entity.h"

// STL
#include <vector>


class FollowPathComponent : public Component
{
public:
    struct Node
    {
        glm::vec3 position;
        float timeToReach;
        float delay;
    };

    FollowPathComponent(void);
    ~FollowPathComponent(void);

    void FamilyID(componentId_t& out) const { out = "PathComponent"; }
    void ComponentID(componentId_t& out) const { out = "BasicPathComponent"; }

    void Update(float time);

    /**
     * Add Node with given params.
     */
    void AddNode(const glm::vec3& position, float arrivalTime, float delay);
    /**
     * Adds a node at position. Sets arrival time to 1.0f and delay to 0.0f. Mostly useful for 
     * adding control nodes which make no use of arrival time or delay.
     */
    void AddNode(const glm::vec3& position) { AddNode(position, 1.0f, 0.0f); }
    /**
     * Add the node using the provided entities position.
     * Sets arrival time to 1.0f and delay to 0.0f.
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
    std::vector<Node>          m_nodes;
    //std::vector<Entity const*> m_nodes;
    int                        m_nextNode;
    float                      m_speed;
    float                      m_t;
    bool                       m_delayed;
    float                      m_currentDelayTime;
    bool                       m_reverse;
};

