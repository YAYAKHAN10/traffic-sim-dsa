### Smart Traffic Management System Simulator
 ## Introduction
   The simulator aims to optimize traffic flow within a city by leveraging data structures and algorithms to dynamically manage various aspects of traffic, such as vehicle routing, traffic signals, congestion monitoring, and emergency handling.
   This project focuses on real-time decision-making and adjustment without relying on predictive features, ensuring efficient handling of traffic conditions as they occur.
   By combining foundational computer science concepts with real-world applications, the simulator provides an interactive way to address challenges in urban traffic management. 
   ## Project Requirements
   The task requires developing a modular system with the following components:
1) ## City Traffic Network Representation: Represent the city infrastructure as a directed, weighted graph and manage its dynamic updates.
2) ### Vehicle Routing System: Implement a routing mechanism to calculate the shortest paths dynamically and monitor vehicles in real-time.
3) ### Traffic Signal Management: Control traffic signals at intersections based on road conditions, with special handling for emergencies.
4) ### Congestion Monitoring: Continuously assess road congestion levels and reroute traffic accordingly.
5) ### Emergency Vehicle Handling: Provide prioritized routes for emergency vehicles and ensure minimal delays.
6) ### Accident and Road Closure Simulation: Simulate disruptions and update the traffic network dynamically to handle such situations effectively.
Each component must utilize specific data structures and algorithms, such as graphs, heaps, priority queues, BFS, DFS, and Dijkstra’s algorithm.
The system should operate interactively, allowing users to simulate various scenarios like vehicle movement, road closures, and emergencies.

## Key Features
1) ## Dynamic Traffic Network Management:
Represent the city as a graph with intersections (nodes) and roads (edges).
Dynamically add or remove intersections and roads to reflect changing city layouts.
### 2) Real-Time Vehicle Routing:
Calculate the shortest or fastest route using Dijkstra's Algorithm.
Dynamically adapt routes based on live traffic conditions.
## 3) Traffic Signal Control:
Use priority queues to manage incoming traffic density.
Automatically adjust green light durations to reduce wait times.
Provide an emergency override system for special situations.
## 4) Congestion Monitoring:
Monitor road congestion levels using hash tables to track vehicle counts.
Identify and display congested areas, rerouting traffic using BFS or DFS algorithms.
## 5) Emergency Handling:
Provide special paths for emergency vehicles using A Search Algorithm*.
Override normal traffic rules to ensure minimal delays.
## 6) Disruption Simulation:
Simulate road closures and accidents dynamically.
Recalculate and update routes for all affected vehicles.

## Code Explanation
 # Module1
# a)Stack Class:
 * Implements a simple stack using a linked list structure.
 * Provides basic operations:
  push: Adds an element (e.g., string data) to the top of the stack.
  pop: Removes and returns the top element from the stack.
  isEmpty: Checks whether the stack is empty.
# b)CSVReader Class:
Handles reading data from CSV files.
Counts the number of rows in a file (ignoring empty rows).
Reads and parses data from a CSV file into a dynamically allocated 2D array of strings.
Used to load road network details from a CSV file, such as start points, endpoints, and travel times.
# c)Timer Class:
  Tracks time elapsed since a specific point.
 # start: Records the start time.
 # isTimeout: Checks whether a specified amount of time (in seconds) has passed since the start.
 
# d)RoadNetwork Class:
  Adding bidirectional roads between locations (both forward and reverse links).
  Checking if a road exists between two locations.
  Loading all road connections from a CSV file and dynamically building the graph.
# Overview:
Reads road network data from a CSV file containing rows with a start point, endpoint, and travel time.
Dynamically builds the graph, representing the city’s road network.
# Graph Representation:
Each location (vertex) is linked to its connected locations (neighbors) along with the travel time between them.
Ensures the network is undirected by adding reverse edges for bidirectional roads.

### Module2: Vehicle Routing System
The Vehicle class represents a vehicle in a traffic management system. 
It holds details such as the vehicle's unique ID, its starting location, its current position, its destination, a timer for tracking its movement, and the path it follows.
It implements a dynamic route calculation system using Dijkstra's Algorithm to find the shortest or fastest route for vehicles within a city traffic network.
 The goal is to calculate the optimal path between two points in the road network, dynamically adjusting for changes like blocked roads or altered traffic conditions.
 
The algorithm works by iterating through all the nodes (locations in the city), updating the shortest known distance from the starting point to each node, and keeping track of the best route to reach each destination.
The algorithm starts by assigning infinite distances to all nodes except the starting node, then explores the network by selecting the node with the smallest known distance, marking it as visited, and updating the distances to its neighboring nodes.

The implementation also takes into account real-time traffic conditions by considering road statuses.
If a road is "Blocked," it is skipped during the calculation.
Once the algorithm finishes processing, it reconstructs the optimal path by backtracking from the destination to the starting point.
If no valid path is found, it returns an empty string.


### addStatus(string start, string end, string status):
This function updates the status of a road segment between two nodes (locations) in the traffic network.
It searches for the start node in the graph and then traverses the list of roads connected to it.
If the road to the destination node is found, it updates the status (e.g., "Blocked", "Open") of that road.
Then it displays the entire traffic network, by traversing the graph, printing each start node followed by its connected destination nodes and the travel time for each road. 
## Module7: Simulation Dashboard
# The Simulation class simulates a traffic network with vehicles and road conditions.
*Initialization: The constructor calls the setup() method from the roadNetwork object to initialize the road network.
*Adding Vehicles: The addVehicle() method sets up the vehicles by reading data from a CSV file called vehicles.csv.
The vehicle data includes information like vehicle ID, start location, and destination.
This data is used to initialize the vehicle array.
*Assigning Paths: After setting up the vehicles, the setup() method calculates the shortest route for each vehicle using Dijkstra’s algorithm.
The algorithm finds the best path from the vehicle’s starting point to its destination.
This calculated path is then assigned to the vehicle.
*Updating Road Status: The setup() method also reads road status information from another CSV file, road_closures.csv, which provides data on which roads are blocked or open.
It updates the road network with this information by calling the addStatus() function in the roadNetwork class.
## Module5: Emergency Vehicle Handling
It first asks the user to input the start and end intersections for the emergency vehicle's route.
These intersections are represented as characters (e.g., 'A', 'B').
 # Convert to Uppercase:
It ensures that both the start and end intersections are uppercase letters.
If the user enters lowercase letters, the function converts them to uppercase by subtracting 32 from the ASCII value of the character.
 # Validate Input:
The function checks whether both the start and end intersections are valid (i.e., between 'A' and 'Z').
If the input is invalid, it displays an error message and returns from the function, not proceeding further.
## Find Path Using Dijkstra's Algorithm:
The function uses Dijkstra's algorithm (via the roadNetwork.dijkstra() function) to find the shortest path between the start and end intersections.
The path is returned as a string (representing a sequence of intersections).
# Handle No Path Found:
If no path is found (i.e., the path string is empty), the function displays a message informing the user that no path exists between the two intersections.
# Display the Emergency Vehicle Path:
If a path is found, the function prints the emergency vehicle's route. 
It iterates through the path string and prints each intersection, separating them with " -> ", until it reaches the last intersection.
### Module6:Accident and Road Closure Simulation
 It first reads the data from a CSV file named road_closures.csv using a CSVReader object.
   # Displaying Blocked Roads:
   It then loops through the rows of the CSV file (skipping the header).
   If the status of a road is marked as "Blocked", it prints a message showing the road segment (from start to end) that is blocked 
  # Input for Road Blocking:
  The function enters a loop where it asks the user to input the road they want to block by specifying the starting and ending intersections (letters). 
  The program ensures that the inputs are uppercase letters and validates whether the intersections are valid.
  It checks if the specified road (represented by start and end intersections) exists in the road network using roadNetwork.checkVertexLink().
  If the road does not exist, the program asks the user to try again.
  # Checking and Updating Road Status:
  The function then checks whether the road is already in the road_closures.csv file. 
  If the road is found, it updates the status to "Blocked" if it is not already blocked.
  If the road is already blocked, it informs the user.
  If the road is not found, it adds a new entry to the data array for the blocked road. 
