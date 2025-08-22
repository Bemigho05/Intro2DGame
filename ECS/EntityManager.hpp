#pragma once

#include "Entity.hpp"
#include <algorithm>

using EntityVec = std::vector<std::shared_ptr<Entity>>;

class EntityManager
{
	EntityVec							m_entities;
	EntityVec							m_entitiesToAdd;
	std::map<std::string, EntityVec>	m_entityMap;
	size_t								m_totalEntities = 0;

	void removeDeadEntities(EntityVec& vec) {
		auto it = remove_if(vec.begin(), vec.end(), [](std::shared_ptr<Entity> entity) { return !entity->isActive(); });
		vec.erase(it, vec.end());
	}

public:
	EntityManager() = default;

	void update() {
		removeDeadEntities(m_entities);
		for (auto& [tag, entityVec] : m_entityMap) {
			removeDeadEntities(entityVec);
		}
	}

	std::shared_ptr<Entity> addEntity(const std::string& tag) {
		auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
		m_entitiesToAdd.push_back(entity);

		if (m_entityMap.find(tag) == m_entityMap.end()) { m_entityMap[tag] = EntityVec(); }
		m_entityMap[tag].push_back(entity);

		return entity;
	}

	const EntityVec& getEntities()
	{
		return m_entities;
	}

	std::map<std::string, EntityVec>& getEntityMap() {
		return m_entityMap;
	}

	const EntityVec& getEntities(const std::string& tag) {
		return m_entityMap[tag];
	}
};