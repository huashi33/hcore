#include "hlist.h"



// LIST
int hlist_clear(hlist_t* l) {
  HC_RET_WHEN(!l, HC_RET(HC_RET_BASE_LIST, HC_RET_PARAM(0)));
  l->head = NULL;
  l->size = 0;
  return HC_RET_OK;
}

int hlist_append(hlist_t* l, hlistitem_t* i) {
  HC_RET_WHEN(!l, HC_RET(HC_RET_BASE_LIST, HC_RET_PARAM(0)));
  HC_RET_WHEN(!i, HC_RET(HC_RET_BASE_LIST, HC_RET_PARAM(1)));

  i->next = NULL;
  hlistitem_t* cur = l->head;
  if (!cur) {
    l->head = i;
  } else {
    while (cur->next) {
      cur = cur->next;
    }
    cur->next = i;
  }
  ++l->size;
  return HC_RET_OK;
}
int hlist_insert(hlist_t* l, hlistitem_t* i, uint32_t index) {
  HC_RET_WHEN(!l, HC_RET(HC_RET_BASE_LIST, HC_RET_PARAM(0)));
  HC_RET_WHEN(!i, HC_RET(HC_RET_BASE_LIST, HC_RET_PARAM(1)));
  HC_RET_WHEN(index >= l->size, HC_RET(HC_RET_BASE_LIST, HC_RET_PARAM(2)));


  if (0 == index) {
    i->next = l->head;
    l->head = i;
  } else {
    hlistitem_t* cur = l->head;
    for (size_t i = 0; i < index - 1; ++i) {
      cur = cur->next;
    }

    i->next = cur->next;
    cur->next = i;
  }
  ++l->size;
  return HC_RET_OK;
}
hlistitem_t* hlist_at(hlist_t* l, uint32_t index) {
  HC_RET_WHEN(!l, NULL);
  HC_RET_WHEN(index >= l->size, NULL);
  HC_RET_WHEN(0 == index, l->head);

  hlistitem_t* cur = l->head;
  for (size_t i = 0; i < index; ++i) {
    cur = cur->next;
  }
  return cur;
}
hlistitem_t* hlist_remove(hlist_t* l, uint32_t index) {
  HC_RET_WHEN(!l || index >= l->size, NULL);

  hlistitem_t* toRemove;
  if (0 == index) {
    toRemove = l->head;
    l->head = l->head->next;
  } else {
    hlistitem_t* cur = l->head;
    for (size_t i = 0; i < index - 1; ++i) {
      cur = cur->next;
    }
    toRemove = cur->next;
    cur->next = toRemove->next;
  }
  --l->size;
  return toRemove;
}
hlistitem_t* hlist_find_first(hlist_t* l, hlistitem_t* tar,hlist_compare_t cmp) {
  HC_RET_WHEN(!l || !tar || !cmp || !l->size, NULL);

  hlistitem_t* toRet = NULL;
  hlistitem_t* cur = l->head;
  for (size_t i = 0; i < l->size; ++i) {
    if (cmp(cur, tar)) {
      toRet = cur;
      break;
    }
    cur = cur->next;
  }
  return toRet;
}
hlistitem_t* hlist_remove_first(hlist_t* l, hlistitem_t* tar,hlist_compare_t cmp) {
  HC_RET_WHEN(!l || !tar || !cmp || !l->size, NULL);

  hlistitem_t* toRemove;
  if (cmp(l->head, tar)) {
    toRemove = l->head;
    l->head = l->head->next;
  } else {
    toRemove = l->head;
    hlistitem_t* cur = toRemove->next;
    for (size_t i = 0; i < l->size - 1; ++i) {
      if (cmp(cur, tar)) {
        toRemove->next = cur->next;
        toRemove = cur;
        break;
      }
      toRemove = cur;
      cur = cur->next;
    }
  }
  --l->size;
  return toRemove;
}
int hislist_foreach(hlist_t* l, hlist_foreach_t f) {
  HC_RET_WHEN(!l, HC_RET(HC_RET_BASE_LIST, HC_RET_PARAM(0)));
  HC_RET_WHEN(!f, HC_RET(HC_RET_BASE_LIST, HC_RET_PARAM(1)));

  hlistitem_t* it = l->head;
  for (int i = 0; i < l->size; ++i) {
    f(it, i);
    it = it->next;
  }
  return HC_RET_OK;
}