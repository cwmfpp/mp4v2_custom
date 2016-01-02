#ifndef WIN32
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
//#else
//#include <WinSock2.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>


#define DEBUG_LOCK_NUM 			64
#define DEBUG_LOCK_TIME  		5000 //ms
#define DEBUG_TRYLOCK_TIME  	500  //ms 

#define DEBUG_LOCK_FIFO  "/tmp/debug_lock_fifo"

typedef struct 
{
  pthread_mutex_t *	lock_addr;
  time_t 			lock_time;
  struct timeval 	ttt;
  pid_t  			tid;
  char 				info[512];
}debug_lock_t;



debug_lock_t lock_list[DEBUG_LOCK_NUM];
int fifo_fd;
sem_t sem;
pthread_once_t once = PTHREAD_ONCE_INIT;

char out_buf[4096];

void debug_lock_init()
{
    sem_init(&sem, 0, 1);
    mkfifo(DEBUG_LOCK_FIFO, 0777);
    fifo_fd = open(DEBUG_LOCK_FIFO, O_RDWR|O_NONBLOCK);
    if(fifo_fd > 0)
    {
    	write(fifo_fd, "open debug_lock_fifo OK !", strlen("open debug_lock_fifo OK !"));
    }
}

void _pthread_mutex_lock(pthread_mutex_t *_lock, char *_name, const char *_file, const char *_fun, int _line)
{
  int i;
  int j;
  int k;
  int flag;
  
  pthread_once(&once, debug_lock_init);
  
  i = DEBUG_LOCK_TIME*100;
  while(pthread_mutex_trylock(_lock) == EBUSY)
  {
    if(i-- < 0)
    {
      i = DEBUG_LOCK_TIME*100;
      sem_wait(&sem); 
     
      strcpy(out_buf,"\n");
      flag = 0;
      for(j = 0; j < DEBUG_LOCK_NUM; j++)
      {
        //if((lock_list[j].lock_addr == _lock) && (time(NULL) - lock_list[j].lock_time > DEBUG_LOCK_TIME2))
        if((lock_list[j].lock_addr == _lock))
        {
              flag = 1;
              sprintf(out_buf+strlen(out_buf),">>>>>list[%d]:%s, get=%ld,%ld, lock=%ld\n", \
                            j, lock_list[j].info, lock_list[j].ttt.tv_sec, lock_list[j].ttt.tv_usec,time(NULL)-lock_list[j].lock_time);
              for(k = 0; k < DEBUG_LOCK_NUM; k++)
              {
                if(lock_list[k].lock_addr != NULL && k != j && lock_list[k].tid == lock_list[j].tid)
                {
                  sprintf(out_buf+strlen(out_buf),"=====list[%d]: %s, get=%ld,%ld, lock=%ld\n",\
                                  k, lock_list[k].info, lock_list[k].ttt.tv_sec, lock_list[k].ttt.tv_usec,time(NULL)-lock_list[k].lock_time);
                }
              }
              break;
        }
      }
      if(flag == 0)
      {
        sprintf(out_buf+strlen(out_buf), "########");
        for(k = 0; k < sizeof(pthread_mutex_t); k++)
        {
          sprintf(out_buf+strlen(out_buf), "%02x ", *(((char *)_lock)+k));
        }
         sprintf(out_buf+strlen(out_buf), "\n");
      }
      sprintf(out_buf+strlen(out_buf),"<<<<<<<<<<<<Req: %s(%p), %s, %s, %d, tid=%ld\n", _name, _lock, _file, _fun, _line, (long int)getpid());
      for(k = 0; k < DEBUG_LOCK_NUM; k++)
      {
         if(lock_list[k].lock_addr != NULL && lock_list[k].tid == getpid())
         {
             sprintf(out_buf+strlen(out_buf),"=====Req:lock_list[%d]: %s, get=%ld,%ld, lock=%ld\n",\
                             k, lock_list[k].info, lock_list[k].ttt.tv_sec, lock_list[k].ttt.tv_usec,time(NULL)-lock_list[k].lock_time);
         }
      }
     write(fifo_fd, out_buf, strlen(out_buf)+1);
     sem_post(&sem); 
     
    }

    usleep(10);

  }
 
   sem_wait(&sem);
  for(i = 0; i < DEBUG_LOCK_NUM; i++)
  {
    if(lock_list[i].lock_addr == NULL)
    {
      lock_list[i].lock_addr = _lock;
      lock_list[i].tid = getpid();
      lock_list[i].lock_time = time(NULL);
      gettimeofday(&(lock_list[i].ttt), NULL);
      sprintf(lock_list[i].info, "%s(%p):%s, %s, %d, tid=%ld", _name,_lock, _file, _fun, _line, (long int)getpid());
      break;
    }
  }
   sem_post(&sem);
}

int _pthread_mutex_trylock(pthread_mutex_t *_lock, char *_name, const char *_file, const char *_fun, int _line)
{
  int i;
  int j;
  int k;
  int flag;

  pthread_once(&once, debug_lock_init);
  
  i = DEBUG_TRYLOCK_TIME*100;
  while(pthread_mutex_trylock(_lock) == EBUSY && i >= 0)
  {

    if(--i < 0)
    {
      sem_wait(&sem); 
     
      strcpy(out_buf,"\n");
      flag = 0;
      for(j = 0; j < DEBUG_LOCK_NUM; j++)
      {
        //if((lock_list[j].lock_addr == _lock) && (time(NULL) - lock_list[j].lock_time > DEBUG_LOCK_TIME2))
        if((lock_list[j].lock_addr == _lock))
        {
              flag = 1;
              sprintf(out_buf+strlen(out_buf),"ttt>>>>>list[%d]: %s, get=%ld,%ld, lock=%ld\n", \
                            j, lock_list[j].info, lock_list[j].ttt.tv_sec, lock_list[j].ttt.tv_usec,time(NULL)-lock_list[j].lock_time);
              for(k = 0; k < DEBUG_LOCK_NUM; k++)
              {
                if(lock_list[k].lock_addr != NULL && k != j && lock_list[k].tid == lock_list[j].tid)
                {
                  sprintf(out_buf+strlen(out_buf),"ttt=====list[%d]: %s, get=%ld,%ld, lock=%ld\n",\
                                  k, lock_list[k].info, lock_list[k].ttt.tv_sec, lock_list[k].ttt.tv_usec,time(NULL)-lock_list[k].lock_time);
                }
              }
              break;
        }

      }
      if(flag == 0)
      {
        sprintf(out_buf+strlen(out_buf), "########");
        for(k = 0; k < sizeof(pthread_mutex_t); k++)
        {
          sprintf(out_buf+strlen(out_buf), "%02x ", *(((char *)_lock)+k));
        }
         sprintf(out_buf+strlen(out_buf), "\n");
      }
      sprintf(out_buf+strlen(out_buf),"ttt<<<<<<<<<<Req: %s(%p), %s, %s, %d, tid=%ld\n", _name, _lock, _file, _fun, _line, (long int)getpid());
      for(k = 0; k < DEBUG_LOCK_NUM; k++)
      {
         if(lock_list[k].lock_addr != NULL && lock_list[k].tid == getpid())
         {
             sprintf(out_buf+strlen(out_buf),"ttt=====Req:lock_list[%d]: %s, get=%ld,%ld, lock=%ld\n",\
                             k, lock_list[k].info, lock_list[k].ttt.tv_sec, lock_list[k].ttt.tv_usec,time(NULL)-lock_list[k].lock_time);
         }
      }
     write(fifo_fd, out_buf, strlen(out_buf)+1);
     sem_post(&sem); 
     
    }

    usleep(10);

  }
 
  if(i >= 0)
  {
     sem_wait(&sem);
    for(i = 0; i < DEBUG_LOCK_NUM; i++)
    {
      if(lock_list[i].lock_addr == NULL)
      {
        lock_list[i].lock_addr = _lock;
        lock_list[i].tid = getpid();
        lock_list[i].lock_time = time(NULL);
        gettimeofday(&(lock_list[i].ttt), NULL);
        sprintf(lock_list[i].info, "%s(%p): %s, %s, %d, tid=%ld", _name,_lock, _file, _fun, _line, (long int)getpid());
        break;
      }
    }
     sem_post(&sem);
     return 0;
  }
  else
  {
    return -1;
  }
}

#if 0
int _pthread_mutex_trylock(pthread_mutex_t *_lock, char *_name, const char *_file, const char *_fun, int _line)
{
  _pthread_mutex_lock(_lock, _name, _file, _fun, _line);
  return 0;
}
#endif

void _pthread_mutex_unlock(pthread_mutex_t *_lock, char *_name, const char *_file, const char *_fun, int _line)
{

  int i;
  sem_wait(&sem);
  for(i = 0; i < DEBUG_LOCK_NUM; i++)
  {
    if(lock_list[i].lock_addr ==  _lock)
    {
      lock_list[i].lock_addr = NULL;
      if(lock_list[i].tid != getpid())
      {
        sprintf(out_buf, "\nWARRING: unlock %s(%p) from a diffrent thread, %s, %s, %d, tid = %ld!\n",  _name, _lock, _file, _fun, _line, (long int)getpid());
        write(fifo_fd, out_buf, strlen(out_buf)+1);
      }
      break;
    }
  }
  sem_post(&sem);
  pthread_mutex_unlock(_lock);
  
}

