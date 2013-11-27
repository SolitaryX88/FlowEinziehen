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

typedef struct _stat_ip_t{


}stat_ip_t;

typedef struct _stat_tran_t{
	uint16_t	aver_win_size;
	uint32_t	tcp_pkts;
	uint32_t	udp_pkts;

}stat_trans_t;

typedef struct _stat_pkt_t{
	uint32_t 		num_of_pkts;
	uint32_t 		aver_pkt_len;
	stat_ip_t 		ip;
	stat_trans_t	trans;
}stat_pkt_t;



// Main exit procedure
extern void exit_proc();


stat_pkt_t * analyze_pkts (GQueue *q);

void *stats_analysis_deque(void *args);

int stats_init();

void stats_exit();

void switch_queues();

#endif /* STATISTICS_H_ */
