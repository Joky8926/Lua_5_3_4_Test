#pragma once
/* chars used as small naturals (so that 'char' is reserved for characters) */
typedef unsigned char lu_byte;

/*
** Common type for all collectable objects
*/
typedef struct GCObject GCObject;

/*
** Common Header for all collectable objects (in macro form, to be
** included in other objects)
*/
#define CommonHeader	GCObject *next; lu_byte tt; lu_byte marked

/*
** Common type has only the common header
*/
struct GCObject {
    CommonHeader;
};

/*
** Type for C functions registered with Lua
*/
typedef int(*lua_CFunction) (lua_State *L);

#define LUA_INTEGER long long

/* type for integer functions */
typedef LUA_INTEGER lua_Integer;

#define LUA_NUMBER	double

/* type of numbers in Lua */
typedef LUA_NUMBER lua_Number;

/*
** Union of all Lua values
*/
typedef union Value {
    GCObject *gc;    /* collectable objects */
    void *p;         /* light userdata */
    int b;           /* booleans */
    lua_CFunction f; /* light C functions */
    lua_Integer i;   /* integer numbers */
    lua_Number n;    /* float numbers */
} Value;

#define TValuefields	Value value_; int tt_

typedef struct lua_TValue {
    TValuefields;
} TValue;

typedef TValue *StkId;  /* index to stack elements */

                        /*
                        ** Type for memory-allocation functions
                        */
typedef void * (*lua_Alloc) (void *ud, void *ptr, size_t osize, size_t nsize);

/*
** 'global state', shared by all threads of this state
*/
typedef struct global_State {
    lua_Alloc frealloc;  /* function to reallocate memory */
    void *ud;         /* auxiliary data to 'frealloc' */
    l_mem totalbytes;  /* number of bytes currently allocated - GCdebt */
    l_mem GCdebt;  /* bytes allocated not yet compensated by the collector */
    lu_mem GCmemtrav;  /* memory traversed by the GC */
    lu_mem GCestimate;  /* an estimate of the non-garbage memory in use */
    stringtable strt;  /* hash table for strings */
    TValue l_registry;
    unsigned int seed;  /* randomized seed for hashes */
    lu_byte currentwhite;
    lu_byte gcstate;  /* state of garbage collector */
    lu_byte gckind;  /* kind of GC running */
    lu_byte gcrunning;  /* true if GC is running */
    GCObject *allgc;  /* list of all collectable objects */
    GCObject **sweepgc;  /* current position of sweep in list */
    GCObject *finobj;  /* list of collectable objects with finalizers */
    GCObject *gray;  /* list of gray objects */
    GCObject *grayagain;  /* list of objects to be traversed atomically */
    GCObject *weak;  /* list of tables with weak values */
    GCObject *ephemeron;  /* list of ephemeron tables (weak keys) */
    GCObject *allweak;  /* list of all-weak tables */
    GCObject *tobefnz;  /* list of userdata to be GC */
    GCObject *fixedgc;  /* list of objects not to be collected */
    struct lua_State *twups;  /* list of threads with open upvalues */
    unsigned int gcfinnum;  /* number of finalizers to call in each GC step */
    int gcpause;  /* size of pause between successive GCs */
    int gcstepmul;  /* GC 'granularity' */
    lua_CFunction panic;  /* to be called in unprotected errors */
    struct lua_State *mainthread;
    const lua_Number *version;  /* pointer to version number */
    TString *memerrmsg;  /* memory-error message */
    TString *tmname[TM_N];  /* array with tag-method names */
    struct Table *mt[LUA_NUMTAGS];  /* metatables for basic types */
    TString *strcache[STRCACHE_N][STRCACHE_M];  /* cache for strings in API */
} global_State;

/*
** 'per thread' state
*/
struct lua_State {
    CommonHeader;
    unsigned short nci;  /* number of items in 'ci' list */
    lu_byte status;
    StkId top;  /* first free slot in the stack */
    global_State *l_G;
    CallInfo *ci;  /* call info for current function */
    const Instruction *oldpc;  /* last pc traced */
    StkId stack_last;  /* last free slot in the stack */
    StkId stack;  /* stack base */
    UpVal *openupval;  /* list of open upvalues in this stack */
    GCObject *gclist;
    struct lua_State *twups;  /* list of threads with open upvalues */
    struct lua_longjmp *errorJmp;  /* current error recover point */
    CallInfo base_ci;  /* CallInfo for first level (C calling Lua) */
    volatile lua_Hook hook;
    ptrdiff_t errfunc;  /* current error handling function (stack index) */
    int stacksize;
    int basehookcount;
    int hookcount;
    unsigned short nny;  /* number of non-yieldable calls in stack */
    unsigned short nCcalls;  /* number of nested C calls */
    l_signalT hookmask;
    lu_byte allowhook;
};
