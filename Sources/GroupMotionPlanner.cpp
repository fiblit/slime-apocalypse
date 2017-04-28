#include "GlobalMotionPlanner.hpp"

/* custom uniform cost search (aka Djikstra's search) for a PRM Graph
simplifciation of A*. (h = 0)
*/
VecPoint * GMP::findPathUCS(Graph<glm::vec2> * roadmap) {
    return GMP::findPathAstar(0, roadmap);
}



/* custom A* search for a PRM Graph */
//can't handle non-positive edges
VecPoint * GMP::findPathAstar(float e, Graph<glm::vec2> * roadmap) {
    //for readability
    using namespace std;
    typedef Node<glm::vec2> * Vert;
    typedef pair<Vert, float> PQ_item;
    
    //init start/goal
    VecPoint verts = *(roadmap->vertices);
    Vert start = verts[0];
    Vert goal = verts[1];

    //init PQ
    auto cmp = [](PQ_item l, PQ_item r) { return l.second  > r.second; }; //normally <
    priority_queue<PQ_item, vector<PQ_item>, decltype(cmp)> pq(cmp);
    pq.push(PQ_item(start, 0.f));

    // init parent and gcost (also used as a visited set)
    unordered_map<Vert, Vert> parent;
    unordered_map<Vert, float> gcost;
    parent[start] = nullptr;
    gcost[start] = 0.f;

    //while there are nodes to process
    while (!pq.empty()) {
        PQ_item cur_i = pq.top();
        Vert cur_v = cur_i.first;
        pq.pop();

        //quit if we hit the goal, we have a path
        if (cur_v == goal)
            break;

        for (Node<glm::vec2> * adj : *(cur_v->edges)) {
            //new potential path = path to here + edge to adjacent
            float g_alt = gcost[cur_v] + glm::distance(adj->data, cur_v->data);

            //if new path is better than current path to adjacent
            if (!gcost.count(adj) || g_alt < gcost[adj]) {
                gcost[adj] = g_alt;
                float fcost = g_alt + e * glm::distance(goal->data, adj->data);
                //it's okay if we get multiple nodes on the PQ with diff fcosts
                //whichever has the lowest will show up first and the later ones will
                //have no effect as they have the same gcost
                pq.push(PQ_item(adj, fcost));
                parent[adj] = cur_v;
            }
        }
    }

    //reconstruct path
    VecPoint * path = new VecPoint();
    for (Vert v = goal; v != nullptr; v = parent[v])
        path->insert(path->begin(), v);
    return (*path)[0] == start ? path : new VecPoint();
}
