#ifndef _RRDP_h
#define _RRDP_h

#include "Heap_Dijkstra.h"

/**
 * @brief Traces the changes caused by the vertices in the queue,
 * Since their tentative shortest path value are reduced.
 * 
 * @param Q The queue of vertices to be processed.
 * @param is_present A boolean array indicating the presence of vertices.
 * @param D An array holding the shortest distances from the source vertex.
 * @param P An array holding the predecessor of each vertex in the shortest path.
 */
void trace_change(queue<int> &Q, bool* is_present, int* D, int* P) {
    int cur_vtx, tmp_cost;

    while (!Q.empty()) { // Process the queue until it's empty
        cur_vtx = Q.front(); Q.pop(); // Pop a vertex from the queue

        // Iterate through the neighbors of the current vertex
        for (int next_vertex : g_neighbor[cur_vtx]) {
            if (!is_present[next_vertex]) continue; // Skip if the vertex is not present

            // Calculate the new cost to reach the vertex
            tmp_cost = D[cur_vtx] + g_arcs[cur_vtx][next_vertex];
            // Update if the new cost is lower
            if (tmp_cost < D[next_vertex]) {
                D[next_vertex] = tmp_cost;
                P[next_vertex] = cur_vtx;
                Q.push(next_vertex); // Add the vertex to the queue for further processing
            }
        }    
    }
}

/**
 * @brief Implements the RRDP algorithm to find shortest paths from a source vertex.
 * 
 * @param D An array holding the shortest distances from the source vertex.
 * @param P An array holding the predecessor of each vertex in the shortest path.
 */
void rrdp(int* D, int* P) {
    bool* is_present = new bool[g_n_vtx]; // Track if vertices are present
    memset(is_present, false, g_n_vtx * sizeof(bool)); // Initialize to false

    int* row_lengths = new int[g_dim];  // Arrays to hold lengths of horizontal edges among one row
    int* col_lengths = new int[g_dim];  // Arrays to hold lengths of vertical edges between two rows

    queue<int> Q; // Queue for processing vertices
    int col, row;
    int tmp_cost1, tmp_cost2;
    int vtx, tmp_vtx1, tmp_vtx2;
    int pre_begin_vtx, cur_begin_vtx;

    // Initialize the source vertex distance and predecessor
    D[g_src_vtx] = 0;
    P[g_src_vtx] = g_src_vtx;
    is_present[g_src_vtx] = true;

    // *****************************************************************************************************
    // Process the row containing the source vertex
    // Process from right to left
    vtx = g_src_vtx - 1;    
    tmp_vtx1 = g_src_vtx;
    for (col = g_src_vtx_col - 1; col >= 0; --col) {
        is_present[vtx] = true;

        // Update distance and predecessor for the vertex
        D[vtx] = D[tmp_vtx1] + g_arcs[tmp_vtx1][vtx];    
        P[vtx] = tmp_vtx1;
        --tmp_vtx1, --vtx;
    }

    // Process from left to right
    vtx = g_src_vtx + 1;
    tmp_vtx1 = g_src_vtx;
    for (col = g_src_vtx_col + 1; col < g_dim; ++col) {
        is_present[vtx] = true;

        // Update distance and predecessor for the vertex
        D[vtx] = D[tmp_vtx1] + g_arcs[tmp_vtx1][vtx];
        P[vtx] = tmp_vtx1;
        ++tmp_vtx1, ++vtx;
    }
    // *****************************************************************************************************

    // *****************************************************************************************************
    // Process the rows below the source vertex
    cur_begin_vtx = g_src_vtx - g_src_vtx_col;
    for (row = g_src_vtx_row + 1; row < g_dim; ++row) {
        // Update the first vertices in rows that just be processed and will be processed
        pre_begin_vtx = cur_begin_vtx;
        cur_begin_vtx += g_dim;

        is_present[cur_begin_vtx] = true; // Mark the vertex as present
        col_lengths[0] = g_arcs[pre_begin_vtx][cur_begin_vtx]; // Record the column length
        // Update distance and predecessor for the first vertex in this row
        D[cur_begin_vtx] = D[pre_begin_vtx] + col_lengths[0];
        P[cur_begin_vtx] = pre_begin_vtx;

        // Process from left to right
        vtx = cur_begin_vtx;
        tmp_vtx2 = pre_begin_vtx;
        for (col = 1; col < g_dim; ++col) {
            tmp_vtx1 = vtx++;
            ++tmp_vtx2;

            is_present[vtx] = true; // Mark the vertex as present
            row_lengths[col] = g_arcs[vtx][tmp_vtx1]; // Record the row length
            col_lengths[col] = g_arcs[vtx][tmp_vtx2]; // Record the column length
            tmp_cost1 = D[tmp_vtx1] + row_lengths[col]; // New cost from the left neighbor
            tmp_cost2 = D[tmp_vtx2] + col_lengths[col]; // New cost from above neighbor
            // Update based on the lower cost
            if (tmp_cost1 < tmp_cost2) {
                D[vtx] = tmp_cost1;  P[vtx] = tmp_vtx1;
            } else {
                D[vtx] = tmp_cost2;  P[vtx] = tmp_vtx2;
            }
        }

        // For the vertex in the previous row, calculate the new cost from the below neighbor
        tmp_cost2 = D[vtx] + col_lengths[--col];
        if (tmp_cost2 < D[tmp_vtx2]) {	// Update if the new cost is lower
            D[tmp_vtx2] = tmp_cost2; P[tmp_vtx2] = vtx;
            Q.push(tmp_vtx2);
        }

        // Process from right to left
        for (; col > 0;) {
            tmp_vtx1 = vtx--;
            --tmp_vtx2;

            tmp_cost1 = D[tmp_vtx1] + row_lengths[col]; // New cost from the right neighbor
            if (tmp_cost1 < D[vtx]) {
                D[vtx] = tmp_cost1;  P[vtx] = tmp_vtx1;
            }

			// For the vertex in the previous row
            tmp_cost2 = D[vtx] + col_lengths[--col]; // New cost from below neighbor
            if (tmp_cost2 < D[tmp_vtx2]) {	// Update if the new cost is lower
                D[tmp_vtx2] = tmp_cost2; P[tmp_vtx2] = vtx;
                Q.push(tmp_vtx2);
            }
        } 
        trace_change(Q, is_present, D, P); // Update distances using trace_change
    }
    // *****************************************************************************************************

    // *****************************************************************************************************
    // Process the rows above the source vertex
    cur_begin_vtx = g_src_vtx - g_src_vtx_col;
    for (row = g_src_vtx_row - 1; row >= 0; --row) {
		// Update the first vertices in rows that just be processed and will be processed
        pre_begin_vtx = cur_begin_vtx;
        cur_begin_vtx -= g_dim;

        is_present[cur_begin_vtx] = true; // Mark the vertex as present
        col_lengths[0] = g_arcs[pre_begin_vtx][cur_begin_vtx]; // Record the column length
        // Update distance and predecessor for the first vertex in this row
        D[cur_begin_vtx] = D[pre_begin_vtx] + col_lengths[0];
        P[cur_begin_vtx] = pre_begin_vtx;

        // Process from left to right
        vtx = cur_begin_vtx;
        tmp_vtx2 = pre_begin_vtx;
        for (col = 1; col < g_dim; ++col) {
            tmp_vtx1 = vtx++;
            ++tmp_vtx2;

            is_present[vtx] = true; // Mark the vertex as present
            row_lengths[col] = g_arcs[vtx][tmp_vtx1]; // Record the row length
            col_lengths[col] = g_arcs[vtx][tmp_vtx2]; // Record the column length
            tmp_cost1 = D[tmp_vtx1] + row_lengths[col]; // New cost from the left neighbor
            tmp_cost2 = D[tmp_vtx2] + col_lengths[col]; // New cost from below neighbor
            // Update based on the lower cost
            if (tmp_cost1 < tmp_cost2) {
                D[vtx] = tmp_cost1;  P[vtx] = tmp_vtx1;
            } else {
                D[vtx] = tmp_cost2;  P[vtx] = tmp_vtx2;
            }
        }

        // For the vertex in the previous row, calculate the new cost from the above neighbor
        tmp_cost2 = D[vtx] + col_lengths[--col];
        if (tmp_cost2 < D[tmp_vtx2]) {	// Update if the new cost is lower
            D[tmp_vtx2] = tmp_cost2; P[tmp_vtx2] = vtx;
            Q.push(tmp_vtx2);
        }

        // Process from right to left
        for (; col > 0;) {
            tmp_vtx1 = vtx--;
            --tmp_vtx2;

            tmp_cost1 = D[tmp_vtx1] + row_lengths[col]; // New cost from the right neighbor
            if (tmp_cost1 < D[vtx]) {
                D[vtx] = tmp_cost1;  P[vtx] = tmp_vtx1;
            }

			// For the vertex in the previous row
            tmp_cost2 = D[vtx] + col_lengths[--col]; // New cost from above neighbor
            if (tmp_cost2 < D[tmp_vtx2]) {	// Update if the new cost is lower
                D[tmp_vtx2] = tmp_cost2; P[tmp_vtx2] = vtx;
                Q.push(tmp_vtx2);
            }
        } 
        trace_change(Q, is_present, D, P); // Update distances using trace_change
    }
    // *****************************************************************************************************

    // Free allocated memory
    delete[] is_present;
    delete[] col_lengths;
    delete[] row_lengths;
}

#endif // !_RRDP_h
