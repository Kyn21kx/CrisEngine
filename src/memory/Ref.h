#pragma once

#include "MemoryArena.h"
#include <cassert>
#include <algorithm>

using Handle = uintptr_t;
constexpr Handle NIL = 0;

// SFINAE (Substitution Failure Is Not An Error)
template <class T>
class Ref {
public:
	static Ref<T> Create(MemoryArena* allocator = nullptr) {
		// Sacar el ptr de algún lado
		T* instance;
		if (allocator != nullptr) {
			instance = static_cast<T*>(arena_push(allocator, sizeof(T), false));
		}
		else {
			instance = new T();
		}

		Ref<T> result;
		result.m_ptr = reinterpret_cast<Handle>(instance); // Converts the ptr into a number

		return result;
	}

	bool IsNil() const {
		return this->m_ptr == NIL;
	}
	
	void Increment() {
		#ifdef DEBUG
		// Solo DEBUG
		assert(!this->IsNil() && "Tried to increase ref count for a nil reference!");
		#else
		if (this->IsNil()) return;
		#endif
		this->m_refCount++;
	}

	void UnRef() {
		if (this->IsNil()) return;
		this->m_refCount = std::max(this->m_refCount - 1, 0);
	}
	
	T *operator->() const {
		assert(!this->IsNil() && "Cannot dereference a NIL reference");
		T *result = reinterpret_cast<T*>(this->m_ptr);
		return result;
	}

private:
	Handle m_ptr = NIL;
	int32_t m_refCount = 0;
};

