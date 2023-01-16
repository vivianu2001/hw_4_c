#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

int main(){
    pnode node = NULL;
    pnode *head = &node;
    char c= 'v';
    while (scanf("%c", &c) != EOF)
    {
        if (c == 'A'){
            build_graph_cmd(head);
        }
        else if (c == 'B'){
            insert_node_cmd(head);
        }
        else if (c == 'D'){
            delete_node_cmd(head);
        }
        else if (c == 'S'){
            int src = -1;
            int dest = -1;
            
            scanf("%d", &src);
            scanf("%d",&dest);
            int shortest = shortsPath_cmd(*head,src,dest);
            printf("Dijsktra shortest path: %d \n", shortest);
        }
        else if (c == 'T'){
            int tsp = TSP_cmd(*head);
            printf("TSP shortest path: %d \n", tsp);
        }
    }
    deleteGraph_cmd(head);
    return 0;
}
