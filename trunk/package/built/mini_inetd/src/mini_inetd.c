/*

Mini-inetd is a simple, fork-style TCP server.

This is already implemented better, faster and more secure in the normal inetd
or xinetd. However, adding your small service to the system's inetd/xinetd
usually requires root privilges. mini-inetd does not. If you can convince your
system administrator to add your program to the normal inetd/xinetd then do
that - mini-inet is a last resort.

Features:
  TCP (both IPv4 and IPv6)
  Limit on number of children
  Works with diet-libc

mini-inetd is public domain.

*/

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif
static sig_atomic_t sigterm_received=0;
static void sig_term(int sig) {
    sigterm_received=1;
}
static void sig_child(int sig) {
}
#ifdef __cplusplus
}
#endif

static int verbose=0;

static int startListen(const char *argv0, const char *port_str) {
	struct addrinfo hint;
	struct addrinfo *ai;
	struct addrinfo *pai;
	int rc;
	int fd=-1;
	int most_recent_errno=0;
	
	if(verbose) printf("Parsing port '%s'\n",port_str);
	memset(&hint,0,sizeof(hint));
	hint.ai_flags = AI_PASSIVE;
	hint.ai_socktype = SOCK_STREAM;
	rc = getaddrinfo(0, port_str, &hint,&ai);
	if(rc!=0) {
		fprintf(stderr,"%s: getaddrinfo() returned %d (%s)\n",argv0,rc,gai_strerror(rc));
		exit(EXIT_FAILURE);
	}

	for(pai=ai; pai; pai=pai->ai_next) {
		if(verbose) printf("Found ai_family: %d, ai_protocol: %d\n",pai->ai_family, pai->ai_protocol);
		if(pai->ai_protocol==IPPROTO_TCP) {
			if(verbose) printf("Trying family %d protocol %d\n",pai->ai_family, pai->ai_protocol);
			fd = socket(pai->ai_family, pai->ai_socktype, pai->ai_protocol);
			if(fd<0) {
				most_recent_errno=errno;
				if(verbose) printf("socket(%d,%d,%d) failed, errno=%d\n", pai->ai_family, pai->ai_socktype, pai->ai_protocol, errno);
				continue;
			}
			if(bind(fd,pai->ai_addr, pai->ai_addrlen)!=0) {
				most_recent_errno=errno;
				if(verbose) printf("bind() failed, errno=%d\n", errno);
				close(fd);fd=-1;
				continue;
			}
			if(listen(fd,10)!=0) {
				most_recent_errno=errno;
				if(verbose) printf("listen() failed, errno=%d\n", errno);
				close(fd);fd=-1;
				continue;
			}
			break;
		}
	}
	freeaddrinfo(ai);
	if(fd<0) {
		if(most_recent_errno)
			fprintf(stderr,"%s: found no suitable protocol/family for service %s. %s\n",argv0,port_str,strerror(most_recent_errno));
		else
			fprintf(stderr,"%s: found no suitable protocol/family for service %s\n",argv0,port_str);
		exit(EXIT_FAILURE);
	}
	if(verbose) printf("listening on fd %d\n",fd);
	return fd;
}


int main(int argc, char **argv) {
	int max_children=99999;
	int c;
	int fd_listen;
	fd_set fds;
	int number_of_children=0;
	
	if(argc<3 ||
	   strcmp(argv[1],"-help")==0 ||
	   strcmp(argv[1],"--help")==0 ||
	   strcmp(argv[1],"-usage")==0 ||
	   strcmp(argv[1],"--usage")==0)
	{
		fprintf(stderr,"usage: %s [-v] [-m max_children] <port|service> <cmd>\n",argv[0]);
		fprintf(stderr,"%s will liste on on <port> (TCP only)\n",argv[0]);
		fprintf(stderr,"and whenever a connection is established it will spawn the specified command\n");
		fprintf(stderr,"with fd 0,1 and 2 set to the connection.\n");
		return EXIT_FAILURE;
	}
	
	while((c=getopt(argc,argv,"vm:"))!=EOF) {
		switch(c) {
			case 'v':
				verbose=1;
				break;
			case 'm':
				max_children = (int)strtol(optarg,0,0);
				break;
			default:
				fprintf(stderr,"%s: Invalid option. Use --usage to see what is supported\n",argv[0]);
				return EXIT_FAILURE;
		}
	}
	if(argc-optind < 2) {
		fprintf(stderr,"%s: Invalid usage.\n",argv[0]);
		return EXIT_FAILURE;
	}
	
	/*parse port*/
	fd_listen=startListen(argv[0],argv[optind]);
	
	if(verbose) printf("Setting up SIGTERM handling\n");
	signal(SIGTERM,sig_term);
	if(verbose) printf("Setting up SIGCHLD handling\n");
	signal(SIGCHLD,sig_child);
	
	/*switch socket to non-blocking to avoid rare hangs in accept()*/
	fcntl(fd_listen,F_SETFL,O_NONBLOCK);
	
	FD_ZERO(&fds);
	FD_SET(fd_listen,&fds);
	while(sigterm_received==0) {
		fd_set fds_tmp=fds;
		int rc = select(fd_listen+1,&fds_tmp,0,0,0);
		if(rc==1) {
			int client_fd;
			if(verbose) printf("Listen socket ready\n");
			client_fd = accept(fd_listen,0,0);
			if(client_fd>=0 && number_of_children==max_children) {
				close(client_fd); /*go away*/
			} else if(client_fd>=0) {
				fcntl(client_fd,F_SETFL,0); /*set blocking*/
				if(verbose) printf("Forking\n");
				if(fork()==0) {
					close(fd_listen);
					dup2(client_fd,0);
					dup2(client_fd,1);
					dup2(client_fd,2);
					close(client_fd);
					execvp(argv[optind+1],argv+optind+1);
					_exit(EXIT_FAILURE);
				} else {
					close(client_fd);
					number_of_children++;
				}
			}
		} else {
			if(verbose) printf("select() returned non-1, probably due to signal\n");
		}
		
		/*reap children*/
		for(;;) {
			int status;
			pid_t child_pid = waitpid(0,&status,WNOHANG);
			if(child_pid<=0) break;
			number_of_children--;
			if(verbose) printf("Reaped child %ld status %d\n", (long)child_pid, status);
		}
	}
	if(verbose) printf("Cleaning up\n");
	close(fd_listen);
	if(verbose) printf("Exiting\n");
	return 0;
}
