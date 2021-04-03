#include <iostream> 
#include <list> 
using namespace std; 
  
// Function to print the list 
void printList(list<int> mylist) 
{ 
  
    // Get the iterator 
    list<int>::iterator it; 
  
    // printing all the elements of the list 
    for (it = mylist.begin(); it != mylist.end(); ++it) 
        cout << ' ' << *it; 
    cout << '\n'; 
} 
  
int main() 
{ 
    // Create a list with the help of constructor 
    // This will insert 100 10 times in the list 
    list<int> myList(10, 100); 
  
    printList(myList); 
  
    return 0; 
} 