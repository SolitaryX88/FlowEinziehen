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

#define BUFSIZE 2048
#define COMMAND_LEN 256
#define MAX_LOG_MSG 1024

#define FAIL 0
#define SUCCESS 1

typedef unsigned short __us;


typedef struct _pkt_stat_min_t {

	char sIP_dec[15];
	char protocol_name;
	__us sPort;
	__us length;
	__us proto;
	struct _pkt_stat_min_t *next;

} pkt_stat_min_t;


enum {critical=1, error, info, debug};




/*

*/

#endif /* GLOBALVAR_H_ */
