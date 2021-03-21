#include <cstdlib>
#include <iostream>
#include <cstring>
#include <pthread.h>
#include <cmath>
#include <ctime>
#include <list>

using namespace std;

#define err_exit(code, str) { cerr << str << ": " << strerror(code) \
<< endl; exit(EXIT_FAILURE); }
#define PI 3.14159265

const int TASKS_COUNT = 10;
int task_list[TASKS_COUNT]; // Массив заданий
int current_task = 0; // Указатель на текущее задание
pthread_mutex_t pth_mutex; // Мьютекс
pthread_spinlock_t spinlock; // Спинлок
std::list<int> map_result = { };

struct Max{
    void operator()(int val){
        value = std::max(value, val);
    }
    int value{std::numeric_limits<int>::min()};
} f_max;

void do_task(int task_no)
{
    double result;
    for (unsigned int i = 0; i < 100'000'000; i++) {
        result = sin(30 * PI / 180) * pow(cos(60 * PI / 180), 2);
    }
}

void *thread_job(void *arg)
{
    int task_no;
    int err;
    while(true) {
        err = pthread_mutex_lock(&pth_mutex);
        if(err != 0) err_exit(err, "Cannot lock mutex");
        task_no = current_task;
        current_task++;
        if(task_no < TASKS_COUNT)
            std::cout << "Thread number " + std::to_string(*(int *)arg) + " took task"  + std::to_string(task_no) + " for execution" << std::endl;
        err = pthread_mutex_unlock(&pth_mutex);
        if(err != 0) err_exit(err, "Cannot unlock mutex");
        if(task_no < TASKS_COUNT)
            do_task(task_no);
        else
            return NULL;
    }
}

void *MapFunction(void *arg)
{
    int d = *(int*)arg;
    int err;
    err = pthread_spin_lock(&spinlock);
    if(err != 0) err_exit(err, "Cannot lock spinlock");
    if(d % 2 == 0) map_result.push_front(d);
    err = pthread_spin_unlock(&spinlock);
    if(err != 0) err_exit(err, "Cannot unlock spinlock");
}

void *ReduceFunction(void *arg)
{
    int d = *(int*)arg;
    int err;
    err = pthread_spin_lock(&spinlock);
    if(err != 0) err_exit(err, "Cannot lock spinlock");
    f_max(d);
    err = pthread_spin_unlock(&spinlock);
    if(err != 0) err_exit(err, "Cannot unlock spinlock");
}

template <class FuncMap, class FuncReduce>
void MapReduce(int* array, FuncMap Map, FuncReduce Reduce, const int num_threads, const int array_size)
{
    int err;
    auto threads = new pthread_t[num_threads];
    auto thread_attrs = new pthread_attr_t[num_threads];

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
            err = pthread_create(&threads[j], &thread_attrs[j], Map, (void*)&array[i * num_threads + j]); // start threads
            if(err != 0) err_exit(err, "Cannot create a thread");
        }
        for (int j = 0; j < num_elem_to_calc; j++){
            err = pthread_join(threads[j], NULL); // wait for threads completion
            if(err != 0) err_exit(err, "Cannot join a thread");
        }
    }

    parts = ceil((double)map_result.size() / num_threads);
    auto iter = map_result.begin();
    for (int i = 0; i < parts; i++) {
        if ((map_result.size() - i * num_threads) < num_threads)
            num_elem_to_calc = map_result.size() - i * num_threads;
        else num_elem_to_calc = num_threads;
        for (int j = 0; j < num_elem_to_calc; j++){
            err = pthread_create(&threads[j], &thread_attrs[j], Reduce, (void*)&(*iter)); // start threads
            if(err != 0) err_exit(err, "Cannot create a thread");
            iter++;
        }
        for (int j = 0; j < num_elem_to_calc; j++){
            err = pthread_join(threads[j], NULL); // wait for threads completion
            if(err != 0) err_exit(err, "Cannot join a thread");
        }
    }

}

int main()
{
    std::srand(std::time(nullptr));
    int err; // Код ошибки
// Инициализируем спинлок
    err = pthread_spin_init(&spinlock, NULL);
    if(err != 0) err_exit(err, "Cannot initialize spinlock");

    // Инициализируем мьютекс
    err = pthread_mutex_init(&pth_mutex, NULL);
    if(err != 0) err_exit(err, "Cannot initialize mutex");
    pthread_t thread1, thread2; // Идентификаторы потоков
    const int thread_1_number = 1, thread_2_number = 2;
// Инициализируем массив заданий случайными числами
    for(int i=0; i<TASKS_COUNT; ++i)
        task_list[i] = rand() % TASKS_COUNT;
// Создаём потоки
    err = pthread_create(&thread1, NULL, thread_job, (void *)&(thread_1_number));
    if(err != 0)
    err_exit(err, "Cannot create thread 1");
    err = pthread_create(&thread2, NULL, thread_job, (void *)&(thread_2_number));
    if(err != 0)
    err_exit(err, "Cannot create thread 2");
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    // Освобождаем ресурсы, связанные с мьютексом
    pthread_mutex_destroy(&pth_mutex);

    int array_size, num_threads;
    std::cout << "Введите размер массива: ";
    std::cin >> array_size;
    std::cout << "Введите количество потоков: ";
    std::cin >> num_threads;
    auto arr = new int[array_size];
    for (int i = 0; i < array_size; i++)
        arr[i] = 1 + std::rand() % array_size;
    MapReduce(arr, MapFunction, ReduceFunction, num_threads, array_size);
    pthread_spin_destroy(&spinlock);
    for (int n : map_result)
        std::cout << n << "\t";
    std::cout << std::endl;
    std::cout << "max: " << f_max.value;
}