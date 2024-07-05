// include/net/dummy_fw.h
#ifndef _NET_DUMMY_FW_H
#define _NET_DUMMY_FW_H

#include <net/sch_generic.h>
#include <net/pkt_cls.h>

struct fw_filter {
	struct fw_filter __rcu	*next;
	u32			id;
	struct tcf_result	res;
	int			ifindex;
	struct tcf_exts		exts;
	struct tcf_proto	*tp;
	struct rcu_work		rwork;
};

int noinline __used __visible dummy_fw_set_parms(struct net *net, struct tcf_proto *tp,
                                struct fw_filter *f, struct nlattr **tb,
                                struct nlattr **tca, unsigned long base, u32 flags,
                                struct netlink_ext_ack *extack);

#endif /* _NET_DUMMY_FW_H */