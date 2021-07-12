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
	mrb_int addr;
} mrb_subaco_data;

static const struct mrb_data_type mrb_subaco_data_type = {
	"mrb_subaco_data", mrb_free,
};

static int
vmmcall_set_global_network (int macaddr, int value)
{
	call_vmm_function_t vmm_func;
	call_vmm_arg_t vmm_arg;
	call_vmm_ret_t vmm_ret;

        CALL_VMM_GET_FUNCTION ("set_global_network_permission", &vmm_func);

        if (!call_vmm_function_callable (&vmm_func))
		return 0;

	vmm_arg.rbx = (long)macaddr;
	vmm_arg.rcx = (long)value;
	call_vmm_call_function (&vmm_func, &vmm_arg, &vmm_ret);

	return 1;
}

static mrb_value
mrb_subaco_init (mrb_state *mrb, mrb_value self)
{
	struct RClass *util;
	mrb_value mrb_pid_inum;
	mrb_subaco_data *data;
	mrb_int addr;
  
	data = (mrb_subaco_data *)DATA_PTR(self);
	if (data)
		mrb_free(mrb, data);

	DATA_TYPE (self) = &mrb_subaco_data_type;
	DATA_PTR (self) = NULL;

	mrb_get_args (mrb,"i",&addr);
	data = (mrb_subaco_data *)mrb_malloc(mrb, sizeof(mrb_subaco_data));

	data->addr = addr;
	DATA_PTR(self) = data;

	return self;
}

static mrb_value
mrb_subaco_global_network (mrb_state *mrb, mrb_value self)
{
	mrb_int value;
	mrb_subaco_data *data = DATA_PTR(self);
	mrb_get_args (mrb, "i", &value);

	if (!vmmcall_set_global_network (data->addr, value))
		fprintf (stderr, "[Error] vmmcall \"set_global_network\" failed\n");

	return self;
}

void
mrb_mruby_subaco_gem_init (mrb_state *mrb)
{
	struct RClass *subaco;
	subaco = mrb_define_class (mrb, "Subaco",mrb->object_class);
	mrb_define_method (mrb, subaco, "initialize", mrb_subaco_init, MRB_ARGS_REQ(1));
	mrb_define_method (mrb, subaco, "set_global_network", mrb_subaco_global_network, MRB_ARGS_REQ(2));

	DONE;
}

void
mrb_mruby_subaco_gem_final (mrb_state *mrb)
{

}

