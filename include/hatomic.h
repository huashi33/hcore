#ifndef __HATOMIC_H__
#define __HATOMIC_H__
#include <stdint.h>

#define hatomic_int32_t int32_t
#define hatomic_uint32_t uint32_t
#define hatomic_int64_t int64_t
#define hatomic_uint64_t uint64_t

#define hatomic_add(ptr, val) __atomic_add_fetch(ptr, val, __ATOMIC_RELAXED);
#endif // __HATOMIC_H__