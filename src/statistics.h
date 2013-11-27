/*
 ============================================================================
 Name        : statistics.h
 Author      : Charalampos Mysirlidis
 Copyright   : GPLv3
 Description : Statistic analysis of the packets
 ============================================================================
 */

#ifndef STATISTICS_H_
#define STATISTICS_H_


// Main exit procedure
extern void exit_proc();



void stats_analyze_data ();

void *stats_analysis_deque(void *args);

void init_stats();

void stats_exit ();


#endif /* STATISTICS_H_ */
