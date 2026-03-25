#pragma once
#include <cstdint>
#include <queue>

using Entity_t = uint64_t;


struct ComponentDesc {
	size_t size;
	size_t align;
};

struct ComponentInstance {
	void* data;
	ComponentDesc sizeInfo;
};

class EntityManager {
public:
	static constexpr Entity_t MAX_ENTITIES = 1024;

	static constexpr Entity_t INVALID = 0;

	void Initialize();

	Entity_t New();

	void Destroy(Entity_t entity);

	Entity_t AddComponent(Entity_t entity, const ComponentDesc& desc);
	
	template <class T>
	inline Entity_t AddComponent(Entity_t entity) {
		ComponentDesc desc{};
		desc.size = sizeof(T);
		desc.align = alignof(T);
		this->AddComponent(entity, desc);
	}

private:
	std::queue<Entity_t> m_readyEntities;
	uint64_t m_livingEntities = 0;
};


