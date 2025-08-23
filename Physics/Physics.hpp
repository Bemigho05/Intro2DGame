#pragma once


#include <math.h>

#include "../ECS/Entity.hpp"

namespace Physics {
	Vec2f GetOVerlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
		auto delta = abs(a->get<CTransform>().pos - b->get<CTransform>().pos);
		auto& a_bb = a->get<CBoundingBox>().halfSize;
		auto& b_bb = b->get<CBoundingBox>().halfSize;

		return Vec2f(a_bb + b_bb - delta);
	}

	Vec2f GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
		auto delta = abs(a->get<CTransform>().prevPos - b->get<CTransform>().pos);
		auto& a_bb = a->get<CBoundingBox>().halfSize;
		auto& b_bb = b->get<CBoundingBox>().halfSize;

		return Vec2f(a_bb + b_bb - delta);
	}
}