#include "main.h"


void *fnC()
{
    int i;
    for(i=0;i<1000000;i++)
    {
        c++;
    }
}


pthread_mutex_t count_mutex;
pthread_spinlock_t count_spin;
my_spinlock_t count_my_spinlock;
my_mutex_t count_myMutex;
my_queuelock_t count_myQueue;


void *pthreadMutexTest()
{

    int i;
	int j;
	int k;

	int localCount = 0;

    for(i=0;i<numItterations;i++)
    {

		for(j=0;j<workOutsideCS;j++)/*How much work is done outside the CS*/
		{
			localCount++;
		}

		pthread_mutex_lock(&count_mutex);
		for(k=0;k<workInsideCS;k++)/*How much work is done inside the CS*/
		{
			c++;
		}
		pthread_mutex_unlock(&count_mutex);

    }
}

void *pthreadSpinlockTest()
{

    int i;
  int j;
  int k;

  int localCount = 0;

    for(i=0;i<numItterations;i++)
    {

    for(j=0;j<workOutsideCS;j++)/*How much work is done outside the CS*/
    {
      localCount++;
    }

    pthread_spin_lock(&count_spin);
    for(k=0;k<workInsideCS;k++)/*How much work is done inside the CS*/
    {
      c++;
    }
    pthread_spin_unlock(&count_spin);
  }
}

void *myspinLockTASTest()
{
  int i;
int j;
int k;

int localCount = 0;

  for(i=0;i<numItterations;i++)
  {

  for(j=0;j<workOutsideCS;j++)/*How much work is done outside the CS*/
  {
    localCount++;
  }

  my_spinlock_lockTAS(&count_my_spinlock);
  for(k=0;k<workInsideCS;k++)/*How much work is done inside the CS*/
  {
    c++;
  }
  my_spinlock_unlock(&count_my_spinlock);
  }
}

void *myspinLockTTASTest()
{
  int i;
int j;
int k;

int localCount = 0;

  for(i=0;i<numItterations;i++)
  {

  for(j=0;j<workOutsideCS;j++)/*How much work is done outside the CS*/
  {
    localCount++;
  }

  my_spinlock_lockTTAS(&count_my_spinlock);
  for(k=0;k<workInsideCS;k++)/*How much work is done inside the CS*/
  {
    c++;
  }
  my_spinlock_unlock(&count_my_spinlock);
  }
}

void *myMutexTest()
{
	int i;
int j;
int k;

int localCount = 0;

  for(i=0;i<numItterations;i++)
  {

  for(j=0;j<workOutsideCS;j++)/*How much work is done outside the CS*/
  {
    localCount++;
  }

  my_mutex_lock(&count_myMutex);
  for(k=0;k<workInsideCS;k++)/*How much work is done inside the CS*/
  {
    c++;
  }
  my_mutex_unlock(&count_myMutex);
  }
}

void *myQueueTest()
{
	int i;
	int j;
	int k;

	int localCount = 0;

	  for(i=0;i<numItterations;i++)
	  {

	  for(j=0;j<workOutsideCS;j++)/*How much work is done outside the CS*/
	  {
	    localCount++;
	  }

	  my_queuelock_lock(&count_myQueue);
	  for(k=0;k<workInsideCS;k++)/*How much work is done inside the CS*/
	  {
	    c++;
	  }
	  my_queuelock_unlock(&count_myQueue);
	  }
}

int runTest(int testID)
{

/*You must create all data structures and threads for you experiments*/

if (testID == 0 || testID == 1 ) /*Pthread Mutex*/
{
	c=0;
	struct timespec start;
	struct timespec stop;
	unsigned long long result; //64 bit integer

	pthread_t *threads = (pthread_t* )malloc(sizeof(pthread_t)*numThreads);
	int i;
	int rt;

	clock_gettime(CLOCK_MONOTONIC, &start);
	for(i=0;i<numThreads;i++)
	{

	 if( rt=(pthread_create( threads+i, NULL, &pthreadMutexTest, NULL)) )
	{
		printf("Thread creation failed: %d\n", rt);
		return -1;
	}

	}

	for(i=0;i<numThreads;i++) //Wait for all threads to finish
	{
		 pthread_join(threads[i], NULL);
	}
	clock_gettime(CLOCK_MONOTONIC, &stop);

	printf("Threaded Run Pthread (Mutex) Total Count: %d\n", c);
	result=timespecDiff(&stop,&start);
	printf("Pthread Mutex time(ms): %llu\n",result/1000000);

}

if(testID == 0 || testID == 2) /*Pthread Spinlock*/
{
  c=0;
  struct timespec start;
  struct timespec stop;
  unsigned long long result; //64 bit integer
  pthread_spin_init(&count_spin, 0);

  pthread_t *threads = (pthread_t* )malloc(sizeof(pthread_t)*numThreads);
  int i;
  int rt;

  clock_gettime(CLOCK_MONOTONIC, &start);
  for(i=0;i<numThreads;i++)
  {

   if( rt=(pthread_create( threads+i, NULL, &pthreadSpinlockTest, NULL)) )
  {
    printf("Thread creation failed: %d\n", rt);
    return -1;
  }

  }

  for(i=0;i<numThreads;i++) //Wait for all threads to finish
  {
     pthread_join(threads[i], NULL);
  }
  clock_gettime(CLOCK_MONOTONIC, &stop);

  printf("Threaded Run Pthread (Spin Lock) Total Count: %d\n", c);
  result=timespecDiff(&stop,&start);
  printf("Pthread Spin Lock time(ms): %llu\n",result/1000000);
}

if(testID == 0 || testID == 3) /*MySpinlockTAS*/
{
  c=0;
  struct timespec start;
  struct timespec stop;
  unsigned long long result; //64 bit integer

  pthread_t *threads = (pthread_t* )malloc(sizeof(pthread_t)*numThreads);
  int i;
  int rt;
  my_spinlock_init(&count_my_spinlock);
  clock_gettime(CLOCK_MONOTONIC, &start);
  for(i=0;i<numThreads;i++)
  {

   if( rt=(pthread_create( threads+i, NULL, &myspinLockTASTest, NULL)) )
  {
    printf("Thread creation failed: %d\n", rt);
    return -1;
  }

  }

  for(i=0;i<numThreads;i++) //Wait for all threads to finish
  {
     pthread_join(threads[i], NULL);
  }
  clock_gettime(CLOCK_MONOTONIC, &stop);

  printf("Threaded Run mySpinLockTAS Total Count: %d\n", c);
  result=timespecDiff(&stop,&start);
  printf("mySpinLockTAS time(ms): %llu\n",result/1000000);
}

if(testID == 0 || testID == 4)
{
  c=0;
  struct timespec start;
  struct timespec stop;
  unsigned long long result; //64 bit integer

  pthread_t *threads = (pthread_t* )malloc(sizeof(pthread_t)*numThreads);
  int i;
  int rt;
  my_spinlock_init(&count_my_spinlock);
  clock_gettime(CLOCK_MONOTONIC, &start);
  for(i=0;i<numThreads;i++)
  {

   if( rt=(pthread_create( threads+i, NULL, &myspinLockTTASTest, NULL)) )
  {
    printf("Thread creation failed: %d\n", rt);
    return -1;
  }

  }

  for(i=0;i<numThreads;i++) //Wait for all threads to finish
  {
     pthread_join(threads[i], NULL);
  }
  clock_gettime(CLOCK_MONOTONIC, &stop);

  printf("Threaded Run mySpinLockTTAS Total Count: %d\n", c);
  result=timespecDiff(&stop,&start);
  printf("mySpinLockTTAS time(ms): %llu\n",result/1000000);
}

if(testID == 0 || testID == 5)
{
  c=0;
  struct timespec start;
  struct timespec stop;
  unsigned long long result; //64 bit integer

  pthread_t *threads = (pthread_t* )malloc(sizeof(pthread_t)*numThreads);
  int i;
  int rt;
  my_mutex_init(&count_myMutex);
  clock_gettime(CLOCK_MONOTONIC, &start);
  for(i=0;i<numThreads;i++)
  {

   if( rt=(pthread_create( threads+i, NULL, &myMutexTest, NULL)) )
  {
    printf("Thread creation failed: %d\n", rt);
    return -1;
  }

  }

  for(i=0;i<numThreads;i++) //Wait for all threads to finish
  {
     pthread_join(threads[i], NULL);
  }
  clock_gettime(CLOCK_MONOTONIC, &stop);

  printf("Threaded Run myMutexTest Total Count: %d\n", c);
  result=timespecDiff(&stop,&start);
  printf("myMutexTest time(ms): %llu\n",result/1000000);
}

if(testID == 0 || testID == 6)
{
  c=0;
  struct timespec start;
  struct timespec stop;
  unsigned long long result; //64 bit integer
  

  pthread_t *threads = (pthread_t* )malloc(sizeof(pthread_t)*numThreads);
  int i;
  int rt;
  my_queuelock_init(&count_myQueue);
  clock_gettime(CLOCK_MONOTONIC, &start);
  for(i=0;i<numThreads;i++)
  {

   if( rt=(pthread_create( threads+i, NULL, &myQueueTest, NULL)) )
  {
    printf("Thread creation failed: %d\n", rt);
    return -1;
  }

  }

  for(i=0;i<numThreads;i++) //Wait for all threads to finish
  {
     pthread_join(threads[i], NULL);
  }
  clock_gettime(CLOCK_MONOTONIC, &stop);

  printf("Threaded Run myQueueTest Total Count: %d\n", c);
  result=timespecDiff(&stop,&start);
  printf("myQueueTest time(ms): %llu\n",result/1000000);
}
/*....you must implement the other tests....*/

	return 0;
}

int testAndSetExample()
{
volatile long test = 0; //Test is set to 0
printf("Test before atomic OP:%d\n",test);
tas(&test);
printf("Test after atomic OP:%d\n",test);
}

int processInput(int argc, char *argv[])
{

	//	my method of parse input from command line is using argv[] and library function atoi() to convert input string to a int
	numThreads=atoi(argv[2]) ;
	numItterations=atoi(argv[4]);
	testID=atoi(argv[10]);
	workOutsideCS=atoi(argv[6]);
	workInsideCS=atoi(argv[8]);

	/*printf("%d\n", numThreads);
	printf("%d\n", numItterations);
	printf("%d\n", testID);
	printf("%d\n", workOutsideCS);
	printf("%d\n", workInsideCS);	*/
	return 0;
}


int main(int argc, char *argv[])
{


	printf("Usage of: %s -t #threads -i #Itterations -o #OperationsOutsideCS -c #OperationsInsideCS -d testid\n", argv[0]);
	printf("testid: 0=all, 1=pthreadMutex, 2=pthreadSpinlock, 3=mySpinLockTAS, 4=mySpinLockTTAS, 5=myMutexTAS, 6=myQueueLock, \n");

	//testAndSetExample(); //Uncomment this line to see how to use TAS

	processInput(argc,argv);
	runTest(testID);
	return 0;

}
