#include "heapq.h"
#include "lua.h"
#include "lauxlib.h"

#define check_heapq(L, idx)\
    *(HeapQueue**)luaL_checkudata(L, idx, "heapq_meta")

static int
heapq_create(lua_State* L) {
    size_t capacity = luaL_checkinteger(L, 1);
    HeapQueue* q = create_queue(capacity);
    *(HeapQueue**)lua_newuserdata(L, sizeof(void*)) = q;
    luaL_getmetatable(L, "heapq_meta");
    lua_setmetatable(L, -2);
    return 1;
}

static int
heapq_release(lua_State* L) {
    HeapQueue* q = check_heapq(L, 1);
    delete_queue(q);
    return 0;
}

static int
heapq_reset(lua_State* L) {
    HeapQueue* q = check_heapq(L, 1);
    reset_queue(q);
    return 0;
}

static int
heapq_size(lua_State* L) {
    HeapQueue* q = check_heapq(L, 1);
    lua_pushinteger(L, queue_size(q));
    return 1;
}

static int
heapq_push(lua_State* L) {
    HeapQueue* q = check_heapq(L, 1);
    int session = luaL_checkinteger(L, 2);
    double priority = luaL_checknumber(L, 3);
    HeapNode* n = malloc(sizeof(*n));
    n->session = session;
    n->priority = priority;
    if (queue_push(q, n)) {
        lua_pushboolean(L, 1);
    }else{
        free(n);
        lua_pushboolean(L, 0);
    }
    return 1;
}

static int
heapq_pop(lua_State* L) {
    HeapQueue* q = check_heapq(L, 1);
    HeapNode *n = queue_pop(q);
    if (n) {
        lua_pushinteger(L, n->session);
        free(n);
        return 1;
    }else{
        return 0;
    }
}

static int
heapq_pushpop(lua_State* L) {
    HeapQueue* q = check_heapq(L, 1);
    int session = luaL_checkinteger(L, 2);
    double priority = luaL_checknumber(L, 3);
    HeapNode* n = malloc(sizeof(*n));
    n->session = session;
    n->priority = priority;
    HeapNode* top = queue_pushpop(q, n);
    if (top) {
        lua_pushinteger(L, top->session);
        free(top);
        return 1;
    }else {
        free(n);
        return 0;
    }
}

static int
heapq_cmpltepop(lua_State* L) {
    HeapQueue* q = check_heapq(L, 1);
    double cmp_val = luaL_checknumber(L, 2);
    HeapNode* n = queue_cmpltepop(q, cmp_val);
    if (n) {
        lua_pushinteger(L, n->session);
        free(n);
        return 1;
    }else {
        return 0;
    }
}

static int
heapq_top(lua_State* L) {
    HeapQueue* q = check_heapq(L, 1);
    HeapNode* n = queue_top(q);
    if (n) {
        lua_pushinteger(L, n->session);
        return 1;
    }else {
        return 0;
    }
}

static int
heapq_query(lua_State* L) {
    HeapQueue* q = check_heapq(L, 1);
    lua_settop(L, 1);
    lua_newtable(L);
    lua_pushstring(L, "size");
    lua_pushinteger(L, q->size);
    lua_rawset(L,2);
    lua_pushstring(L, "capacity");
    lua_pushinteger(L, q->capacity);
    lua_rawset(L,2);

    lua_pushstring(L, "data");
    lua_newtable(L);
    int i;
    for (i=1; i<=q->size; i++) {
        HeapNode* n = q->data[i];
        lua_pushinteger(L, i);
        lua_newtable(L);
        lua_pushstring(L, "session");
        lua_pushinteger(L, n->session);
        lua_rawset(L,6);
        lua_pushstring(L, "priority");
        lua_pushnumber(L, n->priority);
        lua_rawset(L,6);
        lua_rawset(L,4);
    }
    lua_rawset(L,2);
    return 1;
}


int luaopen_heapq(lua_State* L) {
    luaL_checkversion(L);
    luaL_Reg l1[] = {
        {"create", heapq_create},
        {NULL, NULL},
    };
    luaL_Reg l2[] = {
        {"push", heapq_push},
        {"pop", heapq_pop},
        {"pushpop", heapq_pushpop},
        {"cmpltepop", heapq_cmpltepop},
        {"size", heapq_size},
        {"reset", heapq_reset},
        {"top", heapq_top},
        {"query", heapq_query},
        {NULL, NULL},
    };
    luaL_newmetatable(L, "heapq_meta");
    luaL_newlib(L, l2);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, heapq_release);
    lua_setfield(L, -2, "__gc");

    luaL_newlib(L, l1);
    return 1;
}