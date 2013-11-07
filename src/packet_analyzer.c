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

	char *config_xml;

	if (argc != 2) {
		printf("Usage: %s config.xml \n", argv[0]);
		return (EXIT_SUCCESS);
	}


	// GET Ctrl+C signal;
	signal(SIGINT, signal_handler);

	/** Read configure file */

	config_xml = argv[1];
	parse_doc(config_xml);


	/** Initialize NFQUEUE forwarding process, logging files e.t.c */

	if(!nfqp_init() || !glb_init()){
		exit(EXIT_FAILURE);
	}

	nfqp_analyzer_function();


	return(EXIT_SUCCESS);

}



void signal_handler() {
	    nfqp_exit();
	    glb_exit();
	    exit(EXIT_FAILURE);
}

