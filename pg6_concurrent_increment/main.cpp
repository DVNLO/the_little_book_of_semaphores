#include <pthread.h>
#include <cstdio>
#include <cstdlib>

long long unsigned count{ 0 };

void * run(void * arg)
{
    int n{ static_cast<int>(*(static_cast<int *>(arg))) };
    for(int i{ 0 }; i < n; ++i)
    {
        int temp = count;
        count = temp + 1;
    }
    return nullptr;
}

int main()
{
    int n{ 100 };
    int rc;
    pthread_t tids[100];
    for(int i{ 0 }; i < n; ++i)
    {
        rc = pthread_create(&tids[i],
                            nullptr,
                            run,
                            static_cast<void *>(&n));
        if(rc)
            std::exit(EXIT_FAILURE);
    }
    // https://stackoverflow.com/questions/11624545/how-to-make-main-thread-wait-for-all-child-threads-finish
    for(int i{ 0 }; i < n; ++i)
        pthread_join(tids[i], nullptr);
    std::printf("%llu\n", count);
    std::exit(EXIT_SUCCESS);
}

// In reponse to the two questions:
// 1) The maximum count possible is 10,000 = 100 * 100.
//      this supposes that no access collisions occur.
// 2) The minum count possible will occur when an access
//      collision occurs on every possible read. So, 
//      t0 reads 0, increments, and writes 1
//      t1 reads 0, increments, and writes 1
//      ...
//      t99 reads 0, increments, and writes 1.
//      If this possible sequence of execution happened
//      evey time, then the minimum value can be achieved
//      This can occur 100 times, so 100 is the maximum 
//      value obtainable. 
