/*
 ============================================================================
 Name        : packet_analyzer.c
 Author      : Charalampos Mysirlidis
 Copyright   : GPLv3
 Description : Main process in C, Ansi-style
 ============================================================================
 */

#include "global.h"
#include "packet_analyzer.h"
#include <signal.h>


int main(int argc, char **argv) {

	signal(SIGINT, signal_handler);



//
//	int arguments;
//
//	    while ( -1 != (arguments = getopt (argc, argv, "o:h")))
//	    {
//	        switch (arguments)
//	        {
//	            case 'o' :
//	                pcap_destination = (char *) malloc(strlen(optarg) * sizeof(char));
//	                memcpy(pcap_destination,optarg,strlen(optarg));
//	                fprintf(stdout,"pcap recording into %s\n",pcap_destination);
//	                use_pcap = 1;
//	                break;
//	            case 'h':
//	                fprintf(stdout,"nfqueue_recorder: record/display traffic passing through a netfilter queue\n\n"
//	                    "-h: this help\n"
//	                    "-o <file> : record in pcap <file>\n"
//	                    "\n First of all install all the required depedencies\n"
//	                    "\t iptables -I INPUT -p tcp --dport 443 -j NFQUEUE\n"
//	                    "\t iptables -I FORWARD -j NFQUEUE\n"
//	                    "\n ex: ./nfqueue_recorder -o traffic.pcap\n");
//	                return 0;
//	            default:
//	                fprintf(stdout,"use -h for help\n");
//	                return -1;
//	        }
//	    }

// GET Ctrl+C signal;

	/** Read configure file */


	/** Initialize NFQUEUE forwarding process, logging files e.t.c */

	if(!nfqp_init() || !glb_init()){
		exit(EXIT_FAILURE);
	}

	nfqp_analyzer_function();




	return(EXIT_SUCCESS);

}



void signal_handler() {
	    nfqp_exit();
	    exit(EXIT_FAILURE);
}

