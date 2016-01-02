#ifndef  _DEBUG_LOCK_H
#define  _DEBUG_LOCK_H

#include <pthread.h>

#ifdef DEBUG_LOCK
void _pthread_mutex_lock(pthread_mutex_t *_lock, char *_name, const char *_file, const char *_fun, int _line);
int _pthread_mutex_trylock(pthread_mutex_t *_lock, char *_name, const char *_file, const char *_fun, int _line);
void _pthread_mutex_unlock(pthread_mutex_t *_lock, char *_name, const char *_file, const char *_fun, int _line);

#define  pthread_mutex_lock(a)  	_pthread_mutex_lock(a, #a, __FILE__, __FUNCTION__, __LINE__)
#define  pthread_mutex_trylock(a)  	_pthread_mutex_trylock(a, #a, __FILE__, __FUNCTION__, __LINE__)
#define  pthread_mutex_unlock(a)   	_pthread_mutex_unlock(a, #a, __FILE__, __FUNCTION__, __LINE__)
#endif

#endif
