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


#define set_freq(hash)\
void set_freq_##hash(uint32_t key, int value) {\
	if (value > freq_##hash.freq) {\
		glb_log(debug, "New max hurray! key: %u with value: %d\n", key, value);\
		freq_##hash.key = key;\
		freq_##hash.freq = value;\
	}\
}

#define hashes(hash)\
void increase_##hash(uint32_t key){\
	gpointer gen = g_hash_table_lookup(hash, GINT_TO_POINTER(key));\
	if (GPOINTER_TO_INT(gen)) {\
		glb_log(debug, "Key %u found, increase %d by 1\n", key, GPOINTER_TO_INT(gen));\
		g_hash_table_replace(hash, GINT_TO_POINTER(key), GINT_TO_POINTER(GPOINTER_TO_INT(gen) + 1));\
		set_freq_##hash(key, GPOINTER_TO_INT(gen) + 1);\
	} else {\
		glb_log(debug, "New key %u, set value 1\n", key);\
		g_hash_table_insert(hash, GINT_TO_POINTER(key), GINT_TO_POINTER(1));\
	}\
}


int stats_refresh = 1; // in secs
int stats_keep = 1;
int id;

pthread_t stats_thread;
pthread_mutex_t stats_lock;

GQueue* stats_pkt_q;
GQueue* pkt_queues[2];

GHashTable *hash_port_d, *hash_port_s, *hash_ip;

freq_value_t freq_hash_port_s, freq_hash_port_d, freq_hash_ip;

struct timeval stop, start;


/*

void value_destroyed(gpointer data) {
  printf("Got a value destroy call for %d\n", GPOINTER_TO_INT(data));

}*/


// Function: 	Main function that analyze the packets of the queue (thread *)
// 				This functionality could be integrated in nfq_proc.c while processing pkts
//				and could be even more efficient but this project is for educational
//				reasons first. So i wanted to play around with glib-2.0! :-P
//				p.s. The hash data structure of the glib may be a slower version of just
//				using an array and the port N# as the indexes of the array.

void *stats_analysis_deque(void *args){

	stat_pkt_t *stat = NULL;

	while(stats_keep){
		sleep(stats_refresh);

		gettimeofday(&start, NULL);

		switch_stat();
		glb_log(info, "Stats: Queue is now switched, the pkt_q is: %d\n", id);

		// We analyze the opposite queue of which is now filled with packets
		// get the average length of a pkt and average window if TCP
		stat = analyze_pkts(!id);
		gettimeofday(&stop, NULL);

		glb_log(info, "Stats: Packets have been analyzed in %lu (usecs) \n", stop.tv_usec - start.tv_usec);
		glb_log(info, "Stats: Packet analysis info: \n");
		glb_log(info, "\t Packet N#: %u \tUDP: %u \tTCP: %u \n",
				stat->num_of_pkts, stat->trans.udp_pkts, stat->trans.tcp_pkts);
		glb_log(info, "\t Average Pkt Length: %u \t Window: %u \n",
				stat->aver_pkt_len, stat->trans.aver_win_size);
		glb_log(info, "Most Frequent IP: %s \t S_Port: %u \t D_Port %u\n",
				inet_ntoa(*(struct in_addr *) &stat->ip.freq_source),
				stat->trans.freq_source, stat->trans.freq_dest);
		free(stat);
		//the most common port, transport layer

	}

	return((int*)SUCCESS);
};


set_freq(hash_port_d)
set_freq(hash_port_s)
set_freq(hash_ip)

hashes(hash_port_d)
hashes(hash_port_s)
hashes(hash_ip)


stat_pkt_t* analyze_pkts(int id){

	GQueue *q  = pkt_queues[id];

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

		increase_hash_port_d(p->trans.port_num.d_port);
		increase_hash_port_s(p->trans.port_num.s_port);
		increase_hash_ip(p->ip.s_addr);

		free(p);

	} //while

	g_queue_clear(q);

	//Store statistics

	if(num_pkt)
		stat->aver_pkt_len = size / num_pkt;
	else
		stat->aver_pkt_len = size;

	stat->num_of_pkts = num_pkt;

	stat->trans.tcp_pkts = num_tcp;
	stat->trans.udp_pkts = num_udp;

	if(num_pkt)
		stat->trans.aver_win_size = window / num_pkt;
	else
		stat->trans.aver_win_size = window ;

	//Get the most frequent values

	stat->trans.freq_dest	= freq_hash_port_d;
	stat->trans.freq_source	= freq_hash_port_s;
	stat->ip.freq_source	= freq_hash_ip;

	//Clear the hash tables / max values

	reset_freq();

	return (stat);

}


void reset_freq(){

	freq_hash_port_d.freq = 0; freq_hash_port_s.freq =0 ; freq_hash_ip.freq = 0;
	freq_hash_port_d.key = 0; freq_hash_port_s.key =0 ; freq_hash_ip.key = 0;

	g_hash_table_remove_all (hash_port_d);
	g_hash_table_remove_all (hash_port_s);
	g_hash_table_remove_all (hash_ip);

	glb_log(info, "Stat: Hashes removed keys, most frequent values reseted to 0\n");
}

int stats_init(){

	id = 0;

	//Setting up the queues
	pkt_queues[1] = g_queue_new();
	pkt_queues[0] = g_queue_new();

	stats_pkt_q = pkt_queues[id];

	hash_port_d = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL,NULL); 	// There is no need to provide a destroyer.
	hash_port_s = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL,NULL);	// The values are not dynamically allocated
	hash_ip 	= g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL,NULL);

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


void switch_stat(){

	pthread_mutex_lock(&stats_lock);

	if(pkt_queues[0]==stats_pkt_q){
		id = 1;
		stats_pkt_q = pkt_queues[id];
	}
	else{
		id = 0;
		stats_pkt_q = pkt_queues[id];
	}

	pthread_mutex_unlock(&stats_lock);

}


void stats_exit (){

	stats_keep = 0;

	pthread_join(stats_thread, NULL);

	//TODO: Do we need to destroy the thread at last?

	g_queue_free(pkt_queues[1]);
	g_queue_free(pkt_queues[0]);

	g_hash_table_destroy(hash_ip);
	g_hash_table_destroy(hash_port_d);
	g_hash_table_destroy(hash_port_s);

}
