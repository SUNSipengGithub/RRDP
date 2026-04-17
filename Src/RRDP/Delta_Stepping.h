#ifndef _Delta_Stepping_h
#define _Delta_Stepping_h

#include <vector>
#include <algorithm>
#include "Global_Var.h"


namespace delta_stepping_detail {
    inline int choose_delta() {
        long long sum_w = 0;
        long long cnt_w = 0;

        for (int u = 0; u < g_n_vtx; ++u) {
            for (int v : g_neighbor[u]) {
                int w = g_arcs[u][v];
                if (w > 0) {
                    sum_w += w;
                    ++cnt_w;
                }
            }
        }

        if (cnt_w == 0) return 1;

        int delta = (int)(sum_w / cnt_w);
        if (delta <= 0) delta = 1;
        return delta;
    }

    inline void ensure_bucket(std::vector<std::vector<int>>& B, int idx) {
        if (idx >= (int)B.size()) B.resize(idx + 1);
    }

    inline void relax_vertex(
        int v,
        int new_dist,
        int parent,
        int delta,
        int* D,
        int* P,
        std::vector<std::vector<int>>& B
    ) {
        if (new_dist >= D[v]) return;

        D[v] = new_dist;
        P[v] = parent;

        int b = D[v] / delta;
        ensure_bucket(B, b);
        B[b].push_back(v);
    }

    inline int find_next_nonempty_bucket(
        const std::vector<std::vector<int>>& B,
        int start_idx
    ) {
        for (int i = start_idx; i < (int)B.size(); ++i) {
            if (!B[i].empty()) return i;
        }
        return -1;
    }
}

/**
 * @brief Delta-Stepping shortest path algorithm.
 *
 * @param D shortest distance array
 * @param P predecessor array
 */
void delta_stepping(int* D, int* P) {
    using namespace delta_stepping_detail;

    const int delta = 5000;
    // const int delta = choose_delta();

    for (int i = 0; i < g_n_vtx; ++i) {
        D[i] = INF;
        P[i] = -1;
    }
    D[g_src_vtx] = 0;
    P[g_src_vtx] = g_src_vtx;

    std::vector<std::vector<int>> B(1);
    B[0].push_back(g_src_vtx);

    std::vector<char> in_R(g_n_vtx, 0);

    int current_bucket = 0;

    while (true) {
        int i = find_next_nonempty_bucket(B, current_bucket);
        if (i == -1) break;

        current_bucket = i;

        std::vector<int> R;  // vertices settled from bucket i

        // Repeatedly process light edges until bucket i becomes empty
        while (!B[i].empty()) {
            std::vector<int> frontier;
            frontier.swap(B[i]);

            for (int u : frontier) {
                // stale entry check
                if (D[u] >= INF) continue;
                if (D[u] / delta != i) continue;

                if (!in_R[u]) {
                    in_R[u] = 1;
                    R.push_back(u);
                }

                // light edges: w <= delta
                for (int v : g_neighbor[u]) {
                    int w = g_arcs[u][v];
                    if (w <= delta) {
                        relax_vertex(v, D[u] + w, u, delta, D, P, B);
                    }
                }
            }
        }

        // After closure under light edges, process heavy edges once for R
        for (int u : R) {
            if (D[u] >= INF) continue;

            for (int v : g_neighbor[u]) {
                int w = g_arcs[u][v];
                if (w > delta) {
                    relax_vertex(v, D[u] + w, u, delta, D, P, B);
                }
            }
            in_R[u] = 0; // clear mark for next outer iteration
        }

        ++current_bucket;
    }
}

#endif // !_Delta_Stepping_h