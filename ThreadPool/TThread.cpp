#include <iostream>
#include <pthread.h>

class TThread
{
protected:
    pthread_t _thread_id; // ID of the thread
    pthread_attr_t _thread_attr; // thread attributes
    bool _running; // true if thread is running
    int _thread_no; // opt. number of thread
public:
    // constructor and destructor
    TThread ( int p = -1 );
    virtual ~TThread ();
    // access local data
    int thread_no () const
    {
        return _thread_no;
    }
    void set_thread_no ( int p )
    {
        _thread_no = p;
    }
    bool on_proc ( int p ) const;
    // user interface
    virtual void run () = 0;
    virtual void start ( bool d = false,
                         bool s = false )
    {
        create( d, s );
    }
    virtual void stop ()
    {
        cancel();
    }
    // thread management
    void create ( bool d = false, bool s = false );
    void join ();
    void cancel ();
};
