#include "PaperPlaneBC.h"
#include "../../Entities/Entity.h"

PaperPlaneBC::PaperPlaneBC(D3D& d3d)
    : m_fireSystem(d3d, "flameParticleEffect.txt"),
    m_onFire(false),
    m_fireTime(0.0f)
{
}


PaperPlaneBC::~PaperPlaneBC(void)
{
}


void PaperPlaneBC::Update(float time)
{
    // Ensure the particle system's parent is set as the same as this components before updating.
    m_fireSystem.SetParent(this->GetParent());
    m_fireSystem.Update(time);

    UpdateFire(time);   
}

void PaperPlaneBC::UpdateFire(float time)
{
    //ParticleSystemComponent* fireComp = 0;
    if(!m_onFire)
    {
        CollisionComponent* collisionComp = 
            (CollisionComponent*)GetParent().GetComponent("CollisionComponent");
        if(collisionComp)
        {
            if(collisionComp->IsColliding())
            {
                m_fireSystem.Start();
                m_onFire = true;              
            }
        }
    }
    else
    {
        // Currently on fire. 
        m_fireTime += time;
        if(m_fireTime > 5.0f)
        {
            // Turn fire off.
            m_fireTime = 0.0f;
            m_onFire = false;
            m_fireSystem.Stop();
            /*if(fireComp)
            {
                fireComp->Stop();
            }*/
            // TODO: Get Path component and respawn!
            FollowPathComponent* pathComp = 
                static_cast<FollowPathComponent*>(GetParent().GetComponent("PathComponent"));
            if(pathComp)
            {
                pathComp->Restart();
            }

        }
    }
}

void PaperPlaneBC::Draw(D3D& d3d)
{
    m_fireSystem.Draw(d3d);
}
