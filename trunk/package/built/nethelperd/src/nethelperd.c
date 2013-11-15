#define _GNU_SOURCE
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <signal.h> /*signal() */

#ifndef FALSE
#define FALSE (0)
#define TRUE (!(FALSE))
#endif

#define DEFAULT_TIMEOUT 300

/********************************************************
 *                     nethelperd                       *
 * Daemon to trace network activity of another process  *
 * and run commands if it's appears or if no activity   *
 * in some time.                                        *
 * Copyright: Soul Trace <S-trace@list.ru>, 14.11.2013  *
 * Used some parts from net-tools-1.60/netstat.c        *
 * License: GPLv2. You can freely distribute and modify *
 * this program as You want, but please send patches me *
 * Changelog:                                           *
 * 0.00 - initial release                               *
 * 0.01 - added daemonizing option, log-file option,    *
 * correct shutdown services on killing daemon,         *
 * verbose mode, PID-file working, thanks               *
 * http://dixx.livejournal.com/12665.html               *
 ********************************************************/

enum {
  TCP_ESTABLISHED = 1,
  TCP_SYN_SENT,
  TCP_SYN_RECV,
  TCP_FIN_WAIT1,
  TCP_FIN_WAIT2,
  TCP_TIME_WAIT,
  TCP_CLOSE,
  TCP_CLOSE_WAIT,
  TCP_LAST_ACK,
  TCP_LISTEN,
  TCP_CLOSING                 /* now a valid state */
};
char *script=NULL, *up_command=NULL, *down_command=NULL;
char *pidfile_fname = NULL;
int pidfile_fd = -1;
int services_is_started=FALSE;

void finalize(int exit_code)
{
  /* закрываем и удаляем pid-файл */
  if (pidfile_fd >= 0) {
    printf("Removing pid file \"%s\"", pidfile_fname);
    unlink(pidfile_fname);
    close(pidfile_fd);
  }
  exit(exit_code);
}

int initialize(char *pid_fname)
{
  int   ok      = 0;
  char *pid_buf = NULL;
  do {
    int   pid_len;
    /* собственно, открываем файл */
    pidfile_fd = open(pid_fname, O_CREAT | O_WRONLY);
    if (pidfile_fd < 0) {
      printf("can't open pid file: %s\n", pid_fname);
      break;
    }
    
    /* пытаемся его заблокировать - собственно проверка наличия копий процесса */
    if (flock(pidfile_fd, LOCK_EX | LOCK_NB) < 0) {
      if (errno == EWOULDBLOCK)
      {
        printf("FAILURE: nethelperd already running with this pidfile!\n");
        finalize(EXIT_FAILURE);
      }
      else
        printf("FAILURE: flock() failed: %s\n", pid_fname);
      break;
    }
    
    /* файл заблокирован - всё хорошо, надо записать туда наш pid */
    
    /* получаем строковое представление идентификатора (getpid() не может вернуть ошибку) */
    pid_len = asprintf(&pid_buf, "%lld", (long long)getpid());
    if (!pid_buf) {
      printf("FAILURE: asprintf() failed\n");
      break;
    }
    
    /* теперь надо обрезать pid-файл до нужного размера, иначе после записи там может остаться лишнее */
    if (ftruncate(pidfile_fd, (off_t)pid_len) < 0) {
      printf("FAILURE: ftruncate() failed\n");
      break;
    }
    
    /* собственно, записываем pid. ввод/вывод блокирующий, поэтому достаточно одного вызова */
    if (write(pidfile_fd, (void *)pid_buf, (size_t)pid_len) < (ssize_t)pid_len) {
      printf("FAILURE: write() failed\n");
      break;
    }
    
    /* вот теперь всё чики-пуки */
    ok = 1;
  } while (0);
  
  /* корректно завершаемся при ошибках */
  if (!ok) {
    /* закрываем и удаляем pid-файл, если мы его таки открыли */
    if (pidfile_fd >= 0) {
      unlink(pid_fname);
      close(pidfile_fd);
    }
  }
  
  /* освобождаем память */
  if (pid_buf)
    free(pid_buf);
  
  return ok ? 0 : -1;
}

void stop_services(char *script, char *down_command)
{
  if (services_is_started)
  {
    char *command;
    printf("Bringing down helpers\n");
    fflush(stdout);
    asprintf(&command, "%s %s", script, down_command);
    (void)system(command);
    free(command);
    fflush(stdout);
    services_is_started=FALSE;
  }
}

void start_services(char *script, char *up_command)
{
  if (!services_is_started)
  {
    char *command;
    printf("Bringing up helpers\n");
    fflush(stdout);
    asprintf(&command, "%s %s", script, up_command);
    (void)system(command);
    free(command);
    fflush(stdout);
    services_is_started=TRUE;
  }
}

void signal_handler(int signal_number)
{
  printf("Shutting down nethelperd (got signal %d)\n", signal_number);
  stop_services(script, down_command);
  fflush(stdout);
  finalize(EXIT_SUCCESS);
}

int tcp_do_one(int lnr, const char *line, unsigned long int **inodes_numbers, int successed)
{
  /* These enums are used by IPX too. :-( */
  int i;
  unsigned long rxq, txq, time_len, retr;
  unsigned int local_port, rem_port, state, timer_run;
  int num, d, uid, timeout;
  unsigned long int inode;
  char rem_addr[128], local_addr[128], more[513];
  
  if (lnr == 0)
    return FALSE;
  
  num = sscanf(line,
               "%d: %64[0-9A-Fa-f]:%X %64[0-9A-Fa-f]:%X %X %lX:%lX %X:%lX %lX %d %d %lud %512s\n",
               &d, local_addr, &local_port, rem_addr, &rem_port, &state,
               &txq, &rxq, &timer_run, &time_len, &retr, &uid, &timeout, &inode, more);
  
  if (num < 11)
  {
    fprintf(stderr, "WARNING: got bogus tcp line.\n");
    return FALSE;
  }
  
  for (i=0; i<successed; i++)
  {
    if (inode == (*inodes_numbers)[i])
    {
      //       printf("matched inode: %d\n", (*inodes_numbers)[i]);
      break;
    }
    else if (i == successed-1)
      return FALSE;
  }
  if (state == TCP_LISTEN)
  {
    time_len = 0;
    retr = 0L;
    rxq = 0L;
    txq = 0L;
    //     printf("Socket %lu is listening!\n", inode);
    return FALSE;
  }
  else
    printf("Socket %lu is active!\n", inode);
  return TRUE;
}

int get_socket_inodes_list(int pid, unsigned long int *inodes_numbers[])
{
  struct dirent **namelist;
  int successed=0, n = 0;
  char *proc_path;
  asprintf (&proc_path, "/proc/%d/fd", pid);
  n = scandir(proc_path, &namelist, 0, alphasort);
  if (n >= 0)
  {
    int i;
    (*inodes_numbers)=calloc(n, sizeof(unsigned long int));
    if ((*inodes_numbers) == NULL)
    {
      printf ("FAILURE: out of memory allocating inode_numbers buffer!\n");
      finalize(EXIT_FAILURE);
    }
    for( i = 0; i < n ; i++ )
    {
      struct stat stat_p;
      char *current_name;
      errno=0;
      asprintf(&current_name, "%s/%s", proc_path, namelist[i]->d_name);
      free (namelist[i]);
      if (stat(current_name, &stat_p) == 0)
      {
        //         printf("Processing %s\n", current_name);
        if (S_ISSOCK(stat_p.st_mode))
        {
          //           printf("%s is socket (inode=%d)\n", current_name, (unsigned)stat_p.st_ino);
          (*inodes_numbers)[successed]=(unsigned)stat_p.st_ino;
          successed++;
        }
      }
      else
      {
        printf("stat() for '%s' failed (%s)\n",current_name, strerror(errno));
      }
      free(current_name);
    }
    free (namelist);
  }
  free (proc_path);
  //   printf("found %d inodes\n", successed);
  return successed;
}

int get_tcp_activity(int pid)
{
  unsigned long int *inodes_numbers;
  int inodes_count=get_socket_inodes_list(pid, &inodes_numbers);
  FILE *procinfo = fopen("/proc/net/tcp", "r");
  int has_activity=0;
  if (procinfo != NULL)
  {
    char buffer[512];
    int lnr = 0;
    do
    {
      if (fgets(buffer, (int)sizeof(buffer), procinfo))
        if (tcp_do_one(lnr++, buffer, &inodes_numbers, inodes_count))
          has_activity++;
    }
    while (!feof(procinfo));
    (void)fclose(procinfo);
    free(inodes_numbers);
  }
  if (has_activity > 0)
  {
    printf("Got activity!\n");
    return TRUE;
  }
  else
    return FALSE;
}

void usage(char *name)
{
  printf("Usage: %s [keys]\n\
  -p, --pid pid         Pid of traced process\n\
  -s, --script {file}   Path to script starting/stopping helpers\n\
  -u, --up {command}    Script command to bring helpers up\n\
  -d, --down {command}  Script command to bring helpers down\n\
  -t, --timeout seconds Inactivity timeout before bringing helpers down (default is %d)\n\
  -l, --logfile {log}   File to save log of nethelperd\n\
  -v, --verbose         Log actions to console\n\
  -f, --pidfile {file}  Pidfile to avoid several instances of same nethelperd staring\n\
  -b, --background      Go to background after startup\n\
  -h, --help            This help message\n\
  Example: %s --pid `pidof httpd` --script /media/DISK_A1/system/etc/init.d/S90extended_webface --up start_servers --down stop_servers --timeout 60\n\
  This will run /media/DISK_A1/system/etc/init.d/S90extended_webface start_servers on network activity of httpd process and run /media/DISK_A1/system/etc/init.d/S90extended_webface stop_servers after 60 seconds of no network activity of httpd\n\
  Version 0.01, Soul Trace <S-trace@list.ru>, 14.11.2013\n", name, DEFAULT_TIMEOUT, name);
  finalize (EXIT_SUCCESS);
}

int main (int argc, char *argv[])
{
  int i;
  int lop;
  int pid=-1, timeout=DEFAULT_TIMEOUT;
  char *log_file = NULL;
  int background=FALSE;
  int inactivity_timer=0;
  int log=FALSE;
  struct option longopts[] =
  {
    {"pid", 1, 0, (int)'p'},
    {"script", 1, 0, (int)'s'},
    {"up", 1, 0, (int)'u'},
    {"down", 1, 0, (int)'d'},
    {"timeout", 1, 0, (int)'t'},
    {"pidfile", 1, 0, (int)'f'},
    {"verbose", 0, 0, 'v'},
    {"logfile", 1, 0, 'l'},
    {"background", 0, 0, (int)'b'},
    {"help", 0, 0, (int)'h'},
    
    {NULL, 0, 0, 0}
  };
  
  while ((i = getopt_long(argc, argv, "p:t:s:u:d:t:f:vl:bh", longopts, &lop)) != EOF)
  {
    switch (i) {
      case -1:
        break;
      case '?':
      case 'h':
      case ':':
        usage(argv[0]);
        break;
      case 'p':
        if (optarg)
          pid=atoi(optarg);
        else
          usage(argv[0]);
        break;
      case 's':
        if (optarg)
        {
          script=strdup(optarg);
          if (access(script, F_OK) == -1)
          {
            printf("FAILURE: file %s is not exist!\n", script);
            finalize (EXIT_FAILURE);
          }
          if (access(script, X_OK) == -1)
          {
            printf("FAILURE: executable permission for file %s is disabled\n", script);
            finalize (EXIT_FAILURE);
          }
        }
        else
          usage(argv[0]);
        break;
      case 'f':
        if (optarg)
          pidfile_fname=strdup(optarg);
        else
          usage(argv[0]);
        break;
      case 'l':
      case 'v':
        log = TRUE;
        if (optarg)
        {
          if(log_file == NULL)
            log_file=strdup(optarg);
        }
        else
          break;
      case 'b':
        background=TRUE;
        break;
      case 'u':
        if (optarg)
          up_command=strdup(optarg);
        else
          usage(argv[0]);
        break;
      case 'd':
        if (optarg)
          down_command=strdup(optarg);
        else
          usage(argv[0]);
        break;
      case 't':
        if (optarg)
          timeout=atoi(optarg);
        else
          usage(argv[0]);
        break;
    }
  } 
  if (script==NULL)
  {
    printf("FAILURE: Please specify script file!\n");
    usage(argv[0]);
  }
  if (up_command==NULL)
  {
    printf("FAILURE: Please specify script command to bring helpers up!\n");
    usage(argv[0]);
  }
  if (down_command==NULL)
  {
    printf("FAILURE: Please specify script command to bring helpers down!\n");
    usage(argv[0]);
  }
  if (down_command==NULL)
  {
    printf("FAILURE: Please specify script command to bring helpers down!\n");
    usage(argv[0]);
  }
  
  errno=0;
  if (kill((pid_t)pid, 0) != 0)
  {
    printf("FAILURE: unable to access process with pid=%d (%s)!\n", pid, strerror(errno));
    finalize(EXIT_FAILURE);
  }
  if (background)
  {
    if (fork() != 0)
      finalize(EXIT_SUCCESS);
  }
  if (pidfile_fname)
    initialize(pidfile_fname);
  
  // Closing stdio - we don't need it anymore
  fclose(stdin);
  fclose(stderr);
  if (log_file)
    freopen(log_file, "w", stdout);
  else if (! log)
    fclose(stdout);
  
  struct sigaction act;
  memset(&act, 0, sizeof(act));
  act.sa_sigaction = signal_handler;
  sigset_t   set;
  sigemptyset(&set);
  sigaddset(&set, SIGTERM);
  sigaddset(&set, SIGQUIT);
  sigaddset(&set, SIGINT);
  act.sa_mask = set;
  sigaction(SIGTERM, &act, 0);
  sigaction(SIGQUIT, &act, 0);
  sigaction(SIGINT, &act, 0);
  
  
  while (TRUE)
  {
    if (get_tcp_activity(pid))
    {
      inactivity_timer=0;
      start_services(script, up_command);
    }
    else
      inactivity_timer++;
    
    if (inactivity_timer > timeout && services_is_started)
    {
      printf("Inactivity timeout exceeded!\n");
      stop_services(script, down_command);
    }
    else
      printf("No activity for %d seconds\n", inactivity_timer);
    (void)usleep (1000000);
  }
}
