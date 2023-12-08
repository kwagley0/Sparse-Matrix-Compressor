#ifndef CompressedSparseRow_H
#define CompressedSparseRow_H

#include <iostream>
#include <string>
#include <stdexcept>

using std::runtime_error;
using namespace std;

// CompressedSparseRow class represents a matrix
class CompressedSparseRow {
public:
    friend class CompressedSparseRowList;

    // Constructors and Destructor
    CompressedSparseRow();
    ~CompressedSparseRow();
    CompressedSparseRow(const CompressedSparseRow &rhs);

    // Member Functions
    void compressMatrix(int m, int n, int array[], int arraySize);
    int getElementAt(int row, int col) const;
    int calculateSparseRatio();
    bool operator==(const CompressedSparseRow &rhs) const;
    void clearData();
    bool isEmpty() const;
    void printData(); // prints data for the node

private:
    // Member Variables
    int* values;       // array to store non-zero values
    int* colIndices;   // array to store column indices
    int* rowIndices;   // array to store row indices
    int numNonZeros;   // number of non-zero values
    int numRows;       // number of rows
    int numCols;       // number of columns
    CompressedSparseRow* next; // pointer to the next CompressedSparseRow object in the linked list
};

// CompressedSparseRowList class represents a list of matrices (CompressedSparseRow objects)
class CompressedSparseRowList {
public:

    // Constructors and Destructor
    CompressedSparseRowList();
    ~CompressedSparseRowList();
    CompressedSparseRowList(const CompressedSparseRowList &rhs);

    // Member Functions
    void insertMatrixAtHead(const CompressedSparseRow &matrix);
    const CompressedSparseRowList& operator=(const CompressedSparseRowList &rhs);
    bool operator==(const CompressedSparseRowList &rhs) const;
    int getMatrixElementAt(int CompressedSparseRowIndex, int row, int col) const;
    int calculateAverageSparseRatio();
    void clearList();
    bool isListEmpty() const;
    void printListData(); // prints data for all nodes

private:
    // Member Variables
    CompressedSparseRow* head; // the pointer to the head of the list
    int size;                  // the current number of nodes in the list
};

#endif
