/*
 ============================================================================
 Name        : global.c
 Author      : Charalampos Mysirlidis
 Copyright   : GPLv3
 Description : Global functions, variables, structures, includes and all the stuff
 ============================================================================
 */


#include "global.h"

#include <stdarg.h>

FILE *glb_log_file;
char *glb_log_fpath = "../logs/global.log";

int glb_logging_level = 4, glb_printf_log_lvl = 3;


int glb_init(){

	glb_log_file = fopen(glb_log_fpath, "w");

	if(glb_log_file==NULL){
		printf("Error while opening Global log file!\n");
		return(FAIL);
	}

	return(SUCCESS);
}

void glb_log(int log_lvl, char *msg, ...){

	va_list arg;
	va_start(arg, msg);
	char log[MAX_LOG_MSG];
	vsprintf(log, msg, arg);

	if ( log_lvl <= glb_logging_level )
		fputs(log, glb_log_file);

	if ( log_lvl <= glb_printf_log_lvl )
		puts(log);

	va_end(arg);
}

void glb_exit(){

	fclose(glb_log_file);
}

void glb_test_logging(){

	glb_log(debug, "This is a debug");
	glb_log(info, "This is a info");
	glb_log(error, "This is a error");
	glb_log(critical, "This is a critical");

}


char *ip_proto_name[150] = { "HOPOPT", "ICMP", "IGMP", "GGP", "IPv4", "ST",
		"TCP", "CBT", "EGP", "IGP", "BBN-RCC-MON", "NVP-II", "PUP", "ARGUS",
		"EMCON", "XNET", "CHAOS", "UDP", "MUX", "DCN-MEAS", "HMP", "PRM",
		"XNS-IDP", "TRUNK-1", "TRUNK-2", "LEAF-1", "LEAF-2", "RDP", "IRTP",
		"ISO-TP4", "NETBLT", "MFE-NSP", "MERIT-INP", "DCCP", "3PC", "IDPR",
		"XTP", "DDP", "IDPR-CMTP", "TP++", "IL", "IPv6", "SDRP", "IPv6-Route",
		"IPv6-Frag", "IDRP", "RSVP", "GRE", "MHRP", "BNA", "ESP", "AH",
		"I-NLSP", "SWIPE", "NARP", "MOBILE", "TLSP", "SKIP", "IPv6-ICMP",
		"IPv6-NoNxt", "IPv6-Opts", "", "CFTP", "", "SAT-EXPAK", "KRYPTOLAN",
		"RVD", "IPPC", "", "SAT-MON", "VISA", "IPCV", "CPNX", "CPHB", "WSN",
		"PVP", "BR-SAT-MON", "SUN-ND", "WB-MON", "WB-EXPAK", "ISO-IP", "VMTP",
		"SECURE-VMTP", "VINES", "IPTM", "NSFNET-IGP", "DGP", "TCF", "EIGRP",
		"OSPF", "Sprite-RPC", "LARP", "MTP", "AX.25", "IPIP", "MICP", "SCC-SP",
		"ETHERIP", "ENCAP", "", "GMTP", "IFMP", "PNNI", "PIM", "ARIS", "SCPS",
		"QNX", "A/N", "IPComp", "SNP", "Compaq-Peer", "IPX-in-IP", "VRRP",
		"PGM", "", "L2TP", "DDX", "IATP", "STP", "SRP", "UTI", "SMP", "SM",
		"PTP", "IS-IS over IPv4", "FIRE", "CRTP", "CRUDP", "SSCOPMCE", "IPLT",
		"SPS", "PIPE", "SCTP", "FC", "RSVP-E2E-IGNORE", "Mobility pkt_stat_header",
		"UDPLite", "MPLS-in-IP", "manet", "HIP", "Shim6", "WESP", "ROHC" };




