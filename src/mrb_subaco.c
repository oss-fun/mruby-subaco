/*
** mrb_subaco.c - Subaco class
**
** Copyright (c) Yuki Nakata 2019
**
** See Copyright Notice in LICENSE
*/

#include "mruby.h"
#include "mruby/data.h"
#include "mruby/array.h"
#include "mrb_subaco.h"
#include "call_vmm.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define DONE mrb_gc_arena_restore(mrb, 0);

typedef struct {
  mrb_int pid;
  mrb_int pid_inum;
} mrb_subaco_data;

static const struct mrb_data_type mrb_subaco_data_type = {
  "mrb_subaco_data", mrb_free,
};

static int vmmcall_set_pid_inum(int pid)
{
  call_vmm_function_t vmm_func;
  call_vmm_arg_t vmm_arg;
  call_vmm_ret_t vmm_ret;

  CALL_VMM_GET_FUNCTION ("set_pid_inum", &vmm_func);
  if (!call_vmm_function_callable (&vmm_func)) {
	return -1;
  }

  vmm_arg.rbx = (long)pid;
  call_vmm_call_function (&vmm_func, &vmm_arg, &vmm_ret);
  return (int)vmm_ret.rax;
}

static int vmmcall_set_whitelist(unsigned char *addr)
{
  unsigned long pack_ipaddr;
  call_vmm_function_t vmm_func;
  call_vmm_arg_t vmm_arg;
  call_vmm_ret_t vmm_ret;

  CALL_VMM_GET_FUNCTION ("set_whitelist", &vmm_func);
  if (!call_vmm_function_callable (&vmm_func)) {
	return -1;
  }

  memcpy(&pack_ipaddr, addr,4);
  vmm_arg.rbx = pack_ipaddr;
  call_vmm_call_function (&vmm_func, &vmm_arg, &vmm_ret);

  return (int)vmm_ret.rax;
}

static mrb_value mrb_subaco_init(mrb_state *mrb, mrb_value self)
{
  struct RClass *util;
  mrb_value mrb_pid_inum;
  mrb_subaco_data *data;
  mrb_int pid, pid_inum;
  
  data = (mrb_subaco_data *)DATA_PTR(self);
  if (data) {
    mrb_free(mrb, data);
  }
  DATA_TYPE(self) = &mrb_subaco_data_type;
  DATA_PTR(self) = NULL;


  mrb_get_args(mrb,"i",&pid);
 
  util = mrb_module_get(mrb,"Util"); 
  mrb_pid_inum = mrb_funcall(mrb,mrb_obj_value(util),"pid_inum",1,mrb_fixnum_value(pid));
  pid_inum = mrb_fixnum(mrb_pid_inum);

  data = (mrb_subaco_data *)mrb_malloc(mrb, sizeof(mrb_subaco_data));
  data->pid = pid;
  data->pid_inum = pid_inum;
  DATA_PTR(self) = data;

  if(vmmcall_set_pid_inum(pid_inum) == -1){
    fprintf (stderr, "[Error] vmmcall \"set_pid_inum\" failed\n");
  }
  return self;
}

static mrb_value mrb_subaco_getpid(mrb_state *mrb, mrb_value self)
{
	mrb_subaco_data *data = DATA_PTR(self); 
	return mrb_fixnum_value(data->pid);
}

static mrb_value mrb_subaco_set_whitelist(mrb_state *mrb, mrb_value self)
{
	unsigned char ipaddr[4];
	mrb_value ip_ary;
	mrb_get_args(mrb,"A", &ip_ary);

	for(int i = 0; i < RARRAY_LEN(ip_ary); i++){
		ipaddr[i] = mrb_fixnum(mrb_ary_ref(mrb, ip_ary, i));
	}
	if (vmmcall_set_whitelist(ipaddr) == -1){
		fprintf (stderr, "[Error] vmmcall \"set_Whitelist\" failed\n");
	}
	return self;
}
void mrb_mruby_subaco_gem_init(mrb_state *mrb)
{
  struct RClass *subaco;
  subaco = mrb_define_class(mrb, "Subaco",mrb->object_class);
  mrb_define_method(mrb, subaco, "initialize", mrb_subaco_init, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, subaco, "getpid", mrb_subaco_getpid, MRB_ARGS_NONE());
  mrb_define_method(mrb, subaco, "set_whitelist", mrb_subaco_set_whitelist, MRB_ARGS_REQ(1));
  DONE;
}

void mrb_mruby_subaco_gem_final(mrb_state *mrb)
{
}

