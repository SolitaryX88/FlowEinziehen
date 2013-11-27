/*
 ============================================================================
 Name        : statistics.c
 Author      : Charalampos Mysirlidis
 Copyright   : GPLv3
 Description : Statistic analysis of the packets
 ============================================================================
 */

#include "global.h"
#include <pthread.h>
#include "statistics.h"


int stats_refresh = 1*1000*MICROSECS; // Every (var) * MICROSECS (ms) that the deque is refreshed
int stats_keep = 1;
int qid;
int opos[] = {1,0};

pthread_t stats_thread;
pthread_mutex_t stats_lock;

GQueue* stats_pkt_q;
GQueue* pkt_queues[2];


// Function: 	Main function that analyze the packets of the queue (thread *)
// 				This functionality could be integrated in nfq_proc.c while processing pkts
//				and could be even more efficient but this project is for educational
//				reasons first. So i wanted to play around with glib-2.0! :-P

void *stats_analysis_deque(void *args){

	stat_pkt_t *stat = NULL;
	GQueue *q  = NULL;
	while(stats_keep){
		usleep(stats_refresh);

		switch_queues();
		glb_log(info, "Stats: Queue is now switched, the pkt_q is: %d\n", qid);

		// We analyze the opposite queue of which is now filled with packets
		q = pkt_queues[opos[qid]];

		//get the average length of a pkt and average window if TCP
		stat = analyze_pkts(q);
		glb_log(info, "Stats: Packets have been analyzed\n");
		glb_log(info, "Stats: Packet analysis info: \n");
		glb_log(info, "\t Packet N#: %ud \tUDP: %ud \tTCP: %ud \n",
				stat->num_of_pkts, stat->trans.udp_pkts, stat->trans.tcp_pkts);
		glb_log(info, "\t Average Pkt Length: %ud \t Window: %ud \n",
				stat->aver_pkt_len, stat->trans.aver_win_size);

		free(stat);
		//the most common port, transport layer

		g_queue_clear(q);

	}

	return((int*)SUCCESS);
};

stat_pkt_t* analyze_pkts(GQueue *q){

	pkt_t* p = NULL;
	stat_pkt_t* stat = mem_alloc(stat_pkt_t,1);

	uint32_t num_pkt = 0, num_tcp = 0, num_udp = 0;
	uint64_t size = 0, window = 0;

	//Check the opposite packet queue
	while (!(g_queue_is_empty(q))) {

		p = (pkt_t *) g_queue_pop_head(q);

		num_pkt++;
		size += p->length;

		if (p->trans.proto == (__us_t) 6) {
			window+=ntohs(p->trans.window_size);
			num_tcp++;
		}

		if (p->trans.proto == (__us_t) 17) {
			num_udp++;
		}

		free(p);

	} //while

	//Store statistics
	stat->aver_pkt_len = size / num_pkt;
	stat->num_of_pkts = num_pkt;

	stat->trans.tcp_pkts = num_tcp;
	stat->trans.udp_pkts = num_udp;

	stat->trans.aver_win_size = window / num_pkt;

	return (stat);

}

int stats_init(){

	//Setting up the queues
	pkt_queues[1] = g_queue_new();
	pkt_queues[0] = g_queue_new();
	qid = 0;
	stats_pkt_q = pkt_queues[qid];

	//Threads and mutexes
    int err = pthread_create(&stats_thread, NULL, &stats_analysis_deque, NULL);

    if (pthread_mutex_init(&stats_lock, NULL) != 0)
    {
        glb_log(critical, "Stats: Mutex stats_lock init failed\n");
        return(FAIL);
    }

    if (err != 0)
        glb_log(error, "Stats: Can't create thread :[%s] \n", strerror(err));

    return(SUCCESS);
}

void switch_queues(){

	pthread_mutex_lock(&stats_lock);

	if(pkt_queues[0]==stats_pkt_q){
		qid = 1;
		stats_pkt_q = pkt_queues[qid];
	}
	else{
		qid = 0;
		stats_pkt_q = pkt_queues[qid];
	}

	pthread_mutex_unlock(&stats_lock);

}

void stats_exit (){

	stats_keep = 0;


	pthread_join(stats_thread, NULL);

	g_queue_free(pkt_queues[1]);
	g_queue_free(pkt_queues[0]);


	//TODO: Do we need to destroy the thread at last?

}
