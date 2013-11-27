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
	signal(SIGINT, exit_proc);

	/** Read configure file */

	config_xml = argv[1];
	parse_doc(config_xml);


	/** Initialize NFQUEUE forwarding process, logging files e.t.c */

	if(!nfqp_init() || !glb_init() || !stats_init() /* MYSQL , */){
		exit_proc();
	}




	nfqp_analyzer_function();



	return(EXIT_SUCCESS);

}



void exit_proc() {
	    nfqp_exit();
	    stats_exit();
	    glb_exit();
	    exit(EXIT_FAILURE);
}

