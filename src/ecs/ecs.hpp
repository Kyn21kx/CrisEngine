#pragma once
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cstdio>


inline constexpr size_t MAX_ENTITIES   = 1024;
inline constexpr size_t MAX_COMPONENTS = 256;
inline constexpr size_t BITSET_WORDS = (MAX_COMPONENTS + 63) / 64;

using Entity      = std::uint64_t;
using ComponentId = std::uint16_t;
using MaskWord    = std::uint64_t;

struct Mask {
    MaskWord bits[BITSET_WORDS] = {};

    void clear() {
        for (size_t i = 0; i < BITSET_WORDS; ++i) bits[i] = 0;
    }
    void set(ComponentId id) {
        bits[id / 64] |= (MaskWord(1) << (id % 64));
    }
    void clear_bit(ComponentId id) {
        bits[id / 64] &= ~(MaskWord(1) << (id % 64));
    }
    bool test(ComponentId id) const {
        return (bits[id / 64] >> (id % 64)) & MaskWord(1);
    }
    bool operator==(const Mask& other) const {
        for (size_t i = 0; i < BITSET_WORDS; ++i)
            if (bits[i] != other.bits[i]) return false;
        return true;
    }
    bool contains(const Mask& required) const {
        for (size_t i = 0; i < BITSET_WORDS; ++i)
            if ((bits[i] & required.bits[i]) != required.bits[i]) return false;
        return true;
    }
};

struct Column {
    char*    data      = nullptr;
    int32_t  size      = 0;
    int32_t  capacity  = 0;
    size_t   elem_size = 0;
};

inline void column_init(Column* col, size_t elem_size) {
    col->data      = nullptr;
    col->size      = 0;
    col->capacity  = 0;
    col->elem_size = elem_size;
}

inline void column_fini(Column* col) {
    std::free(col->data);
    col->data = nullptr;
}

inline void* column_push(Column* col, const void* elem) {
    if (col->size >= col->capacity) {
        int32_t new_cap = (col->capacity == 0) ? 4 : col->capacity * 2;
        char* new_data = static_cast<char*>(std::realloc(col->data, new_cap * col->elem_size));
        if (!new_data) return nullptr;
        col->data     = new_data;
        col->capacity = new_cap;
    }
    void* slot = col->data + col->size * col->elem_size;
    if (elem)
        std::memcpy(slot, elem, col->elem_size);
    else
        std::memset(slot, 0, col->elem_size);
    ++col->size;
    return slot;
}

inline void column_remove_swap(Column* col, int32_t index) {
    if (index < 0 || index >= col->size) return;
    int32_t last = col->size - 1;
    if (index != last) {
        void* dst = col->data + index * col->elem_size;
        void* src = col->data + last * col->elem_size;
        std::memcpy(dst, src, col->elem_size);
    }
    --col->size;
}

inline void* column_get(const Column* col, int32_t index) {
    if (index < 0 || index >= col->size) return nullptr;
    return col->data + index * col->elem_size;
}

struct Archetype {
    Mask         mask;
    int32_t      entity_count = 0;
    int32_t      capacity     = 0;
    Entity*      entity_ids   = nullptr;
    int32_t      num_components = 0;
    ComponentId* comp_ids     = nullptr;
    Column*      columns      = nullptr;
};

inline Archetype* archetype_new(const Mask* mask) {
    Archetype* arch = static_cast<Archetype*>(std::calloc(1, sizeof(Archetype)));
    arch->mask = *mask;
    arch->entity_count = 0;
    arch->capacity = 0;
    arch->entity_ids = nullptr;

    int32_t comp_count = 0;
    for (ComponentId i = 0; i < MAX_COMPONENTS - 1; ++i)
        if (mask->test(i)) ++comp_count;

    arch->num_components = comp_count;
    arch->comp_ids = static_cast<ComponentId*>(std::malloc(comp_count * sizeof(ComponentId)));
    arch->columns  = static_cast<Column*>(std::malloc(comp_count * sizeof(Column)));

    int idx = 0;
    for (ComponentId i = 0; i < MAX_COMPONENTS - 1; ++i) {
        if (mask->test(i)) {
            arch->comp_ids[idx] = i;
            column_init(&arch->columns[idx], 0);
            ++idx;
        }
    }
    return arch;
}

inline void archetype_destroy(Archetype* arch) {
    if (!arch) return;
    for (int i = 0; i < arch->num_components; ++i)
        column_fini(&arch->columns[i]);
    std::free(arch->entity_ids);
    std::free(arch->comp_ids);
    std::free(arch->columns);
    std::free(arch);
}

inline int32_t archetype_add_entity(Archetype* arch, Entity e) {
    if (arch->entity_count >= arch->capacity) {
        int32_t new_cap = (arch->capacity == 0) ? 4 : arch->capacity * 2;
        Entity* new_ids = static_cast<Entity*>(std::realloc(arch->entity_ids, new_cap * sizeof(Entity)));
        if (!new_ids) return -1;
        arch->entity_ids = new_ids;
        arch->capacity   = new_cap;
    }
    int32_t row = arch->entity_count;
    arch->entity_ids[row] = e;
    ++arch->entity_count;
    return row;
}

inline void archetype_remove_entity(Archetype* arch, int32_t row) {
    if (row < 0 || row >= arch->entity_count) return;
    int32_t last = arch->entity_count - 1;
    if (row != last) {
        // movemos la última entidad a la posición vacía (swap‑pop O(1))
        arch->entity_ids[row] = arch->entity_ids[last];
        for (int i = 0; i < arch->num_components; ++i) {
            Column* col = &arch->columns[i];
            void* dst = col->data + row * col->elem_size;
            void* src = col->data + last * col->elem_size;
            std::memcpy(dst, src, col->elem_size);
        }
    }
    --arch->entity_count;
    for (int i = 0; i < arch->num_components; ++i)
        --arch->columns[i].size;
}

inline void archetype_set_column_size(Archetype* arch, ComponentId comp_id, size_t size) {
    for (int i = 0; i < arch->num_components; ++i)
        if (arch->comp_ids[i] == comp_id)
            arch->columns[i].elem_size = size;
}


struct World {
    uint8_t    entity_used[MAX_ENTITIES] = {};
    Archetype* entity_arch[MAX_ENTITIES] = {};
    int32_t    entity_row[MAX_ENTITIES]  = {};

    ComponentId next_component_id = 0;
    size_t      comp_size[MAX_COMPONENTS] = {};
    size_t      comp_align[MAX_COMPONENTS] = {};

    Archetype** archetypes       = nullptr;
    int32_t     archetype_count  = 0;
    int32_t     archetype_cap    = 0;
    Archetype*  empty_archetype  = nullptr;
};

inline World g_world;

inline Archetype* find_archetype(const Mask* mask) {
    for (int i = 0; i < g_world.archetype_count; ++i)
        if (g_world.archetypes[i]->mask == *mask)
            return g_world.archetypes[i];
    return nullptr;
}

inline Archetype* get_or_create_archetype(const Mask* mask) {
    Archetype* arch = find_archetype(mask);
    if (arch) return arch;

    arch = archetype_new(mask);
    // asignar tamaños de columna ya conocidos
    for (int i = 0; i < arch->num_components; ++i) {
        ComponentId cid = arch->comp_ids[i];
        arch->columns[i].elem_size = g_world.comp_size[cid];
    }

    if (g_world.archetype_count >= g_world.archetype_cap) {
        int32_t new_cap = (g_world.archetype_cap == 0) ? 4 : g_world.archetype_cap * 2;
        g_world.archetypes = static_cast<Archetype**>(std::realloc(
            g_world.archetypes, new_cap * sizeof(Archetype*)));
        g_world.archetype_cap = new_cap;
    }
    g_world.archetypes[g_world.archetype_count++] = arch;
    return arch;
}


inline void ecs_init() {
    // los arreglos de entidad ya están puestos a cero por el inicializador de World
    g_world.next_component_id = 0;
    g_world.archetypes = nullptr;
    g_world.archetype_count = 0;
    g_world.archetype_cap   = 0;

    Mask empty_mask{}; // ya limpio por inicialización de valor
    g_world.empty_archetype = archetype_new(&empty_mask);

    g_world.archetype_cap = 4;
    g_world.archetypes = static_cast<Archetype**>(std::malloc(g_world.archetype_cap * sizeof(Archetype*)));
    g_world.archetypes[g_world.archetype_count++] = g_world.empty_archetype;
}

inline void ecs_shutdown() {
    for (int i = 0; i < g_world.archetype_count; ++i)
        archetype_destroy(g_world.archetypes[i]);
    std::free(g_world.archetypes);
    g_world.archetypes = nullptr;
}


inline ComponentId ecs_component_register(size_t size, size_t alignment) {
    if (g_world.next_component_id >= MAX_COMPONENTS)
        return static_cast<ComponentId>(-1); // valor inválido
    ComponentId id = g_world.next_component_id++;
    g_world.comp_size[id] = size;
    g_world.comp_align[id] = alignment;

    // Actualizar todos los arquetipos existentes que posean este componente
    for (int i = 0; i < g_world.archetype_count; ++i)
        archetype_set_column_size(g_world.archetypes[i], id, size);
    return id;
}

// --------------------------------------------------------------
// Manejo de entidades
// --------------------------------------------------------------
inline Entity ecs_new_entity() {
    for (Entity i = 0; i < MAX_ENTITIES; ++i) {
        if (!g_world.entity_used[i]) {
            g_world.entity_used[i] = 1;
            int32_t row = archetype_add_entity(g_world.empty_archetype, i);
            g_world.entity_arch[i] = g_world.empty_archetype;
            g_world.entity_row[i]  = row;
            return i;
        }
    }
    return static_cast<Entity>(-1);
}

inline void ecs_delete_entity(Entity e) {
    if (e >= MAX_ENTITIES || !g_world.entity_used[e]) return;
    Archetype* arch = g_world.entity_arch[e];
    int32_t row = g_world.entity_row[e];
    archetype_remove_entity(arch, row);

    // si la entidad removida no era la última, actualizar la fila de la que se movió
    if (row < arch->entity_count) {
        Entity moved = arch->entity_ids[row];
        g_world.entity_row[moved] = row;
    }

    g_world.entity_used[e] = 0;
    g_world.entity_arch[e] = nullptr;
    g_world.entity_row[e]  = -1;
}


inline void ecs_add(Entity e, ComponentId comp_id, const void* data) {
    if (e >= MAX_ENTITIES || !g_world.entity_used[e]) return;
    Archetype* src = g_world.entity_arch[e];
    int32_t src_row = g_world.entity_row[e];
    if (src->mask.test(comp_id)) return; // ya lo tiene

    Mask new_mask = src->mask;
    new_mask.set(comp_id);
    Archetype* dst = get_or_create_archetype(&new_mask);
    int32_t dst_row = archetype_add_entity(dst, e);

    // copiar componentes existentes + añadir el nuevo
    for (int i = 0; i < dst->num_components; ++i) {
        ComponentId cid = dst->comp_ids[i];
        if (cid == comp_id) {
            column_push(&dst->columns[i], data);
        } else {
            // buscar en el arquetipo origen
            int src_col = -1;
            for (int j = 0; j < src->num_components; ++j)
                if (src->comp_ids[j] == cid) { src_col = j; break; }
            void* src_ptr = column_get(&src->columns[src_col], src_row);
            column_push(&dst->columns[i], src_ptr);
        }
    }

    archetype_remove_entity(src, src_row);
    if (src_row < src->entity_count) {
        Entity moved = src->entity_ids[src_row];
        g_world.entity_row[moved] = src_row;
    }

    g_world.entity_arch[e] = dst;
    g_world.entity_row[e]  = dst_row;
}

inline void ecs_remove(Entity e, ComponentId comp_id) {
    if (e >= MAX_ENTITIES || !g_world.entity_used[e]) return;
    Archetype* src = g_world.entity_arch[e];
    int32_t src_row = g_world.entity_row[e];
    if (!src->mask.test(comp_id)) return;

    Mask new_mask = src->mask;
    new_mask.clear_bit(comp_id);
    Archetype* dst = get_or_create_archetype(&new_mask);
    int32_t dst_row = archetype_add_entity(dst, e);

    // copiar todos los componentes excepto el eliminado
    for (int i = 0; i < dst->num_components; ++i) {
        ComponentId cid = dst->comp_ids[i];
        int src_col = -1;
        for (int j = 0; j < src->num_components; ++j)
            if (src->comp_ids[j] == cid) { src_col = j; break; }
        void* src_ptr = column_get(&src->columns[src_col], src_row);
        column_push(&dst->columns[i], src_ptr);
    }

    archetype_remove_entity(src, src_row);
    if (src_row < src->entity_count) {
        Entity moved = src->entity_ids[src_row];
        g_world.entity_row[moved] = src_row;
    }

    g_world.entity_arch[e] = dst;
    g_world.entity_row[e]  = dst_row;
}

inline void* ecs_get(Entity entity, ComponentId compId) {
    if (entity >= MAX_ENTITIES || !g_world.entity_used[entity]) return nullptr;
    Archetype* arch = g_world.entity_arch[entity];
    int32_t row = g_world.entity_row[entity];

    for (int32_t i = 0; i < arch->num_components; ++i)
        if (arch->comp_ids[i] == compId)
            return column_get(&arch->columns[i], row);

    return nullptr;
}


using QueryCallback = void (*)(Entity e, void** comp_ptrs);

inline void ecs_query(const ComponentId* required, int required_count, QueryCallback callback) {
    Mask req_mask{};
    for (int i = 0; i < required_count; ++i)
        req_mask.set(required[i]);

    // buffer en stack (máximo MAX_COMPONENTS, que es pequeño)
    void* comp_ptrs[MAX_COMPONENTS];

    for (int ai = 0; ai < g_world.archetype_count; ++ai) {
        Archetype* arch = g_world.archetypes[ai];
        if (!arch->mask.contains(req_mask)) continue;

        // mapear índices de componentes requeridos a las columnas del arquetipo
        int col_idx[MAX_COMPONENTS];
        for (int r = 0; r < required_count; ++r) {
            col_idx[r] = -1;
            for (int c = 0; c < arch->num_components; ++c)
                if (arch->comp_ids[c] == required[r]) {
                    col_idx[r] = c;
                    break;
                }
        }

        // iterar entidades del arquetipo
        for (int row = 0; row < arch->entity_count; ++row) {
            Entity e = arch->entity_ids[row];
            for (int r = 0; r < required_count; ++r)
                comp_ptrs[r] = column_get(&arch->columns[col_idx[r]], row);
            callback(e, comp_ptrs);
        }
    }
}
