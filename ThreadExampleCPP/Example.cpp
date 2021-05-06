/* BASIC THREAD
 * 1. Create threads
 * 2. join and detach methods
 * 3. Pass/Out value as argument in thread
 */

#include <iostream>
#include <thread>
#include <string>
#include <chrono>

#include <mutex>
#include <ctime>  
#include <vector>

#include <condition_variable>
#include <random>

#include <future>

#include <atomic>
using namespace std;

//class SimpleThread
class ThreadClass
{
public:

	void SimpleAbstractWork(string tmp)
	{
		for (int i = 0; i < 10; i++)
		{
			cout << "Id Thread - " << std::this_thread::get_id() << "\t" << tmp << " DoWork -" << i << std::endl;
			this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	}

	void LoopSimpleAbstractWork(string tmp)
	{
		int i = 0;
		while (true)
		{
			cout << "Id Thread Loop - " << std::this_thread::get_id() << "\t" << tmp << " DoLoopWork -" << i << std::endl;
			this_thread::sleep_for(std::chrono::milliseconds(1000));
			i++;
		}
	}

	int Sum(int a, int b)
	{
		int result = -1;
		this_thread::sleep_for(std::chrono::milliseconds(1000));
		cout << "---- " << "Id Thread- " << std::this_thread::get_id() << "----Sum Start---- " << a << " + " << b << " ---------------" << endl;

		this_thread::sleep_for(std::chrono::milliseconds(3000));

		result = a + b;

		this_thread::sleep_for(std::chrono::milliseconds(1000));
		cout << "---- " << "Id Thread- " << std::this_thread::get_id() << "----Sum Done---- " << "Result = " << result << " ---------------" << endl;

		return result;
	}

	void Assignment(int& a)
	{
		this_thread::sleep_for(std::chrono::milliseconds(1000));
		cout << "---- " << "Id Thread- " << std::this_thread::get_id() << "----Assignment Start---- " << a << " * 2 " << " ----" << endl;

		this_thread::sleep_for(std::chrono::milliseconds(3000));

		a = a * 2;
		cout << "---- " << "Id Thread- " << std::this_thread::get_id() << "----Assignment Done---- " << "Result = " << a << "  ----" << endl;
	}

	void GreatJobWithoutParametr()
	{
		cout << "---- " << "Id Thread- " << std::this_thread::get_id() << "----GreatJobWithoutParametr Start---- " << endl;
		this_thread::sleep_for(std::chrono::milliseconds(4000));
		cout << "---- " << "Id Thread- " << std::this_thread::get_id() << "----GreatJobWithoutParametr End---- " << endl;
	}
};

//SimpleThread
void SimpleAbstractWork(string tmp)
{
	for (int i = 0; i < 10; i++)
	{
		cout << "Id Thread - " << std::this_thread::get_id() << "\t" << tmp << " DoWork -" << i << std::endl;
		this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}
void LoopSimpleAbstractWork(string tmp)
{
	int i = 0;
	while (true)
	{
		cout << "Id Thread Loop - " << std::this_thread::get_id() << "\t" << tmp << " DoLoopWork -" << i << std::endl;
		this_thread::sleep_for(std::chrono::milliseconds(1000));
		i++;
	}
}
int Sum(int a, int b)
{
	int result = -1;
	this_thread::sleep_for(std::chrono::milliseconds(1000));
	cout << "---- " << "Id Thread- " << std::this_thread::get_id() << "----Sum Start---- " << a << " + " << b << " ---------------" << endl;

	this_thread::sleep_for(std::chrono::milliseconds(3000));

	result = a + b;

	this_thread::sleep_for(std::chrono::milliseconds(1000));
	cout << "---- " << "Id Thread- " << std::this_thread::get_id() << "----Sum Done---- " << "Result = " << result << " ---------------" << endl;

	return result;
}
void Assignment(int& a)
{
	this_thread::sleep_for(std::chrono::milliseconds(1000));
	cout << "---- " << "Id Thread- " << std::this_thread::get_id() << "----Assignment Start---- " << a << " * 2 " << " ----" << endl;

	this_thread::sleep_for(std::chrono::milliseconds(3000));

	a = a * 2;
	cout << "---- " << "Id Thread- " << std::this_thread::get_id() << "----Assignment Done---- " << "Result = " << a << "  ----" << endl;
}

//Mutex
mutex myMutex1;
mutex myMutex2;
recursive_mutex myRecMutex;

int GetRandomInRange(const int min, const int max)
{
	static std::default_random_engine gen(
		static_cast<unsigned>(
			std::chrono::system_clock::now().time_since_epoch().count()
			)
	);
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(gen);
}
void PrintVector(vector<int> VectorToPrint)
{
	for (int i : VectorToPrint)
	{
		printf("%d  ", i);		
	}
	printf("\n");
}
void RandomFillVectorBySize(vector<int> &VectorToFill, int sizeVector)
{
	myMutex1.lock();	
	myMutex2.lock();

	VectorToFill.clear();
	for (int i = 0; i < sizeVector; i++)
	{
		int randNumber = GetRandomInRange(0,100);
		VectorToFill.push_back(randNumber);
		printf("%d  ", randNumber);
		this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	printf("\n");

	myMutex1.unlock();
	myMutex2.unlock();
}
void FillVectorBySize(vector<int>& VectorToFill, int Value, int sizeVector)
{
	//unique_lock<mutex> myUnique_lock(myMutex1, defer_lock);
	//myUnique_lock.lock();
	//myUnique_lock.unlock();
	
	myMutex1.lock();	
	myMutex2.lock();

	VectorToFill.clear();
	for (int i = 0; i < sizeVector; i++)
	{
		VectorToFill.push_back(Value + i);
		printf("%d  ", Value+i);
		this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	printf("\n");

	myMutex1.unlock();
	myMutex2.unlock();

	this_thread::sleep_for(std::chrono::milliseconds(2000));
}
void RecMethod(int r)
{
	myRecMutex.lock();
	printf("%d  ", r);
	this_thread::sleep_for(std::chrono::milliseconds(100));

	if (r<0)
	{
		printf("\n");
		myRecMutex.unlock();
		return;
	}

	r--;
	RecMethod(r);
	myRecMutex.unlock();
}

//condition_variable
condition_variable CV_FillVector;
bool EndConsume = false;

void Consume(int &value, vector<int> &Vector, int neededInt, mutex& locker)
{
	unique_lock<mutex> lock(locker);

	bool bIsConsumeFill = false; 
	while (!bIsConsumeFill)
	{		
		CV_FillVector.wait(lock);
		if (value < 10)
		{
			unsigned int i = 0;
			while(i < Vector.size())
			{
				if (Vector[i] == neededInt)
				{
					value++;
					Vector.erase(Vector.begin() + i);
					i--;
					printf("Hey i found what i want - %d ", neededInt);	
					printf("\n");
					if (value > 10)
					{
						bIsConsumeFill = true;
					}
				}	
				i++;
			}
			Vector.clear();		
		}
	}	
	EndConsume = true;
}
void Produce(vector<int>& VectorToFill, mutex &locker)
{
	unique_lock<mutex> UL(locker, defer_lock);
	
	while (true)
	{
		this_thread::sleep_for(std::chrono::milliseconds(100));
		int randNumber = GetRandomInRange(0, 10);

		UL.lock();
		VectorToFill.push_back(randNumber);
		UL.unlock();
		if (VectorToFill.size() > 50)
		{
			CV_FillVector.notify_one();			
		}		
	}
}

//Future
int GenerateValue(int value)
{
	int i = 0;
	while (i < 2)
	{
		printf("work Second GenerateValue\n");
		this_thread::sleep_for(std::chrono::milliseconds(1000));
		i++;
	}
	return GetRandomInRange(0,100) + value;
}
void GetFutureValue(shared_future<int> myFuture)
{
	printf("\n GetFutureValue Thread - %d", myFuture.get());
}
void GenerateValueWithPromise(int value, promise<int>& promise)
{
	int i = 0;
	while (i < 2)
	{
		printf("work Second GenerateValueWithPromise\n");
		this_thread::sleep_for(std::chrono::milliseconds(1000));
		i++;
	}
	promise.set_value(GetRandomInRange(0, 100) + value);

	this_thread::sleep_for(std::chrono::milliseconds(3000));
	printf("\n END GenerateValueWithPromise ");
}

long NonAtomicValue = 0;
atomic< long> AtomicValue = 0;

void reWriteVariable(long& value)
{
	int i = 0;
	while (i < 1000000)
	{
		value++;
		i++;
	}
	cout << "Thread finish"<<endl;
}

void reWriteVariableAtomic(atomic<long>& value)
{
	int i = 0;
	while (i < 1000000)
	{		
		value = value + 1;
		i++;
	}
	cout << "Atomic Thread finish" << endl;
}

int main()
{
//Atomic
	/*
	thread myThread1(reWriteVariable, std::ref(NonAtomicValue));
	thread myThread2(reWriteVariable, std::ref(NonAtomicValue));
	thread myThread3(reWriteVariable, std::ref(NonAtomicValue));
	thread myThread4(reWriteVariable, std::ref(NonAtomicValue));
	thread myThread5(reWriteVariable, std::ref(NonAtomicValue));
	thread myThread6(reWriteVariable, std::ref(NonAtomicValue));
	myThread1.detach();
	myThread2.detach();
	myThread3.detach();
	myThread4.detach(); 
	myThread5.detach();
	myThread6.detach();
	
	thread myThread1A(reWriteVariableAtomic, std::ref(AtomicValue));
	thread myThread2A(reWriteVariableAtomic, std::ref(AtomicValue));
	thread myThread3A(reWriteVariableAtomic, std::ref(AtomicValue));
	thread myThread4A(reWriteVariableAtomic, std::ref(AtomicValue));
	thread myThread5A(reWriteVariableAtomic, std::ref(AtomicValue));
	thread myThread6A(reWriteVariableAtomic, std::ref(AtomicValue));
	myThread1A.detach();
	myThread2A.detach();
	myThread3A.detach();
	myThread4A.detach();
	myThread5A.detach();
	myThread6A.detach();

	this_thread::sleep_for(std::chrono::milliseconds(2000));

	cout<<"Value ATOMIC - "<< AtomicValue <<endl;
	cout << "Value NonAtomicValue - " << NonAtomicValue << endl;
	*/
//PackageTask

	//packaged_task<int(int)> mytask(GenerateValue);
	//future<int> myfuture = mytask.get_future();
	//thread myThread(std::ref(mytask), 10);
	//myfuture.get();
	//myThread.join();

//Future
	/*promise<int> FuturePromise;
	shared_future<int> SharedFuture = FuturePromise.get_future().share();

	thread PromiseThread(GenerateValueWithPromise, 5, std::ref(FuturePromise));
	thread GetFutureValueThread(GetFutureValue, std::ref(SharedFuture));

	int FutureValue1 = 0;
	shared_future<int> SimpleFuture = async(std::launch::async, GenerateValue, 5);

	int i = 0;
	while (i < 3)
	{
		printf("work Main \n");
		this_thread::sleep_for(std::chrono::milliseconds(2000));
		i++;
	}
	printf("FutureValue1 - %d \n", FutureValue1 = SimpleFuture.get());
	GetFutureValueThread.join();
	PromiseThread.join();*/


//Condition Variable

	/*vector<int> myVector;
	vector<thread> threads;
	
	int SuccessConsumeCout = 0;

	mutex m;
	mutex &lockerFormyVector = m;

	for (int i = 0; i < 2; i++)
	{
		threads.push_back(thread([&]()
		{
			Produce(myVector, lockerFormyVector);
		}));
	}
	for (thread& t : threads)
	{
		t.detach();
	}
	thread ConsumeThread(Consume, std::ref(SuccessConsumeCout), std::ref(myVector), 5, std::ref(lockerFormyVector));

	while (!EndConsume)
	{
		printf("%d ", SuccessConsumeCout);
		
		this_thread::sleep_for(std::chrono::milliseconds(100));
	}	
	printf("\n");
	printf("End work %d ", SuccessConsumeCout);
	ConsumeThread.join();*/
	
//Mutex:

	//chrono::time_point<chrono::steady_clock> StartClock, EndClock;
	//chrono::duration<float> DurationClock;
	//StartClock = chrono::high_resolution_clock::now();
	//
	//srand(static_cast<unsigned int>(time(0)));
	//
	//vector<int> myVector;
	//
	//thread FillVectorTrhead1([&]()
	//{
	//	FillVectorBySize(myVector, 55, 5);
	//});
	//
	//thread FillVectorTrhead2([&]()
	//{
	//	RandomFillVectorBySize(myVector,5);
	//});
	//
	////for (int i = 0; i < 10; i++)
	////{
	////	this_thread::sleep_for(std::chrono::milliseconds(500));
	////}
	//	
	//FillVectorTrhead2.join();
	//FillVectorTrhead1.join();
	//
	//PrintVector(myVector);
	//
	////thread myThread(RecMethod, 3);
	////myThread.join();
	//
	//EndClock = chrono::high_resolution_clock::now();
	//DurationClock = EndClock - StartClock;
	//printf("%f", DurationClock.count());
	//system("pause");

//input output Thread with class:

/*
	cout << "Core thread number - " << thread::hardware_concurrency() << endl;
	cout << "Sleep - 0,1 s";
	this_thread::sleep_for(std::chrono::milliseconds(100));

	cout << "Main thread ID - " << std::this_thread::get_id() << endl;

	ThreadClass myThreadClass;
	int Result;
	int SecondResult = 5;

	thread ThreadObjectWithLamda([&]()
		{
			Result = myThreadClass.Sum(5, 10);
			myThreadClass.Assignment(SecondResult);
		});

	thread ThreadClassWithoutParametr([&]()
		{
			myThreadClass.GreatJobWithoutParametr();
		});

	thread ThreadClassWithoutParametrSecond(&ThreadClass::GreatJobWithoutParametr, myThreadClass);

	myThreadClass.SimpleAbstractWork("Main");

	ThreadObjectWithLamda.join();
	ThreadClassWithoutParametr.join();
	ThreadClassWithoutParametrSecond.join();

	cout << Result << endl;
	cout << SecondResult << endl;
*/

//Simple Thread:

	//thread JoinThread(SimpleAbstractWork, "JoinThread");
	//
	////ждем, пока поток завершит свое выполнение	
	////Любые выделенные ресурсы будут освобождены после выхода из потока.
	//
	//cout << JoinThread.joinable() << endl;//Вывод булевой переменной проверяет не ассоциорван ли обьект(поток) ни с каким потоком
	//JoinThread.join();
	//
	//thread DetachThread(LoopSimpleAbstractWork, "DetachThread");
	//
	////Отделяет поток выполнения от объекта потока(будь это main поток, или другой)
	////позволяет потоку выполняться независимо от дескриптора(токен) потока
	////Любые выделенные ресурсы будут освобождены после выхода из потока.
	//
	//DetachThread.detach();
	//
	//cout << DetachThread.joinable()<< endl;
	//
	////Assignment, доступ к переменной из потока по ссылки
	//int GlobalVar = 8;
	//cout << GlobalVar << endl;
	//
	////Шаблоны функций ref и cref являются вспомогательными функциями
	////которые генерируют объект типа std::reference_wrapper
	////используя вывод аргументов шаблона для определения аргумента шаблона результата
	//
	//thread AssignmentThread(Assignment,std::ref(GlobalVar));
	//cout<< GlobalVar<<endl;
	//
	//int SumThreadResult;
	////Sum доступ к выводу функции с помощью лямда-выражения
	//thread SumThread([&SumThreadResult]() 
	//{
	//	SumThreadResult = Sum(4,6);
	//});
	//SimpleAbstractWork("Main");
	//
	//AssignmentThread.join();
	//SumThread.join();
	//
	////LoopSimpleAbstractWork("Main");


	return 0;
}