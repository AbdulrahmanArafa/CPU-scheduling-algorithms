// CPU Scheduling1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//



#include <iostream>
#include<iomanip>
#include<random>
#include<ctime>
#include<utility>
#include<Windows.h>
#include"Queue.h"
#include<list>
#include<cmath>
//using namespace std;
using std::cout;
using std::cin;
using std::list;
using std::pair;
using std::endl;
using std::setfill;
using std::setw;

// prototype function 
int dispatched(list<Proc> l);
void info_SJF(int completion[4], int Response[4]);
void SJF(Queue start);
void FCFS(Queue start);
// globle varibles
int Time = 0;
typedef long long ll;
// we use  that pair to know which is smallest burst in Ready queue
typedef pair<int, int>  Pair;
/********************************************************************************/
// 1-first-come-first-served FCFS  non-preemptive  
void FCFS(Queue start) {

	Queue Running;
	Queue leaves;
	Queue wait;

	int leave = 0;
	int compeletion[4] = { 0 }; // process done
	int Turnaround[4] = { 0 };  // Turn around
	int starts[4] = { 0 }; // when to start Running

	while (true)
	{

		if (Time == leave and !Running.ISemty())
		{
			compeletion[Running.ID() - 1] = Time;
			Turnaround[Running.ID() - 1] = compeletion[Running.ID() - 1] - Running.Arrival();
			leaves.insert(Running.data());
			cout << setw(60) << setfill('-') << " \n";
			cout << "<system time " << Time << " > Pocess <" << Running.ID() << "> finished\n";
			cout << setw(60) << setfill('-') << " \n";
			Running.Delete();


		}


		while (!start.ISemty() and start.Arrival() == Time)
		{

			wait.insert(start.data());

			start.Delete();

		}

		// make sure one proucess is process
		if (!wait.ISemty() and Running.ISemty())
		{
			Running.insert(wait.data());
			starts[Running.ID() - 1] = Time;
			leave = Time + Running.Burst();
			wait.Delete();
		}
		// Runing print 
		if (!Running.ISemty())
		{
			cout << setw(60) << setfill('-') << " \n";

			cout << "<system time " << Time << " > Pocess <" << Running.ID() << ">Running \n";
			cout << setw(60) << setfill('-') << " \n";


		}
		else
		{
			cout << setw(60) << setfill('-') << " \n";
			cout << "<system time " << Time << " > \t idel \n";
			cout << setw(60) << setfill('-') << " \n";

		}
		// break the loop 
		if (start.ISemty() and Running.ISemty() and wait.ISemty())
		{
			// turnaround 
			for (int i = 0; i < 4; i++)
			{
				cout << setw(50) << setfill(' ') << "Turnaround(" << i + 1 << ")" << Turnaround[i] << " | "
					<< "Completed Time(" << i + 1 << ")" << compeletion[i] << " | "
					<< "Waiting Time(" << i + 1 << ")" << starts[i] - leaves.Arrival() << " | "
					<< endl;
				leaves.Delete();

			}
			break;

		}

		Sleep(300);
		Time++;


	}

}
//Shortest job first (SJF) // non-preemptive 
void SJF(Queue start) {
	// use to cal the main process Time 
	int completion[4];
	int Response[4];
	int wait[4] = { 0 };
	// tis list is used as queues to determint stage of process
	list <Proc> Start_;
	list <Proc> Ready_;
	list <Proc> Runing_;
	// if enter function running in line   175 we dont need to increament it agine in end of SJF
	bool increament = true;
	int leave = 0;
	int max_time = 0;
	while (!start.ISemty())
	{
		Start_.push_back(start.data());

		max_time += start.Arrival() + start.Burst() + 1;
		start.Delete();
	}
	while (true) {
		if (!Start_.empty()) {

			int c = 0;
			for (auto x : Start_)
			{
				if (x.arrival <= Time)
				{
					Ready_.push_back(x);
					c++;
				}
			}
			while (c-- and !Start_.empty())
			{
				Start_.pop_front();
			}



		}
		if (!Ready_.empty())
		{
			auto it = Ready_.begin();
			int count = 0;
			int choose_ID = dispatched(Ready_);
			for (auto x : Ready_)
			{
				if (x.procID == choose_ID)
				{
					Runing_.push_back(x);
					leave = Runing_.front().burst + Time;
					Response[x.procID - 1] = Time;
					completion[x.procID - 1] = leave;
					advance(it, count);
					Ready_.erase(it);
					break;

				}
				count++;

			}


		}
		while (!Runing_.empty())
		{
			increament = false;

			cout << setw(60) << setfill('-') << " \n";

			cout << "<system time " << Time << " > Pocess <" << Runing_.front().procID << ">Running \n";
			cout << setw(60) << setfill('-') << " \n";
			if (Time == leave)
			{
				cout << setw(60) << setfill('-') << " \n";

				cout << "<system time " << Time << " > Pocess <" << Runing_.front().procID << ">Leave \n";
				cout << setw(60) << setfill('-') << " \n";
				Runing_.pop_front();
				break;
			}

			Sleep(300);
			Time++;
		}


		if (max_time == Time)
		{
			cout << " program will termined becouse we arrival to max time  press any key";
			system("pause");
			break;

		}
		if (Ready_.empty() and Start_.empty())
			break;
		if (increament) {
			Sleep(300);
			Time++;
		}
		increament = true;

	}

	info_SJF(completion, Response);

}
/// Short Remaber Job first (SRF) // preemptive
void SRF(Queue start) {
	//global var
	int max_time = 0;
	int leave = 0;
	bool interrupt = false;
	bool increament = true;
	// tis list is used as queues to determint stage of process
	list <Proc> Start_;
	list <Proc> Ready_;
	list <Proc> Runing_;
	while (!start.ISemty())
	{
		Start_.push_back(start.data());

		max_time += start.Arrival() + start.Burst() + 1;
		start.Delete();
	}
	while (true)
	{
		if (!Start_.empty()) {

			int c = 0;
			for (auto x : Start_)
			{
				if (x.arrival <= Time)
				{
					Ready_.push_back(x);
					c++;
				}
			}
			while (c-- and !Start_.empty())
			{
				Start_.pop_front();
			}

		}
		if (!Ready_.empty())
		{
			auto it = Ready_.begin();
			int count = 0;
			int choose_ID = dispatched(Ready_);
			for (auto x : Ready_)
			{
				if (x.procID == choose_ID)
				{
					Runing_.push_back(x);
					leave = Runing_.front().burst + Time;
					advance(it, count);
					Ready_.erase(it);
					break;

				}
				count++;

			}


		}
		while (!Runing_.empty())
		{
			increament = false;
			cout << setw(60) << setfill('-') << " \n";

			cout << "<system time " << Time << " > Pocess <" << Runing_.front().procID << ">Running \n";
			cout << setw(60) << setfill('-') << " \n";
			Runing_.front().burst -= 1;
			if (Time == leave)
			{
				cout << setw(60) << setfill('-') << " \n";

				cout << "<system time " << Time << " > Pocess <" << Runing_.front().procID << ">Leave \n";
				cout << setw(60) << setfill('-') << " \n";
				Runing_.pop_front();
				break;

			}
			for (auto x : Start_)
			{
				if (x.arrival == Time)
				{
					if (x.burst < Runing_.front().burst)
					{
						interrupt = true;
						cout << "<system time " << Time << " > Pocess <" << Runing_.front().procID << ">interrupt \n";
						cout << setw(60) << setfill('-') << " \n";
						Ready_.push_front(Runing_.front());
						Runing_.pop_front();
						break;
					}
				}
			}

			Sleep(300);
			Time++;


		}
		if (Ready_.empty() and Start_.empty())
			break;
		if (max_time == Time)
		{
			cout << " program will termined becouse we arrival to max time  press any key";
			system("pause");
			break;

		}

		if (increament) {
			Sleep(300);
			Time++;
		}
		increament = true;


	}

}
void RR(Queue start, int Quantum)
{
	//global var
	int max_time = 0;
	int leave = 0;
	int leve[4] = { 0 };
	int respon[4] = { 0 };
	bool interrupt = false;
	bool increament = true;
	// tis list is used as queues to determint stage of process
	list <Proc> Start_;
	list <Proc> Ready_;
	list <Proc> Runing_;
	while (!start.ISemty())
	{
		Start_.push_back(start.data());
		max_time += start.Arrival() + start.Burst() + 1;
		start.Delete();
	}

	while (true)
	{
		int TempQuantum = 0;
		if (!Start_.empty()) {

			int c = 0;
			for (auto x : Start_)
			{
				if (x.arrival <= Time)
				{
					Ready_.push_front(x);
					c++;
				}
			}
			while (c-- and !Start_.empty())
			{
				Start_.pop_front();
			}

		}
		if (!Ready_.empty())
		{
			Runing_.push_back(Ready_.front());
			leave = Runing_.front().burst + Time;
			respon[Runing_.front().procID-1] = Time;
			Ready_.pop_front();

		}



		while (!Runing_.empty())
		{
			increament = false;
			if (Time == leave)
			{
				cout << setw(60) << setfill('-') << " \n";

				cout << "<system time " << Time << " > Pocess <" << Runing_.front().procID << ">Leave \n";
				cout << setw(60) << setfill('-') << " \n";
				leve[Runing_.front().procID - 1] = Time;
				Runing_.pop_front();
				break;

			}
			if (TempQuantum == Quantum) {
				interrupt = true;
				cout << "<system time " << Time << " > Pocess <" << Runing_.front().procID << ">interrupt \n";
				cout << setw(60) << setfill('-') << " \n";
				Ready_.push_back(Runing_.front());
				Runing_.pop_front();
				break;
			}



			cout << setw(60) << setfill('-') << " \n";
			cout << "<system time " << Time << " > Pocess <" << Runing_.front().procID << ">Running \n";
			cout << setw(60) << setfill('-') << " \n";
			Runing_.front().burst -= 1;





			Sleep(300);
			Time++;
			TempQuantum++;


		}
		if (Ready_.empty() and Start_.empty())
			break;
		if (max_time == Time)
		{
			cout << " program will termined becouse we arrival to max time  press any key";
			system("pause");
			break;

		}

		if (increament) {
			Sleep(300);
			Time++;
		}
		increament = true;

	}
	info_SJF(leve, respon);

}
/// CPU dispatched that choose who will enter to runnig 
int dispatched(list<Proc> l)
{
	if (l.size() == 1)
	{
		return l.front().procID;
	}
	else
	{
		pair<int, int > min_burst;
		min_burst.first = l.front().burst;
		min_burst.second = l.front().procID;
		for (auto x : l)
		{
			if (x.burst < min_burst.first)
			{
				min_burst.second = x.procID;

			}
		}
		return min_burst.second;
	}
	return 0;
}
/*
*
* Waiting time = |turnaround time – burst time |
* Response time =|start time -arrival time|
*
*/
void info_SJF(int completion[4], int Response[4])
{

	cout << setw(80) << setfill('-') << " " << endl;
	cout << setw(5) << setfill(' ')
		<< "Completed Time | "
		<< "Response Time | \n";
	cout << setw(40) << setfill('-') << endl;
	for (int i = 0; i < 4; i++)
	{

		cout << setw(5) << setfill(' ')
			<< " | " << completion[i] << " | "
			<< Response[i] << " | \n";

	}
}


int main()
{
	int count = 4;
	Proc p[4];


	Queue start;
	cout << setw(120) << setfill('-') << " \n";
	cout << setw(70) << setfill(' ')
		<< " | Process scheduling  | \n";
	cout << setw(120) << setfill('-') << " ";


	int arrival;
	int burst = 0;
	for (int i = 0; i < 4; i++)
	{
		srand(time(0));
		burst = rand() % 10 + 1;
		cout << "Enter process " << i + 1 << " arrival Time:";
		cin >> arrival;
		cout << "Random process  Burst" << i + 1 << " :" << burst << endl;

		p[i].arrival = arrival;
		p[i].burst = burst;
		p[i].procID = i + 1;


		cout << setw(120) << setfill('-') << " \n";
		start.insert(p[i]);

	}
	int select;
	cout << "select :  1- FCFS\n\t 2- SJF \n\t 3-SRJ \n\t 4-RR\n";
	cin >> select;

	if (select == 2)
		SJF(start);
	else if (select == 3)
	{
		SRF(start);
	}
	else if (select == 4)
	{
		cout << "please enter Quantum vuale:";
		int Quantum = 1;
		cin >> Quantum;
		RR(start, Quantum);
	}
	else
	{
		FCFS(start);
	}
}

