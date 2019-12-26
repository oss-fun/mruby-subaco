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
  mrb_int pid;
} mrb_cvisor_data;

static const struct mrb_data_type mrb_cvisor_data_type = {
  "mrb_cvisor_data", mrb_free,
};

static int vmmcall_setpid(int pid)
{
  call_vmm_function_t vmm_func;
  call_vmm_arg_t vmm_arg;
  call_vmm_ret_t vmm_ret;

  CALL_VMM_GET_FUNCTION ("setpid", &vmm_func);
  if (!call_vmm_function_callable (&vmm_func)) {
	return -1;
  }

  vmm_arg.rbx = (long)pid;
  call_vmm_call_function (&vmm_func, &vmm_arg, &vmm_ret);
  return (int)vmm_ret.rax;
}

static mrb_value mrb_cvisor_init(mrb_state *mrb, mrb_value self)
{
  mrb_cvisor_data *data;
  mrb_int pid;

  data = (mrb_cvisor_data *)DATA_PTR(self);
  if (data) {
    mrb_free(mrb, data);
  }
  DATA_TYPE(self) = &mrb_cvisor_data_type;
  DATA_PTR(self) = NULL;


  mrb_get_args(mrb,"i",&pid);
  data = (mrb_cvisor_data *)mrb_malloc(mrb, sizeof(mrb_cvisor_data));
  data->pid = pid;
  DATA_PTR(self) = data;

  if(vmmcall_setpid(pid) == -1){
    fprintf (stderr, "vmmcall \"setpid\" failed\n");
  }
  return self;
}

static mrb_value mrb_cvisor_getpid(mrb_state *mrb, mrb_value self)
{
	mrb_cvisor_data *data = DATA_PTR(self); 
	return mrb_fixnum_value(data->pid);
}

void mrb_mruby_cvisor_gem_init(mrb_state *mrb)
{
  struct RClass *cvisor;
  cvisor = mrb_define_class(mrb, "CVisor", mrb->object_class);
  mrb_define_method(mrb, cvisor, "initialize", mrb_cvisor_init, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, cvisor, "getpid", mrb_cvisor_getpid, MRB_ARGS_NONE());
  DONE;
}

void mrb_mruby_cvisor_gem_final(mrb_state *mrb)
{
}

