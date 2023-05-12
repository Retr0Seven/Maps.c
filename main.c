#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CITIES 100

typedef struct city{
char name[50];
}city;

typedef struct node{
int vertex;
struct node *next;
}node;

typedef struct{
int numVer;
city cities[MAX_CITIES];
node *adjlist[MAX_CITIES];
}graph;

//this function allows to create a graph by initializing the adjacency list of each vertex to null
graph *creategraph(int numVer){
graph *g=(graph*)malloc(sizeof(graph)); 
g->numVer = numVer;
  for (int i=0 ; i<numVer; i++)
    {
      g->adjlist[i]=NULL;
    }
  return g;
}

//this function allows us to add an edge between a source and a destination both present in the graph which the function takes a pointer to
void addEdge(graph *g, int s, int d){
node *newnode = (node*)malloc(sizeof(node));
newnode->vertex = d;
newnode->next = g->adjlist[s];
g->adjlist[s] = newnode;

newnode = (node*)malloc(sizeof(node));
newnode->vertex = s;
newnode->next = g->adjlist[d];
g->adjlist[d] = newnode;
}

//this function allows to return the index of a city by comparing the name provided to the names of every index until the name provided is found, basically allowing us to know the index of any city in the graph by providing the name of that city
int find_city_index(graph *g, char *city_name) {
    for (int i = 0; i < g->numVer; i++) {
        if (strcmp(g->cities[i].name, city_name) == 0) {
            return i;
        }
    }

    return -1;
}

//this function allows to add a new vertex (city) to the graph by adding one to the number of vertices
void add_city(graph *g, char *city_name) {
    int num_vertices = g->numVer;
    strcpy(g->cities[num_vertices].name, city_name);
    g->numVer++;
}

//this function allows to create a connection between the cities by adding an edge using the already implemented addEdge() function between the two vertices (cities)
void addcon(graph *g, char *city1, char *city2) {
    int city1_index = find_city_index(g, city1);
    int city2_index = find_city_index(g, city2);

    if (city1_index != -1 && city2_index != -1) {
        addEdge(g, city1_index, city2_index);
    }
}


//this function takes a pointer to a graph, an integer vertex representing the starting vertex for the DFS, and an integer array that keeps track of which vertices have been visited
void dfs_helper(graph *g, int vertex, int *visited) {
    visited[vertex] = 1;
    printf("%s ", g->cities[vertex].name);

    node *adjlist = g->adjlist[vertex];

    while (adjlist != NULL) {
        int adjvertex = adjlist->vertex;

        if (!visited[adjvertex]) {
            dfs_helper(g, adjvertex, visited);
        }

        adjlist = adjlist->next;
    }
}

//this function takes the pointer to the graph, then checks the adjacency list of each vertex to check whether all vertices are connected, if they are then the graph is connected, it checks the adjacency list of the first city then proceeds to the next vertex
void isconnected_helper(graph *g, int vertex, int *visited) {
    visited[vertex] = 1;

    node *adjlist = g->adjlist[vertex];

    while (adjlist != NULL) {
        int adjvertex = adjlist->vertex;

        if (!visited[adjvertex]) {
            isconnected_helper(g, adjvertex, visited);
        }

        adjlist = adjlist->next;
    }
}

//this function takes the pointer to the graph and the pointer to the name of the first city visited, then proceeds to do a DFS search using the dfs_helper of the entire graph starting from the first city
void dfs(graph *g, char *st_city) {
    int visited[MAX_CITIES] = {0};
    int st_index = find_city_index(g, st_city);

    dfs_helper(g, st_index, visited);
}

//this function takes the pointer to the graph and the pointer to the name of the first city visited, then proceeds to do a BFS search of the entire graph starting from the first city
void bfs(graph *g, char *st_city) {
    int visited[MAX_CITIES] = {0};
    int st_index = find_city_index(g, st_city);

    // create a queue for BFS
    int queue[MAX_CITIES];
    int front = 0, rear = 0;

    visited[st_index] = 1;
    queue[rear++] = st_index;

    while (front != rear) {
        // dequeue a vertex from queue and print it
        int vertex = queue[front++];
        printf("%s ", g->cities[vertex].name);

        // get all adjacent vertices of the dequeued vertex
        node *adjlist = g->adjlist[vertex];

        while (adjlist != NULL) {
            int adjvertex = adjlist->vertex;

            if (!visited[adjvertex]) {
                visited[adjvertex] = 1;
                queue[rear++] = adjvertex;
            }

            adjlist = adjlist->next;
        }
    }
}

//this function takes a pointer to the graph and checks all connections to check wether the graph is connected, it returns true if it is and false if it isn't connected completely
bool is_connected(graph *g) {
    int visited[MAX_CITIES] = {0};
    isconnected_helper(g, 0, visited);

    for (int i = 0; i < g->numVer; i++) {
        if (!visited[i]) {
            return false;
        }
    }

    return true;
}


//this function takes a pointer to the graph and the name of 2 cities, and checks to see if there is a direct connection (edge) between them
bool is_connected_cities(graph *g, char *city1, char *city2) {
    int city1_index = find_city_index(g, city1);
    int city2_index = find_city_index(g, city2);

    if (city1_index == -1 || city2_index == -1) {
        return false;
    }

    int visited[MAX_CITIES] = {0};

    node *adjlist = g->adjlist[city1_index];
    while (adjlist != NULL) {
        int adjvertex = adjlist->vertex;
        if (adjvertex == city2_index) {
            return true;
        }
        adjlist = adjlist->next;
    }

    return false;
}


//this function takes the pointer the the graph and proceeds to print out the graph while also printing the connections of each city in the graph
void display(graph *g) {
    printf("List of cities:\n");
    for (int i = 0; i < g->numVer; i++) {
        printf("%d. %s\n", i+1, g->cities[i].name);
    }

    printf("\nConnections:\n");
    for (int i = 0; i < g->numVer; i++) {
        node *adjlist = g->adjlist[i];
        printf("%s: ", g->cities[i].name);
        while (adjlist != NULL) {
            printf("%s ", g->cities[adjlist->vertex].name);
            adjlist = adjlist->next;
        }
        printf("\n");
    }
}


int main() {
    graph *g = creategraph(0);
    char city_name[50];
    int choice;

    do {
        printf("\n1. Add city\n");
        printf("2. Add connection\n");
        printf("3. Display graph\n");
        printf("4. DFS traversal\n");
        printf("5. BFS traversal\n");
        printf("6. Check if graph is connected\n");
        printf("7. Check if two cities are connected\n");
        printf("8. Exit\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nEnter city name: ");
                scanf("%s", city_name);
                add_city(g, city_name);
                break;
            case 2:
                printf("\nEnter first city name: ");
                scanf("%s", city_name);
                char city1[50];
                strcpy(city1, city_name);

                printf("Enter second city name: ");
                scanf("%s", city_name);
                char city2[50];
                strcpy(city2, city_name);

                addcon(g, city1, city2);
                break;
            case 3:
                display(g);
                break;
            case 4:
                printf("\nEnter starting city name: ");
                scanf("%s", city_name);
                dfs(g, city_name);
              if(is_connected(g) != 1){
                 printf("\nRemark: the graph is not connected");
               }
          break;
            case 5:
                printf("\nEnter starting city name: ");
                scanf("%s", city_name);
                bfs(g, city_name);
               if(is_connected(g) != 1){
                 printf("\nRemark: the graph is not connected");
               }
                break;
            case 6:
                if (is_connected(g)) {
                    printf("\nGraph is connected.\n");
                } else {
                    printf("\nGraph is not connected.\n");
                }
                break;
            case 7:
                printf("\nEnter first city name: ");
                scanf("%s", city_name);
                strcpy(city1, city_name);

                printf("Enter second city name: ");
                scanf("%s", city_name);
                strcpy(city2, city_name);

                if (is_connected_cities(g, city1, city2)) {
                    printf("\n%s and %s are connected.\n", city1, city2);
                } else {
                    printf("\n%s and %s are not connected.\n", city1, city2);
                }
                break;
            case 8:
                printf("\nExiting...\n");
                break;
            default:
                printf("\nInvalid choice. Try again.\n");
                break;
        }
    } while (choice != 8);

    return 0;
}


