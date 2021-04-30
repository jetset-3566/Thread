/* BASIC THREAD
 * 1. Create threads
 * 2. join and detach methods
 * 3. Pass/Out value as argument in thread
 */

#include <iostream>
#include <thread>
#include <string>
#include <chrono>

using namespace std;

void SimpleAbstractWork(string tmp)
{
	for (int i = 0; i < 10; i++)
	{
		cout << "Id Thread - " << std::this_thread::get_id() << "\t" << tmp <<  " DoWork -" << i << std::endl;
		this_thread::sleep_for(std::chrono::milliseconds(1500));
	}
}

void LoopSimpleAbstractWork(string tmp)
{
	int i = 0;
	while (true)
	{
		cout << "Id Thread Loop - " << std::this_thread::get_id() << "\t" << tmp << " DoLoopWork -" << i << std::endl;
		this_thread::sleep_for(std::chrono::milliseconds(1500));
		i++;
	}
}

int Sum(int a, int b)
{
	int result = -1;
	this_thread::sleep_for(std::chrono::milliseconds(1000));
	cout << "---- " << "Id Thread- " << std::this_thread::get_id() << "----Sum Start---- "<< a << " + " << b <<" ---------------"<< endl;

	this_thread::sleep_for(std::chrono::milliseconds(3000));

	result = a+b;

	this_thread::sleep_for(std::chrono::milliseconds(1000));
	cout << "---- " << "Id Thread- " << std::this_thread::get_id() << "----Sum Done---- "<< "Result = "<< result <<" ---------------" << endl;
	
	return result;
}

void Assignment(int &a)
{
	this_thread::sleep_for(std::chrono::milliseconds(1000));
	cout << "---- " << "Id Thread- " << std::this_thread::get_id() << "----Assignment Start---- " << a << " * 2 "<< " ----" << endl;

	this_thread::sleep_for(std::chrono::milliseconds(3000));

	a = a * 2;
	cout << "---- "<<"Id Thread- " << std::this_thread::get_id() << "----Assignment Done---- " << "Result = "<< a << "  ----"<< endl;
}

int main()
{
	cout << "Core thread number - " << thread::hardware_concurrency() << endl;
	cout << "Sleep - 0,1 s";
	this_thread::sleep_for(std::chrono::milliseconds(100));

	cout << "Main thread ID - "<< std::this_thread::get_id()<< endl;
	
	thread JoinThread(SimpleAbstractWork, "JoinThread");
	/*
	* ждем, пока поток завершит свое выполнение	
	* Любые выделенные ресурсы будут освобождены после выхода из потока.
	*/
	cout << JoinThread.joinable() << endl;//Вывод булевой переменной проверяет не ассоциорван ли обьект(поток) ни с каким потоком
	JoinThread.join();

	thread DetachThread(LoopSimpleAbstractWork, "DetachThread");
	/*
	* Отделяет поток выполнения от объекта потока(будь это main поток, или другой)
	* позволяет потоку выполняться независимо от дескриптора(токен) потока
	* Любые выделенные ресурсы будут освобождены после выхода из потока.
	*/
	DetachThread.detach();

	cout << DetachThread.joinable()<< endl;
	
	//Assignment, доступ к переменной из потока по ссылки
	int GlobalVar = 8;
	cout << GlobalVar << endl;
	/*Шаблоны функций ref и cref являются вспомогательными функциями
	* которые генерируют объект типа std::reference_wrapper
	* используя вывод аргументов шаблона для определения аргумента шаблона результата
	*/
	thread AssignmentThread(Assignment,std::ref(GlobalVar));
	cout<< GlobalVar<<endl;

	int SumThreadResult;
	//Sum доступ к выводу функции с помощью лямда-выражения
	thread SumThread([&SumThreadResult]() 
	{
		SumThreadResult = Sum(4,6);
	});
	SimpleAbstractWork("Main");
	
	AssignmentThread.join();
	SumThread.join();
	
	//LoopSimpleAbstractWork("Main");
	return 0;
}