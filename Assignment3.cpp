// Programming Assignment 3
//

#include <iostream>
#include <vector>
#include <ctime>
#include <string>

using namespace std;
using std::vector;

//constant for frame size and our vector lists 
int FRAME_SIZE = 1;
vector<int> freeFrameList;
vector<int> processList;

// a data structure to help read/write and map to logical address
struct Process {
    int frameNumber;
    int data;
};

void printProcessList();
// creates a freeFrame List
void memoryManager(int memSize, int frameSize) {
    freeFrameList.push_back(1);
    freeFrameList.clear();
    for (int i = 0; i < memSize; ++i) {
        freeFrameList.push_back(i + 1);
    }
}

// prints the contents of the FreeFrameList and Processes
void printMemory() {
    cout << "Below are the free frames in the FreeFrameList" << endl;
    for (int i = 0; i < freeFrameList.size(); ++i) {
        cout << freeFrameList.at(i) << " ";
    }
    cout << endl;

    cout << "Below is the process pid followed by number of pages" << endl;
    printProcessList();
    cout << endl;
}

// a node for our processList
class Node {
public:
    int pid;
    vector<Process> processList;
    Node* next;
};

//push new process "node"
void push(Node** head_ref, vector<Process> new_data, int pid) {
    Node* new_node = new Node();

    new_node->processList = new_data;
    new_node->pid = pid;

    new_node->next = (*head_ref);

    (*head_ref) = new_node;
}
//head of linked list
Node* head = NULL;
//helper function to output contents of processes
void printProcessList() {
    Node* current;

    current = head;

    while (current != nullptr)
    {
        cout << current->pid << " " << current->processList.size() << " ";
        current = current->next;
    }
}

//allocate function to use up free frames and allocate to processes
int allocate(int allocSize, int pid) {
    int index = 0;
    int random = 0;
    vector<int> it;
    vector<Process> newData;
    Process temp{};
    if (allocSize > freeFrameList.size()) {
        return -1;
    }

    else {
        for (int i = 0; i < allocSize; ++i) {
            srand((unsigned int)time(NULL));
            random = rand();
            index = random % freeFrameList.size();
            temp.frameNumber = freeFrameList[index];
            temp.data = 0;
            newData.push_back(temp);
            freeFrameList.erase(freeFrameList.begin() + index);
        }

        push(&head, newData, pid);

        return 1;
    }
}
// deallocate a process and return to free frames
int deallocate(int pid) {

    Node* current;

    current = head;

    while (current != nullptr)
    {
        if (current->pid == pid) {
            int n = current->processList.size() - 1;
            int temp = current->processList[n].frameNumber;
            freeFrameList.push_back(temp);
            current->processList.erase(current->processList.begin() + n);
            return 1;
        }
    }

    return -1;
}
// write a 1 to a specified page
int write(int pid, int logical_address) {
    Node* current;

    current = head;

    while (current != nullptr)
    {
        if (current->pid == pid) {
            current->processList[logical_address].data = 1;
            return 1;
        }
    }

    return -1;
}
// read the contents of a specified page
int read(int pid, int logical_address) {
    Node* current;

    current = head;

    while (current != nullptr)
    {
        if (current->pid == pid) {
            cout<< "The data located in memory location specified by page " << logical_address << " is: " 
                << current->processList[logical_address].data << endl;
            return 1;
        }
    }

    return -1;
}

// main driver
// reads in user input and continues until user is finished
int main() {
    string firstCommand, secondCommand, thirdCommand;
    int size, address, pid;

    while (firstCommand != "exit") {
        cout << "Enter command or type 'exit' to terminate." << endl;
        cin >> firstCommand;

        if (firstCommand == "M") {
            cin >>secondCommand >> thirdCommand;
            size = stoi(secondCommand);
            memoryManager(size, FRAME_SIZE);
        }

        else if (firstCommand == "A") {
            cin >> secondCommand >> thirdCommand;
            size = stoi(secondCommand);
            pid = stoi(thirdCommand);

            if (allocate(size, pid) < 0) {
                cout << "Error: the number of pages could not be allocated" << endl;
            }
        }

        else if (firstCommand == "W") {
            cin >> secondCommand >> thirdCommand;
            pid = stoi(secondCommand);
            address = stoi(thirdCommand);

            if (write(pid, address) < 0) {
                cout << "Error: Could not find memory location at specified PID" << endl;
            }
        }

        else if (firstCommand == "R") {
            cin >> secondCommand >> thirdCommand;
            pid = stoi(secondCommand);
            address = stoi(thirdCommand);

            if (read(pid, address) < 0) {
                cout << "Error: Could not find memory location at specified PID" << endl;
            }
        }

        else if (firstCommand == "D") {
            cin >> secondCommand;
            pid = stoi(secondCommand);

            if (deallocate(pid) < 0) {
                cout << "Could not find process with specified PID" << endl;
            }
        }

        else if (firstCommand == "P") {
            printMemory();
        }

        else {
            if (firstCommand == "exit") {
                break;
            }
            cout << "invalid input try again" << endl;
        }
    }
    return 0;
}