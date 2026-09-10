#ifndef _HDS_H_
#define _HDS_H_
#include "hdef.h"

// BUF
#define HC_RET_BUF_BASE HC_RET_BASE_DS
typedef struct hbuf_ {
  size_t len;
  size_t size;
  uint8_t* data;
} hbuf_t;
HFC_EXPORT int hbuf_init(hbuf_t* b, size_t s);
HFC_EXPORT int hbuf_deinit(hbuf_t* b);
HFC_EXPORT int hbuf_push(hbuf_t* b, const void* d, size_t dSize);
HFC_EXPORT int hbuf_clear(hbuf_t* b);
HFC_EXPORT int hbuf_grow(hbuf_t* b, size_t wish_size);
HFC_EXPORT int hbuf_print(hbuf_t* b, size_t ofs, size_t size);

// RQ
// TODO cache alive!!
#define HC_RET_BASE_RB HC_RET_BASE_DS
#define HRB_MIN(A, B) ((A) < (B) ? (A) : (B))

typedef struct hrb_ {
  size_t size;  // 2^n
  size_t in;
  size_t out;
  uint8_t data[];
} hrb_t;
size_t hrb_alloc_size(size_t s);
HFC_EXPORT hrb_t* hrb_init(size_t s);
HFC_EXPORT void hrb_deinit(hrb_t* rq);
HFC_EXPORT size_t hrb_push(hrb_t* rq, void* data, size_t size);
HFC_EXPORT size_t hrb_pop(hrb_t* rq, void* data, size_t size);
HFC_EXPORT size_t hrb_peek(hrb_t* rq, void* data, size_t size);

// LIST
#define HC_RET_BASE_LIST HC_RET_BASE_DS
typedef struct hlistitem_ {
  struct hlistitem_* next;
} hlistitem_t;
typedef struct hlist_ {
  hlistitem_t* head;
  size_t size;
} hlist_t;
typedef int (*hlist_compare_t)(hlistitem_t* i, hlistitem_t* target);
typedef int (*hlist_foreach_t)(hlistitem_t* i, int index);

// without mem free
HFC_EXPORT int hlist_clear(hlist_t*);
HFC_EXPORT int hlist_append(hlist_t* l, hlistitem_t* i);
HFC_EXPORT int hlist_insert(hlist_t* l, hlistitem_t* i, uint32_t index);
HFC_EXPORT hlistitem_t* hlist_at(hlist_t* l, uint32_t index);
HFC_EXPORT hlistitem_t* hlist_remove(hlist_t* l, uint32_t index);
// need set compare-func
HFC_EXPORT hlistitem_t* hlist_remove_first(hlist_t* l, hlistitem_t* tar,
                                           hlist_compare_t cmp);
HFC_EXPORT hlistitem_t* hlist_find_first(hlist_t* l, hlistitem_t* tar,
                                         hlist_compare_t cmp);
// need set foreach-func
HFC_EXPORT int hislist_foreach(hlist_t* l, hlist_foreach_t f);

// HASH
#define HC_RET_BASE_HASH HC_RET_BASE_DS

#include "stb_ds.h"

// define
#define HHASH_ENTRY(TK, TV)      \
  typedef struct hhash_##TV##_ { \
    TK key;                      \
    TV value;                    \
  } HHASH_TYPEDEF(TK, TV);
#define HHASH_DEFINE(TK, TV) HHASH_ENTRY(TK, TV)
// iterater
#define HHASH_TYPEDEF(TK, TV) hhash_##TV
#define HHASH_ITERATE ptrdiff_t
#define HHASH_ITERATE_VALID(h, IT) (0 <= (IT) && (IT) < hmlen(h))
#define HHASH_ITERATE_BEGIN(h) 0
#define HHASH_ITERATE_NEXT(IT) ((IT) + 1)
// get
#define HHASH_GETV(h, it) (h[it].value)
#define HHASH_GETVPTR(h, it) (&h[it].value)
#define HHASH_GETK(h, it) (h[it].key)
#define HHASH_GETKPTR(h, it) (&h[it].key)
// char*
#define HSHASH_DEFINE(TK, TV) HHASH_DEFINE(TK, TV)
#define HSHASH_TYPEDEF(TK, TV) HHASH_TYPEDEF(TK, TV)
#define HSHASH_INIT(h) sh_new_strdup(h)
#define HSHASH_FINI(h) shfree(h)
#define HSHASH_SET(h, k, v) shput(h, k, v)
#define HSHASH_DEL(h, k) shdel(h, k)
#define HSHASH_SIZE(h) shlen(h)
#define HSHASH_EXIST(h, k) (shgeti(h, k)) >= 0 ? HC_RET_OK : HC_RET_NOTFOUND
#define HSHASH_FIND(h, k) shgeti(h, k)
#define HSHASH_GETV(h, it) HHASH_GETV(h, it)
#define HSHASH_GETVPTR(h, it) HHASH_GETVPTR(h, it)
#define HSHASH_GETK(h, it) HHASH_GETK(h, it)
#define HSHASH_GETKPTR(h, it) HHASH_GETKPTR(h, it)
#define HSHASH_ITERATE HHASH_ITERATE
#define HSHASH_ITERATE_VALID(h, IT) HHASH_ITERATE_VALID(h, IT)
#define HSHASH_ITERATE_BEGIN(h) HHASH_ITERATE_BEGIN(h)
#define HSHASH_ITERATE_NEXT(IT) HHASH_ITERATE_NEXT(IT)
// int
#define HIHASH_DEFINE(TK, TV) HHASH_DEFINE(TK, TV)
#define HIHASH_TYPEDEF(TK, TV) HHASH_TYPEDEF(TK, TV)
#define HIHASH_INIT(h)
#define HIHASH_FINI(h) hmfree(h)
#define HIHASH_SET(h, k, v) hmput(h, k, v)
#define HIHASH_DEL(h, k) hmdel(h, k)
#define HIHASH_SIZE(h) hmlen(h)
#define HIHASH_EXIST(h, k) (hmgeti(h, k)) >= 0 ? HC_RET_OK : HC_RET_NOTFOUND
#define HIHASH_FIND(h, k) hmgeti(h, k)
#define HIHASH_GETV(h, it) HHASH_GETV(h, it)
#define HIHASH_GETVPTR(h, it) HHASH_GETVPTR(h, it)
#define HIHASH_GETK(h, it) HHASH_GETK(h, it)
#define HIHASH_GETKPTR(h, it) HHASH_GETKPTR(h, it)
#define HIHASH_ITERATE HHASH_ITERATE
#define HIHASH_ITERATE_VALID(h, IT) HHASH_ITERATE_VALID(h, IT)
#define HIHASH_ITERATE_BEGIN(h) HHASH_ITERATE_BEGIN(h)
#define HIHASH_ITERATE_NEXT(IT) HHASH_ITERATE_NEXT(IT)
#endif
