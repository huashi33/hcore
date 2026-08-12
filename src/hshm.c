#include "hshm.h"
#include "hdef.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

int hshm_init(hshm_t* shm, const char* name, size_t size, int init_type){
  HC_RET_WHEN(!shm || !name || !size, HC_RET(HC_RET_BASE_SHM, HC_RET_PARAM(0)));
  HC_RET_WHEN(HSHM_INIT_TYPE_ATTACH != init_type && HSHM_INIT_TYPE_CREATE != init_type, HC_RET(HC_RET_BASE_SHM, HC_RET_PARAM(1)));
  
  shm->init_type = init_type;
  shm->size = size;
  shm->addr = NULL;
  strncpy(shm->name, name, sizeof(shm->name) - 1);
  shm->name[sizeof(shm->name) - 1] = '\0';
  fprintf(stdout, "hshm_init,shm_name:%s,size:%zu,inittype:%d\n",shm->name,shm->size,shm->init_type);

  if(HSHM_INIT_TYPE_ATTACH == init_type){
    const int fd = shm_open(name, O_RDWR, 0600);
    HC_EXEC_RET_WHEN(fd < 0,fprintf(stderr, "shm_open:%s\n",strerror(errno)), HC_RET(HC_RET_BASE_SHM, HC_RET_FILE));
    shm->addr = mmap(NULL, shm->size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    HC_EXEC_RET_WHEN(MAP_FAILED == shm->addr,fprintf(stderr, "mmap:%s\n",strerror(errno)), HC_RET(HC_RET_BASE_SHM, HC_RET_FILE));
    close(fd);
  }else if(HSHM_INIT_TYPE_CREATE == init_type){
    shm_unlink(name);
    const int fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
    HC_EXEC_RET_WHEN(fd < 0,fprintf(stderr, "shm_open:%s\n",strerror(errno)), HC_RET(HC_RET_BASE_SHM, HC_RET_FILE));
    ftruncate(fd, shm->size);
    shm->addr = mmap(NULL, shm->size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    HC_EXEC_RET_WHEN(MAP_FAILED == shm->addr,fprintf(stderr, "mmap:%s\n",strerror(errno)), HC_RET(HC_RET_BASE_SHM, HC_RET_FILE));
  
    close(fd);
  }
  return HC_RET_OK;
}
int hshm_deinit(hshm_t* shm){
  HC_RET_WHEN(!shm || !shm->size, HC_RET(HC_RET_BASE_SHM, HC_RET_PARAM(0)));
  HC_RET_WHEN(HSHM_INIT_TYPE_ATTACH != shm->init_type && HSHM_INIT_TYPE_CREATE != shm->init_type, HC_RET(HC_RET_BASE_SHM, HC_RET_PARAM(1)));
  
  munmap(shm->addr, shm->size);
  if(HSHM_INIT_TYPE_CREATE == shm->init_type){
    shm_unlink(shm->name);
  }
  shm->size = 0;
  shm->addr = NULL;
  return HC_RET_OK;
}

int hshm_write(hshm_t* shm, const void* data, size_t size){
  return 1;
}
int hshm_read(hshm_t* shm, void* data, size_t size){
  return 1;
}
int hshm_realloc(hshm_t* shm, size_t new_size){
  return 1;
}