/*
 * Sorting.cpp
 *
 * This program explains several sorting algorithms and its
 * functionalities.
 *
 *  Created on: Jul 7, 2014
 *      Author: ucastro
 */

#include <iostream>		//console input - output
#include "stdio.h"		//formated input library
#include <stdlib.h>		//random library
#include "vector"		//standard vector library
#include "windows.h"	//delay the execution, is OS dependent...
#include <time.h>		//clock estimations
using namespace std;

const int npos = -1;

void randomvector(vector<int> &v, int size);
void printvector(vector<int> &v);
void selectSort(vector<int> &v);
void insertSort(vector<int> &v);
void mergeSort(vector<int> &v);
void quickSort(vector<int> &v, int start=0, int stop=npos);
vector<int> copyOfvector(vector<int> &v, int index, int size);

int main() {
	cout << "This program generates a random vector and explains the differences between" << endl <<
			"the sorting algorithms." << endl << endl;
	cout << "Indicate the size of the vector" << endl;
	int size;
	scanf("%d",&size);
	vector<int> v;
	randomvector(v, size);
	cout << "This is the original array: " << endl;
	printvector(v);
	cout << endl;
	vector<int> c;
	clock_t t;

	c = v;
	t = clock();
	selectSort(c);
	t = clock() - t;
	printf("Using Selection sorting took: %f seconds.\n",((float) t)/CLOCKS_PER_SEC);

	c = v;
	t = clock();
	insertSort(c);
	t = clock() - t;
	printf("Using Insertion sorting took: %f seconds.\n",((float) t)/CLOCKS_PER_SEC);

	c = v;
	t = clock();
	mergeSort(c);
	t = clock() - t;
	printf("Using Merge sorting took: %f seconds.\n",((float) t)/CLOCKS_PER_SEC);

	c = v;
	t = clock();
	quickSort(c);
	t = clock() - t;
	printf("Using Quick sorting took: %f seconds.\n",((float) t)/CLOCKS_PER_SEC);

	cout << endl << "This is the sorted array:" << endl;
	printvector(c);

	return (0);
}

void randomvector(vector<int> &v, int size) {
	srand (time(NULL));
	for (int i=0; i<size; i++) {
		v.push_back(rand() % 1000 + 1);
	}
}

void printvector(vector<int> &v) {
	for (unsigned i=0; i<v.size(); i++)
		printf("%5d",v[i]);
	cout << endl;
}

void swap(int &i, int &j) {
	int temp = i;
	i = j;
	j = temp;
}

/* Selection Sorting:
 * This algorithm checks in the vector for the minimal value and place it in
 * the front of the vector, then it repeats for the rest of the vector until
 * the end.
 *
 * it will produce (N-1) checks each time, thus:
 * O = (n-1) + (n-2) + ... + 2 + 1 = (1/2)(n-1)n = n^2 + n
 *
 * Big O => O(n^2)
 *
 */
void selectSort(vector<int> &v) {
	for (unsigned i=0; i< v.size(); i++) {
		int min = i;
		for (unsigned j=i+1; j< v.size(); j++)
			if (v[j] < v[min])
				min = j;
		swap(v[i],v[min]);
	}
}

/* Insertion Sorting:
 * This algorithm picks each element and moves it to the front of the array
 * until the next element is smaller than the one it's moving.
 *
 * it will move each element (i-1) times, thus:
 * O = (n-1) + (n-2) + ... + 2 + 1 = (1/2)(n-1)n = n^2 + n
 *
 * Big O => O(n^2)
 *
 * NOTE:
 * In reality, some elements wont require moving, as they would end up in the right
 * place, why that produce a bit more optimal algorithm than selection sort.
 * This algorithm is optimal for vectors some-sorted.
 */
void insertSort(vector<int> &v) {
	for (unsigned i=0; i<v.size(); i++) {
		int j, curr = v[i];
		for (j=i-1; j>=0 && v[j]>curr; j--)		// <- moves each element to make space to the current,
			v[j+1] = v[j];						//    until the element is less than the current
		v[j+1] = curr;
	}
}

vector<int> copyOfvector(vector<int> &v, int index, int size) {
	vector<int> result;
	for (int i=0; i<size; i++)
		result.push_back(v[index+i]);
	return (result);
}

/* This function merge to vector in a sorted manner, taking
 * the smallest element of each of the vectors
 *
 */

vector<int> mergeVectors(vector<int> &v1, vector<int> &v2) {
	vector<int> result;
	while ( v1.size() > 0 || v2.size()>0) {
		if (v1.size()==0) {
			result.push_back(v2[0]);
			v2.erase(v2.begin());
		} else if (v2.size()==0 || v1[0] < v2[0]) {
			result.push_back(v1[0]);
			v1.erase(v1.begin());
		} else {
			result.push_back(v2[0]);
			v2.erase(v2.begin());
		}
	}
	return (result);
}


/* Merge Sorting
 * As the previous sorting algorithms turned O(n^2) big O, result that if the input
 * is doubled, will return in a process 4 times longer, ergo, if the input is cut by
 * the half, that will take a quarter the time of the original array.
 *
 * Split the array is easy, the hard part is to merge both "sorted" arrays. that is
 * resolved with a simple selection, select the first of first half or of the second
 * half, then select the next of the first or of the second, and so on.
 *
 * This algorithm divides the vector in half and each resulting vector in half until
 * the smallest case where each vector is a single element, then sortedly-merge the
 * pieces.
 *
 * This algorithm divides the vector (n-1) times, the copies takes n times as the merges
 * O = n + (n/2 + n/2) + (4 * n/4) + ... +  (2^k * n/(2^k) )   ==>   n/2^k = 1  => n=2^k
 *
 * k = log2(n)   -->  there will be k levels of divisions
 *
 * Big O => O(n log(n))  --> linear-logarithmic growth
 *
 */
void mergeSort(vector<int> &v) {
	if (v.size() > 1) {
		vector<int> v1, v2;
		v1.clear();
		v2.clear();
		int m1 = v.size() / 2;
		int m2 = v.size() - m1;
		v1 = copyOfvector(v, 0, m1);
		v2 = copyOfvector(v, m1, m2);
		mergeSort(v1);					// <-recursive call to sort the first half of the vector
		mergeSort(v2);					// <-recursive call to sort the second half of the vector
		v=mergeVectors(v1,v2);
	}
}

/*
 * Divides a vector, placing the values lowers to some arbitrary pivot to one side
 * of the array and the higher ones in the other, then place the pivot either in the
 * start or in the middle of the array.
 */
int partition(vector<int> &v, int start, int stop) {
	int lh = start+1, rh=stop;
	int pivot = v[start]; 			//Assume this value as in the middle range of the vector
	while (true) {
		while (lh < rh && v[rh] >= pivot) rh--;
		while (lh < rh && v[lh] < pivot) lh++;
		if (lh == rh) break;
		swap(v[lh], v[rh]);
		//printvector(v);
		//Sleep(1000);
		//Invariant: v[start+1..lh] < pivot  &&  v[rh..stop] > pivot
	}
	if (v[lh] >= pivot) return (start);
	swap(v[lh],v[start]);
	return (lh);
}

/* Quick Sorting
 * Takes the benefits of divide of the Merge sorting, selecting the lower part
 * and separating it form the higher part to easily joint them together.
 *
 * This algorithm takes the vector and estimates a pivot to place higher numbers in one
 * side and smaller in the other, and repeats sorting each side individually.
 *
 * Again, the array is divided subsequently and sorted each side individually:
 * O =>  T(n) = N + 2T(n/2)
 *
 * Big O(n log2(n))  --> linear-logarithmic growth
 *
 * In the case of an already sorted array, this algorithm will produce n divisions of
 * the array, as each pivot will be in the right place and there is no right side to sort
 * then it will produce:
 * O = (n-1) * n
 *
 * Big O =>  O(n^2)  --> quadratic growth (that's a lot of time)
 *
 *
 */
void quickSort(vector<int> &v, int start, int stop) {
	if (stop == npos) stop =v.size()-1;
	if (start < stop) {
		int pivot = partition(v, start, stop);
		quickSort(v, start, pivot);
		quickSort(v, pivot+1, stop);
	}
}
