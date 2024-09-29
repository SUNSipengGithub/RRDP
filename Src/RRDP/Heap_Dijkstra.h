#ifndef _Heap_Dijkstra_h
#define _Heap_Dijkstra_h

#include <queue>
#include <cstring>

#include "Global_Var.h"

// Structure representing a node in the priority queue
struct t_queue_Node {
    int vex; // Vertex index
    int dis; // Distance from the source vertex

    t_queue_Node(int v, int d) : vex(v), dis(d) {}

    // Overload the '<' operator to compare nodes based on distance
    bool operator<(const t_queue_Node& other) const {
        return dis > other.dis;
    }
};

/**
 * @brief Dijkstra's algorithm implementation using a priority queue.
 * 
 * @param D An array holding the shortest distances from the source vertex.
 * @param P An array holding the predecessor of each vertex in the shortest path.
 */
void dijkstra_heap(int* D, int* P) {
    priority_queue<t_queue_Node> pq; // Min-heap to store vertices based on distance
    vector<bool> is_sure(g_n_vtx, false); // Array to track finalized vertices

    // Initialize all distances to infinity
    for (int i = 0; i < g_n_vtx; i++) D[i] = INF;

    // Set distance for the source vertex and its predecessor
    D[g_src_vtx] = 0;
    P[g_src_vtx] = g_src_vtx;
    
    pq.push(t_queue_Node(g_src_vtx, 0));    // Push the source vertex into the priority queue
    while (!pq.empty()) {   // Process the priority queue until it's empty
        // Get the vertex with the smallest distance from the queue
        t_queue_Node current = pq.top();    pq.pop();
        int sure_vex = current.vex;

        if (is_sure[sure_vex]) continue;    // If this vertex is already finalized, skip it
        is_sure[sure_vex] = true;   // Mark the current vertex as finalized

        // Iterate through the neighbors of the current vertex
        for (int vex : g_neighbor[sure_vex]) {
            if (is_sure[vex]) continue; // Skip if the neighbor is already finalized

            // Calculate the new distance
            int new_dis = D[sure_vex] + g_arcs[sure_vex][vex];
            if (new_dis < D[vex]) { // If the new distance is shorter, update it
                D[vex] = new_dis;
                P[vex] = sure_vex;
                pq.push(t_queue_Node(vex, new_dis)); // Push the updated distance into the queue
            }
        }
    }
}

#endif // !_Heap_Dijkstra_h
