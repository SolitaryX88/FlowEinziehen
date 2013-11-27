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


int refresh_deque = 1000*MICROSECS; // Every (var) * MICROSECS (ms) that the deque is refreshed
int stats_keep = 1;

pthread_t stats_thread;
pthread_mutex_t stats_lock;
GQueue* stats_pkt_q;


void stats_analyze_data (){

	// g = (Get the current length of the Dequeue - 1);

	// For (g ; g>0 ; g--)


		// pop head
		// Analyze

}

void *stats_analysis_deque(void *args){



	while(stats_keep){
		usleep(stats_keep);

		pthread_mutex_lock(&stats_lock);



		pthread_mutex_unlock(&stats_lock);

	}


	return((int*)SUCCESS);
}

void init_stats(){


    int err = pthread_create(&stats_thread, NULL, &stats_analysis_deque, NULL);

    if (pthread_mutex_init(&stats_lock, NULL) != 0)
    {
        glb_log(critical, "Mutex: stats_lock init failed\n");
        exit_proc();
    }

    if (err != 0)
        glb_log(error, "Can't create thread :[%s] \n", strerror(err));

}

void stats_exit (){

	stats_keep = 0;

	pthread_join(stats_thread, NULL);

}
