
#include "hbuf.h"
// BUF
#define HBUF_DATA_SIZE_MAX 1024 * 1024 * 1024  // 1 GB
#define HBUF_DATA_SIZE_MIN 32                  // 32 byte


int hbuf_init(hbuf_t* b, size_t s) {
  HC_RET_WHEN(!b, HC_RET(HC_RET_BUF_BASE, HC_RET_PARAM(0)));
  HC_RET_WHEN(HBUF_DATA_SIZE_MAX < s, HC_RET(HC_RET_BUF_BASE, HC_RET_PARAM(1)));

  b->size = HBUF_DATA_SIZE_MIN;
  HC_GETMINPOW(s, b->size);
  b->len = 0;
  b->data = (uint8_t*)HC_ALLOC(b->size);
  HC_RET_WHEN(!b->data, HC_RET(HC_RET_BUF_BASE, HC_RET_MALLOC));
  return HC_RET_OK;
}
int hbuf_fini(hbuf_t* b) {
  HC_RET_WHEN(!b, HC_RET(HC_RET_BUF_BASE, HC_RET_PARAM(0)));
  HC_RET_WHEN(!b->size, HC_RET(HC_RET_BUF_BASE, HC_RET_PARAM(1)));
  HC_RET_WHEN(!b->data, HC_RET(HC_RET_BUF_BASE, HC_RET_PARAM(2)));

  HC_FREE(b->data);
  b->len = b->size = 0;
  b->data = NULL;
  return HC_RET_OK;
}
int hbuf_grow(hbuf_t* b, size_t wish_size) {
  HC_RET_WHEN(wish_size <= b->size, HC_RET_OK);

  hbuf_t target = *b;
  b->size = HBUF_DATA_SIZE_MIN;
  HC_GETMINPOW(wish_size, target.size);
  HC_RET_WHEN(HBUF_DATA_SIZE_MAX < target.size,
              HC_RET(HC_RET_BUF_BASE, HC_RET_NOSPACE));

  target.data = HC_REALLOC(target.data, target.size);
  HC_RET_WHEN(!target.data, HC_RET(HC_RET_BUF_BASE, HC_RET_MALLOC));

  *b = target;
  return HC_RET_OK;
}
int hbuf_push(hbuf_t* b, const void* d, size_t dsize) {
  HC_RET_WHEN(!b || !d || !dsize || !b->size, HC_RET(HC_RET_BUF_BASE, HC_RET_PARAM(0)));

  int r = HC_RET_OK;
  r = hbuf_grow(b, b->len + dsize);
  HC_RET_WHEN(r, r);
  memcpy(b->data + b->len, d, dsize);
  b->len += dsize;
  return HC_RET_OK;
}

int hbuf_remove(hbuf_t* b, size_t index, size_t dsize){
 HC_RET_WHEN(!b  || !dsize, HC_RET(HC_RET_BUF_BASE, HC_RET_PARAM(0)));
 HC_RET_WHEN(0 != (b->len%dsize), HC_RET(HC_RET_BUF_BASE, HC_RET_PARAM(2)));
 HC_RET_WHEN((index+1)*dsize >b->len, HC_RET(HC_RET_BUF_BASE, HC_RET_NOTFOUND));
  // hsession_t* s = (hsession_t*)sbuf->data;
  size_t n = b->len / dsize;
  memmove(b->data + index*dsize ,b->data + (index+1)*dsize, (n - index - 1)*dsize);
  b->len -= dsize;
  return HC_RET_OK;
}

int hbuf_clear(hbuf_t* b) {
  HC_RET_WHEN(!b, HC_RET(HC_RET_BUF_BASE, HC_RET_PARAM(0)));
  b->len = 0;
  return HC_RET_OK;
}

// int hbuf_print(hbuf_t* b, size_t ofs, size_t size) {
//   for (uint64_t i = ofs; size--; i++) {
//     printf("%02X\t", b->data[i]);
//   }
//   return 0;
// }
