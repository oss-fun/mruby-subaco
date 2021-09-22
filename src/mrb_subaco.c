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
	mrb_int ip_addr[4];
	mrb_int mac;
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

static int
vmmcall_set_label (int macaddr, int value)
{
	call_vmm_function_t vmm_func;
	call_vmm_arg_t vmm_arg;
	call_vmm_ret_t vmm_ret;

        CALL_VMM_GET_FUNCTION ("set_container_network_label", &vmm_func);

        if (!call_vmm_function_callable (&vmm_func))
		return 0;

	vmm_arg.rbx = (long)macaddr;
	vmm_arg.rcx = (long)value;
	call_vmm_call_function (&vmm_func, &vmm_arg, &vmm_ret);

	return 1;
}

static int
vmmcall_get_vnic_ipaddr(mrb_subaco_data *data, int device_no, int func_no)
{
	call_vmm_function_t vmm_func;
	call_vmm_arg_t vmm_arg;
	call_vmm_ret_t vmm_ret;

	CALL_VMM_GET_FUNCTION("get_vnic_ipaddr", &vmm_func);
	if (!call_vmm_function_callable (&vmm_func))
		return 0;

	vmm_arg.rbx = (long)device_no;
	vmm_arg.rcx = (long)func_no;

	call_vmm_call_function (&vmm_func, &vmm_arg, &vmm_ret);

	data->ip_addr[0] = (int)vmm_ret.rax;
	data->ip_addr[1] = (int)vmm_ret.rbx;
	data->ip_addr[2] = (int)vmm_ret.rcx;
	data->ip_addr[3] = (int)vmm_ret.rdx;

	return 1;
}

static mrb_value
mrb_connect_with_vmm (mrb_state *mrb, mrb_value self)
{
	struct RClass *util;
	mrb_subaco_data *data;
	mrb_int mac, device_no, func_no;
  
	data = (mrb_subaco_data *)DATA_PTR(self);
	if (data)
		mrb_free(mrb, data);

	DATA_TYPE (self) = &mrb_subaco_data_type;
	DATA_PTR (self) = NULL;
	data = (mrb_subaco_data *)mrb_malloc(mrb, sizeof(mrb_subaco_data));

	mrb_get_args (mrb,"iii",&mac, &device_no, &func_no);
	data->mac = mac;

	if (!vmmcall_get_vnic_ipaddr(data, device_no, func_no))
		fprintf (stderr, "[Error] vmmcall \"get_vnic_ipaddr\" failed\n");

	DATA_PTR(self) = data;
	return self;
}

static mrb_value
mrb_subaco_set_label (mrb_state *mrb, mrb_value self)
{
	mrb_int value;
	mrb_subaco_data *data = DATA_PTR(self);
	mrb_get_args (mrb, "i", &value);

	if (!vmmcall_set_label (data->mac, value))
		fprintf (stderr, "[Error] vmmcall \"set_container_network_label\" failed\n");

	return self;
}

static mrb_value
mrb_subaco_global_network (mrb_state *mrb, mrb_value self)
{
	mrb_int value;
	mrb_subaco_data *data = DATA_PTR(self);
	mrb_get_args (mrb, "i", &value);

	if (!vmmcall_set_global_network (data->mac, value))
		fprintf (stderr, "[Error] vmmcall \"set_global_network\" failed\n");

	return self;
}

static mrb_value
mrb_subaco_get_ipaddr ( mrb_state *mrb, mrb_value self)
{
	mrb_int dev_no, func_no,digit;
	mrb_get_args (mrb, "iii", &dev_no, &func_no, &digit);
	mrb_subaco_data *data = DATA_PTR(self);
	return mrb_fixnum_value(data->ip_addr[digit]);
}

void
mrb_mruby_subaco_gem_init (mrb_state *mrb)
{
	struct RClass *subaco;
	subaco = mrb_define_class (mrb, "Subaco",mrb->object_class);
	mrb_define_method (mrb, subaco, "connect_with_vmm", mrb_connect_with_vmm, MRB_ARGS_REQ(3));
	mrb_define_method (mrb, subaco, "set_global_network", mrb_subaco_global_network, MRB_ARGS_REQ(1));
	mrb_define_method (mrb, subaco, "set_label", mrb_subaco_set_label, MRB_ARGS_REQ(1));
	mrb_define_method (mrb, subaco, "get_ip_addr", mrb_subaco_get_ipaddr, MRB_ARGS_REQ(3));

	DONE;
}

void
mrb_mruby_subaco_gem_final (mrb_state *mrb)
{

}

