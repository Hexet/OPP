#include <pthread.h>
#include <cstdio>
#include <cstdlib>
//#include <time.h>
#include <iostream>
#include <cmath>
#include <cstring>
#include <mutex>

#define err_exit(code, str) { std::cerr << str << ": " << std::strerror(code) \
<< std::endl; exit(EXIT_FAILURE); }

//clock_t t;

struct Params
{
    int cParam;
    pthread_attr_t thread_attr;
};

std::mutex mtx;

void printAttrInfo(pthread_attr_t thread_attr)
{
    int err;
    mtx.lock();
    int state = 0;
    err = pthread_attr_getdetachstate(&thread_attr, &state);
    if(err != 0) err_exit(err, "Getting detach state attribute failed");
    std::cout << "PTHREAD DETACHED: " << state << std::endl;
    size_t value;
    err = pthread_attr_getguardsize(&thread_attr, &value);
    if(err != 0) err_exit(err, "Getting guard size attribute failed");
    std::cout << "guardsize: " << value << std::endl;
    void *stackaddr;
    err = pthread_attr_getstack(&thread_attr, &stackaddr, &value);
    if(err != 0) err_exit(err, "Getting stack attribute failed");
    std::cout << "stackaddr: " << stackaddr << "  stacksize: " << value << std::endl;
    err = pthread_attr_getstacksize(&thread_attr, &value);
    if(err != 0) err_exit(err, "Getting stack size attribute failed");
    std::cout << "stacksize: " << value << std::endl;
    std::cout << "----------------------" << std::endl;
    mtx.unlock();
}

void* threadFunction(void *arg) {
    auto p = (Params*)arg;
    const int el = p->cParam;
    const pthread_attr_t attr = p->thread_attr;

//    std::cout << "time starting thread: " << ((double)clock() - t) / ((double) CLOCKS_PER_SEC) << std::endl;
//    t = clock();
    int result = el;
    for (int i = 0; i < 10000000; i++){ result += i; }
//    std::cout << "time calculating task: " << ((double)clock() - t) / ((double) CLOCKS_PER_SEC) << std::endl;

//    std::cout << el << " ";
    printAttrInfo(attr);

    return 0;
}


int main() {

//    int err;
//    pthread_t test_thread;
//    Params test_p;
//    pthread_attr_t test_attr;
//    err = pthread_attr_init(&test_attr);
//    if(err != 0) err_exit(err, "Cannot create thread attribute");
//    test_p.cParam = 5;
//    test_p.thread_attr = test_attr;
//    t = clock();
//    err = pthread_create(&test_thread, &test_attr, threadFunction, (void*)&test_p);
//    if(err != 0) err_exit(err, "Cannot create a thread");
//    pthread_exit(NULL);

    int array_size, num_threads, err;
    std::cout << "Введите размер массива: ";
    std::cin >> array_size;
    std::cout << "Введите количество потоков: ";
    std::cin >> num_threads;

    auto threads = new pthread_t[num_threads];
    auto thread_attrs = new pthread_attr_t[num_threads];
    auto arr = new int[array_size];
    auto pp = new Params[array_size];

    int parts = ceil((double)array_size / num_threads);

    for (int i = 0; i < num_threads; i++) {
        err = pthread_attr_init(&thread_attrs[i]);
        if(err != 0) err_exit(err, "Cannot create thread attribute");
        err = pthread_attr_setdetachstate(&thread_attrs[i], PTHREAD_CREATE_JOINABLE); // set detach state
        if(err != 0) err_exit(err, "Setting detach state attribute failed");
        err = pthread_attr_setstacksize(&thread_attrs[i], 1*1024*1024);
        if(err != 0) err_exit(err, "Setting stack size attribute failed");
    }

    int num_elem_to_calc = 0;
    for (int i = 0; i < parts; i++) {
        if ((array_size - i * num_threads) < num_threads)
            num_elem_to_calc = array_size - i * num_threads;
        else num_elem_to_calc = num_threads;
        for (int j = 0; j < num_elem_to_calc; j++){
            pp[i * num_threads + j].cParam = i * num_threads + j;
            pp[i * num_threads + j].thread_attr = thread_attrs[j];
            err = pthread_create(&threads[j], &thread_attrs[j], &threadFunction, (void*)&pp[i * num_threads + j]); // start threads
            if(err != 0) err_exit(err, "Cannot create a thread");
        }
        for (int j = 0; j < num_elem_to_calc; j++){
            err = pthread_join(threads[j], NULL); // wait for threads completion
            if(err != 0) err_exit(err, "Cannot join a thread");
        }
    }

    return 0;
}