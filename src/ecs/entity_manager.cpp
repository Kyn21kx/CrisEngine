#include "entity_manager.h"

void EntityManager::Initialize() {
	this->m_livingEntities = 0;
	for (uint64_t i = 1; i < MAX_ENTITIES + 1; i++) {
		this->m_readyEntities.push(i);
	}
}

Entity_t EntityManager::New() {
	Entity_t result = this->m_readyEntities.front();
	this->m_readyEntities.pop();
	this->m_livingEntities++;
	return result;
}


void EntityManager::Destroy(Entity_t entity) {
	this->m_readyEntities.push(entity);
	this->m_livingEntities--;
}


Entity_t EntityManager::AddComponent(Entity_t entity, const ComponentDesc& desc) {
	Entity_t componentId = this->New();

	
	
	return componentId;
}
