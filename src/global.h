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

#define BUFSIZE 4096
#define COMMAND_LEN 256
#define MAX_LOG_MSG 1024

#define FAIL 0
#define SUCCESS 1

typedef unsigned short __us;

typedef uint32_t addr_t;

typedef struct _ip_addrs_t{
    addr_t s_addr;
    addr_t d_addr;
} ip_addrs_t;

typedef struct _port_nums_t{
	__us s_port;
	__us d_port;
} port_nums_t;


typedef struct _pkt_t {

	ip_addrs_t ip_addr;
	port_nums_t port_num;
	__us length;
	__us proto;

} pkt_t;



enum {critical=1, error, info, debug};

int glb_init();

void glb_exit();




#endif /* GLOBALVAR_H_ */
