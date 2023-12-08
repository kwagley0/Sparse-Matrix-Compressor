#include "compressedSparseRow.h"

// ------ CompressedSparseRow Class Implementation ------

CompressedSparseRow::CompressedSparseRow(){
    //Set the integer member variables to 0
    numNonZeros = 0;
    numRows = 0;
    numCols = 0;
    //Set the pointers to nullptr
    values = nullptr; 
    colIndices = nullptr;
    rowIndices = nullptr;
    next = nullptr;
}     

CompressedSparseRow::~CompressedSparseRow(){
    //Deallocate memory with the clearData function
    clearData();
}

CompressedSparseRow::CompressedSparseRow(const CompressedSparseRow & rhs){  
    //Copy integers from rhs to lhs
    numNonZeros = rhs.numNonZeros;
    numRows = rhs.numRows;
    numCols = rhs.numCols;
    next = nullptr;
    
    //Check if lhs is empty
    if(numNonZeros == 0){
        //Set arrays that use numNonZeros to nullptr
        values = nullptr;
        colIndices = nullptr;
    }else{
        //Create new arrays if lhs is not empty
        values = new int [rhs.numNonZeros];
        colIndices = new int [rhs.numNonZeros];

        //Copy rhs.values to values of lhs
        for(int i = 0; i < rhs.numNonZeros; i++){
            values[i] = rhs.values[i];
        }

        //Copy rhs.colIndices to colIndices of lhs
        for(int i = 0; i < rhs.numNonZeros; i++){
                    colIndices[i] = rhs.colIndices[i];
        }
    }

    //Check if the number of rows of lhs is 0
    if(numRows == 0){
        //Set array that uses numRows to nullptr
        rowIndices = nullptr;
    }else{
        //If it is not 0, create a new array
        rowIndices = new int [rhs.numRows+1];
        //Copy rhs.rowIndices to rowIndices of lhs
        for(int i = 0; i < rhs.numRows+1; i++){
            rowIndices[i] = rhs.rowIndices[i];
        }
    }
}

void CompressedSparseRow::clearData(){
    //Check if values is not nullptr
    if(values != nullptr){
        //Delete memory for the array
        delete[] values;
        //Set pointer to nullptr
        values = nullptr;
    }

    //Check if colIndices is not nullptr
    if(colIndices != nullptr){
        //Delete memory for the array
        delete[] colIndices;
        //Set pointer to nullptr
        colIndices = nullptr;
    }

    //Check if rowIndices is not nullptr
    if(rowIndices != nullptr){
        //Delete memory for the array
        delete[] rowIndices;
        //Set pointer to nullptr
        rowIndices = nullptr;
    }

    //Reset member variables to 0 and set next to nullptr
    next = nullptr;
    numNonZeros = 0;
    numRows = 0;
    numCols = 0;
}

bool CompressedSparseRow::isEmpty() const{
    //Check if values is nullptr, meaning the object is empty
    return values == nullptr;
}

void CompressedSparseRow::compressMatrix(int m, int n, int array[], int arraySize){
    //Check if rows or cols are not positive
    if(m <= 0 or n <= 0 or arraySize == 0){
        //Make the object empty
        //Set counters to 0
        numNonZeros = 0;
        numRows = 0;
        numCols = 0;
        //Set pointers to nullptr
        values = nullptr; 
        colIndices = nullptr;
        rowIndices = nullptr;
        next = nullptr;
        return;
    }

    //Update rows and cols
    numRows = m;
    numCols = n;

    //Loop through the array
    for(int i = 0; i < arraySize; i++){
        //If the number is not zero
        if(array[i] != 0){
            //Increment numNonZeros
            numNonZeros++;
        }
    }   

    //Allocate memory for the values, columns and rows arrays
    values = new int[numNonZeros]();
    colIndices = new int[numNonZeros]();
    rowIndices = new int[m+1]();

    int count = 0;
    
    //Iterate through the rows and cols of the matrix
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            //If the current element in the array is not zero
            if(i*n+j < arraySize && array[i*n+j] != 0){
                //Put the non-zero value in values
                values[count] = array[i*n+j];
                //Put the column index in colIndices
                colIndices[count] = j;
                count++;
            }
        }
        //Update row index to start a new row
        rowIndices[i+1] = count;
    }
}

int CompressedSparseRow::getElementAt(int row, int col) const{
    //Check if the row or col is out of bounds
    if(row >= numRows or col >= numCols or row < 0 or col < 0){
        //Throw a runtime_error exception
        throw runtime_error("Out of bounds");
    }

    //Find the start and end positions for the row
    int rowStart = rowIndices[row];
    int rowEnd = rowIndices[row+1];
    
    //Iterate through non-zero elements in the row
    for(int i = rowStart; i < rowEnd; i++){
        //Check if the column index equals the column given
        if(colIndices[i] == col){
            //Return value at the current position
            return values[i];
        }
    }
    
    return 0;
} 

bool CompressedSparseRow::operator==(const CompressedSparseRow & rhs) const{
    bool result = true;

    //Check if both rhs and lhs are empty
    if(rhs.values == nullptr && rhs.colIndices == nullptr && rhs.rowIndices == nullptr 
        && rhs.numRows == 0 && rhs.numCols == 0 && rhs.numNonZeros == 0 &&
        values == nullptr && colIndices == nullptr && rowIndices == nullptr  
        && numRows == 0 && numCols == 0 && numNonZeros == 0){
        return true;
    }

    //Check if numRows or numCols or numNonZeros are not equal within both rhs and lhs
    if(numRows != rhs.numRows or numCols != rhs.numCols or numNonZeros != rhs.numNonZeros){
        return false;
    }

    //Iterate through numNonZeros
    for(int i = 0; i < numNonZeros; i++){
        //Compare values of values and colIndices
        result = result && (values[i] == rhs.values[i]) && (colIndices[i] == rhs.colIndices[i]);
        //If any of the values are different, return false
        if(result == false){
            return false;
        }
    }
    
    //Iterate through numRows
    for(int i = 0; i < numRows+1; i++){
        //Compare values of rowIndices
        result = result && (rowIndices[i] == rhs.rowIndices[i]);
        //If any of the values are different, return false
        if(result == false){
            return false;
        }
    }

    //If all comparisons are true, both objects are equal
    return result;

}

int CompressedSparseRow::calculateSparseRatio(){
    //Check if matrix is empty
    if(numNonZeros == 0){
        return 0;
    }

    //Divide nonzero elements by total number of elements and multiply by 100
    //Return the sparce ratio percentage
    return (double(numRows*numCols-numNonZeros)/(numRows*numCols)) * 100;
}

void CompressedSparseRow::printData(){
    cout << endl;
    if (!isEmpty()){
        for (int i=0;i<numNonZeros;i++)
            cout << values[i] << " ";
        cout << endl;
        for (int i=0;i<numNonZeros;i++)
            cout << colIndices[i] << " ";
        cout << endl;
        for (int i=0;i<numRows+1;i++)
            cout << rowIndices[i] << " ";
    }
    else
        cout << "The object is empty!";
    cout << endl;
}

// ------ CompressedSparseRowList Class Implementation ------

CompressedSparseRowList::CompressedSparseRowList(){
    //Set head to nullptr and size to 0
    head = nullptr;
    size = 0;
}

CompressedSparseRowList::CompressedSparseRowList(const CompressedSparseRowList & rhs){
    //Set head to nullptr and set size to that of rhs
    head = nullptr;
    size = rhs.size;
    
    //Initialize pointer at head of rhs and a temp pointer to traverse the list
    CompressedSparseRow* rhs_curr = rhs.head;
    CompressedSparseRow* temp = nullptr;

    //Iterate through each CompressedSparseRow Object in rhs
    while(rhs_curr != nullptr){
        //Make a deep copy of each CompressedSparseRow Object
        CompressedSparseRow* CompressedSparseRowObject = new CompressedSparseRow(*rhs_curr);
        if(head == nullptr){
            //If the list is empty, set head pointer to the new object
            head = CompressedSparseRowObject;
            temp = head;
        }else{
            //If the list is not empty, link the new object to the list
            temp->next = CompressedSparseRowObject;
            temp = temp->next;
        }
        //Move to the next CompressedSparseRow Object in the list
        rhs_curr = rhs_curr->next;
    }
}

CompressedSparseRowList::~CompressedSparseRowList(){
    //Deallocate memory with the clearList function
    clearList();
}

bool CompressedSparseRowList::isListEmpty() const{
    //Return true if the size of the list is 0, meaning it's empty
    return size == 0;
}

void CompressedSparseRowList::insertMatrixAtHead(const CompressedSparseRow & matrix){
    //Pointer to create a new CompressedSparseRow object
    CompressedSparseRow* curr = nullptr;

    if(head == nullptr){
        //If the list is empty, make the first node a CompressedSparseRow object with the given matrix
        head = new CompressedSparseRow(matrix);
    }else{
        //If the list is not empty, make a new CompressedSparseRow object with the given matrix
        curr = new CompressedSparseRow(matrix);
        //Link the new node to head
        curr->next = head;
        //Update head to the new node
        head = curr;
    }   
    
    //Increment size of the list
    size++;
}   

void CompressedSparseRowList::clearList(){
    //Pointers to traverse the list
    CompressedSparseRow* curr = head;
    CompressedSparseRow* temp = nullptr;

    //Iterate through the list
    while(curr != nullptr){
        //Get the address of the next node
        temp = curr->next;
        //Delete the current node
        delete curr;
        //Move to the next node
        curr = temp;
    }
    
    //Set head to nullptr and size to 0
    head = nullptr;
    size = 0;
}

int CompressedSparseRowList::getMatrixElementAt(int CompressedSparseRowIndex, int row, int col) const{
    //Pointer to traverse the list
    CompressedSparseRow* curr = head;
    //Current index and value
    int index = 0;
    int value = 0;

    //Check if CompressedSparseRowIndex is out of bounds
    if (CompressedSparseRowIndex < 0 || CompressedSparseRowIndex > size){
        throw runtime_error("Out of bounds");
    }

    //Traverse list until specified CompressedSparseRowIndex
    while(curr != nullptr && index < CompressedSparseRowIndex){
        index++;
        curr = curr->next;
    }

    //Check if the index exists
    if(index < CompressedSparseRowIndex || curr == nullptr){
        throw runtime_error("The index does not exist");
    }
    
    //Get the value of the specified row and col
    value = curr->getElementAt(row, col);
    
    return value;
}

bool CompressedSparseRowList::operator== (const CompressedSparseRowList & rhs) const{
    bool result = true;

    //Return false if both lists are not the same size
    if(rhs.size != size){
        return false;
    }
    
    //Pointers to traverse lhs and rhs
    CompressedSparseRow* lhs_curr = head;
    CompressedSparseRow* rhs_curr = rhs.head;

    //Traverse through both lhs and rhs
    while(lhs_curr != nullptr){
        //Check if integer member variables are equal
        result = result && (lhs_curr->numRows == rhs_curr->numRows) && (lhs_curr->numCols == rhs_curr->numCols) 
        && (lhs_curr->numNonZeros == rhs_curr->numNonZeros);

        //Compare values of values and colIndices
        for(int i = 0; i < lhs_curr->numNonZeros; i++){
           result = result && (lhs_curr->values[i] == rhs_curr->values[i]);
           result = result && (lhs_curr->colIndices[i] == rhs_curr->colIndices[i]);
        }

        //Compare values of rowIndices
        for(int i = 0; i < lhs_curr->numRows+1; i++){
           result = result && (lhs_curr->rowIndices[i] == rhs_curr->rowIndices[i]);
        }

        //Move to next CompressedSparseRow object in both lists
        lhs_curr = lhs_curr->next;
        rhs_curr = rhs_curr->next;
    }
    
    //Return final result
    return result;
}

const CompressedSparseRowList& CompressedSparseRowList::operator=(const CompressedSparseRowList & rhs){
    //Check for self assignment 
    if(&rhs == this){
        //Return object without changing anything
        return *this;
    }

    //Clear current list to make sure everything is reset
    clearList();
    
    //Set head to nullptr and set size to that of rhs    
    head = nullptr;
    size = rhs.size;
    
    //Initialize pointer at head of rhs and a temp pointer to traverse the list
    CompressedSparseRow* rhs_curr = rhs.head;
    CompressedSparseRow* temp = nullptr;

    //Iterate through each CompressedSparseRow Object in rhs
    while(rhs_curr != nullptr){
        //Make a deep copy of each CompressedSparseRow Object
        CompressedSparseRow* CompressedSparseRowObject = new CompressedSparseRow(*rhs_curr);
        if(head == nullptr){
            //If the list is empty, set head pointer to the new object
            head = CompressedSparseRowObject;
            temp = head;
        }else{
            //If the list is not empty, link the new object to the list
            temp->next = CompressedSparseRowObject;
            temp = temp->next;
        }
        //Move to the next CompressedSparseRow Object in the list
        rhs_curr = rhs_curr->next;
    }

    //Return reference to the modified object
    return *this;
}

int CompressedSparseRowList::calculateAverageSparseRatio(){
    //Pointer to traverse the list
    CompressedSparseRow* curr = head;

    //Total sparse ratios
    int total = 0;

    //Traverse through each CompressedSparseRow object in the list
    while(curr != nullptr){
        //Add sparse ratio of current object to the total
        total += curr->calculateSparseRatio();
        //Move to next CompressedSparseRow object
        curr = curr->next;
    }

    //Return average sparse ratio by total sparse ratio by the size
    return (total/size);
}

void CompressedSparseRowList::printListData(){
    if (!isListEmpty()){
        CompressedSparseRow* temp = head;
        while (temp != nullptr){
            temp->printData();
            temp = temp->next;
        }
    }
    else
        cout << "Error: List is empty!" << endl;
}