#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>  /* GIMME a libpcap plz! */
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <ctype.h>

#define SIZE_ETHERNET1 14
#define ETHER_ADDR_LEN1  6
#define ARP_REQUEST 1
#define ARP_REPLY 2

/* ARP Packet Header */
typedef struct arphdr
{
	u_int16_t htype;
	u_int16_t ptype;
	u_char hlen;
	u_char plen;
	u_int16_t oprcode;
	u_char sha[6];
	u_char sipa[4];
	u_char tha[6];
	u_char tipa[4];
}arphdr_t;


/* Ethernet header */
    struct sniff_ethernet {
        u_char ether_dhost[ETHER_ADDR_LEN1]; /* Destination host address */
        u_char ether_shost[ETHER_ADDR_LEN1]; /* Source host address */
        u_short ether_type; /* IP? ARP? RARP? etc */
    };

    /* IP header */
    struct sniff_ip {
        u_char ip_vhl;      /* version << 4 | header length >> 2 */
        u_char ip_tos;      /* type of service */
        u_short ip_len;     /* total length */
        u_short ip_id;      /* identification */
        u_short ip_off;     /* fragment offset field */
    #define IP_RF 0x8000        /* reserved fragment flag */
    #define IP_DF 0x4000        /* dont fragment flag */
    #define IP_MF 0x2000        /* more fragments flag */
    #define IP_OFFMASK 0x1fff   /* mask for fragmenting bits */
        u_char ip_ttl;      /* time to live */
        u_char ip_p;        /* protocol */
        u_short ip_sum;     /* checksum */
        struct in_addr ip_src;
        struct in_addr ip_dst; /* source and dest address */
    };
    #define IP_HL(ip)       (((ip)->ip_vhl) & 0x0f)
    #define IP_V(ip)        (((ip)->ip_vhl) >> 4)

    /* TCP header */
    struct sniff_tcp {
       u_short th_sport;               /* source port */
        u_short th_dport;               /* destination port */
        uint32_t th_seq;                 /* sequence number */
        uint32_t th_ack;                 /* acknowledgement number */
        u_char  th_offx2;               /* data offset, rsvd */
#define TH_OFF(th)      (((th)->th_offx2 & 0xf0) >> 4)
        u_char  th_flags;
        #define TH_FIN  0x01
        #define TH_SYN  0x02
        #define TH_RST  0x04
        #define TH_PUSH 0x08
        #define TH_ACK  0x10
        #define TH_URG  0x20
        #define TH_ECE  0x40
        #define TH_CWR  0x80
        #define TH_FLAGS        (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
        u_short th_win;                 /* window */
        u_short th_sum;                 /* checksum */
        u_short th_urp;                 /* urgent pointer */
};

void
print_hex_ascii_line(const char *payload, int len, int offset)
{



	int i;
	int gap;
	const char *ch;

	/* offset */
	printf("%05d   ", offset);
	
	/* hex */
	ch = payload;
	for(i = 0; i < len; i++) {
		printf("%02x ", *ch);
		ch++;
		/* print extra space after 8th byte for visual aid */
		if (i == 7)
			printf(" ");
	}
	/* print space to handle line less than 8 bytes */
	if (len < 8)
		printf(" ");
	
	/* fill hex gap with spaces if not full line */
	if (len < 16) {
		gap = 16 - len;
		for (i = 0; i < gap; i++) {
			printf("   ");
		}
	}
	printf("   ");
	
	/* ascii (if printable) */
	ch = payload;
	for(i = 0; i < len; i++) {
		if (isprint(*ch))
			printf("%c", *ch);
		else
			printf(".");
		ch++;
	}

	printf("\n");

return;
}

/*
 * print packet payload data (avoid printing binary data)
 */
void
print_payload(const char *payload, int len)
{


	int len_rem = len;
	int line_width = 16;			/* number of bytes per line */
	int line_len;
	int offset = 0;					/* zero-based offset counter */
	const char *ch = payload;

	if (len <= 0)
		{	printf("Inside hex_Ascii print\n");return;}

	/* data fits on one line */
	if (len <= line_width) {
		print_hex_ascii_line(ch, len, offset);
		return;
	}

	/* data spans multiple lines */
	for ( ;; ) {
		/* compute current line length */
		line_len = line_width % len_rem;
		/* print line */
		print_hex_ascii_line(ch, line_len, offset);
		/* compute total remaining */
		len_rem = len_rem - line_len;
		/* shift pointer to remaining bytes to print */
		ch = ch + line_len;
		/* add offset */
		offset = offset + line_width;
		/* check if we have line width chars or less */
		if (len_rem <= line_width) {
			/* print last line and get out */
			print_hex_ascii_line(ch, len_rem, offset);
			break;
		}
	}

return;
}

int main(int argc, char *argv[])
{
	char *temp_addr; char *temp_addr_d;

    //get file
     char *filename = argv[1];

     //error buffer
     char errbuff[PCAP_ERRBUF_SIZE];

     //open file and create pcap handler
     pcap_t * handler = pcap_open_offline(filename, errbuff);

     //The header that pcap gives us
    struct pcap_pkthdr *header;

    //The actual packet 
    const u_char *packet;   

      int packetCount = 0;
      int i;


      //tcp info
    const struct sniff_ethernet *ethernet; /* The ethernet header */
    const struct sniff_ip *ip; /* The IP header */
    const struct sniff_tcp *tcp; /* The TCP header */
	struct ether_header *ethr_header=NULL;
	struct iphdr* ip2=NULL;
	arphdr_t *arpheader=NULL;
    u_int size_ip;
    u_int size_tcp;
	u_int size_payload;

	int total_size_packets=0;

    while (pcap_next_ex(handler, &header, &packet) >= 0)
    {
	
        // Show the packet number
        printf("\nPacket # %i\n", ++packetCount);


		ethr_header=(struct ether_header *)packet;
		//ethernet=(struct sniff_ethernet *)packet;
		if(ntohs(ethr_header->ether_type)==ETHERTYPE_IP)
		{
			

					// Show the size in bytes of the packet
					printf("Packet size: %d bytes\n", header->len);
	
					total_size_packets+=header->len;

					// Show a warning if the length captured is different
					if (header->len != header->caplen)
						printf("Warning! Capture size different than packet size: %ld bytes\n", header->len);

					// Show Epoch Time
					printf("Epoch Time: %d:%d seconds\n", header->ts.tv_sec, header->ts.tv_usec);

					//ethernet = (struct sniff_ethernet*)(packet);
					ip = (struct sniff_ip*)(packet + SIZE_ETHERNET1);
					ip2 = (struct iphdr*)(packet + SIZE_ETHERNET1);
					
					if(ip2->protocol==6)
						{
									//printf("***************** TRANSFER ********************");
									size_ip = IP_HL(ip)*4;
									if (size_ip < 20) {
										printf("   * Invalid IP header length: %u bytes\n", size_ip);
										return 0;
									}
									tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET1 + size_ip);

									printf("src port: %d dest port: %d \n", ntohs(tcp->th_sport), ntohs(tcp->th_dport));
	
									//temp_addr=inet_ntoa(ip->ip_src);
									//temp_addr_d=inet_ntoa(ip->ip_dst);
									size_tcp = TH_OFF(tcp)*4;
									//size_tcp=0;
									const char * payload=(u_char *)(packet + SIZE_ETHERNET1 + size_ip + size_tcp);
								size_payload = ntohs(ip->ip_len) - (size_ip + size_tcp);

	
								/*
								 * Print payload data; it might be binary, so don't just
								 * treat it as a string.
								 */
								if (size_payload > 0) {
									printf("   Payload (%d bytes):\n", size_payload);
									print_payload(payload, size_payload);
								}
								
		
									printf("Src Address: %s\n", inet_ntoa(ip->ip_src));
									printf("Dest Address: %s\n", inet_ntoa(ip->ip_dst));

									printf("Sequence Number: %u\n",ntohl(tcp->th_seq));
									printf("Acknowledgement Number: %u\n",ntohl(tcp->th_ack));

									// Add two lines between packets
									printf("\n");
									
									//printf("\n******************Summary******************\n");
									//printf("Total size of all the packets: %d\n",total_size_packets);
					}//TCP Packet CONCLUDES
			}//IP PACKET CONCLUDES

			else if(ntohs(ethr_header->ether_type)==ETHERTYPE_ARP)
			{
			printf("ARP Data Found");
			arpheader=(arphdr_t *)(packet + ETHER_HDR_LEN);  /* Point to ARP Header */
			printf("\n");
			printf("Hardware Type : %s \n",(ntohs(arpheader->htype)==1)? "Ethernet":"Unknown");
			printf("Protocol Type : %s \n",(ntohs(arpheader->ptype)==0X0800)? "IPv4":"Unknown");
			printf("Operation : %s \n",(ntohs(arpheader->oprcode)==ARP_REQUEST)? "ARP Request":"ARP Reply");
			
			
			if((ntohs(arpheader->htype)==1) && (ntohs(arpheader->ptype)==0X0800))
			{
				printf("\nSender MAC Address: ");
				for(i=0;i<6;i++)
					printf("%02X:",arpheader->sha[i]);
				printf("\nSender IP Address : ");
				for(i=0;i<4;i++)
					printf("%d.",arpheader->sipa[i]);
				printf("\nReceiver MAC Address: ");
				for(i=0;i<6;i++)
					printf("%02X:",arpheader->tha[i]);
				printf("\nReceiver IP Address : ");				
				for(i=0;i<4;i++)
					printf("%d.",arpheader->tipa[i]);
			}
			printf("\n");
			
			}
			else
			{
				printf("Ethernet Type not known: %x",ntohs(ethr_header->ether_type));
			}
    }

     return(0);
}
