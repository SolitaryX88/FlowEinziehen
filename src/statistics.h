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

typedef struct _max_value_t {
	uint32_t key,
			freq;
} freq_value_t; // The most frequent key -> (IP, Transport layer port)


typedef struct _stat_ip_t{

	freq_value_t freq_source;
}stat_ip_t;

typedef struct _stat_tran_t{
	uint16_t	aver_win_size;
	uint32_t	tcp_pkts;
	uint32_t	udp_pkts;
	freq_value_t freq_source, freq_dest; // The most frequent source/destination port

}stat_trans_t;

typedef struct _stat_pkt_t{
	uint32_t 		num_of_pkts;
	uint32_t 		aver_pkt_len;
	stat_ip_t 		ip;
	stat_trans_t	trans;
}stat_pkt_t;



// Main exit procedure
extern void exit_proc();


stat_pkt_t * analyze_pkts (int id);

void *stats_analysis_deque(void *args);

int stats_init();

void stats_exit();

void switch_stat();

void reset_freq();

#endif /* STATISTICS_H_ */
