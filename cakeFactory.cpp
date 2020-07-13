#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <queue>
#include<string>
#include <unistd.h>
using namespace std;

//semaphore to control sleep and wake up
sem_t empty1;
sem_t full1;
sem_t empty2;
sem_t full2;
sem_t empty3;
sem_t full3;
pthread_mutex_t lock1;
pthread_mutex_t lock2;
pthread_mutex_t lock3;
pthread_mutex_t consolLock;

class cakeItem
{
  public:
    string cake_type;
    int item_no;
    cakeItem(string t, int item){
        cake_type = t;
        item_no = item;
    }
};

queue<cakeItem> q1;
queue<cakeItem> q2;
queue<cakeItem> q3;

void init_semaphore()
{
    sem_init(&empty1, 0, 5);
    sem_init(&full1, 0, 0);
    pthread_mutex_init(&lock1, 0);

    sem_init(&empty2, 0, 5);
    sem_init(&full2, 0, 0);
    pthread_mutex_init(&lock2, 0);

    sem_init(&empty3, 0, 5);
    sem_init(&full3, 0, 0);
    pthread_mutex_init(&lock3, 0);
    pthread_mutex_init(&consolLock, 0);
}

void *chefX(void *arg)
{
    pthread_mutex_lock(&consolLock);
    printf("%s\n", (char *)arg);\
    pthread_mutex_unlock(&consolLock);

    int i;
    for (i = 1; ; i++)
    {
        pthread_mutex_lock(&consolLock);
        printf("chef X is waiting....\n");
        pthread_mutex_unlock(&consolLock);

        sem_wait(&empty1);

        pthread_mutex_lock(&lock1);
        sleep(1);
        q1.push(cakeItem("chocolate",i));
        pthread_mutex_lock(&consolLock);
        printf("chef X produced chocolate item %d\nQueue 1 size: %d\n\n", i,q1.size());
        pthread_mutex_unlock(&consolLock);
        pthread_mutex_unlock(&lock1);

        sem_post(&full1);
    }
}

void *chefY(void *arg)
{
    pthread_mutex_lock(&consolLock);
    printf("%s\n", (char *)arg);
    pthread_mutex_unlock(&consolLock);

    int i;
    for (i = 1; ; i++)
    {
        pthread_mutex_lock(&consolLock);
        printf("chef Y is waiting....\n");
        pthread_mutex_unlock(&consolLock);
        
        sem_wait(&empty1);

        pthread_mutex_lock(&lock1);
        sleep(1);
        q1.push(cakeItem("vanilla", i));

        pthread_mutex_lock(&consolLock);
        printf("chef Y produced vanilla item %d\nQueue 1 size: %d\n\n", i,q1.size());
        pthread_mutex_unlock(&consolLock);

        pthread_mutex_unlock(&lock1);

        sem_post(&full1);
    }
}

void *chefZ(void *arg)
{
    pthread_mutex_lock(&consolLock);
    printf("%s\n", (char *)arg);
    pthread_mutex_unlock(&consolLock);
    int i;
    int c = 0;
    int v = 0;

    for (i = 1; ; i++)
    {
        pthread_mutex_lock(&consolLock);
        printf("chef Z is waiting for queue 1....\n");
        pthread_mutex_unlock(&consolLock);
        sem_wait(&full1);

        pthread_mutex_lock(&lock1);
        sleep(1);
        cakeItem item = q1.front();
        q1.pop();

        pthread_mutex_lock(&consolLock);
        printf("chef Z consumed %s item %d\nQueue 1 size: %d\n\n", item.cake_type.c_str(), item.item_no,q1.size());
        pthread_mutex_unlock(&consolLock);

        pthread_mutex_unlock(&lock1);

        sem_post(&empty1);

        if (item.cake_type == "chocolate")
        {
            pthread_mutex_lock(&consolLock);
            printf("chef Z is waiting for queue 2....\n");
            pthread_mutex_unlock(&consolLock);

            sem_wait(&empty2);
            pthread_mutex_lock(&lock2);

            sleep(1);
            q2.push(item);

            pthread_mutex_lock(&consolLock);
            printf("chef Z put chocolate item %d on q2\nQueue 2 size: %d\n\n", item.item_no,q2.size());
            pthread_mutex_unlock(&consolLock);

            pthread_mutex_unlock(&lock2);
            sem_post(&full2);
        }
        else
        {
            pthread_mutex_lock(&consolLock);
            printf("chef Z is waiting for queue 3....\n");
            pthread_mutex_unlock(&consolLock);

            sem_wait(&empty3);
            pthread_mutex_lock(&lock3);

            sleep(1);
            q3.push(item);

            pthread_mutex_lock(&consolLock);
            printf("chef Z put vanilla item %d on q3\nQueue 3 size:%d \n\n", item.item_no,q3.size());
            pthread_mutex_unlock(&consolLock);

            pthread_mutex_unlock(&lock3);
            sem_post(&full3);
        }
    }
}

void *waiterA(void *arg)
{
    pthread_mutex_lock(&consolLock);
    printf("%s\n", (char *)arg);
    pthread_mutex_unlock(&consolLock);
    int i;
    for (i = 1; ; i++)
    {
        pthread_mutex_lock(&consolLock);
        printf("waiter A is waiting....\n");
        pthread_mutex_unlock(&consolLock);

        sem_wait(&full2);
        pthread_mutex_lock(&lock2);

        sleep(1);
        cakeItem item = q2.front();
        q2.pop();

        pthread_mutex_lock(&consolLock);
        printf("waiterA consumed %s item %d\nQueue 2 size: %d\n\n", item.cake_type.c_str(), item.item_no,q2.size());
        pthread_mutex_unlock(&consolLock);

        pthread_mutex_unlock(&lock2);
        sem_post(&empty2);
    }
}

void *waiterB(void *arg)
{
    pthread_mutex_lock(&consolLock);
    printf("%s\n", (char *)arg);
    pthread_mutex_unlock(&consolLock);
    int i;
    for (i = 1; ; i++)
    {
        pthread_mutex_lock(&consolLock);
        printf("waiter B is waiting....\n");
        pthread_mutex_unlock(&consolLock);

        sem_wait(&full3);
        pthread_mutex_lock(&lock3);

        sleep(1);
        cakeItem item = q3.front();
        q3.pop();

        pthread_mutex_lock(&consolLock);
        printf("waiterB consumed %s item %d\nQueue 3 size: %d\n\n", item.cake_type.c_str(), item.item_no,q3.size());
        pthread_mutex_unlock(&consolLock);

        pthread_mutex_unlock(&lock3);
        sem_post(&empty3);
    }
}

int main(void)
{
    pthread_t xThread;
    pthread_t yThread;
    pthread_t zThread;
    pthread_t aThread;
    pthread_t bThread;

    init_semaphore();

    char *message1 = "i am chef X";
    char *message2 = "i am chef Y";
    char *messege3 = "i am chef Z";
    char *messege4 = "i am waiter A";
    char *messege5 = "i am waiter B";

    pthread_create(&xThread, NULL, chefX, (void *)message1);
    pthread_create(&yThread, NULL, chefY, (void *)message2);
    pthread_create(&zThread, NULL, chefZ, (void *)messege3);
    pthread_create(&aThread, NULL, waiterA, (void *)messege4);
    pthread_create(&bThread, NULL, waiterB, (void *)messege5);

    while (1);
        
    return 0;
}