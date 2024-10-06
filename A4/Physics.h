#pragma once
#include "Vec2.hpp"
#include "Entity.hpp"
class Physics
{
public:
	Vec2 getOverlap(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Entity>& object) const;
	Vec2 getPrevOverlap(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Entity>& object) const;

	bool isIntersect(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Entity>& object) const;
	bool isPrevIntersect(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Entity>& object) const;

	bool compareBy(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Entity>& object) const;
};

