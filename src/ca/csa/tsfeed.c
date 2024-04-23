#include <sys/ioctl.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <ost/dmx.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DVR "/dev/ost/dvr0"
#define DEMUX "/dev/ost/demux0"

#define TS_PORT 2350

int main(int argc, char **argv){
  struct dmxPesFilterParams pesFilterParams;
  struct sockaddr_in addr;
  int demux1, demux2, dvr;
  int len, sockfd;
  unsigned int vpid, apid;
  unsigned char b[188*7], *buf=b;

  if(argc<4) {
    fprintf(stderr,"usage: %s <dst-ip> <vpid> <pid>\n",argv[0]);
    return 0;
  }
  
  if((sockfd=socket(AF_INET, SOCK_STREAM, 0))<0) {
    fprintf(stderr,"can't create socket: %d\n",sockfd);
    return -1;
  }

  memset(&addr,0,sizeof(addr));

  addr.sin_family=AF_INET;
  addr.sin_port=htons(TS_PORT);

  if(inet_pton(AF_INET, argv[1], &addr.sin_addr)<=0) {
    fprintf(stderr,"invalid dst-ip: %s\n",argv[1]);
    return -1;
  }

  if(((sscanf(argv[2],"%x",&vpid))!=1) || (vpid>=0x1fff)) {
    fprintf(stderr,"vpid %s invalid\n",argv[2]);
    return -1;
  }

  if(((sscanf(argv[3],"%x",&apid))!=1) || (apid>=0x1fff)) {
    fprintf(stderr,"apid %s invalid\n",argv[3]);
    return -1;
  }

  if(connect(sockfd, (const struct sockaddr *) &addr, sizeof(addr))!=0) {
    fprintf(stderr,"can't connect to %s:%d\n",argv[1],TS_PORT);
    return -1;
  }

  printf("connected to %s:%d - streaming pids 0x%04X 0x%04X\n",argv[1],TS_PORT,vpid&0x1fff,apid&0x1fff);

  /* open dvr device */

  if ((dvr = open(DVR, O_RDONLY)) < 0) {
    perror("DVR DEVICE");

    return -1;
  }

  /* switch video and audio demuxes to dvr */
  if ((demux1=open(DEMUX, O_RDWR))< 0) {
    perror("DEMUX DEVICE: ");
    return -1;
  }

  if (ioctl(demux1, DMX_SET_BUFFER_SIZE, 1024*1024) < 0) {
    perror("DEMUX DEVICE");
    return -1;
  }


  if ((demux2=open(DEMUX, O_RDWR))< 0) {
    perror("DEMUX DEVICE: ");
    return -1;
  }

  if (ioctl(demux2, DMX_SET_BUFFER_SIZE, 1024*1024) < 0) {
    perror("DEMUX DEVICE");
    return -1;
  }

  pesFilterParams.pid = vpid&0x1fff;
  pesFilterParams.input = DMX_IN_FRONTEND;
  pesFilterParams.output = DMX_OUT_TS_TAP;
  pesFilterParams.pesType = DMX_PES_OTHER;
  pesFilterParams.flags = DMX_IMMEDIATE_START;
  if (ioctl(demux1, DMX_SET_PES_FILTER, &pesFilterParams) < 0) {
    perror("DEMUX DEVICE");
    return -1;
  }

  pesFilterParams.pid = apid&0x1fff;
  pesFilterParams.input = DMX_IN_FRONTEND;
  pesFilterParams.output = DMX_OUT_TS_TAP;
  pesFilterParams.pesType = DMX_PES_OTHER;
  pesFilterParams.flags = DMX_IMMEDIATE_START;
  if (ioctl(demux2, DMX_SET_PES_FILTER, &pesFilterParams) < 0) {
    perror("DEMUX DEVICE");
    return -1;
  }

  while(1) {
    if((len = read(dvr, buf, 188))<=0)
      fprintf(stderr,"read dvr: %d\n",len);
    else {
      buf+=len;
      if(buf==(b+(188*7))) {
        buf=b;
	if((len=write(sockfd, buf, 188*7))!=(188*7)) {
	  if(len<=0) {
            break;
	  }
	  else
	    fprintf(stderr,"write error: returned %d should be: %d\n",len,(188*7));
	}
      }
    }
  }
  printf("Connection closed by remote host.\n");

  return 0;
}
