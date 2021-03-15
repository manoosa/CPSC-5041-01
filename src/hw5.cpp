#include <iostream>
#include <string>
#include <list>
#include <bits/stdc++.h>
#include <queue>
#include <iomanip>
using namespace std;

class Calculations {
	double idle, totalBurstTime, worstWaitingTime, totalWaitingtime;
public:
	Calculations() {
	}
	void setIdle(int);
	void setWorst(int);
	void setTotalWaiting(int);
	void setTotalBurstTime(int);
	void finalCal(int);
};

void Calculations::setIdle(int t) {
	cout << "Time " << t << " idle" << endl;
	this->idle += t;
}

void Calculations::setWorst(int t) {
	if (t > worstWaitingTime) {
		this->worstWaitingTime = t;
	}
}

void Calculations::setTotalWaiting(int t) {
	this->totalWaitingtime += t;
}

void Calculations::setTotalBurstTime(int t) {
	this->totalBurstTime += t;
}

void Calculations::finalCal(int total) {
	cout << "Worst waiting time: " << worstWaitingTime << endl;
	cout << "Average waiting time: " << setprecision(2) << fixed
			<< totalWaitingtime / total << endl;
	cout << "CPU Utilization: "
			<< round((totalBurstTime / (totalBurstTime + idle)) * 100) << "%"
			<< endl;
}

class Process {
	int processId, arrivalTime, burstTime, terminateTime, priority, turnAround,
			waitingTime, remainingBurstTime, lastStart;
	Calculations *cal;
public:
	Process(int, int, int, int, Calculations*);
	int getBurstTime() {
		return burstTime;
	}
	int getRemainingBurstTime() {
		return remainingBurstTime;
	}
	int getProcessId() {
		return processId;
	}
	int getArrivalTime() {
		return arrivalTime;
	}
	int getPriority() {
		return priority;
	}
	void start(int);
	void end(int);
	void terminate(int);
	bool isFinished(int);
	bool operator ==(const Process &p) const {
		return processId == p.processId;
	}
	bool operator !=(const Process &p) const {
		return !operator==(p);
	}
};

Process::Process(int processId, int arrivalTime, int burstTime, int priority,
		Calculations *cal) {
	this->processId = processId;
	this->arrivalTime = arrivalTime;
	this->burstTime = burstTime;
	this->priority = priority;
	this->remainingBurstTime = burstTime;
	this->cal = cal;
}

void Process::end(int t) {

	if (this->isFinished(t)) {
		this->remainingBurstTime -= (t - lastStart);
		this->terminate(t);
		return;
	}

	this->remainingBurstTime -= (t - lastStart);

}

void Process::terminate(int t) {
	terminateTime = t;
	turnAround = terminateTime - arrivalTime;
	waitingTime = turnAround - burstTime;
	cal->setTotalWaiting(waitingTime);
	cal->setWorst(waitingTime);
	cal->setTotalBurstTime(burstTime);
}

void Process::start(int t) {
	cout << "Time " << t << " Process " << processId << endl;
	lastStart = t;
}

bool Process::isFinished(int t) {
	return this->remainingBurstTime - (t - lastStart) == 0;
}

/////    Compare /////////////
bool compareByBurstTime(Process *p1, Process *p2) {
	if (p1->getBurstTime() == p2->getBurstTime()) {
		return p1->getProcessId() < p2->getProcessId();
	}
	return p1->getRemainingBurstTime() < p2->getRemainingBurstTime();
}

bool compareByPriority(Process *p1, Process *p2) {
	if (p1->getPriority() == p2->getPriority()) {
		return p1->getProcessId() < p2->getProcessId();
	}
	return p1->getPriority() < p2->getPriority();
}

bool compareByArrivalTime(Process *p1, Process *p2) {
	return p1->getArrivalTime() < p2->getArrivalTime();
}

///////// Pool class ////////
class Pool {
	bool (*comparetor)(Process*, Process*);
	list<Process*> readyProcesses;
public:
	Pool(bool (*comparetor)(Process*, Process*));
	Process* get();
	bool isEmpty();
	void removeProcess(Process*);
	void add(Process*);
};

Pool::Pool(bool (*comparetor)(Process*, Process*)) {
	this->comparetor = comparetor;
}

bool Pool::isEmpty() {
	return readyProcesses.empty();
}

Process* Pool::get() {
	readyProcesses.sort(comparetor);
	return readyProcesses.front();
}

void Pool::removeProcess(Process *process) {
	readyProcesses.remove(process);
}

void Pool::add(Process *process) {
	readyProcesses.push_back(process);
}

////// simulator ////////
Process* calculateAndStartCurrent(Process*, Pool&, int,
		bool (*comparetor)(Process*, Process*));

class Simulator {
	std::queue<Process*> queue;
public:
	Simulator(list<Process*>);
	void nonPreemptiveAlgo(bool (*comparetor)(Process*, Process*),
			Calculations*);
	void preemptiveAlgo(bool (*comparetor)(Process*, Process*), Calculations*);
	void sjf(Calculations*);
	void np(Calculations*);
	void srtf(Calculations*);
	void pp(Calculations*);
};

Simulator::Simulator(list<Process*> listofProcesses) {
	std::list<Process*>::iterator it;

	for (it = listofProcesses.begin(); it != listofProcesses.end(); ++it) {
		queue.push(*it);
	}
}

void Simulator::nonPreemptiveAlgo(bool (*comparetor)(Process*, Process*),
		Calculations *cal) {
	int t = 0;
	Pool pool = Pool(comparetor);

	while (!queue.empty() || !pool.isEmpty()) {

		while (!queue.empty() && queue.front()->getArrivalTime() <= t) {
			pool.add(queue.front());
			queue.pop();
		}

		if (pool.isEmpty()) {
			cal->setIdle(t);
			t = queue.front()->getArrivalTime();
		} else {
			Process *process = pool.get();
			process->start(t);
			t += process->getBurstTime();
			process->terminate(t);
			pool.removeProcess(process);
		}
	}
	cal->finalCal(6);
}

void Simulator::preemptiveAlgo(bool (*comparetor)(Process*, Process*),
		Calculations *cal) {
	Process *currentProcess = nullptr;

	int t = -1;
	Pool pool = Pool(comparetor);

	while (!queue.empty() || !pool.isEmpty() || currentProcess != nullptr) {

		if (currentProcess == nullptr && t >= 1) {
			cal->setIdle(t - 1);
		}

		bool newProcess = false;
		t++;
		while (!queue.empty() && queue.front()->getArrivalTime() <= t) {
			pool.add(queue.front());
			queue.pop();
			newProcess = true;
		}

		if (currentProcess != nullptr && currentProcess->isFinished(t)) {
			currentProcess->end(t);
			currentProcess = nullptr;
		}

		if (newProcess || currentProcess == nullptr) {
			currentProcess = calculateAndStartCurrent(currentProcess, pool, t,
					comparetor);
		}

	}

	if (currentProcess != nullptr) {
		currentProcess->end(t);
	}
	cal->finalCal(4);
}

Process* calculateAndStartCurrent(Process *currentProcess, Pool &pool, int t,
		bool (*comparetor)(Process*, Process*)) {
	if (pool.isEmpty()) {
		return currentProcess;
	}

	Process *newProcess = currentProcess;
	Process *process = pool.get();

	if (currentProcess == nullptr || !comparetor(currentProcess, process)) {
		newProcess = process;

		if (currentProcess != nullptr) {
			currentProcess->end(t);
			pool.add(currentProcess);
		}

		newProcess->start(t);
		pool.removeProcess(newProcess);
	}

	return newProcess;
}

void Simulator::sjf(Calculations *cal) {
	this->nonPreemptiveAlgo(compareByBurstTime, cal);
}

void Simulator::np(Calculations *cal) {
	this->nonPreemptiveAlgo(compareByPriority, cal);
}

void Simulator::srtf(Calculations *cal) {
	this->preemptiveAlgo(compareByBurstTime, cal);
}

void Simulator::pp(Calculations *cal) {
	this->preemptiveAlgo(compareByPriority, cal);
}

int main(int argc, char *argv[]) {

	// check args size and print you must enter ...
	char *methodName = argv[1];
	char *fileName = argv[2];

	cout << methodName << endl;
	Calculations *cal = new Calculations();
	list<Process*> listofProcesses = { new Process(1, 0, 7, 3, cal),
			new Process(2, 2, 4, 3, cal), new Process(3, 4, 1, 5, cal),
			new Process(4, 5, 3, 2, cal), };

//   list<Process*> listofProcesses = { new Process (0,0,5,3,cal),
//                                     new Process (1,10,20,3,cal),
//                                     new Process (2,10,10,5,cal),
//                                     new Process (3,15,3,2,cal),
//                                     new Process (4,20,30,1,cal),
//                                     new Process (5,25,5,2,cal),
//   };

	listofProcesses.sort(compareByArrivalTime);
	Simulator simulator = Simulator(listofProcesses);
	//simulator.sjf(cal);
	simulator.srtf(cal);
	return 0;
}

