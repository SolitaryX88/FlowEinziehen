/*
 ============================================================================
 Name        : packet_analyzer.c
 Author      : Charalampos Mysirlidis
 Copyright   : GPLv3
 Description : Main process in C, Ansi-style
 ============================================================================
 */


#include "packet_analyzer.h"

#include "global.h"
#include "config_xml.h"
#include "gui_socket.h"
#include "nfq_proc.h"


#include <linux/ip.h>

//#include "Socket.h"



int main(int argc, char **argv) {

	nfqp_init();

	nfqp_test_logging();

	nfqp_exit();
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
	//nfqp_init();




	return(EXIT_SUCCESS);

}
