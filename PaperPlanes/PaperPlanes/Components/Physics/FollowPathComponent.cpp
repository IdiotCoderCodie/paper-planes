#include "FollowPathComponent.h"


FollowPathComponent::FollowPathComponent(void)
    : m_nodes(),
      m_nextNode(3),
      m_speed(1.0f),
      m_t(0.0f)
{
}


FollowPathComponent::~FollowPathComponent(void)
{
}


void FollowPathComponent::Update(float time)
{
    if(m_nodes.size() > m_nextNode)
    {
        m_t += time * 1.0f;
        glm::vec3 nextPoint = CalculateBezierPoint(m_t, 
            m_nodes[m_nextNode-3]->GetPos(), // p0
            m_nodes[m_nextNode-2]->GetPos(), // p1
            m_nodes[m_nextNode-1]->GetPos(), // p2
            m_nodes[m_nextNode]->GetPos());  // p3

        GetParent().SetPos(nextPoint);
/*

        glm::vec3 velocity = m_nodes[m_nextNode]->GetPos() - GetParent().GetPos();
        velocity = glm::normalize(velocity) * m_speed * time;

        GetParent().MoveGlobalX(velocity.x);
        GetParent().MoveGlobalY(velocity.y);
        GetParent().MoveGlobalZ(velocity.z);*/

        if(m_t >= 1.0f)
        {
            GetNextNode();
            m_t = 0.0f;
        }

        //if( glm::distance( m_nodes[m_nextNode]->GetPos(), GetParent().GetPos()) < 0.05f)
        //{
        //    // Reached the destination... get next node to head to.
        //    GetNextNode();
        //}
    }
}


int FollowPathComponent::GetNextNode()
{
    m_nextNode += 3;
    if(m_nextNode >= m_nodes.size())
    {
        m_nextNode = 3;
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


glm::vec3 FollowPathComponent::CalculateBezierPoint(float t, const glm::vec3& p0, const glm::vec3& p1, 
                               const glm::vec3& p2, const glm::vec3& p3) const
{
    // [x,y,z] = ((1-t)^3)*P0 + (3(1-t)^2)t*P1 + (3(1-t)t^2)*P2 + (t^3)*P3
    float d   = 1.0f - t;
    float dd  = d*d;
    float ddd = dd*d;
    float tt  = t*t;
    float ttt = tt*t;

    glm::vec3 point = ddd * p0;
    point += 3 * dd * t * p1;
    point += 3 * d * tt * p2;
    point += ttt * p3;

    return point;
}
