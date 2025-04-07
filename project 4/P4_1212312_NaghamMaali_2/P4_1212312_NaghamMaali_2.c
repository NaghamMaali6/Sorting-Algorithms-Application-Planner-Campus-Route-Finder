/*
  Name: Nagham Maali
  ID: 1212312
  Sec.: 2
*/

//libraries:
/*1*/#include <stdio.h>
/*2*/#include <stdlib.h>
/*3*/#include <string.h>
/*4*/#include <limits.h>

//constants:
/*1*/#define Max_number_of_Buildings 100
/*2*/#define Max_number_of_courses 100
/*3*/#define Max_Name_Lenght 50

struct BNode  //Structure representing a node in the building graph
{
    int dest ;
    int weight ;  //Weight of the edge = distance between the buildings
    struct BNode *Next ;  //Pointer to the next node in the adjacency list
};

struct BGraph  //Structure representing the building graph
{
    int number_of_vertices ;
    char buildings[Max_number_of_Buildings][Max_Name_Lenght] ;  //Array to store building names
    struct BNode *List[Max_number_of_Buildings] ;  //Array of pointers to adjacency lists
};

struct CNode  //Structure representing a node in the course graph
{
    char course[Max_Name_Lenght] ;
    struct CNode *Next ;  //Pointer to the next node in the adjacency list
};

struct CGraph  //Structure representing the course graph
{
    int number_of_courses ;
    char courses[Max_number_of_courses][Max_Name_Lenght] ;  //Array to store course names
    struct CNode *List[Max_number_of_courses] ;  //Array of pointers to adjacency lists
};

struct Qnode  //Structure representing a node in the queue
{
    int vertex ;
    int distance ;
};

struct Queue  //Structure representing a queue
{
    struct Qnode *node ;
    int size  ;
    int capacity ;
};

//function Definition:
/*1*/void displayMainMenu() ;
/*2*/struct BNode *createBNode(int dest , int weight) ;
/*3*/struct BGraph *createBGraph(int vertices) ;
/*4*/void BAddEdge(struct BGraph *g , int src , int dest , int weight) ;  //function to add an edge to the graph
/*5*/int findBuildingIndex(struct BGraph *g , char *building) ;  //function to find the index of the building
/*6*/void load_Building(struct BGraph *g , char *file_name) ;
/*7*/void printBGraph(struct BGraph *g) ;
/*8*/void printBfile(struct BGraph *g) ;
/*9*/struct CGraph *createCGraph() ;
/*10*/struct CNode *createCNode(char *course) ;
/*11*/void CAddEdge(struct CGraph *g , char *src , char *dest) ;  //function to add an edge to the graph
/*12*/void load_Course(struct CGraph *g , char *file_name) ;
/*13*/int findCourseIndex(struct CGraph *g , char *course) ;
/*14*/void printCGraph(struct CGraph *g) ;
/*15*/void printCfile(struct CGraph *g) ;
/*16*/void DijkstraAlgorithm(struct BGraph *g , int src , int dest) ;
/*17*/void min(struct Queue *q , int idx) ;
/*18*/struct Queue *createQ(int capacity) ;
/*19*/void swap(struct Qnode *a , struct Qnode *b) ;
/*20*/void Enqueue(struct Queue *q , int vertex , int distance) ;
/*21*/struct Qnode Dequeue(struct Queue *q) ;
/*22*/void topologicalSort(struct CGraph *g) ;
/*23*/void ts(struct CGraph *g , int v , int visited[] , int *stack , int *stackI) ;

int main()
{
    printf("Hi!\n") ;

    int x ;  //user choice
    int condition = 1 ;

    struct BGraph *Graph1 = createBGraph(Max_number_of_Buildings) ;  //Create a new building graph
    struct CGraph *Graph2 = createCGraph() ;  //Create a new course graph

    char B1[Max_Name_Lenght] ;
    char B2[Max_Name_Lenght] ;
    int srcI ;
    int destI ;

    while(condition == 1)
    {
        displayMainMenu() ;
        scanf("%d" , &x) ;

        if(x == 1)
        {
            printf("loading process started ... \n") ;

            load_Building(Graph1 , "input_buildings.txt") ;  //Load building data from file
            printBfile(Graph1) ;  //Print building data
            printBGraph(Graph1) ;  //Print the building graph

            printf("\n") ;

            load_Course(Graph2 ,"input_courses.txt") ;  //Load course data from file
            printCfile(Graph2) ;  //Print course data
            printCGraph(Graph2) ;  //Print the course graph

            printf("done loading files.\n") ;

        }
        else if(x == 2)
        {
            printf("Enter the Building u r in:\n") ;
            scanf("%s" , B1) ;
            printf("Enter the Building u wanna go to:\n") ;
            scanf("%s" , B2) ;

            srcI = findBuildingIndex(Graph1 , B1) ;  //Find index of source building
            destI = findBuildingIndex(Graph1 , B2) ;  //Find index of destination building

            if(srcI == -1 | destI == -1)  //If either building isn't found
            {
                printf("one or both buildings not found in the graph.\n") ;
            }
            else  //If both buildings are found
            {
                DijkstraAlgorithm(Graph1 , srcI , destI) ;  //Calculate and print shortest path between 'em using Dijkstra Algorithm
            }
        }
        else if(x == 3)
        {
            //3
        }
        else if(x == 4)
        {
            topologicalSort(Graph2) ;  //Perform topological sort
            printf("done sorting.\n") ;
        }
        else if(x == 5)
        {
            printf("courses after sorting:\n") ;
            //loop to print
        }
        else if(x == 6)
        {
            printf("Bye!\n") ;
            break ;  //Exit the application
        }
        else
        {
            printf("invalid option!!!\n") ;
            printf("try again :)\n") ;
        }
    }

    return 0;
}

void displayMainMenu()
{
    printf("please select an option(1-11):\n") ;
    printf("1. Load the two files.\n") ;
    printf("2. Calculate the shortest distance between two buildings.\n") ;
    printf("3. Print the shortest route between two buildings and the total distance.\n") ;
    printf("4. Sort the courses using the topological sort.\n") ;
    printf("5. Print the sorted courses.\n") ;
    printf("6. Exit the application.\n") ;
}

struct BGraph *createBGraph(int vertices)
{
    struct BGraph *g ;
    int i ;

    g = (struct BGraph *)malloc(sizeof(struct BGraph)) ;
    g -> number_of_vertices = 0 ;  //initially no vertices
    for(i = 0 ; i < vertices ; i++)
    {
        g -> List[i] = NULL ;
    }
    return g ;
}

void load_Building(struct BGraph *g , char *file_name)
{
    FILE *input_file ;
    char Line[100] ;
    char src[Max_Name_Lenght] ;
    char dest[Max_Name_Lenght] ;
    char w[1000] ;  //weight as a string
    int weight ;
    int srcI ;
    int destI ;
    char *token ;

    input_file = fopen(file_name , "r") ;  //open input file
    if(input_file == NULL)
    {
        printf("No file with such a name!\n") ;
        return ;
    }

    while(fgets(Line , sizeof(Line) , input_file) != NULL)  //read each line from input file
    {
        token = strtok(Line , "#") ;  //split the line into tokens using '#' as delimiter
        if(token == NULL)
        {
            printf("invalid input format!\n") ;
            fclose(input_file) ;  //Close the input file
            return ;
        }

        strcpy(src , token) ;  //Copy the first token to src
        token = strtok(NULL , "#") ;
        strcpy(dest , token) ;  //Copy the next token to dest
        token = strtok(NULL , "#") ;
        strcpy(w , token) ;  //Copy the next token to w
        weight = atoi(w) ;  //Convert the weight from string to integer

        srcI = findBuildingIndex(g , src) ;  //Find the index of the source building
        if(srcI == -1)  //if building isn't found
        {
            srcI = (g -> number_of_vertices)++ ;  //Assign a new index and increment the number of vertices
            strcpy(g -> buildings[srcI] , src) ;  //Copy the source building name to the graph
        }

        destI = findBuildingIndex(g , dest) ;  //Find the index of the destination building
        if(destI == -1)  //If destination building isn't found
        {
            destI = (g -> number_of_vertices)++ ;  //Assign a new index and increment the number of vertices
            strcpy(g -> buildings[destI] , dest) ;  //Copy the destination building name to the graph
        }

        BAddEdge(g , srcI , destI , weight) ;  //Add an edge from srcI to destI with the given weight
        BAddEdge(g , destI , srcI , weight) ;  //Add an edge from destI to srcI with the given weight (undirected graph)
    }

    fclose(input_file) ;  //Close the input file
}

int findBuildingIndex(struct BGraph *g , char *building)
{
    int i ;

    for(i = 0 ; i < (g -> number_of_vertices) ; i++)  //Loop through all vertices in the graph
    {
        if(strcmp(g -> buildings[i] , building) == 0)  //Compare the building name with the current vertex
        {
            return i ;  //Return the index if found
        }
    }

    return -1 ;  //Return -1 if building isn't found
}

void BAddEdge(struct BGraph *g , int src , int dest , int weight)
{
    struct BNode *newN ;

    newN = createBNode(dest , weight) ;  //Create a new building node
    newN -> Next = g -> List[src] ;  //Point the new node to the current adjacency list of src
    g -> List[src] = newN ;  //Set the new node as the head of the adjacency list of src
}

struct BNode *createBNode(int dest , int weight)
{
    struct BNode *newN ;

    newN = (struct BNode *)malloc(sizeof(struct BNode)) ;  //Allocate memory for the new node
    newN -> dest = dest ;  //Set the destination vertex
    newN -> weight = weight ;  //Set the weight of the edge
    newN -> Next = NULL ;  //Initialize the next pointer to NULL

    return newN ;
}

void printBfile(struct BGraph *g)
{
    int i ;
    struct BNode *temp ;

    printf("Buildings file:\n") ;

    printf("Building 1      Building 2     distance\n") ;  //Print the headers

    for(i = 0 ; i < (g -> number_of_vertices) ; i++)  //Loop through all vertices in the graph
    {
        temp = g -> List[i] ;  //Get the adjacency list of the current vertex
        while(temp != NULL)  //Loop through the adjacency list
        {
            printf(" %-12s   %-14s   %-8d \n" , g -> buildings[i] , g -> buildings[temp -> dest] , temp -> weight) ;
            temp = temp -> Next ;  //Move to the next node in the adjacency list
        }

        printf("\n") ;
    }
}

void printBGraph(struct BGraph *g)
{
    int i ;
    struct BNode *temp ;

    printf("Graph:\n") ;

    for(i = 0 ; i < (g -> number_of_vertices) ; i++)  //Loop through all vertices in the graph
    {
        temp = g -> List[i] ;  //Get the adjacency list of the current vertex
        printf("%s: " , g -> buildings[i]) ;
        while(temp != NULL)  //Loop through the adjacency list
        {
            printf(" -> %s (%d)" , g -> buildings[temp -> dest] , temp -> weight) ;  //Print the connected building and weight
            temp = temp -> Next ;  //Move to the next node in the adjacency list
        }

        printf("\n") ;
    }
}

struct CGraph *createCGraph()
{
    struct CGraph *g ;
    int i ;

    g = (struct CGraph *)malloc(sizeof(struct CGraph)) ;  //Allocate memory for the graph
    g -> number_of_courses = 0 ;  //initially no courses
    for(i = 0 ; i < Max_number_of_courses ; i++)
    {
        g -> List[i] = NULL ;  //Initialize all adjacency lists to NULL
    }
    return g ;
}

void load_Course(struct CGraph *g , char *file_name)
{
    FILE *input_file ;
    char Line[100] ;
    char *token ;
    char course[Max_Name_Lenght] ;

    input_file = fopen(file_name , "r") ;  //open input file
    if(input_file == NULL)
    {
        printf("No file with such a name!\n") ;
        return ;
    }

    while(fgets(Line , sizeof(Line) , input_file) != NULL)  //read each line from input file
    {
        token = strtok(Line , "#") ;  //split the line into tokens using '#' as delimiter
        if(token == NULL)
        {
            printf("invalid input format!\n") ;
            fclose(input_file) ;  //Close the input file
            return ;
        }

        strcpy(course ,token) ;  //Copy the first token to course
        token = strtok(NULL , "#") ;

        while(token)  //Loop through the remaining tokens
        {
            CAddEdge(g , course , token) ;  //Add an edge from course to token (prerequisite)
            token = strtok(NULL , "#") ;
        }
    }

    fclose(input_file) ;  //Close the input file
}

void CAddEdge(struct CGraph *g , char *src , char *dest)
{
    int srcI ;
    int destI ;
    struct CNode *newN ;

    srcI = findCourseIndex(g , src) ;  //Find the index of the source course

    if(srcI == -1)  //If source course isn't found
    {
        srcI = (g -> number_of_courses)++ ;  //Assign a new index and increment the number of courses
        strcpy(g -> courses[srcI] , src) ;  //Copy the source course name to the graph
    }

    destI = findCourseIndex(g , dest) ;  //Find the index of the destination course
    if(destI == -1)  //If destination course isn't found
    {
        destI = (g -> number_of_courses)++ ;  //Assign a new index and increment the number of courses
        strcpy(g -> courses[destI] , dest) ;  //Copy the destination course name to the graph
    }

    newN = createCNode(dest) ;  //Create a new course node
    newN -> Next = g -> List[srcI] ;  //Point the new node to the current adjacency list of srcI
    g -> List[srcI] = newN ;  //Set the new node as the head of the adjacency list of srcI

}

struct CNode *createCNode(char *course)
{
    struct CNode *newN ;

    newN = (struct CNode *)malloc(sizeof(struct CNode)) ;  //Allocate memory for the new node
    strcpy(newN -> course , course) ;  //Copy the course name to the new node
    newN -> Next = NULL ;  //Initialize the next pointer to NULL

    return newN ;
}

int findCourseIndex(struct CGraph *g , char *course)
{
    int i ;

    for(i = 0 ; i < (g -> number_of_courses) ; i++)  //Loop through all courses in the graph
    {
        if(strcmp(g -> courses[i] , course) == 0)  //Compare the course name with the current course
        {
            return i ;  //Return the index if found
        }
    }

    return -1 ;  //Return -1 if course isn't found
}

void printCfile(struct CGraph *g)
{
    int i ;
    struct CNode *temp ;

    printf("courses file:\n") ;

    printf("  course      Prerequisites\n") ;  //Print the headers

    for(i = 0 ; i < (g -> number_of_courses) ; i++)  //Loop through all courses in the graph
    {
        temp = g -> List[i] ;  //Get the adjacency list of the current course
        while(temp != NULL)  //Loop through the adjacency list
        {
            printf(" %-12s   %-14s   \n" , g -> courses[i] , temp -> course) ;  //Print the course and its prerequisite
            temp = temp -> Next ;  //Move to the next node in the adjacency list
        }

        printf("\n") ;
    }
}

void printCGraph(struct CGraph *g)
{
    int i ;
    struct CNode *temp ;

    printf("Graph:\n") ;

    for(i = 0 ; i < (g -> number_of_courses) ; i++)  //// Loop through all courses in the graph
    {
        temp = g -> List[i] ;  //Get the adjacency list of the current course
        printf("%s " , g -> courses[i]) ;
        while(temp != NULL)  //Loop through the adjacency list
        {
            printf(" -> %s" , temp -> course) ;  //Print the prerequisite course
            temp = temp -> Next ;  //Move to the next node in the adjacency list
        }

        printf("\n") ;
    }
}

void DijkstraAlgorithm(struct BGraph *g , int src , int dest)
{
    if (src >= g -> number_of_vertices || dest >= g -> number_of_vertices || src < 0 || dest < 0)  //Check if the source or destination vertices are out of bounds
    {
        printf("Invalid source or destination!\n") ;
        return ;
    }

    int distance[Max_number_of_Buildings] ;  //Array to store shortest distance from source to each vertex
    int parent[Max_number_of_Buildings] ;  //Array to store the parent of each vertex in the shortest path tree
    struct Queue *q ;
    int i ;
    struct Qnode node ;
    int u ;

    q = createQ(Max_number_of_Buildings) ;  //create queue

    for(i = 0 ; i < g -> number_of_vertices ; i++)  //Initialize distance and parent arrays
    {
        distance[i] = INT_MAX ;  //Set all distances to infinity
        parent[i] = -1 ;  //Set all parents to -1 (no parent)
    }

    distance[src] = 0 ;  //Distance to source is 0
    Enqueue(q , src , 0) ;  //Enqueue source vertex with distance 0

    while (q -> size > 0)  //Main loop to process all vertices in the priority queue
    {
        struct Qnode node = Dequeue(q);  //Dequeue the vertex with the smallest distance
        u = node.vertex ;  //Dequeue the vertex with the smallest distance

        struct BNode *temp = g -> List[u] ;  //Get the adjacency list of the vertex
        while (temp != NULL)
        {
            int v = temp -> dest ;  //Get the destination vertex
            int weight = temp -> weight ;  //Get the weight of the edge

            if (distance[u] != INT_MAX && distance[u] + weight < distance[v])  //Check if there is a shorter path to v through u
            {
                distance[v] = distance[u] + weight ;  //Update the distance to v
                parent[v] = u ;  //Update the parent of v
                Enqueue(q , v , distance[v]) ;  //Enqueue v with the updated distance
            }
            temp = temp -> Next ;  //Move to the next adjacent vertex
        }
    }

    if (distance[dest] == INT_MAX)  //Check if there is no path between source and destination
    {
        printf("There is no path between %s and %s\n", g->buildings[src], g->buildings[dest]);
    }
    else
    {
        printf("The shortest distance between %s and %s is: %d\n", g -> buildings[src] , g -> buildings[dest] , distance[dest]);
        printf("Path: ") ;
        int stack[Max_number_of_Buildings] ;  //Stack to store the path
        int top = -1 ;  //Top of the stack
        int crawl = dest ;  //Start from the destination

        while (parent[crawl] != -1)  //Trace the path from destination to source using parent array
        {
            stack[++top] = crawl ;  //Push the vertex to the stack
            crawl = parent[crawl] ;  //Move to the parent
        }

        stack[++top] = src ;  //Push the source to the stack

        while (top >= 0)
        {
            printf("%s ", g-> buildings[stack[top--]]);  //print
            if (top >= 0) printf("-> ");
        }
        printf("\n");
    }

    free(q -> node) ;  //Free the memory allocated for the nodes in the queue
    free(q) ;  //Free the memory allocated for the queue
}

int minDistance(int dist[] , int sptSet[] , int v)
{
    int min ;
    int min_index ;
    int i ;

    min = INT_MAX ;  //Initialize the minimum value to infinity

    for(i = 0 ; i < v ; i++)  //Loop through all vertices to find the minimum distance vertex
    {
        if(sptSet[i] == 0 && dist[i] <= min)
        {
            min = dist[i] ;  //Update the minimum value
            min_index = i ;  //Update the index of the minimum value
        }
    }

    return min_index ;  //Return the index of the minimum distance vertex
}

void swap(struct Qnode *a , struct Qnode *b)
{
    struct Qnode temp ;

    temp = *a ;  //Store the value of a in temp
    *a = *b ;  //Copy the value of b to a
    *b = temp ;  //Copy the value of temp to b
}

void min(struct Queue *q , int idx)
{
    int smallest ;
    int left ;
    int right ;

    smallest = idx ;  //Initialize smallest as the index
    left = 2 * idx + 1 ;  //Calculate left child index
    right = 2 * idx + 2 ;  //Calculate right child index

    if(left < q -> size && q -> node[left].distance < q -> node[smallest].distance)  //Check if left child exists and is smaller than the current smallest
    {
        smallest = left ;
    }

    if(right < q -> size && q -> node[right].distance < q -> node[smallest].distance)  //Check if right child exists and is smaller than the current smallest
    {
        smallest = right ;
    }

    if(smallest != idx)  //If the smallest is not the current index, swap and heapify recursively
    {
        swap(&q -> node[idx] , &q -> node[smallest]) ;
        min(q , smallest) ;
    }
}

void Enqueue(struct Queue *q , int vertex , int distance)
{
    if(q -> size == q -> capacity)  //Check if the queue is full
    {
        printf("priority queue overflow!\n") ;
        return ;
    }

    int i ;

    i = (q -> size)++ ;  //Get the current size and increment it
    q -> node[i].vertex = vertex ;  //Set the vertex
    q -> node[i].distance = distance ;  //Set the distance

    while(i && q -> node[i].distance < q -> node[(i - 1) / 2].distance)  //Fix the min heap property if it is violated
    {
        swap(&q -> node[i] , &q -> node[(i - 1) / 2]) ;  //Swap with the parent
        i = (i - 1) / 2 ;  //Move to the parent index
    }
}


struct Qnode Dequeue(struct Queue *q)
{
    struct Qnode r ;

    if(q -> size == 0)  //Check if the queue is empty
    {
        struct Qnode e = {-1 , -1} ;  //Return a dummy node if the queue is empty
        return e ;
    }

    r = q -> node[0] ;  //Get the root element
    q -> node[0] = q -> node[--(q -> size)] ;  //Replace root with the last element and decrease the size
    min(q , 0) ;  //Heapify the root element

    return r ;  //Return the removed element
}

struct Queue *createQ(int capacity)
{
    struct Queue *q ;

    q = (struct Queue *)malloc(sizeof(struct Queue)) ;  //Allocate memory for the queue
    q -> node = (struct Qnode *)malloc(sizeof(struct Qnode) * capacity) ;  //Allocate memory for the nodes
    q -> size = 0 ;  //Initialize the size to 0
    q -> capacity = capacity ;  //Set the capacity

    return q ;
}

void topologicalSort(struct CGraph *g)
{
    int visited[Max_number_of_courses] ;  //Array to keep track of visited nodes
    int stack[Max_number_of_courses] ;  //Stack to store the topologically sorted order
    int stackIndex = 0 ;  //Index to keep track of the stack position
    int i ;

    for (i = 0; i < g -> number_of_courses ; i++)
    {
        visited[i] = 0 ;  ////Initialize all nodes as not visited
    }

    for (int i = 0 ; i < g -> number_of_courses ; i++)  //Call the recursive helper function for all nodes that have not been visited
    {
        if (visited[i] == 0)
        {
            ts(g , i , visited , stack , &stackIndex) ;
        }
    }
}

//Recursive helper function for topological sorting:
void ts(struct CGraph *g , int v , int visited[] , int *stack , int *stackI)
{
    struct CNode *temp ;

    visited[v] = 1 ;  //Mark the current node as visited

    temp = g -> List[v] ;  //Get the adjacency list of the current node
    while (temp != NULL)
    {
        int adj = findCourseIndex(g , temp -> course);  //Get the index of the adjacent node
        if (!visited[adj])  //If the adjacent node is not visited, recurse for that node
        {
            ts(g , adj , visited , stack , stackI);
        }
        temp = temp -> Next ;  //Move to the next adjacent node
    }

    stack[(*stackI)++] = v ;  //Push the current node to the stack after processing all its adjacent nodes
}
