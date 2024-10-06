#include "Physics.h"

Vec2 Physics::getOverlap(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Entity>& object) const
{
    auto& eT = entity->getComponent<CTransform>();
    auto& eSize = entity->getComponent<CBoundingBox>().halfSize;
    auto& ePos = eT.pos; 


    auto& oT = object->getComponent<CTransform>();
    auto& oSize = object->getComponent<CBoundingBox>().halfSize;
    auto& oPos = oT.pos;

    auto& playerState = entity->getComponent<CState>().state;

    auto dx = abs(ePos.x - oPos.x);
    auto dy = abs(ePos.y - oPos.y);


    if (dx <= eSize.x + oSize.x && dy <= eSize.y + oSize.y) 
    {
        return Vec2((eSize.x + oSize.x) - dx, (eSize.y + oSize.y) - dy);
    }
    return Vec2(0, 0);
}

Vec2 Physics::getPrevOverlap(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Entity>& object) const
{
    auto& eT = entity->getComponent<CTransform>();
    auto& eSize = entity->getComponent<CBoundingBox>().halfSize;
    auto& ePos = eT.prevPos;


    auto& oT = object->getComponent<CTransform>();
    auto& oSize = object->getComponent<CBoundingBox>().halfSize;
    auto& oPos = oT.prevPos;

    auto& playerState = entity->getComponent<CState>().state;

    auto dx = abs(ePos.x - oPos.x);
    auto dy = abs(ePos.y - oPos.y);


    if (dx <= eSize.x + oSize.x && dy <= eSize.y + oSize.y)
    {
        return Vec2((eSize.x + oSize.x) - dx, (eSize.y + oSize.y) - dy);
    }
    return Vec2(0, 0);
}

bool Physics::isIntersect(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Entity>& object) const
{
    auto& eT = entity->getComponent<CTransform>(); 
    auto& eBound = entity->getComponent<CBoundingBox>();
    auto& ePos = eT.pos; 
    auto eAnim = entity->getComponent<CAnimation>().animation.getSize();

    auto eAnimSize = Vec2(eAnim.x * abs(eT.scale.x), eAnim.y * abs(eT.scale.y));

    auto& oT = object->getComponent<CTransform>(); 
    auto& oBound = object->getComponent<CBoundingBox>();
    auto& oPos = oT.pos; 
    auto oAnim = object->getComponent<CAnimation>().animation.getSize();

    auto oAnimSize = Vec2(oAnim.x * abs(oT.scale.x), oAnim.y * abs(oT.scale.y));

    sf::IntRect rectEntity((ePos.x - eAnimSize.x/2) + eBound.offRect.x, (ePos.y - eAnimSize.y / 2) + eBound.offRect.y, eBound.size.x, eBound.size.y);
    sf::IntRect rectObject((oPos.x - oAnimSize.x/2) + oBound.offRect.x, (oPos.y - oAnimSize.y / 2) + oBound.offRect.y, oBound.size.x, oBound.size.y);


    return rectEntity.intersects(rectObject);
}

bool Physics::isPrevIntersect(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Entity>& object) const
{
    auto& eT = entity->getComponent<CTransform>();
    auto& eBound = entity->getComponent<CBoundingBox>();
    auto& ePos = eT.prevPos;
    auto eAnim = entity->getComponent<CAnimation>().animation.getSize();

    auto eAnimSize = Vec2(eAnim.x * abs(eT.scale.x), eAnim.y * abs(eT.scale.y));

    auto& oT = object->getComponent<CTransform>();
    auto& oBound = object->getComponent<CBoundingBox>();
    auto& oPos = oT.prevPos;
    auto oAnim = object->getComponent<CAnimation>().animation.getSize();

    auto oAnimSize = Vec2(oAnim.x * abs(oT.scale.x), oAnim.y * abs(oT.scale.y));

    sf::IntRect rectEntity((ePos.x - eAnimSize.x / 2) + eBound.offRect.x, (ePos.y - eAnimSize.y / 2) + eBound.offRect.y, eBound.size.x, eBound.size.y);
    sf::IntRect rectObject((oPos.x - oAnimSize.x / 2) + oBound.offRect.x, (oPos.y - oAnimSize.y / 2) + oBound.offRect.y, oBound.size.x, oBound.size.y);


    return rectEntity.intersects(rectObject);
}

bool Physics::compareBy(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Entity>& object) const
{
    auto& ePos = entity->getComponent<CTransform>().pos;
    auto& oPos = object->getComponent<CTransform>().pos;
    
    return ePos.y < oPos.y;
}
