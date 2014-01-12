#pragma once
#include "../Component.h"

enum CollisionType 
{
    BoundingBox,
    BoundingSphere,
    Mesh
};

struct CollisionComponentDesc
{
    CollisionType collisionType;
    float         radius;
    float         width;
    float         height;
    float         depth;
};

class CollisionComponent : public Component
{
public:
    explicit CollisionComponent(CollisionComponentDesc& desc);
    ~CollisionComponent(void);

    void FamilyID(componentId_t& id) const            { id = "CollisionComponent"; }
    virtual void ComponentID(componentId_t& id) const { id = "BasicCollisionComponent"; }

    virtual void Update(float time);

    void SetCollisionComponentDesc(CollisionComponentDesc& desc) { m_desc = desc; }
    const CollisionComponentDesc& GetCollisionComponentDesc() { return m_desc; }

    bool IsColliding()                { return m_colliding; }
    void SetColliding(bool colliding) { m_colliding = colliding; }

    bool CheckCollision(CollisionComponent* other);

private:
    CollisionComponentDesc  m_desc;
    bool                    m_colliding;
    CollisionComponent*     m_collidingWith;
};

