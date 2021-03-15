#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <math.h>
#include <queue>
#include <iomanip>
#include <vector>
#include <sstream>
using namespace std;

class Calculations {
	double idle, totalBurstTime, worstWaitingTime, totalWaitingtime;
public:
	Calculations() {
	}
	void setIdle(int, int);
	void setWorst(int);
	void setTotalWaiting(int);
	void setTotalBurstTime(int);
	void finalCal(int);
};

void Calculations::setIdle(int t, int period) {
	if (this->idle == 0) {
		cout << "Time " << t << " idle" << endl;
	}
	this->idle = this->idle + period;
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
	cout << "Worst-case waiting time: " << worstWaitingTime << endl;
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
private:
	void init(int, int, int, int, Calculations*);

public:
	Process(int, int, int, int, Calculations*);
	Process(string, Calculations*);
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
	this->init(processId, arrivalTime, burstTime, priority, cal);
}

Process::Process(string str, Calculations *cal) {

	istringstream ss(str);

	string word; // for storing each word
	ss >> word;
	int processId = std::stoi(word);
	ss >> word;
	int arrivalTime = std::stoi(word);
	ss >> word;
	int burstTime = std::stoi(word);
	ss >> word;
	int priority = std::stoi(word);
	this->init(processId, arrivalTime, burstTime, priority, cal);
}

void Process::init(int processId, int arrivalTime, int burstTime, int priority,
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
		this->remainingBurstTime -= (t + 1 - lastStart);
		this->terminate(t);
		return;
	}

	this->remainingBurstTime -= (t + 1 - lastStart);

}

void Process::terminate(int t) {
	terminateTime = t + 1;
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
	return this->remainingBurstTime - (t + 1 - lastStart) == 0;
}

/////    Compare /////////////
bool compareByBurstTime(Process *p1, Process *p2) {
	if (p1->getRemainingBurstTime() == p2->getRemainingBurstTime()) {
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

Process* calculateAndStartCurrent(Process*, Pool&, int,
		bool (*comparetor)(Process*, Process*));

////// simulator ////////
class Simulator {
	std::queue<Process*> queue;
public:
	Simulator(list<Process*>);
	void schedulingAlgorithm(string, Calculations*);
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

void Simulator::schedulingAlgorithm(string schedulingAlgorithmName,
		Calculations *cal) {
	if (schedulingAlgorithmName.compare("SJF") == 0) {
		this->sjf(cal);
	} else if (schedulingAlgorithmName.compare("NP") == 0) {
		this->np(cal);
	} else if (schedulingAlgorithmName.compare("SRTF") == 0) {
		this->srtf(cal);
	} else if (schedulingAlgorithmName.compare("PP") == 0) {
		this->pp(cal);
	} else {
		cout << "invalid Scheduling Algorithm : " << schedulingAlgorithmName;
		return;
	}
}

void Simulator::nonPreemptiveAlgo(bool (*comparetor)(Process*, Process*),
		Calculations *cal) {
	int t = 0;
	Pool pool = Pool(comparetor);
	int queueSize = queue.size();
	while (!queue.empty() || !pool.isEmpty()) {

		while (!queue.empty() && queue.front()->getArrivalTime() <= t) {
			pool.add(queue.front());
			queue.pop();
		}

		if (pool.isEmpty()) {
			cal->setIdle(t, t);
			t = queue.front()->getArrivalTime();
		} else {
			Process *process = pool.get();
			process->start(t);
			t += process->getBurstTime();
			process->terminate(t - 1);
			pool.removeProcess(process);
		}
	}
	cal->finalCal(queueSize);
}

void Simulator::preemptiveAlgo(bool (*comparetor)(Process*, Process*),
		Calculations *cal) {
	Process *currentProcess = nullptr;

	int t = -1;
	Pool pool = Pool(comparetor);
	int queueSize = queue.size();
	while (!queue.empty() || !pool.isEmpty() || currentProcess != nullptr) {

		if (currentProcess == nullptr && t != -1) {
			cal->setIdle(t, 1);
		}

		bool newProcess = false;

		while (!queue.empty() && queue.front()->getArrivalTime() <= t + 1) {
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
		t++;
	}

	if (currentProcess != nullptr) {
		currentProcess->end(t);
	}
	cal->finalCal(queueSize);
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

		newProcess->start(t + 1);
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

list<Process*> readFile(string inputFile, Calculations *cal) {
	ifstream myfile(inputFile);
	string line;
	list<Process*> list;

	if (!myfile.is_open()) {
		return list;
	}
	while (getline(myfile, line)) {
		Process *process = new Process(line, cal);
		list.push_back(process);
	}
	myfile.close();

	return list;
}

int main(int argc, char *argv[]) {
	if (argc != 4) {
		cout << "Invalid input" << endl;
		return 0;
	}

	Calculations *cal = new Calculations();
	string inputFile = argv[2];
	string schedulingAlgorithm = argv[3];

	list<Process*> listofProcesses = readFile(inputFile, cal);
	if (listofProcesses.empty()) {
		cout << "Can't read from file";
		return 0;
	}

	listofProcesses.sort(compareByArrivalTime);
	Simulator simulator = Simulator(listofProcesses);
	simulator.schedulingAlgorithm(schedulingAlgorithm, cal);
	return 0;
}

