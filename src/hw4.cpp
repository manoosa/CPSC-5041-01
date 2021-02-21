// AUTHOR: Marwa Saleh
// ASSIGMENT: Homework 4
// DATE: Winter 5041 2021
// PURPOSE: This program is used to allow the user to enter name-score pairs,
//			sort the scores in ascending order and calculates the average score.
// OUTPUT:  Prints scores in an ascending order with student name and print average of scores.
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

// Function:    sort array
// Purpose:     sort scores with student names in an ascending order
// Parameters:  scores array, student names array, number of test scores
// Returns:     None
void sortArray(int*, string*, int);

// Function:    calculate average
// Purpose:     sort scores with student names in an ascending order
// Parameters:  total number of scores, number of test scores
// Returns:     None
void calculateAvg(int, int);

// Function:    display list
// Purpose:     display scores with student names
// Parameters:  scores array, student names array, number of test scores
// Returns:     None
void displayList(int*, string*, int);

// Function:    sort scores in an ascending order
// Purpose:     sort scores with student names in an ascending order
// Parameters:  scores array, student names array, start index, end index
// Returns:     None
void sort(int*, string*, int, int);

// Function:    swap scores
// Purpose:     swap scores from an index to another
// Parameters:  scores array, index1, index2
// Returns:     None
void swapScores(int*, int, int);

// Function:    swap student names
// Purpose:     swap student names from an index to another
// Parameters:  student names array, index1, index2
// Returns:     None
void swapStudentNames(string*, int, int);

int main() {
	int numScores;
	int total = 0;
	cout << "How many test scores will you enter? ";
	cin >> numScores;

	//Dynamically allocate an array to hold test scores and another one for holding student names
	int *scores = new int[numScores];
	string *studentNames = new string[numScores];

	for (int i = 0; i < numScores; i++) {
		cout << "Enter student " << (i + 1) << "’s last name:";
		cin >> studentNames[i];
		cout << "Enter that student’s test score: ";
		cin >> scores[i];

		while (scores[i] > 100 || scores[i] < 0) {
			cout
					<< "Please try again, this time no negative scores and no score greater than 100: ";
			cin >> scores[i];
		}
	}

	//sort the elements of the array pointers
	sortArray(scores, studentNames, numScores);
	displayList(scores, studentNames, numScores);

	//Calculate the total scores
	for (int num = 0; num < numScores; num++) {
		total += scores[num];
	}

	//Calculate the average scores
	calculateAvg(total, numScores);

	//Free memory.
	delete[] scores;
	delete[] studentNames;
	return 0;
}

void sortArray(int *scores, string *names, int length) {
	int low = 0;
	int high = length - 1;
	sort(scores, names, low, high);
}

void sort(int *scores, string *names, int low, int high) {
	if (low < high) {
		int i = low - 1;
		int pivot = scores[high];

		for (int j = low; j < high; j++) {
			if (scores[j] < pivot) {
				i++;
				swapScores(scores, i, j);
				swapStudentNames(names, i, j);
			}
		}

		i += 1;
		swapScores(scores, i, high);
		swapStudentNames(names, i, high);
		sort(scores, names, i + 1, high);
		sort(scores, names, low, i - 1);
	}
}

void swap(int *arr, int i, int j) {
	int temp = arr[j];
	arr[j] = arr[i];
	arr[i] = temp;
}

void swapString(string *arr, int i, int j) {
	string temp = arr[j];
	arr[j] = arr[i];
	arr[i] = temp;
}

void calculateAvg(int total, int numScores) {
	double average = (double) total / numScores;
	cout << endl << "Average Score: " << setprecision(2) << fixed << average
			<< endl;
}

void displayList(int *scores, string *names, int length) {
	cout << endl
			<< "The test scores in ascending order, and their average, are:"
			<< endl;
	cout << "Name" << setw(10) << "Score" << endl;

	for (int i = 0; i < length; i++) {
		cout << names[i] << setw(10) << scores[i] << endl;
	}
}
