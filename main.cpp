/* @Author
* Student Name: <MELÝH PEKMEZ>
* Student ID : <040140306>
* Date: <4.12.2017>
*/
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#define RED 1
#define YELLOW 2
#define GREEN 3

using namespace std;

struct patient {
	char* name;
	int arr_time;
	int	tre_time;
	char* complaint;
};

struct Queue {
	patient pa[100];
	int front;
	int back;
	int counter;
	void create();
	void enqueue(patient newdata);
	bool isEmpty();
	patient dequeue();
};

void Queue::create()
{
	front = 0;
	back = 0;
	counter = 0;
}

void Queue::enqueue(patient newdata)
{
	pa[back++] = newdata;
	counter++;
}

patient Queue::dequeue()
{
	counter--;
	return pa[front++];
}

bool Queue::isEmpty()
{
	return(counter == 0);
}

bool checkTretime(patient p) /*Tedavi süresi bitmiþ mi kontrol ediyorum*/
{
	return (p.tre_time != 0);
}

void printTime(patient p,int time) /*Zamanlarýný yazdýrýyorum*/
{
	cout << time+1 << "   " << p.name << "   " <<endl;
}

void checkPatient(patient p[],int time,Queue *qRed,Queue *qYellow,Queue *qGreen,int patientNumber)/*Þikayet önceliðine göre Queue'ya alýyorum.*/
{

	for (int i = 0; i < patientNumber-1; i++)
	{
		if (time == p[i].arr_time)
		{
			if (p[i].complaint == "RED")
			{
				qRed->enqueue(p[i]);
			}
			else if (p[i].complaint == "YELLOW")
			{
				qYellow->enqueue(p[i]);
			}
			else
			{
				qGreen->enqueue(p[i]);
			}
		}
	}
}

int main()
{
	int time = 0;
	Queue qRed, qYellow, qGreen;
	qRed.create();
	qYellow.create();
	qGreen.create();
	int counter = 0;
	int patientNumber = 0;
	string word;
	int x;
	int y;
	string word3;
	ifstream check("patientsInfo.txt");/*Kaç hasta var onun sayýsýný çýkarýyorum*/
	ifstream Do("patientsInfo.txt");/*Hastalarýn verilerini çekiyorum*/

	if (!check)
	{
		cout << "There was a problem opening the file. Press any key to close.\n";
		getchar();
		return 0;
	}
	while (check)
	{
		check >> word;
		if (word == "name" || word == "arriving" || word == "time" || word == "complaint" || word == "treatment")
		{
			continue;
		}
		check >> x >> y >> word3;
		patientNumber++;
	}
	patient* p = new patient[patientNumber];
	if (!Do)
	{
		cout << "There was a problem opening the file. Press any key to close.\n";
		getchar();
		return 0;
	}
	while (Do)
	{
		Do >> word;
		if (word == "name" || word == "arriving" || word == "time" || word == "complaint" || word == "treatment")
		{
			continue;
		}
		Do >> x >> y >> word3;
		
		p[counter].name = new char[word.length()+1];
		for (size_t i=0; i < word.length()+1; i++)
		{
			p[counter].name[i] = word[i];
		}
		p[counter].complaint = new char[word.length()+1];
		for (size_t i=0; i < word3.length()+1; i++)
		{
			p[counter].complaint[i] = word3[i];
		}

		p[counter].arr_time = x;
		p[counter].tre_time = y;
		counter++;
	}

	checkPatient(p, time, &qRed, &qYellow, &qGreen,counter); /*Yeni gelen hasta var mý diye bakýyorum*/
	

	while (!qRed.isEmpty() || !qYellow.isEmpty() || !qGreen.isEmpty()) /*Öncelik sýrasýna göre hastalara bakýyorum*/
	{
		while(!qRed.isEmpty())
		{
			patient p1 = qRed.dequeue();
			printTime(p1, time);
			time++;
			checkPatient(p, time, &qRed, &qYellow, &qGreen,counter);
			p1.complaint = "YELLOW";
			p1.tre_time--;
			if (checkTretime(p1)) qYellow.enqueue(p1);
		}
		while (!qYellow.isEmpty())
		{
			patient p1 = qYellow.dequeue();
			for (int i = 0; i < YELLOW; i++)
			{
				printTime(p1, time);
				time++;
				checkPatient(p, time, &qRed, &qYellow, &qGreen,counter);
				p1.tre_time--;
				if (!checkTretime(p1)) break;
			}
			p1.complaint = "GREEN";
			if (checkTretime(p1)) qGreen.enqueue(p1);
			if (!qRed.isEmpty()) break;
		}
		while (!qGreen.isEmpty())
		{
			if (!qRed.isEmpty()) break;
			patient p1 = qGreen.dequeue();
			for (int i = 0; i < GREEN; i++)
			{
				printTime(p1, time);
				time++;
				checkPatient(p, time, &qRed, &qYellow, &qGreen,counter);
				p1.tre_time--;
				if (!checkTretime(p1)) break;
			}
			if (checkTretime(p1)) qGreen.enqueue(p1);
			if (!qRed.isEmpty() || !qYellow.isEmpty()) break;
		}
	}

	for (int i = 0; i < counter-1; i++) /*Hafýzayý geri veriyorum*/
	{
		delete p[i].name;
		delete p[i].complaint;
	}

	delete[] p;

	getchar();
	getchar();
	
}

