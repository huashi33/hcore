#include "hrq.h"
#include "string.h"
#define HRQ_DATA_SIZE_MAX 1024 * 1024 * 1024  // 1 GB
#define HRQ_DATA_SIZE_MIN 32                 // 32 byte
// RQ
size_t hrb_alloc_size(size_t s){
  size_t size = HRQ_DATA_SIZE_MIN;
  HC_GETMINPOW(s, size);
  return sizeof(hrb_t) + size;
}
hrb_t* hrb_alloc(size_t s) {
  size_t size = hrb_alloc_size(s);
  HC_RET_WHEN(HRQ_DATA_SIZE_MAX < size,NULL);
  hrb_t* rb = (hrb_t*)HC_ALLOC(size);
  rb->size = size;
  rb->in = 0;
  rb->out = 0;
  return rb;
}
int hrb_free(hrb_t* rb) {
  if(rb && rb->size){
    HC_FREE(rb);
  }
  return HC_RET_OK;
}
size_t hrb_push(hrb_t* rb, void* data, size_t size) {
  HC_RET_WHEN(!rb || !data || !size || !rb->size, HC_RET(HC_RET_BASE_RB, HC_RET_PARAM(0)));


  size_t len = HRB_MIN(rb->size - (rb->in - rb->out), size);
  size_t trueIn = rb->in & (rb->size - 1);
  size_t l1 = HRB_MIN(len, rb->size - trueIn);
  memcpy(rb->data + trueIn, data, l1);
  memcpy(rb->data, (char*)data + l1, len - l1);
  rb->in += len;
  return len;
}
size_t hrb_pop(hrb_t* rb, void* data, size_t size) {
  HC_RET_WHEN(!rb || !data || !size, HC_RET(HC_RET_BASE_RB, HC_RET_PARAM(0)));
  HC_RET_WHEN(!rb->size, HC_RET(HC_RET_BASE_RB, HC_RET_PARAM(1)));
  size_t len = HRB_MIN(rb->in - rb->out, size);
  size_t trueOut = rb->out & (rb->size - 1);
  size_t l1 = HRB_MIN(len, rb->size - trueOut);
  memcpy(data, rb->data + trueOut, l1);
  memcpy((char*)data + l1, rb->data, len - l1);
  rb->out += len;
  return len;
}
size_t hrb_peek(hrb_t* rb, void* data, size_t size){
  HC_RET_WHEN(!rb || !data || !size, HC_RET(HC_RET_BASE_RB, HC_RET_PARAM(0)));
  HC_RET_WHEN(!rb->size, HC_RET(HC_RET_BASE_RB, HC_RET_PARAM(1)));
  size_t len = HRB_MIN(rb->in - rb->out, size);
  size_t trueOut = rb->out & (rb->size - 1);
  size_t l1 = HRB_MIN(len, rb->size - trueOut);
  memcpy(data, rb->data + trueOut, l1);
  memcpy((char*)data + l1, rb->data, len - l1);
  return len;
}