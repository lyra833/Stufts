//
//  sort3.h
//  Header File for In Place Merge Sort class
//

#ifndef __Sorting__Sort2__
#define __Sorting__Sort2__

#include <iostream>

struct arrayWithLength{
        int length;
        int *arr;
};

class InPlaceMergeSort {
public:
        InPlaceMergeSort(); // constructor
	~InPlaceMergeSort();

        // read in a list of values from stdin
        void readList();
        
        // sort function
        void sort();
	void sort(int from, int to);
	void merge(int from, int midpoint, int to);
	void reverse(int from, int to);
        
        // print the array
        void printArray();
        
private:
        arrayWithLength sortArray;
};

#endif

