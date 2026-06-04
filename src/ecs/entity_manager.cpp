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
	this->m_alive[result] = true;
	this->m_livingEntities++;
	return result;
}


Entity_t EntityManager::AddComponent(Entity_t entity, const ComponentDesc& desc) {
    if (entity == INVALID || entity >= MAX_ENTITIES || !m_alive[entity])
        return INVALID;

    // Find first free slot in the entity's component table
    size_t slot = 0;
    while (slot < MAX_COMPONENTS && m_componentTables[entity][slot] != INVALID)
        ++slot;
    if (slot == MAX_COMPONENTS)
        return INVALID;  // no more component slots for this entity

    // Create a new component entity
    Entity_t compEntity = New();
    if (compEntity == INVALID)
        return INVALID;

    void* data = m_arena.push(desc.size);
    if (!data) {
        // Allocation failed: recycle the component entity
        m_alive[compEntity] = false;
        m_readyEntities.push(compEntity);
        --m_livingEntities;
        return INVALID;
    }
    // Optional, but better
    std::memset(data, 0, adjustedSize);

    m_components[compEntity].data = data;
    m_components[compEntity].sizeInfo = desc;

    // Link it to the owner entity
    m_componentTables[entity][slot] = compEntity;

    return compEntity;
}
void EntityManager::Destroy(Entity_t entity) {
	this->m_readyEntities.push(entity);
	this->m_livingEntities--;
}


Entity_t EntityManager::AddComponent(Entity_t entity, const ComponentDesc& desc) {
	Entity_t componentId = this->New();

	
	
	return componentId;
}
