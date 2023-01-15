#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"


/*
The function initializes a linked list, head, and a current pointer, current, to NULL. It then iterates through
 the linked list of nodes passed in as the h_node parameter. For each node in the list, it creates a new node in the Dijkstra graph representation using the malloc() function, and assigns the current node in the original graph as the node attribute.

If the current node in the original graph is the starting node (i.e. its node number is equal to data),
 it sets the prev attribute of the new Dijkstra graph node to itself and sets its weight attribute to 0. 
 Otherwise, it sets the prev attribute to NULL and the weight attribute to INF (infinity).
It also sets the iNside attribute to 0 and the next attribute to NULL.
If head is NULL, it sets the new node as the head of the Dijkstra graph linked list
, and if current is not NULL, it sets the next attribute of the current node to the new node.
Finally, it updates the current pointer to the new node and continues iterating through the original graph linked list until it reaches the end.
Once all the node in the graph are processed, the function returns the head of the Dijkstra graph linked list.
*/


pnode graph= NULL; //global graph for tsp
int weight=0; //global weight for tsp


/*
uses the free() function to deallocate the memory allocated to the current node.
*/

void delete_list(pdijk list) {
    for (pdijk curr_n = list; curr_n != NULL; curr_n = list) {
        list = list->next;
        free(curr_n);
    }
}


/*
The build_dijkstra_graph function is used to initialize the graph and set the initial distances and previous nodes for each node in the graph.
*/


pdijk build_dijkstra_graph(pnode h_node, int data) {
    pdijk head = NULL;
    pdijk current = NULL;
    while (h_node != NULL) {
        pdijk new_node = (pdijk)malloc(sizeof(dijk));
        if (new_node == NULL) {
            return NULL;
        }
        new_node->node = h_node;
        new_node->prev = h_node->node_num == data ? new_node : NULL;
        new_node->weight = h_node->node_num == data? 0 : INF;
        new_node->iNside = 0;
        new_node->next = NULL;
        if (head == NULL) {
            head = new_node;
        }
        if (current != NULL) {
            current->next = new_node;
        }
        current = new_node;
        h_node = h_node->next;
    }
    return head;
}





/*
The getD_node function is used to retrieve a specific node from the list of nodes that the algorithm is currently processing.
*/

pdijk getD_node(int x, pdijk list){
    while (list != NULL){
        if (list->node->node_num == x){
            return list;
        }
        list = list->next;
    }
    return NULL;
}


/*
The find_min function is used to find the node with the smallest distance that has not yet been processed.
*/
pdijk find_min(pdijk head){
    pdijk result = NULL;
    while (head != NULL){
        if (!head->iNside && head->weight < INF  && (result == NULL || result->weight < head->weight)){
            result = head;
        }
        head = head->next;
    }
    if (result != NULL){
        result->iNside = 1;
    }
    return result;
}



/*
Initialize the graph and set the initial distances and previous nodes for each node in the graph, using the build_dijkstra_graph function.
While there are still unprocessed nodes in the graph:
Find the node with the smallest distance that has not yet been processed, using the find_min function.
For each edge connected to the current node:
Get the destination node of the edge, using the getD_node function.
Calculate the new distance to the destination node by adding the weight of the current node and the weight of the edge.
If the new distance is less than the current distance of the destination node, update the distance and set the previous node of the destination node to the current node.
Get the final distance to the destination node using the getD_node function, 
and check if the distance is equal to infinity (INF) or not.
If the distance is infinity, return -1, otherwise return the final distance.
Finally, use the delete_list function to clean up and deallocate memory used by the algorithm.
*/


int shortsPath_cmd(pnode head, int src, int dest){  
    pdijk list = build_dijkstra_graph(head, src);
    pdijk node_min = find_min(list);
    while (node_min != NULL){
        pedge build_edge = node_min->node->edges;
        while (build_edge != NULL){
            pdijk node_dijk = getD_node(build_edge->dest->node_num,list);
            int dist = node_min->weight + build_edge->weight;
            if (node_dijk->weight > dist){
                node_dijk->weight = dist;
                
                node_dijk->prev = node_min;
            }
            build_edge = build_edge->next;
        }
        node_min = find_min(list);
    }
    int result = getD_node(dest,list)->weight;
    result = (result == INF )? -1: result;
    delete_list(list);
    return result;
}




/*
The function then iterates through the array, starting at index 0 and ending at index len-1-1. For each iteration,
 it calls the shortsPath_cmd function passing the graph variable, and the current node and the next node in the generate_permutations as arguments.
The shortsPath_cmd function returns the shortest path distance between two nodes.
*/


void calculate_distance(int* arr, int len){
    int curr_d = 0;
    for (int i = 0; i < len-1; ++i){
        int dist = shortsPath_cmd(graph,arr[i], arr[i+1]);
        if (dist == -1){
            curr_d = INF;
            return;
        }
        curr_d += dist;
    }
    if (curr_d < weight){
        weight = curr_d;
    }
}


void swap(int* arr, int i, int j){
    int temp = arr[i];
    arr[i]= arr[j];
    arr[j] = temp;
}

/*
The "generate_permutations" function generates all the possible permutations of the TSP path. It takes in the current index,
 an array of integers, and its length. It uses recursion to generate all the possible permutations. 
 If the current index is equal to the length -1, it calls the "calculate_distance" function to calculate the distance of the TSP path. 
 Otherwise, it iterates through the remaining elements and swaps the current element with each of the remaining elements,
  generates permutations for the resulting array, and then frees the allocated memory.
*/

void generate_permutations(int i ,int* arr, int len){
    if (i == len -1 ){
        calculate_distance(arr, len);
        return;
    }
    for (int j = i; j < len; ++j) {
        int* temp_arr = (int*)(calloc(len, sizeof(int)));
        if(temp_arr==NULL)
        {
            break;
        }
        memcpy(temp_arr,arr, len * sizeof(int));
        swap(temp_arr,i, j);
        generate_permutations(i + 1, temp_arr, len);
        free(temp_arr);
    }
}
/*
a function in C that solves the Traveling Salesman Problem (TSP) using permutation. 
The function takes in a pointer to a node (pnode head) and initializes several variables, 
including weight and the length of the TSP (len). It then dynamically allocates memory for two arrays:
 "arr" and "temp_arr" using the "calloc" function. The function then reads in the length of the TSP and the values for the "arr" array using scanf
 . It then makes a copy of the "arr" array to "temp_arr" using memcpy. The function then calls the "generate_permutations" function
 , passing in the starting index, the "temp_arr" array, and the length of the TSP. After the permutations are generated,
  the function frees the memory allocated for both arrays and checks if the final weight is INF, in which case it sets the weight to -1. 
  The function then returns the final weight.
*/

int TSP_cmd(pnode head){   
     weight = INF;
	  int len = 0;
     graph = head;
    scanf("%d", &len);
    int *arr = (int *)(calloc(len, sizeof(int)));
    if(arr==NULL)
    {
        return -1;
    }

    for (int i = 0; i < len; i++) {
        scanf("%d", &arr[i]);
    }

    int *temp_arr = (int *)(calloc(len, sizeof(int)));
    if(temp_arr==NULL)
    {
        return -1;
    }
    memcpy(temp_arr,arr, len * sizeof(int));
    generate_permutations(0,temp_arr,len);
    free(arr);
    free(temp_arr);
    if (weight == INF){
        weight = -1;
    }
    return weight;
}


