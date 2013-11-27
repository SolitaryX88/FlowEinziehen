/*
 ============================================================================
 Name        : packet_analyzer.h
 Author      : Charalampos Mysirlidis
 Copyright   : GPLv3
 Description : Main process in C, Ansi-style
 ============================================================================
 */

#ifndef PACKET_ANALYZER_H_
#define PACKET_ANALYZER_H_

/*	Logging levels
 *
    1 -> CRITICAL - for critical errors
    2 -> ERROR - for regular errors
    2 -> WARNING - for warning messages
    3 -> INFO - for informational messages
    4 -> DEBUG - for debugging messages
*
*/



void exit_proc();

void init();


// 		EXTERN!

// global

extern int glb_init();

extern void glb_exit();


// nfq_proc

extern int nfqp_init();

extern int nfqp_exit();

extern void nfqp_test_logging(void);

extern int nfqp_analyzer_function();


// config_xml

extern void parse_doc(char *docname);

// statistics.c

extern void stats_exit();



#endif /* PACKET_ANALYZER_H_ */
