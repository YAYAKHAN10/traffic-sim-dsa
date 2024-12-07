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

class roadNetwork
{
    // store the list of destinations and time from the start point
    struct Node
    {
        string end;
        int time;
        string status;
        Node *next;
    };

    // store the start point and the pointing to the list of destinations
    struct AdjacencyList
    {
        string start;
        Node *head;
        AdjacencyList *next;
    };
 void addEdge(const string &start, const string &end, int time)
    {
        AdjacencyList *startNode = graph;
        AdjacencyList *prevStartNode = NULL;

        while (startNode != NULL && startNode->start != start)
        {
            prevStartNode = startNode;
            startNode = startNode->next;
        }

        // If the start point is not in the graph, add it
        if (!startNode)
        {
            startNode = new AdjacencyList;
            startNode->start = start;
            startNode->head = NULL;
            startNode->next = NULL;

            if (prevStartNode)
            {
                prevStartNode->next = startNode;
            }
            else
            {
                graph = startNode;
            }
        }

        // Add the destination to the list of destinations
        Node *newNode = new Node;
        newNode->end = end;
        newNode->time = time;
        newNode->next = startNode->head;
        startNode->head = newNode;

        // Now add the edge from end to start (to make it undirected)
        AdjacencyList *endNode = graph;
        AdjacencyList *prevEndNode = NULL;

        // Check if the end point is already in the graph
        while (endNode != NULL && endNode->start != end)
        {
            prevEndNode = endNode;
            endNode = endNode->next;
        }

        // If the end point is not in the graph, add it
        if (!endNode)
        {
            endNode = new AdjacencyList;
            endNode->start = end;
            endNode->head = NULL;
            endNode->next = NULL;

            if (prevEndNode)
            {
                prevEndNode->next = endNode;
            }
            else
            {
                graph = endNode;
            }
        }

        // Add the destination to the list of destinations (reverse direction)
        Node *newNodeReverse = new Node;
        newNodeReverse->end = start;
        newNodeReverse->time = time;
        newNodeReverse->next = endNode->head;
        endNode->head = newNodeReverse;
    }
string returnStatus(string start, string end)
    {
        AdjacencyList *startNode = graph;

        while (startNode)
        {
            if (startNode->start == start)
            {
                Node *temp = startNode->head;
                while (temp)
                {
                    if (temp->end == end)
                    {
                        return temp->status;
                    }
                    temp = temp->next;
                }
            }
            startNode = startNode->next;
        }

        return "";
    }

public:
    roadNetwork() : graph(NULL) {}

    ~roadNetwork()
    {
        while (graph)
        {
            AdjacencyList *current = graph;
            graph = graph->next;

            Node *adj = current->head;
            while (adj)
            {
                Node *temp = adj;
                adj = adj->next;
                delete temp;
            }

            delete current;
        }
    }
 void addAllEdges()
    {
        string **data;
        int cols = 3;
        CSVReader reader;
        int rows = reader.calculateRows("road_network.csv");
        reader.readCSV("road_network.csv", data, cols);

        if (rows > 1)
        {
            for (int i = 1; i < rows; i++)
            {
                string start = data[i][0];
                string end = data[i][1];
                int time = stoi(data[i][2]);

                addEdge(start, end, time);
            }
        }

        // delete the data array
        for (int i = 0; i < rows; i++)
        {
            delete[] data[i];
        }
        delete[] data;
    }

 void setup()
    {
        addAllEdges();

        // setupTrafficSignal(signal);
    }

    bool checkVertexLink(string start, string end)
    {
        AdjacencyList *startNode = graph;

        while (startNode)
        {
            if (startNode->start == start)
            {
                Node *temp = startNode->head;
                while (temp)
                {
                    if (temp->end == end)
                    {
                        return true;
                    }
                    temp = temp->next;
                }
            }
            startNode = startNode->next;
        }

        return false;
    }

 string dijkstra(string start, string end)
    {
        int numberofVertex = 26;

        // Initialize arrays for distances, previous nodes, and visited status
        int distance[numberofVertex];
        int previous[numberofVertex];
        bool visited[numberofVertex];

        for (int i = 0; i < numberofVertex; i++)
        {
            distance[i] = 999;  // Used as infinity
            previous[i] = -1;   // No previous node
            visited[i] = false; // All not visited
        }

        int startIndex = start[0] - 'A';
        int endIndex = end[0] - 'A';

        distance[startIndex] = 0;

        // Dijkstra's algorithm
        for (int count = 0; count < numberofVertex; count++)
        {
            // Find the unvisited node with the smallest distance
            int minDistance = 999;
            int minIndex = -1;

            for (int i = 0; i < numberofVertex; i++)
            {
                if (!visited[i] && distance[i] < minDistance)
                {
                    minDistance = distance[i];
                    minIndex = i;
                }
            }

            // If no node is found, break
            if (minIndex == -1)
                break;

            // Mark the node as visited
            visited[minIndex] = true;

            // Update distances for neighbors
            AdjacencyList *startNode = graph;
            while (startNode)
            {
                if (startNode->start[0] - 'A' == minIndex)
                {
                    Node *temp = startNode->head;
                    while (temp)
                    {
                        // Check if the road is blocked
                        if (temp->status == "Blocked")
                        {
                            temp = temp->next;
                            continue; // Skip this edge if it's blocked
                        }

                        int neighborIndex = temp->end[0] - 'A';
                        int newDistance = distance[minIndex] + temp->time;

                        // If a shorter path is found
                        if (newDistance < distance[neighborIndex])
                        {
                            distance[neighborIndex] = newDistance;
                            previous[neighborIndex] = minIndex;
                        }
                        temp = temp->next;
                    }
                }
                startNode = startNode->next;
            }
        }

        // Reconstruct the path
        string path = "";
        int at = endIndex;

        // Construct the path from destination to source
        while (at != -1)
        {
            path += char(at + 'A');
            at = previous[at];
        }
        // Output the result
        if (distance[endIndex] < 999)
        {
            reverse(path.begin(), path.end()); // Reverse the path to get it from start to end
            return path;
        }
        else
        {
            return "";
        }
    }

 void addStatus(string start, string end, string status)
    {
        AdjacencyList *startNode = graph;

        while (startNode)
        {
            if (startNode->start == start)
            {
                Node *temp = startNode->head;
                while (temp)
                {
                    if (temp->end == end)
                    {
                        temp->status = status;
                        break;
                    }
                    temp = temp->next;
                }
            }
            startNode = startNode->next;
        }
    }
 void allPossiblePath(string start, string end)
    {
    }

    void display()
    {
        AdjacencyList *startNode = graph;

        cout << "------ City Traffic Network ------" << endl;

        while (startNode)
        {
            cout << startNode->start << " -> ";
            Node *temp = startNode->head;
            while (temp)
            {
                cout << "(" << temp->end << ", " << temp->time << ") ";
                temp = temp->next;
            }
            cout << endl;
            startNode = startNode->next;
        }
    } 
};
