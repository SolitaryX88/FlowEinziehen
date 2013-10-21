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



static u_int32_t nfqp_print_pkt (struct nfq_data *tb);

int nfqp_set_queue(int num);

static int nfqp_cb(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg, struct nfq_data *nfa, void *data);

void nfqp_log(int log_lvl, char *msg, ...);

int nfqp_init_nfqueue();

int nfqp_init();

int nfqp_exit();

void nfqp_test_logging();

int nfqp_analyzer_function(void *args);


#endif /* NFQ_PROC_H_ */
