#include "hlog.h"
#include "hcfg.h"
#include "zlog.h"
#include <stdlib.h>


#define HLOG_DIR_DEFAULT "../log"
#define HLOG_FORMAT_DEFAULT "%d(%F %T).%ms %-5V [%p:%t] [%c] [%F:%L] %m%n"
#define HLOG_FILE_SIZE_DEFAULT "10MB"
#define HLOG_FILE_COUNT_DEFAULT "5"
#define HLOG_LEVEL_DEFAULT "INFO"





typedef struct hlog_param_ {
  char dir[256];
  char format[128];
  char file_size[32];
  char file_count[32];
  char level[32];
} hlog_param_t;


static int hlog_read_cfgfile(const char *cfgfilename, hlog_param_t *cfg) {
  HC_RET_WHEN(!cfgfilename, HC_RET(HC_RET_BASE_LOG, HC_RET_PARAM(0)));
  HC_RET_WHEN(!cfg, HC_RET(HC_RET_BASE_LOG, HC_RET_PARAM(1)));

  hcfg_t c;
  int r = hcfg_init(&c, cfgfilename, HCFG_TYPE_INI);
  HC_RET_WHEN(r, r);

  r = hcfg_str(&c, "log:dir", cfg->dir, sizeof(cfg->dir));
  if(r){
    snprintf(cfg->dir, sizeof(cfg->dir), "%s", HLOG_DIR_DEFAULT);
  }
  r = hcfg_str(&c, "log:format", cfg->format, sizeof(cfg->format));
  if(r){
    snprintf(cfg->format, sizeof(cfg->format), "%s",HLOG_FORMAT_DEFAULT);
  }
  r = hcfg_str(&c, "log:filesize", cfg->file_size, sizeof(cfg->file_size));
  if(r){
    snprintf(cfg->file_size, sizeof(cfg->file_size), "%s", HLOG_FILE_SIZE_DEFAULT);
  }
  r = hcfg_str(&c, "log:filecount", cfg->file_count, sizeof(cfg->file_count));
  if(r){
    snprintf(cfg->file_count, sizeof(cfg->file_count), "%s", HLOG_FILE_COUNT_DEFAULT);
  }
  r = hcfg_str(&c, "log:level", cfg->level, sizeof(cfg->level));
  if(r){
    snprintf(cfg->level, sizeof(cfg->level), "%s", HLOG_LEVEL_DEFAULT);
  }

  return hcfg_fini(&c);
}

static int hlog_init_zlog(const char *appname, hlog_param_t *cfg) {
  HC_RET_WHEN(!appname, HC_RET(HC_RET_BASE_LOG, HC_RET_PARAM(0)));
  HC_RET_WHEN(!cfg, HC_RET(HC_RET_BASE_LOG, HC_RET_PARAM(2)));


  const char* zlog_cfg_template =
    "[global]\n"
    "strict init = true\n"
    // "log level = %s\n"
    "[formats]\n"
    "main = \"%s\"\n"
    "[rules]\n"
    "%s.%s \"%s/%s.log\" , %s * %s ; main\n";

  int r = 0;
  const int cfgstr_len = 4096;
  char *cfgstr = (char *)malloc(cfgstr_len);
  snprintf(cfgstr, cfgstr_len -1, zlog_cfg_template,
            cfg->format, appname,cfg->level,cfg->dir,appname, cfg->file_size, cfg->file_count);
  cfgstr[cfgstr_len -1] = 0;

  r = zlog_init_from_string(cfgstr);
  if(r){
    fprintf(stderr, "zlog_init_from_string failed, ret=%d\ncfgstr=%s", r, cfgstr);
  }
  r = dzlog_set_category(appname);
  HLOG_DEBUG("log param:\n%s", cfgstr);



  free(cfgstr);
  return HC_RET_OK;
}


int hlog_init(const char *appname,const char* cfgfilename) {
  HC_RET_WHEN(!appname, HC_RET(HC_RET_BASE_LOG, HC_RET_PARAM(0)));
  HC_RET_WHEN(!cfgfilename, HC_RET(HC_RET_BASE_LOG, HC_RET_PARAM(1)));

  int r = 0;
  hlog_param_t param;
  r = hlog_read_cfgfile(cfgfilename, &param);
  r = hlog_init_zlog(appname, &param);
  return HC_RET_OK;
}

int hlog_fini(const char *appname) { 

  HLOG_INFO("fini %s", appname);
  // the lib will register atexit and call zlog_fini
  // zlog_fini();
  return HC_RET_OK;
}

