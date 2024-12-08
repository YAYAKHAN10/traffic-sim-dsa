#include <iostream>
#include <fstream>
#include <chrono>
using namespace std;
struct Intersection {
    string roadSegment;
    int vehicleCount;

    Intersection() : roadSegment(""), vehicleCount(0) {}
    Intersection(string name, int count) : roadSegment(name), vehicleCount(count) {}
};
class MaxHeap {
private:
    Intersection heap[41];
    int size;

    int parent(int i) { return (i - 1) / 2; }
    int left(int i) { return 2 * i + 1; }
    int right(int i) { return 2 * i + 2; }
 void heapifyDown(int i) {
        int largest = i;
        int l = left(i);
        int r = right(i);

        if (l < size && heap[l].vehicleCount > heap[largest].vehicleCount)
            largest = l;
        if (r < size && heap[r].vehicleCount > heap[largest].vehicleCount)
            largest = r;

        if (largest != i) {
            swap(heap[i], heap[largest]);
            heapifyDown(largest);
        }
    }

    void heapifyUp(int i) {
        while (i > 0 && heap[parent(i)].vehicleCount < heap[i].vehicleCount) {
            swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

public:
    MaxHeap() : size(0) {}

    void insert(string roadSegment, int vehicleCount) {
        if (size == 100) {
            cout << "Heap is full. Cannot insert.\n";
            return;
        }

        heap[size] = Intersection(roadSegment, vehicleCount);
        heapifyUp(size);
        size++;
    }

    Intersection extractMax() {
        if (size == 0) {
            cout << "Heap is empty.\n";
            return Intersection();
        }

        Intersection maxItem = heap[0];
        heap[0] = heap[size - 1];
        size--;
        heapifyDown(0);

        return maxItem;
    }

    void display() {
        for (int i = 0; i < size; i++) {
            cout << heap[i].roadSegment << " -> Vehicles: " << heap[i].vehicleCount << endl;
        }
    }
};
class TrafficHashTable
{
private:
    static const int TABLE_SIZE = 41;
    struct RoadData
    {
        string roadSegment;
        int vehicleCount;
        bool isOccupied;
    };

    RoadData hashTable[TABLE_SIZE];

    // hash function
    int hashFunction(string roadSegment)
    {
        int hash = 0;
        for (char c : roadSegment)
        {
            hash = (hash + c) % TABLE_SIZE;
        }
        return hash;
    }

public:
    TrafficHashTable()
    {
        for (int i = 0; i < TABLE_SIZE; ++i)
        {
            hashTable[i].isOccupied = false;
        }
    }

    // insert or update vehicle count (-ve value of count to remove vehicles)
    void updateVehicleCount(string roadSegment, int count)
    {
        int index = hashFunction(roadSegment);
        while (hashTable[index].isOccupied && hashTable[index].roadSegment != roadSegment)
        {
            index = (index + 1) % TABLE_SIZE; 
        }

        if (!hashTable[index].isOccupied)
        {
            // initilize if empty
            hashTable[index].roadSegment = roadSegment;
            hashTable[index].vehicleCount = 0;
            hashTable[index].isOccupied = true;
        }

        // update count
        hashTable[index].vehicleCount += count;

        // ensure count is not -ve
        if (hashTable[index].vehicleCount < 0)
        {
            hashTable[index].vehicleCount = 0;
        }
    }

    void displayCongestionStatus()
    {
        cout << "------ Congestion Status ------" << endl;
        for (int i = 0; i < TABLE_SIZE; ++i)
        {
            if (hashTable[i].isOccupied)
            {
                cout << hashTable[i].roadSegment << " -> Vehicles: "
                     << hashTable[i].vehicleCount << endl;
            }
        }
    }

    // Display roads that are congested (exceeding the threshold)
    void displayCongestedRoads(int threshold)
    {
        cout << "------ Congested Roads ------" << endl;
        for (int i = 0; i < TABLE_SIZE; ++i)
        {
            if (hashTable[i].isOccupied && hashTable[i].vehicleCount > threshold)
            {
                cout << hashTable[i].roadSegment << " -> Vehicles: "
                     << hashTable[i].vehicleCount << endl;
            }
        }
    }

    void loadToHeap(MaxHeap &heap) {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            if (hashTable[i].isOccupied) {
                heap.insert(hashTable[i].roadSegment, hashTable[i].vehicleCount);
            }
        }
    }
};

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

class Stopwatch
{
private:
    std::chrono::steady_clock::time_point start_time;

public:
    Stopwatch()
    {
        reset();
    }

    void reset()
    {
        start_time = std::chrono::steady_clock::now();
    }

    bool hasTimePassed(int seconds) const
    {
        auto current_time = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time);
        return elapsed_time.count() >= seconds;
    }

    int elapsedSeconds() const
    {
        auto current_time = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time);
        return elapsed_time.count();
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
 void DFSpath(string start, string end, bool visited[], string path, int time)
    {
        visited[start[0] - 'A'] = true;
        path += start;

        if (start == end)
        {
            cout << "Path: ";

            for (int i = 0; i < path.length() - 1; i++)
            {
                cout << path[i] << "  ";
            }

            cout << "| " << time << endl;
        }
        else
        {
            AdjacencyList *startNode = graph;
            while (startNode)
            {
                if (startNode->start == start)
                {
                    Node *temp = startNode->head;
                    while (temp)
                    {
                        if (!visited[temp->end[0] - 'A'])
                        {
                            DFSpath(temp->end, end, visited, path, time + temp->time);
                        }
                        temp = temp->next;
                    }
                }
                startNode = startNode->next;
            }
        }

        path.pop_back();
        visited[start[0] - 'A'] = false;
    }

    void allPossiblePath(string start, string end)
    {
        bool *visited = new bool[26];
        string path = "";
        int time = 0;
        DFSpath(start, end, visited, path, time);
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

class Vehicle
{

public:
    string id;
    string start;
    string currentPosition;
    string end;
    Timer timer;
    string path;

    Vehicle() : id(""), start(""), end(""), path("") {}

    void setupVehicle(Vehicle *&vehicle)
    {
        string **data;
        CSVReader reader;
        int cols = 3;
        int rows = reader.calculateRows("vehicles.csv");
        reader.readCSV("vehicles.csv", data, cols);

        // Allocate memory for vehicle array
        vehicle = new Vehicle[rows - 1];

        // Add vehicles to the array
        for (int i = 1; i < rows; i++)
        {
            vehicle[i - 1].id = data[i][0];
            vehicle[i - 1].start = data[i][1];
            vehicle[i - 1].currentPosition = data[i][1];
            vehicle[i - 1].end = data[i][2];
        }

        // delete the data array
        for (int i = 0; i < rows; i++)
        {
            delete[] data[i];
        }
        delete[] data;
    }

    void addpath(string path)
    {
        this->path = path;
    }
};


class EmergencyVehicle : public Vehicle
{
};

class Simulation
{
    CSVReader reader;
    roadNetwork roadNetwork;
    Vehicle *vehicle;

public:
    Simulation()
    {
        roadNetwork.setup();
    }

    void addVehicle()
    {
        vehicle->setupVehicle(vehicle);
    }

    void setup()
    {
        addVehicle();

        // add path of each vehicle
        for (int i = 0; i < 30; i++)
        {
            string tempPath = "";
            tempPath = roadNetwork.dijkstra(vehicle[i].start, vehicle[i].end);
            vehicle[i].addpath(tempPath);
        }

        // add status of each road
        string **data;
        int cols = 3;
        int rows = reader.calculateRows("road_closures.csv");
        reader.readCSV("road_closures.csv", data, cols);

        if (rows > 1)
        {
            for (int i = 1; i < rows; i++)
            {
                string start = data[i][0];
                string end = data[i][1];
                string status = data[i][2];

                roadNetwork.addStatus(start, end, status);
            }
        }
    }

void displayRoadNetwork()
    {
        roadNetwork.display();
    }

    void displayTrafficSignalStatus()
    {
       
        
    }

    void displayCongestionStatus()
    {
    }

 void displayBlockedRoads()
    {
        string **data;
        CSVReader reader;
        int cols = 3;
        int rows = reader.calculateRows("road_closures.csv");
        reader.readCSV("road_closures.csv", data, cols);

        cout << "------ Blocked Roads ------" << endl;
        for (int i = 1; i < rows; i++)
        {
            if (data[i][2] == "Blocked")
                cout << data[i][0] << " to " << data[i][1] << " is blocked." << endl;
        }

        // delete the data array
        for (int i = 0; i < rows; i++)
        {
            delete[] data[i];
        }
        delete[] data;
    }

 void handleEmergencyVehicleRouting()
    {
        cout << "Enter Start and End intersection for the Emergency Vehicle: ";
        char start, end;
        cin >> start >> end;

        // ensure start and end are uppercase letters
        if (start >= 'a' && start <= 'z')
        {
            start = start - 32;
        }

        if (end >= 'a' && end <= 'z')
        {
            end = end - 32;
        }

        // validate input
        if (start < 'A' || start > 'Z' || end < 'A' || end > 'Z')
        {
            cout << "Invalid input. Please enter valid intersections." << endl;
            return;
        }

        string path = roadNetwork.dijkstra(string(1, start), string(1, end));

        if (path.empty())
        {
            cout << "No path found between " << start << " and " << end << endl;
        }
        else
        {
            cout<<"Emergency Vehicle Path: ";
            for (int i=0; i<path.length()-1; i++)
            {
                cout<< path[i] <<" -> ";
            }
            cout<<path[path.length()-1];
        }

    }

void blockRoadsDueToAccident()
    {
        string **data;
        CSVReader reader;
        int cols = 3;
        int rows = reader.calculateRows("road_closures.csv");
        reader.readCSV("road_closures.csv", data, cols);

        bool isEdge = false;

        while (!isEdge)
        {
            cout << "Enter the Road you want to block (start, end): ";
            char start, end;
            cin >> start >> end;

            // ensure start and end are uppercase letters
            if (start >= 'a' && start <= 'z')
            {
                start = start - 32;
            }
            if (end >= 'a' && end <= 'z')
            {
                end = end - 32;
            }

            // validate input
            if (start < 'A' || start > 'Z' || end < 'A' || end > 'Z')
            {
                cout << "Invalid input. Please enter valid intersections." << endl;
                continue;
            }

            // check if the road exists in the road network
            isEdge = roadNetwork.checkVertexLink(string(1, start), string(1, end));
            if (!isEdge)
            {
                cout << "The road does not exist. Please try again." << endl;
                continue;
            }

            bool alreadyUpdated = false;

            // check if the road is already in the CSV
            for (int i = 1; i < rows; i++)
            {
                if (data[i][0] == string(1, start) && data[i][1] == string(1, end))
                {
                    if (data[i][2] == "Blocked")
                    {
                        cout << "The road is already blocked." << endl;
                        isEdge = false;
                    }
                    else if (data[i][2] == "Clear" || data[i][2] == "UnderRepair")
                    {
                        data[i][2] = "Blocked";
                        cout << "Blocking Road...." << endl;
                        alreadyUpdated = true;
                    }
                    break;
                }
            }

            if (!alreadyUpdated)
            {
                // adding a new entry in data array
                if (isEdge)
                {
                    rows++;
                    string **newData = new string *[rows];

                    // Deep copy of the old data to newData
                    for (int i = 0; i < rows - 1; ++i)
                    {
                        newData[i] = new string[3];
                        newData[i][0] = data[i][0];
                        newData[i][1] = data[i][1];
                        newData[i][2] = data[i][2];
                    }

                    // Adding the new entry
                    newData[rows - 1] = new string[3];
                    newData[rows - 1][0] = string(1, start);
                    newData[rows - 1][1] = string(1, end);
                    newData[rows - 1][2] = "Blocked";

                    // Deleting the old data to avoid memory leak
                    for (int i = 0; i < rows - 1; ++i)
                    {
                        delete[] data[i];
                    }
                    delete[] data;

                    // Assigning newData to data
                    data = newData;

                    cout << "Blocking Road...." << endl;
                }
            }

            // set road status to blocked in graph
            roadNetwork.addStatus(string(1, start), string(1, end), "Blocked");

            // write to CSV file
            ofstream file("road_closures.csv");
            if (!file.is_open())
            {
                cout << "Error opening file!" << endl;
                return;
            }
            for (int i = 0; i < rows; i++)
            {
                file << data[i][0] << "," << data[i][1] << "," << data[i][2] << endl;
            }

            file.close();
        }

        // delete the data array
        for (int i = 0; i < rows; ++i)
        {
            delete[] data[i];
        }
        delete[] data;
    }
 void simulateVehicleRouting()
    {
        cout<<"Simulating Vehicle Routing..."<<endl;
        cout<<"Enter the Starting Intersection: ";
        char start;
        cin>>start;
        cout<<"Enter the Ending Intersection: ";
        char end;
        cin>>end;
        roadNetwork.allPossiblePath(string(1, start), string(1, end));

    }
 void run()
    {
        setup();
        bool terminate = false;
        while (!terminate)
        {
            cout << endl
                 << "------ Simulation Dashboard ------" << endl;
            cout << "1. Display City Traffic Network" << endl;
            cout << "2. Display Traffic Signal Status" << endl;
            cout << "3. Display Congestion Status" << endl;
            cout << "4. Display Blocked Roads" << endl;
            cout << "5. Handle Emergency Vehicle Routing" << endl;
            cout << "6. Block Roads due to Accident" << endl;
            cout << "7. Simulate Vehicle Routing" << endl;
            cout << "8. Exit Simulation" << endl;
            cout << "Enter your choice: ";

            int choice;
            cin >> choice;
            cout << endl;

            switch (choice)
            {
            case 1:
                displayRoadNetwork();
                break;
            case 2:
                displayTrafficSignalStatus();
                break;
            case 3:
                break;
            case 4:
                displayBlockedRoads();
                break;
            case 5:
                handleEmergencyVehicleRouting();
                break;
            case 6:
                blockRoadsDueToAccident();
                break;
            case 7:
                simulateVehicleRouting();
                break;
            case 8:
                terminate = true;
                cout << "Exiting simulation..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }
};
