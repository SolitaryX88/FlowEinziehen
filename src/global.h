/*
 ============================================================================
 Name        : global.h
 Author      : Charalampos Mysirlidis
 Copyright   : GPLv3
 Description : Global functions, variables, structures, includes and all the stuff
 ============================================================================
 */

#ifndef GLOBALVAR_H_
#define GLOBALVAR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <linux/types.h>
#include <glib.h>

#define MICROSECS 1000

#define BUFSIZE 4096
#define COMMAND_LEN 256
#define MAX_LOG_MSG 1024

#define FAIL 0
#define SUCCESS 1


#define mem_alloc(type, size)\
        (type *) malloc (sizeof (type) * size);



typedef unsigned short __us_t;

typedef __be32 addr_t;

typedef uint16_t port_t;

typedef struct _ip_addrs_t{
	addr_t s_addr;
	addr_t d_addr;
} ip_addrs_t;

typedef struct _port_nums_t{
	port_t s_port;
	port_t d_port;
} port_nums_t;

typedef struct _trans_t{
	__us_t proto;
	port_nums_t port_num;
	__us_t window_size; // only for TCP packets
} trans_t;

typedef struct _pkt_t {

	ip_addrs_t ip;
	trans_t trans;
	__us_t length;
	struct timeval time;

} pkt_t;


enum {critical=1, error, info, debug};

// Function declaration

void glb_log(int log_lvl, char *msg, ...);

void glb_test_logging();

int glb_init();

void glb_exit();


#endif /* GLOBALVAR_H_ */
