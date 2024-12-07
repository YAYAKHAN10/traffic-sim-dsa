#include <iostream>
#include <fstream>
#include <chrono>
using namespace std;

class Stack
{
    struct Node
    {
        string data;
        Node *next;
    };
    Node *top;
public:
    Stack() : top(NULL) {}

    void push(string data)
    {
        Node *newNode = new Node;
        newNode->data = data;
        newNode->next = top;
        top = newNode;
    }

    string pop()
    {
        if (top == NULL)
        {
            return "";
        }

        Node *temp = top;
        string data = top->data;
        top = top->next;
        delete temp;
        return data;
    }

    bool isEmpty()
    {
        return top == NULL;
    }
};
