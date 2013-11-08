/*
 ============================================================================
 Name        : nfq_proc.c
 Author      : Charalampos Mysirlidis
 Copyright   : GPLv3
 Description : The packet analysis process out of the NFQueue in C, Ansi-style
 ============================================================================
 */

/*
 *	Part of the code is from https://github.com/jvehent/nfqueue_recorder
 */

#include "nfq_proc.h"
#include "global.h"
#include <glib.h>
#include <stdarg.h>

int nfqp_logging_level = 4, nfqp_printf_log_lvl = 1;
int nfqp_queue_num = 0;
int analyzer_is_alive = 1;
int keep_stats = 1;

char *nfqp_log_fpath = "../logs/nfqp.log";

char nfqp_command[COMMAND_LEN];

FILE *nfqp_log_file;

GQueue* pkt_q;
/*
 #include <glib.h>
typedef struct {
	char* name;
	int priority;
} Task;
Task* make_task(char* name, int priority) {
	Task* t = g_new(Task, 1);
	t->name = name;
	t->priority = priority;
	return t;
}
void prt(gpointer item) {
	printf("%s   ", ((Task*) item)->name);
}
gint sorter(gconstpointer a, gconstpointer b, gpointer data) {
	return ((Task*) a)->priority - ((Task*) b)->priority;
}
int main(int argc, char** argv) {
	GQueue* q = g_queue_new();
	g_queue_push_tail(q, make_task("Reboot server", 2));
	g_queue_push_tail(q, make_task("Pull cable", 2));
	g_queue_push_tail(q, make_task("Nethack", 1));
	g_queue_push_tail(q, make_task("New monitor", 3));
	printf("Original queue: ");
	g_queue_foreach(q, (GFunc) prt, NULL );
	g_queue_sort(q, (GCompareDataFunc) sorter, NULL );
	printf("\nSorted queue: ");
	g_queue_foreach(q, (GFunc) prt, NULL );
	g_queue_free(q);
	return 0;
}
*/
// Function: 	Creates a packet struct from the information given out of the original packet
// Returns: 	A pointer address of the packet.


static void nfqp_print_queue_packets(gpointer item) {
	pkt_t* p = (pkt_t*) item;

    char *saddr = inet_ntoa(*(struct in_addr *) &p->ip.s_addr);

	nfqp_log(debug, "Packet IP info, Address: S: %s",
			saddr);

    char *daddr = inet_ntoa(*(struct in_addr *) &p->ip.d_addr);
    nfqp_log(debug, " D:%s Length: %u \t", daddr, p->length);

	nfqp_log(debug, "Transport info, Protocol: %d "
			"Window Size: %u "
			"Ports: S: %u D:%u \n\n", p->trans.proto, ntohs(p->trans.window_size),
			ntohs(p->trans.port_num.s_port), ntohs(p->trans.port_num.d_port));

}

pkt_t* nfqp_make_pkt(ip_addrs_t ip, __us_t len , trans_t trans ) {
	pkt_t* p = g_new(pkt_t, 1); // allocates one memory packet

	p->ip.s_addr = ip.s_addr;
	p->ip.d_addr = ip.d_addr;
	p->length = len;
	p->trans = trans;

	nfqp_log(debug, "Packet created with values IP: S: %u D: %u, Length: %u,"
			" Proto: %u, sport: %u, dport: %u", p->ip.s_addr, p->ip.d_addr,
			p->length, p->trans.proto, p->trans.port_num.s_port,
			p->trans.port_num.d_port);

	return(p);
}



// Fuction: Process Each Packet. Log them if needed. Packet added to the queue.
// Returns: Packet id

static u_int32_t nfqp_process_pkt (struct nfq_data *tb){
    int id = 0;
    struct nfqnl_msg_packet_hdr *ph;
    u_int32_t mark,ifi;
	int ret;
	char *nf_packet;

	ip_addrs_t ip ;
	__us_t len;
	trans_t trans ;

	//	pkt_t* p = mem_alloc(pkt_t, 1);


	ret = nfq_get_payload(tb, &nf_packet);
	ph = nfq_get_msg_packet_hdr(tb);

	if (ph) {
		id = ntohl(ph->packet_id);
		nfqp_log(debug, "hw_protocol=0x%04x hook=%u id=%u ",
				ntohs(ph->hw_protocol), ph->hook, id);
	}
	mark = nfq_get_nfmark(tb);
	if (mark)
		nfqp_log(debug, "mark=%u ", mark);

	ifi = nfq_get_indev(tb);
	if (ifi)
		nfqp_log(debug, "indev=%u ", ifi);

	ifi = nfq_get_outdev(tb);
	if (ifi)
		nfqp_log(debug, "outdev=%u ", ifi);

	if ((ret >= 0)) {
		nfqp_log(debug, "payload_len=%d bytes\n", ret);
	}


    // parse the packet headers
    struct iphdr *iph = ((struct iphdr *) nf_packet);

    // Computing IP address translation from 32 bits words to 4*8bits decimal
    /* NOTE ON THE LENGTHS
    all lengths used in headers are specified in 32bits words
    thus, to print the size in bytes, we need to multiply this value by 4
    */

    // display IP HEADERS : ip.h line 45
    // ntohs convert short unsigned int, ntohl do the same for long unsigned int


    nfqp_log(debug, "IP{v=%u; ihl=%u; tos=%u; tot_len=%u; id=%u; ttl=%u; protocol=%u; "
        ,iph->version, iph->ihl*4, iph->tos, ntohs(iph->tot_len), ntohs(iph->id), iph->ttl, iph->protocol);


    char *saddr = inet_ntoa(*(struct in_addr *)&iph->saddr);
    nfqp_log(debug, "saddr=%s; ",saddr);

    char *daddr = inet_ntoa(*(struct in_addr *)&iph->daddr);
    nfqp_log(debug, "daddr=%s}\n",daddr);

        if(keep_stats){
        	len = ntohs(iph->tot_len);
        	trans.proto =  iph->protocol;
        	ip.s_addr = iph->saddr;
        	ip.d_addr =  iph->daddr;
        }

    // if protocol is tcp
    if (iph->protocol == 6){
        // extract tcp header from packet
        /* Calculate the size of the IP Header. iph->ihl contains the number of 32 bit
        words that represent the header size. Therfore to get the number of bytes
        multiple this number by 4 */
        struct tcphdr *tcp = ((struct tcphdr *) (nf_packet + (iph->ihl << 2)));

        /* Calculate the size of the TCP Header. tcp->doff contains the number of 32 bit
        words that represent the header size. Therfore to get the number of bytes
        multiple this number by 4 */
        //int tcphdr_size = (tcp->doff << 2);

        /* to print the TCP headers, we access the structure defined in tcp.h line 89
        and convert values from hexadecimal to ascii */
        nfqp_log(debug, "TCP{sport=%u; dport=%u; seq=%u; ack_seq=%u; flags=u%ua%up%ur%us%uf%u; window=%u; urg=%u}\n",
            ntohs(tcp->source), ntohs(tcp->dest), ntohl(tcp->seq), ntohl(tcp->ack_seq)
            ,tcp->urg, tcp->ack, tcp->psh, tcp->rst, tcp->syn, tcp->fin, ntohs(tcp->window), tcp->urg_ptr);

        if(keep_stats){
          	trans.port_num.s_port = (port_t) tcp->source;
          	trans.port_num.d_port = (port_t) tcp->dest;
          	trans.window_size = (__us_t) tcp->window;
          }

    }

    // if protocol is udp
    if(iph->protocol == 17){
        struct udphdr *udp = ((struct udphdr *) (nf_packet + (iph->ihl << 2)));
        nfqp_log(debug, "UDP{sport=%u; dport=%u; len=%u}\n",
            ntohs(udp->source), ntohs(udp->dest), udp->len);

		if (keep_stats) {
			trans.port_num.s_port = (port_t) udp->source;
			trans.port_num.d_port = (port_t) udp->dest;
			trans.window_size = -1;
		}


    }

        if(keep_stats)
        	g_queue_push_tail(pkt_q , nfqp_make_pkt(ip, len, trans));

    nfqp_log(debug, "\n");

    return (id);
}

//static u_int32_t record_pkt (struct nfq_data *tb){
//
//    /*! create pcap specific header
//     */
//    struct pcap_pkthdr phdr;
//
//    /*! init capture time
//     */
//    static struct timeval t;
//    memset (&t, 0, sizeof(struct timeval));
//    gettimeofday(&t, NULL);
//    phdr.ts.tv_sec = t.tv_sec;
//    phdr.ts.tv_usec = t.tv_usec;
//
//    /*! populate pcap struct with packet headers
//     */
//    char *nf_packet;
//    phdr.caplen = nfq_get_payload(tb,&nf_packet);
//    phdr.len = phdr.caplen;
//
//    /*! dump packet data to the file */
//    pcap_dump((u_char *)p_output, &phdr, (const u_char *)nf_packet);
//
//    return 0;
//}


int nfqp_unset_queue(){

	memset(nfqp_command, '\0', COMMAND_LEN);
	//Remove ip table rule;
	snprintf(nfqp_command, COMMAND_LEN, "iptables -D INPUT -j NFQUEUE --queue-num %d", nfqp_queue_num);
	system(nfqp_command);
	return(SUCCESS);
}

int nfqp_set_queue(){

	snprintf(nfqp_command, COMMAND_LEN, "iptables -A INPUT -j NFQUEUE --queue-num %d", nfqp_queue_num);
	system(nfqp_command);

	return(SUCCESS);
}

static int nfqp_cb(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg,
          struct nfq_data *nfa, void *data)
{
    u_int32_t id = nfqp_process_pkt(nfa);

    // Adding to the queue for GUI

    return (nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL));
}


void nfqp_log(int log_lvl, char *msg, ...){

	va_list arg;
	va_start(arg, msg);
	char log[MAX_LOG_MSG];
	vsprintf(log, msg, arg);

	if ( log_lvl <= nfqp_logging_level )
		fputs(log, nfqp_log_file);


	if ( log_lvl <= nfqp_printf_log_lvl )
		puts(log);

	va_end(arg);

}

int nfqp_init(){

	nfqp_log_file = fopen(nfqp_log_fpath,"w");

	if(nfqp_log_file==NULL){
		printf("Error while opening nfqp_log_file! \n");
		return(FAIL);
	}

	//Clearing previous not removed ip tables rule.
	nfqp_unset_queue();

	nfqp_log(info, "Setup the queue of the iptables\n");
	nfqp_set_queue();

	pkt_q = g_queue_new();

	return(SUCCESS);
}

int nfqp_exit(){

	nfqp_unset_queue();

	//Stop packet analyzer
	analyzer_is_alive = 0;

	if (keep_stats)
		g_queue_foreach(pkt_q, (GFunc) nfqp_print_queue_packets, NULL );

	g_queue_free(pkt_q);

	fclose(nfqp_log_file);

	return(SUCCESS);

}

void nfqp_test_logging(){

	nfqp_log(debug, "This is a debug");
	nfqp_log(info, "This is a info");
	nfqp_log(error, "This is a error");
	nfqp_log(critical, "This is a critical");

}


int nfqp_analyzer_function(void *args)
{
    struct nfq_handle *h;
    struct nfq_q_handle *qh;
    struct nfnl_handle *nh;
    int fd;
    int rv;
    char buf[BUFSIZE];

    nfqp_log(info, "opening library handle\n");
    h = nfq_open();
    if (!h) {
        nfqp_log(error,  "error during nfq_open()\n");
        exit(EXIT_FAILURE);
    }

    nfqp_log(info,"unbinding existing nf_queue handler for AF_INET (if any)\n");

    if (nfq_unbind_pf(h, AF_INET) < 0) {
        nfqp_log(error,  "error during nfq_unbind_pf()\n");
        exit(EXIT_FAILURE);
    }

    nfqp_log(info, "binding nfnetlink_queue as nf_queue handler for AF_INET\n");

    if (nfq_bind_pf(h, AF_INET) < 0) {
        nfqp_log(error,  "error during nfq_bind_pf()\n");
        exit(EXIT_FAILURE);
    }

    nfqp_log(info, "binding this socket to queue %d\n", nfqp_queue_num);

    qh = nfq_create_queue(h,  nfqp_queue_num, &nfqp_cb, NULL);
    if (!qh) {
        nfqp_log(error,  "error during nfq_create_queue()\n");
        exit(EXIT_FAILURE);
    }
    nfqp_log(info, "setting copy_packet mode\n");

    if (nfq_set_mode(qh, NFQNL_COPY_PACKET, 0xffff) < 0) {
        nfqp_log(error,  "can't set packet_copy mode\n");
        exit(EXIT_FAILURE);
    }

    nh = nfq_nfnlh(h);
    fd = nfnl_fd(nh);

	while ((rv = recv(fd, buf, sizeof(buf), 0)) && rv >= 0 && analyzer_is_alive) {

		nfqp_log(debug, "-- New packet received --\n");
		nfq_handle_packet(h, buf, rv);

	}

    nfqp_log(info, "unbinding from queue %d\n", nfqp_queue_num);
    nfq_destroy_queue(qh);

#ifdef INSANE
    /* normally, applications SHOULD NOT issue this command, since
     * it detaches other programs/sockets from AF_INET, too ! */
    nfqp_log(info, "unbinding from AF_INET\n");
    nfq_unbind_pf(h, AF_INET);
#endif

    nfqp_log(info, "closing library handle\n");
    nfq_close(h);
    return (SUCCESS);
}

