#include "FollowPathComponent.h"


FollowPathComponent::FollowPathComponent(void)
    : m_nodes(),
      m_nextNode(0),
      m_speed(1.0f)
{
}


FollowPathComponent::~FollowPathComponent(void)
{
}


void FollowPathComponent::Update(float time)
{
    if(m_nodes.size() > m_nextNode)
    {
        glm::vec3 velocity = m_nodes[m_nextNode]->GetPos() - GetParent().GetPos();
        velocity = glm::normalize(velocity) * m_speed * time;

        GetParent().MoveGlobalX(velocity.x);
        GetParent().MoveGlobalY(velocity.y);
        GetParent().MoveGlobalZ(velocity.z);

        if( glm::distance( m_nodes[m_nextNode]->GetPos(), GetParent().GetPos()) < 0.05f)
        {
            // Reached the destination... get next node to head to.
            GetNextNode();
        }
    }
}


int FollowPathComponent::GetNextNode()
{
    if(++m_nextNode >= m_nodes.size())
    {
        m_nextNode = 0;
    }
    return m_nextNode;
}


void FollowPathComponent::AddNode(Entity const* newNode)
{
    // Check the pointer given is not null before pushing onto the vector.
    if(!newNode)
    {
        return;
    }
    m_nodes.push_back(newNode);
}


void FollowPathComponent::ClearNodes()
{
    m_nodes.clear();
}
