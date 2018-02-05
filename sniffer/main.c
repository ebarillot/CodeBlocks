#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/route.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

/* on reecrit la structure de l'entete IP */
struct ip {
  /* little endian */
  unsigned int   ip_length:4;
  unsigned int   ip_version:4;
  unsigned char  ip_tos;
  unsigned short ip_total_length;
  unsigned short ip_id;
  unsigned short ip_flags;
  unsigned short ip_ttl;
  unsigned char  ip_protocole;
  unsigned short ip_chksum;
  unsigned int   ip_source;
  unsigned int   ip_dest;
};

/* entete TCP */
struct tcp {
  unsigned short tcp_source_port;
  unsigned short tcp_dest_port;
  unsigned int   tcp_seqnum;
  unsigned int   tcp_acknum;

  /* little endian */
  unsigned int tcp_res1:4,
      tcp_hlen:4,
      tcp_fin:1,
      tcp_syn:1,
      tcp_rst:1,
      tcp_psh:1,
      tcp_ack:1,
      tcp_urg:1,
      tcp_res2:2;
  unsigned short tcp_winsize;
  unsigned short tcp_chksum;
  unsigned short tcp_urgent;
};

/* prototypes */
int mode_promiscuous(char* interface, int sock);


int main(int ac, char** av)
{
  int sock;
  int octets_recus;
  int segment_taille;
  char buf[65535];
  char* data;
  struct sockaddr_in segment;
  struct ip *ip;    /* IP header */
  struct tcp *tcp;  /* TCP header */

  printf("Sniffer de base\n");
  if (ac<2) {
    printf("Usage : %s interface\n", av[0]);
    exit(1);
  }

  if ((sock = socket(AF_INET,SOCK_RAW,IPPROTO_TCP)) < 0) {
    perror("Impossible de creer la socket");
    exit(0);
  }

  mode_promiscuous(av[1], sock);

  /* on commence à sniffer par une boucle infinie */
  while (42) {
    segment_taille = sizeof(segment);
    octets_recus = recvfrom(sock, buf, sizeof(buf), 0,
                            (struct sockaddr *)&segment,
                            &segment_taille);
    printf("\nOctets recus : %5d\n", octets_recus);
    printf("Adresse source : %s\n",inet_ntoa(segment.sin_addr));
    ip = (struct ip *)buf;

    if (ip->ip_protocole == 6) {
      printf("Longueur de l'entete IP : %d\n", ip->ip_length);
      tcp = (struct tcp *)(buf + (4*ip->ip_length));
      printf("Port source : %d\n", ntohs(tcp->tcp_source_port));
      printf("Port destination : %d\n", ntohs(tcp->tcp_dest_port));
      data = (char*)(buf + (4*ip->ip_length) + (4*tcp->tcp_hlen));
      printf("data = %s\n", data);
    }
  }
  return 0;
}

int mode_promiscuous(char* interface, int sock)
{
  struct ifreq ifr;

  strncpy(ifr.ifr_name, interface, strlen(interface)+1);
  if ((ioctl(sock, SIOCGIFFLAGS, &ifr) == -1)) {
    perror("Impossible de recuperer la configuration de l'interface");
    exit(0);
  }
  printf("L'interface est : %s\n", interface);
  perror("Resuperation de la configuration de l'interface");

  ifr.ifr_flags |= IFF_PROMISC;
  if (ioctl(sock, SIOCSIFFLAGS, &ifr) == -1) {
    perror("Impossible d'activer le mode promiscuous");
    exit(0);
  }
  printf("L'interface %s est en mode promiscuous", interface);
  return(0);
}

