/*
 * NAME, etc.
 *
 * sync.c
 *
 *
 */

#define _REENTRANT

#include "sync.h"



/*
 * Spinlock routines
 */

int my_spinlock_init(my_spinlock_t *lock)
{
	lock->lock_value = 0;
	lock->thr_id = 0;
}

int my_spinlock_destroy(my_spinlock_t *lock)
{
	lock->lock_value = 0;
	lock->thr_id = 0;
}

int my_spinlock_unlock(my_spinlock_t *lock)
{
	lock->lock_value = 0;
	lock->thr_id = 0;
}

int my_spinlock_lockTAS(my_spinlock_t *lock)
{
	
	if(lock->thr_id != pthread_self())
	{
		while(tas(&lock->lock_value) != 0) {;}
		lock->thr_id = pthread_self();
	}

}

int my_spinlock_lockTTAS(my_spinlock_t *lock)
{
	bool condition = true;
	while(condition)
	{
		while(lock->lock_value == 1) {;}
		if(tas(&lock->lock_value) == 0)
		{
			lock->thr_id = pthread_self();
			condition = false;
		}
	}
}

int my_spinlock_trylock(my_spinlock_t *lock)
{
	if(tas(&lock->lock_value) == 0)
	{
		lock->thr_id = pthread_self();
	}
}


/*
 * Mutex routines
 */

int my_mutex_init(my_mutex_t *lock)
{
	lock->sleep_time = 1;
	lock->lock_value = 0;
	lock->thr_id = 0;
}

int my_mutex_destroy(my_mutex_t *lock)
{
	lock->sleep_time = 1;
	lock->lock_value = 0;
	lock->thr_id = 0;
}

int my_mutex_unlock(my_mutex_t *lock)
{
	lock->sleep_time = 1;
	lock->lock_value = 0;
	lock->thr_id = 0;
}

int my_mutex_lock(my_mutex_t *lock)
{
	if(lock->thr_id != pthread_self())
	{
		while(tas(&lock->lock_value) == 1)
		{
			usleep(lock->sleep_time);
			lock->sleep_time *= 2;
		}
		if(tas(&lock->lock_value) == 0)
		{
			lock->thr_id = pthread_self();
		}
	}
}

int my_mutex_trylock(my_mutex_t *lock)
{
	if(tas(&lock->lock_value) == 0)
	{
		lock->thr_id = pthread_self();
	}
}

/*
 * Queue Lock
 */

int my_queuelock_init(my_queuelock_t *lock)
{
	lock->now_serving = 0;
	lock->next_ticket = 0;
	lock->thr_id = 0;
}

int my_queuelock_destroy(my_queuelock_t *lock)
{
	lock->now_serving = 0;
	lock->next_ticket = 0;
	lock->thr_id = 0;
	return 0;
}

int my_queuelock_unlock(my_queuelock_t *lock)
{
	if(lock->thr_id == pthread_self())
	{
		lock->thr_id = 0;
		lock->now_serving += 1;
	}
	return 0;
}

int my_queuelock_lock(my_queuelock_t *lock)
{
	bool condition = true;
	if(lock->thr_id != pthread_self())
	{
		while(condition == true)
		{
			volatile unsigned long original_next = lock->next_ticket;
			volatile unsigned long my_ticket = cas(&(lock->next_ticket),original_next,original_next+1);
			if(my_ticket == original_next)
			{
				while(lock->now_serving != my_ticket) {;}
				if(lock->now_serving == my_ticket)
				{
					lock->thr_id = pthread_self();
					condition = false;
				}
			}
		}
	}
}

int my_queuelock_trylock(my_queuelock_t *lock)
{
	volatile unsigned long original_next = lock->next_ticket;
	volatile unsigned long my_ticket = cas(&(lock->next_ticket),original_next,original_next+1);
	if(lock->now_serving == my_ticket)
	{
		lock->thr_id = pthread_self();
	}
}

