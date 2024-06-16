#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include<syslog.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{
    openlog("Threading - Log message",0,LOG_USER);
    struct thread_data* var_thread_param = (struct thread_data* ) thread_param;

    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    //struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    int re = usleep(var_thread_param->wait_to_obtain_ms*1000);
    if(re!=0)
    {
     printf("sleep command failed");

    var_thread_param->thread_complete_success = false;
    }
    syslog(LOG_ERR,"sleep");
    int rc = pthread_mutex_lock(var_thread_param->mutex);
    if(rc != 0)
    {
	    printf("failed to lock mutex");
	    syslog(LOG_ERR,"ERROR: Mutex cannot be locked");
    ERROR_LOG("Failed to obtain the mutex");
    var_thread_param->thread_complete_success = false;
    }

    re = usleep(var_thread_param->wait_to_release_ms*1000);
    if(re != 0)
    {

    var_thread_param->thread_complete_success = false;
    }
   
    rc = pthread_mutex_unlock(var_thread_param->mutex);
     if(rc != 0)
     {
	     printf("failed to unlock mutex");
     ERROR_LOG("Failed to obtain thr mutex");
     syslog(LOG_ERR,"ERROR: Mutex cannot be unlocked");
     var_thread_param->thread_complete_success = false;
     }
    closelog();
    return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
	struct thread_data *var_thread_data = (struct thread_data* )malloc(sizeof(struct thread_data));

      if(var_thread_data == NULL)
      {
	ERROR_LOG("Failed to allocate the memory for var_thread_data");
  	return false;	
      }
    var_thread_data->mutex = mutex;
    var_thread_data->wait_to_obtain_ms=wait_to_obtain_ms;
    var_thread_data->wait_to_release_ms= wait_to_release_ms;
    var_thread_data->thread_complete_success = true;
    int ret;
    printf("before thread function");
    ret = pthread_create(thread, NULL, threadfunc, (void *) var_thread_data);
    if(ret != 0)
    {
    	ERROR_LOG("Failed to create the thread");
	printf("failed to create thread");
	//free(var_thread_data);
    var_thread_data->thread_complete_success = false;
	return false;
    }

    return true;
}

