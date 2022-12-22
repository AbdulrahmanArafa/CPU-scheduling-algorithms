 #pragma once
#pragma once

struct Proc {
	int procID;
	int arrival;
	int burst;
};

struct Node {
	Proc Data;
	Node* Next;
};

class Queue
{
private:
	Node* front = nullptr;
	Node* rear = nullptr;
	Node* temp = nullptr;

public:
	// incert  return 1 if run correctly else 0
	bool insert(Proc val);
	// delete 
	bool Delete();
	bool ISemty();
	Proc data();
	int ID();
	int Arrival();
	int Burst();




};

bool Queue::insert(Proc val)
{
	Node* newnode = new Node;

	newnode->Data = val;
	newnode->Next = nullptr;
	if (front == nullptr)
	{

		front = rear = newnode;
		rear->Next = nullptr;


	}
	else
	{
		rear->Next = newnode;
		rear = newnode;
		rear->Next = nullptr;

	}
	return 1;
}
inline bool Queue::Delete()
{

	Proc item;
	if (front == nullptr)
		return 0;
	else
	{
		temp = front;

		item = temp->Data;

		front = front->Next;
		delete(temp);

	}
	return 1;
}

inline bool Queue::ISemty()
{
	if (front == nullptr)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
inline Proc Queue::data()
{
	return front->Data;
}
inline int Queue::ID()
{
	return front->Data.procID;
}
inline int Queue::Arrival()
{
	return front->Data.arrival;
}
inline int Queue::Burst()
{
	return front->Data.burst;
}