#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include "csa.h"

#define CW_PORT 2349
#define TS_PORT 2350

int main(int argc, char **argv) {
	unsigned char buf[188*8], *b;
	struct sockaddr_in ts_servaddr, ts_cliaddr;
	struct sockaddr_in cw_servaddr, cw_cliaddr;
	fd_set rfds;
	struct timeval tv;
	int ts_sock, cw_sock;
	int highfd, ret, ts_connfd, cw_connfd;
	int set=0;
	struct key key;
	socklen_t ts_len=sizeof(ts_cliaddr), cw_len=sizeof(cw_cliaddr);
	
	if((ts_sock=socket(AF_INET, SOCK_STREAM, 0))<0) {
		fprintf(stderr,"can't create socket: %d\n",ts_sock);
		return -1;
	}
	if((cw_sock=socket(AF_INET,SOCK_STREAM, 0))<0) {
		fprintf(stderr,"can't create socket: %d\n",cw_sock);
		return -1;
	}
	
	memset(&ts_servaddr,0,sizeof(ts_servaddr));
	memset(&cw_servaddr,0,sizeof(cw_servaddr));
	
	ts_servaddr.sin_family = AF_INET;
	ts_servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ts_servaddr.sin_port = htons(TS_PORT);
	
	cw_servaddr.sin_family = AF_INET;
	cw_servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	cw_servaddr.sin_port = htons(CW_PORT);

	/* CW */
	if((ret=bind(cw_sock, (struct sockaddr *) &cw_servaddr, sizeof(cw_servaddr)))) {
		fprintf(stderr,"can't bind CW_PORT (%d): %d\n",CW_PORT,ret);
		return -1;
	}
	fprintf(stderr,"awaiting CW-feed connection...");
	fflush(stderr);
	if((ret=listen(cw_sock, 0))) {
		fprintf(stderr,"listen error: %d\n",ret);
		return -1;
	}
	if((cw_connfd=accept(cw_sock,(struct sockaddr *) &cw_cliaddr,&cw_len))<0) {
		fprintf(stderr,"accept error: %d\n",cw_connfd);
		return -1;
	}
	else {
		if(!inet_ntop(AF_INET, &cw_cliaddr.sin_addr,buf,sizeof(buf))) {
			fprintf(stderr," inet_ntop error: WAAAAAAAAAAAAAAAH!!!! \n");
			return -1;
		}
		fprintf(stderr," %s:%d connected\n",buf,ntohs(cw_cliaddr.sin_port));
	}

	/* TS */
	if((ret=bind(ts_sock, (struct sockaddr *) &ts_servaddr, sizeof(ts_servaddr)))) {
		fprintf(stderr,"can't bind TS_PORT (%d): %d\n",TS_PORT,ret);
		return -1;
	}
	fprintf(stderr,"awaiting TS-feed connection...");
	fflush(stderr);
	if((ret=listen(ts_sock, 0))) {
		fprintf(stderr,"listen error: %d\n",ret);
		return -1;
	}
	if((ts_connfd=accept(ts_sock,(struct sockaddr *) &ts_cliaddr,&ts_len))<0) {
		fprintf(stderr,"accept error: %d\n",ts_connfd);
		return -1;
	}
	else {
		if(!inet_ntop(AF_INET, &ts_cliaddr.sin_addr,buf,sizeof(buf))) {
			fprintf(stderr," inet_ntop error: WAAAAAAAAAAAAAAAH!!!! \n");
			return -1;
		}
		fprintf(stderr," %s:%d connected\n",buf,ntohs(ts_cliaddr.sin_port));
	}
	
	highfd=cw_connfd;
	if(ts_connfd>cw_connfd)
		highfd=ts_connfd;
	
	FD_ZERO(&rfds);
	FD_SET(ts_connfd,&rfds);
	FD_SET(cw_connfd,&rfds);
	tv.tv_sec=5;
	tv.tv_usec=0;
	while((ret=select(highfd+1,&rfds,NULL,NULL,&tv))>=0) {
		if(ret>=1) {
			/* CW */
			if(FD_ISSET(cw_connfd,&rfds)) {
				if((ret=read(cw_connfd,buf,16))!=16) {
					fprintf(stderr,"dmx: oops! cw_connfd got %d - should: %d\n",ret,16);
				}
				else if(!ret) {
					fprintf(stderr,"CW-feed disconnected - exiting\n");
					return 0;
				}
				else {
					set=1;
					set_cws(buf,&key);
					for(ret=0;ret<16;ret++)
						fprintf(stderr,"%02X ",buf[ret]);
					fprintf(stderr,"\n");
				}
			}
			/* TS */
			if(FD_ISSET(ts_connfd,&rfds)) {
				if((ret=read(ts_connfd,buf,188*7))<0)
					fprintf(stderr,"dmx: oops! ts_sock got %d\n",ret);
				else if(!ret) {
					fprintf(stderr,"TS-feed disconnected - exiting\n");
					return 0;
				}
				else if(ret%188)
					fprintf(stderr,"dmx: oops! ts_sock got (%d*188)+%d\n",ret/188,ret%188);
				else {
					if(!set)
						goto foo;
					b=buf;
					while(ret) {
						if(b[0]==0x47) {
							if(b[3]&0x80) {
								decrypt(&key,b,buf+(7*188));
								write(1,buf+(7*188),188);
							} /* crypted */
							else
								write(1,b,188);
						} /* sync */
						else
							fprintf(stderr,"dmx: sync broken: %02X\n",b[0]);
						b+=188;
						ret-=188;
					} /* while(ret) */
					foo:
				} /* else */
			} /* TS */
		}
		else
			fprintf(stderr,"dmx: *yawn*\n");
		FD_SET(ts_connfd,&rfds);
		FD_SET(cw_connfd,&rfds);
		tv.tv_sec=5;
		tv.tv_usec=0;
	}
	
	return 0;
}
