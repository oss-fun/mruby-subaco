/*
** mrb_cvisor.c - CVisor class
**
** Copyright (c) Yuki Nakata 2019
**
** See Copyright Notice in LICENSE
*/

#include "mruby.h"
#include "mruby/data.h"
#include "mrb_cvisor.h"
#include "call_vmm.h"
#define DONE mrb_gc_arena_restore(mrb, 0);

typedef struct {
  char *str;
  mrb_int len;
} mrb_cvisor_data;

static const struct mrb_data_type mrb_cvisor_data_type = {
  "mrb_cvisor_data", mrb_free,
};

static mrb_value mrb_cvisor_init(mrb_state *mrb, mrb_value self)
{
  mrb_cvisor_data *data;
  char *str;
  mrb_int len;

  data = (mrb_cvisor_data *)DATA_PTR(self);
  if (data) {
    mrb_free(mrb, data);
  }
  DATA_TYPE(self) = &mrb_cvisor_data_type;
  DATA_PTR(self) = NULL;

  mrb_get_args(mrb, "s", &str, &len);
  data = (mrb_cvisor_data *)mrb_malloc(mrb, sizeof(mrb_cvisor_data));
  data->str = str;
  data->len = len;
  DATA_PTR(self) = data;

  return self;
}
void mrb_mruby_cvisor_gem_init(mrb_state *mrb)
{
  struct RClass *cvisor;
  cvisor = mrb_define_class(mrb, "CVisor", mrb->object_class);
  mrb_define_method(mrb, cvisor, "initialize", mrb_cvisor_init, MRB_ARGS_REQ(1));
  DONE;
}

void mrb_mruby_cvisor_gem_final(mrb_state *mrb)
{
}

