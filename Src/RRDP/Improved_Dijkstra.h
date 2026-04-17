#ifndef _Improved_Dijkstra_h
#define _Improved_Dijkstra_h

#include <vector>
#include <cstring>
#include <algorithm>

#include "Global_Var.h"

/*
 * 论文风格实现要点：
 * 1. 用有序表 order[] 维护当前所有顶点，按 D 非降序排列
 * 2. 用 position[v] 记录顶点 v 在 order[] 中的位置
 * 3. 每轮 low 向后推进，order[low] 即当前最小未确定点
 * 4. 对被成功松弛的顶点 v，只在 [low + 1, old_pos - 1] 中查找新位置
 * 5. 用预处理好的 step_size 做“无除法”的定位搜索
 */

namespace improved_dijkstra_detail {

    // step_size[len] = 不超过 len 的最大 2 的幂
    // 用于 Process 1 风格的步长预处理
    static std::vector<int> step_size_cache;
    static int step_size_cache_n = -1;

    inline void build_step_size(int n) {
        if (step_size_cache_n == n) return;

        step_size_cache.assign(n + 1, 0);
        step_size_cache[0] = 0;
        if (n >= 1) step_size_cache[1] = 1;

        int p = 1;
        for (int len = 1; len <= n; ++len) {
            while ((p << 1) <= len) p <<= 1;
            step_size_cache[len] = p;
        }
        step_size_cache_n = n;
    }

    /*
     * 在 order[left..right] 中找 upper_bound(target)：
     * 返回第一个使 D[order[pos]] > target 的位置。
     *
     * 这里不用常规 mid=(l+r)/2，而是用 step_size 做步进搜索，
     * 更接近论文 Process 1 + Process 2 的思路。
     */
    inline int locate_insert_position(
        const std::vector<int>& order,
        const int* D,
        int left,
        int right,
        int target
    ) {
        if (left > right) return left;

        int len = right - left + 1;
        int step = step_size_cache[len];

        // idx 表示“最后一个满足 D[order[idx]] <= target 的位置”
        int idx = left - 1;

        while (step > 0) {
            while (idx + step <= right && D[order[idx + step]] <= target) {
                idx += step;
            }
            step >>= 1;
        }

        return idx + 1; // 第一个 > target 的位置；若不存在则为 right + 1
    }
}

/**
 * @brief 严格按论文思路实现的 improved dijkstra
 *
 * @param D 最短距离数组
 * @param P 前驱数组
 */
void improved_dijkstra(int* D, int* P) {
    using namespace improved_dijkstra_detail;

    build_step_size(g_n_vtx);

    std::vector<bool> is_sure(g_n_vtx, false);
    std::vector<int> order(g_n_vtx, 0);     // 有序表 l
    std::vector<int> position(g_n_vtx, 0);  // 顶点 -> 在有序表中的位置

    // 初始化
    for (int i = 0; i < g_n_vtx; ++i) {
        D[i] = INF;
        P[i] = -1;
        order[i] = i;
        position[i] = i;
    }

    D[g_src_vtx] = 0;
    P[g_src_vtx] = g_src_vtx;

    // 让源点位于最前面，其余顶点初始为 INF，顺序任意
    if (g_src_vtx != 0) {
        std::swap(order[0], order[g_src_vtx]);
        position[order[0]] = 0;
        position[order[g_src_vtx]] = g_src_vtx;
    }

    // 主循环：low 相当于论文中的 low_i
    for (int low = 0; low < g_n_vtx; ++low) {
        int u = order[low];

        if (D[u] >= INF) break;   // 后续均不可达
        if (is_sure[u]) continue;

        is_sure[u] = true;

        // 松弛当前已确定点 u 的邻居
        for (int v : g_neighbor[u]) {
            if (is_sure[v]) continue;

            int new_dis = D[u] + g_arcs[u][v];
            if (new_dis >= D[v]) continue;

            D[v] = new_dis;
            P[v] = u;

            int old_pos = position[v];

            // v 必须仍在未确定区间中
            if (old_pos <= low) continue;

            // 按论文思路：v 的新位置只会向前移动，不会向后
            // 所以只需在 [low+1, old_pos-1] 中找重插入位置
            int insert_pos = old_pos;
            if (old_pos > low + 1) {
                insert_pos = locate_insert_position(
                    order, D,
                    low + 1, old_pos - 1,
                    D[v]
                );
            }

            // 若确实需要前移，则做局部右移插入
            if (insert_pos < old_pos) {
                // [insert_pos, old_pos-1] 整体右移一格
                for (int pos = old_pos; pos > insert_pos; --pos) {
                    order[pos] = order[pos - 1];
                    position[order[pos]] = pos;
                }
                order[insert_pos] = v;
                position[v] = insert_pos;
            }
        }
    }
}

#endif // !_Improved_Dijkstra_h