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


int logging_level = 4;
int queue_num = 0;

char *nfqp_log_fpath = "../logs/nfqp.log";



char nfqp_command[COMMAND_LEN];

int use_pcap = 0;
FILE *nfqp_log_file;

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




/* returns packet id */
static u_int32_t nfqp_print_pkt (struct nfq_data *tb){
    int id = 0;
    struct nfqnl_msg_packet_hdr *ph;
    u_int32_t mark,ifi;
    int ret;
    char *nf_packet;

    ph = nfq_get_msg_packet_hdr(tb);
    if (ph){
        id = ntohl(ph->packet_id);
        printf("hw_protocol=0x%04x hook=%u id=%u ",
            ntohs(ph->hw_protocol), ph->hook, id);
    }

    mark = nfq_get_nfmark(tb);
    if (mark)
        printf("mark=%u ", mark);

    ifi = nfq_get_indev(tb);
    if (ifi)
        printf("indev=%u ", ifi);

    ifi = nfq_get_outdev(tb);
    if (ifi)
        printf("outdev=%u ", ifi);

    ret = nfq_get_payload(tb, &nf_packet);
    if ((ret >= 0)){
        printf("payload_len=%d bytes", ret);
            fputc('\n', stdout);
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
    fprintf(stdout, "IP{v=%u; ihl=%u; tos=%u; tot_len=%u; id=%u; ttl=%u; protocol=%u; "
        ,iph->version, iph->ihl*4, iph->tos, ntohs(iph->tot_len), ntohs(iph->id), iph->ttl, iph->protocol);


    char *saddr = inet_ntoa(*(struct in_addr *)&iph->saddr);
    fprintf(stdout,"saddr=%s; ",saddr);

    char *daddr = inet_ntoa(*(struct in_addr *)&iph->daddr);
    fprintf(stdout,"daddr=%s}\n",daddr);

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
        fprintf(stdout, "TCP{sport=%u; dport=%u; seq=%u; ack_seq=%u; flags=u%ua%up%ur%us%uf%u; window=%u; urg=%u}\n",
            ntohs(tcp->source), ntohs(tcp->dest), ntohl(tcp->seq), ntohl(tcp->ack_seq)
            ,tcp->urg, tcp->ack, tcp->psh, tcp->rst, tcp->syn, tcp->fin, ntohs(tcp->window), tcp->urg_ptr);
    }

    // if protocol is udp
    if(iph->protocol == 17){
        struct udphdr *udp = ((struct udphdr *) (nf_packet + (iph->ihl << 2)));
        fprintf(stdout,"UDP{sport=%u; dport=%u; len=%u}\n",
            ntohs(udp->source), ntohs(udp->dest), udp->len);
    }

    fprintf(stdout,"\n");

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


int nfqp_set_queue(int num){

	snprintf(nfqp_command, COMMAND_LEN, "iptables -A INPUT -j NFQUEUE --queue-num %d", num);
	system(nfqp_command);

	return(SUCCESS);
}

static int nfqp_cb(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg,
          struct nfq_data *nfa, void *data)
{
    u_int32_t id = nfqp_print_pkt(nfa);


    return (nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL));
}

void nfqp_log(int log_lvl, char *msg){

	char log[MAX_LOG_MSG];

	if ((logging_level - log_lvl) + 1) {
		snprintf(log, MAX_LOG_MSG, "%.*s", (int)strlen(msg), msg);
		fputs(log, nfqp_log_file);

	}

}


int nfqp_init(){

	nfqp_log_file = fopen(nfqp_log_fpath,"w");

	if(nfqp_log_file==NULL){
		printf("Error while opening nfqp_log_file! \n");
		return(FAIL);
	}

	nfqp_log(info, "Deleting previous iptables\n");
	system("iptables -F");

	nfqp_log(info, "Setup the queue of the iptables\n");
	nfqp_set_queue(queue_num);

	return(SUCCESS);
}

int nfqp_exit(){

	fclose(nfqp_log_file);
	system("iptables -F");

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

    nfqp_log(info, "binding this socket to queue '0'\n");

    qh = nfq_create_queue(h,  queue_num, &nfqp_cb, NULL);
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

    while ((rv = recv(fd, buf, sizeof(buf), 0)) && rv >= 0) {

    	 nfqp_log(debug, "-- New packet received --\n");

        nfq_handle_packet(h, buf, rv);

    }

    nfqp_log(info, "unbinding from queue\n");
    nfq_destroy_queue(qh);

#ifdef INSANE
    /* normally, applications SHOULD NOT issue this command, since
     * it detaches other programs/sockets from AF_INET, too ! */
    nfqp_log(info, "unbinding from AF_INET\n");
    nfq_unbind_pf(h, AF_INET);
#endif

    nfqp_log(info, "closing library handle\n");
    nfq_close(h);

    exit(EXIT_FAILURE);
}

