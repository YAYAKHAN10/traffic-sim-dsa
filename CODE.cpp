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


class CSVReader
{
public:
    int calculateRows(string filename)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            cout << filename << endl;
            cout << "1. Error opening file!" << endl;
            return 0;
        }

        int rowCount = 0;
        string line;

        while (getline(file, line))
        {
            // Only count non-empty rows
            if (!line.empty())
            {
                rowCount++;
            }
        }

        file.close();
        return rowCount;
    }

    void readCSV(string filename, string **&data, int cols)
    {
        int rows = calculateRows(filename);
        ifstream file(filename);
        if (!file.is_open())
        {
            cout << filename << endl;
            cout << "2. Error opening file!" << endl;
            return;
        }

        // Allocate memory for data array
        data = new string *[rows];
        for (int i = 0; i < rows; ++i)
        {
            data[i] = new string[cols];
        }

        int rowIndex = 0;
        string line;

        while (getline(file, line) && rowIndex < rows)
        {
            int colIndex = 0;
            string temp = "";
            for (int i = 0; i <= line.length(); i++)
            {
                if (i == line.length() || line[i] == ',')
                {
                    if (colIndex < cols)
                    {
                        data[rowIndex][colIndex] = temp;
                    }
                    temp = "";
                    colIndex++;
                }
                else
                {
                    if (line[i] >= '0' && line[i] <= '9' || line[i] >= 'A' && line[i] <= 'Z' || line[i] >= 'a' && line[i] <= 'z')
                    {
                        temp += line[i];
                    }
                }
            }
            rowIndex++;
        }

        file.close();
    }
};

class Timer
{
public:
    chrono::steady_clock::time_point start_time;

    void start()
    {
        start_time = chrono::steady_clock::now();
    }

    bool isTimeout(int seconds)
    {
        chrono::steady_clock::time_point end_time = chrono::steady_clock::now();
        chrono::duration<double> elapsed_seconds = chrono::duration_cast<chrono::duration<double> >(end_time - start_time);

        cout<<elapsed_seconds.count()<<endl;

        return elapsed_seconds.count() > seconds;
    }
};
