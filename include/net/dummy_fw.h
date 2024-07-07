// include/net/dummy_fw.h
#ifndef _NET_DUMMY_FW_H
#define _NET_DUMMY_FW_H

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/skbuff.h>
#include <net/netlink.h>
#include <net/act_api.h>
#include <net/pkt_cls.h>
#include <net/sch_generic.h>
#include <net/tc_wrapper.h>

// CUSTOMIZE: Moved here
struct fw_filter {
	struct fw_filter __rcu	*next;
	u32			id;
	struct tcf_result	res;
	int			ifindex;
	struct tcf_exts		exts;
	struct tcf_proto	*tp;
	struct rcu_work		rwork;
};

// CUSTOMIZE: Create args for the function
struct dummy_fw_set_parms_args {
    struct net *net;
    struct tcf_proto *tp;
    struct fw_filter *f;
    struct nlattr **tb;
    struct nlattr **tca;
    unsigned long base;
    u32 flags;
    struct netlink_ext_ack *extack;
};

// int dummy_fw_set_parms(struct dummy_fw_set_parms_args *args);
int dummy_fw_set_parms(struct net *net, struct tcf_proto *tp,
			struct fw_filter *f, unsigned long base, u32 flags,
			struct netlink_ext_ack *extack);

#endif /* _NET_DUMMY_FW_H */