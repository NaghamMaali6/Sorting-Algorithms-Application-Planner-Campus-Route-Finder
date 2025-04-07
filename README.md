# Sorting-Algorithms-Application-Planner-Campus-Route-Finder
This C project is designed to assist first-year students by providing two key features: a Campus Route Finder using Dijkstra's algorithm, and a Course Prerequisite Planner using Topological Sort. The system takes input from external files and allows the user to interact with a menu-based interface to find shortest paths and plan course schedules.

🔧 Note: This project is still under development. Option 3 (printing the shortest route between two buildings and total distance) is not fully implemented yet, and Option 5 (printing the sorted courses) has not been done yet.

# Features:
1. Load the Two Files:
   * Reads and parses both input_buildings.txt and input_courses.txt into appropriate data structures.
   * Graph representation for buildings can be done using either an adjacency matrix or adjacency list.
   * Courses and their prerequisites are stored in a directed graph.
2. Calculate the Shortest Distance Between Two Buildings:
   * Prompts the user for two building names.
   * Applies Dijkstra's algorithm to compute the shortest distance between them.
   * returns the total distance and prints the whole path
3. Print the Shortest Route and Total Distance: Intended to print the full path between two buildings along with the total distance using the result from Dijkstra's algorithm.
4. Sort Courses Using Topological Sort:
   * Builds a directed acyclic graph (DAG) from the courses and their prerequisites.
   * Performs topological sorting to determine the correct course order for first-year students.
   * Result is stored internally in the program.  
5. Print the Sorted Courses: It will allow users to view the topologically sorted list of courses after performing the sort.
6. Exit the Application: Safely exits the program.

# Algorithms used:
* Dijkstra's Algorithm: To find the shortest path between buildings in a weighted undirected graph.
* Topological Sort (DFS/BFS based): To sort courses based on prerequisite dependencies.

