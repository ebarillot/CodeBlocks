class TMutex
{
friend class TCondition;
protected:
// the mutex itself and the mutex attribute
pthread_mutex_t _mutex;
pthread_mutexattr_t _mutex_attr;
public:
TMutex ()
{ pthread_mutexattr_init( & _mutex_attr );
pthread_mutex_init( & _mutex, & _mutex_attr );
}
~TMutex ()
{ pthread_mutex_destroy( & _mutex );
pthread_mutexattr_destroy( & _mutex_attr );
}
// lock and unlock mutex (return 0 on success)
int lock () { return pthread_mutex_lock( & _mutex ); }
int unlock () { return pthread_mutex_unlock( & _mutex ); }
// try a lock (return 0 if lock was successful)
int trylock () { return pthread_mutex_trylock( & _mutex ); }
};
