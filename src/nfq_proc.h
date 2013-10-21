/*
 ============================================================================
 Name        : nfq_proc.h
 Author      : Charalampos Mysirlidis
 Copyright   : GPLv3
 Description : The packet analysis process out of the NFQueue in C, Ansi-style
 ============================================================================
 */

#ifndef NFQ_PROC_H_
#define NFQ_PROC_H_

#include <getopt.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/netfilter.h>
#include <libnetfilter_queue/libnetfilter_queue.h>
#include <linux/ip.h>

#include "global.h"


static u_int32_t nfqp_print_pkt (struct nfq_data *tb);

static int nfqp_cb(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg, struct nfq_data *nfa, void *data);

int nfqp_set_queue(int queue_num);

void nfqp_log_msg(char *msg);

int nfqp_init();

int nfqp_exit();

int nfqp_analyzer_function(void *args);

void nfqp_test_logging(void);


#endif /* NFQ_PROC_H_ */
