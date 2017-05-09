#include "GlobalMotionPlanner.hpp"
#include "AI.hpp"

/* custom uniform cost search (aka Djikstra's search) for a PRM Graph
simplifciation of A*. (h = 0)
*/
VecPoint * GMP::find_path_UCS(Graph<glm::vec2> * roadmap) {
    return GMP::find_path_Astar(0, roadmap);
}

/* custom A* search for a PRM Graph */
//can't handle non-positive edges
VecPoint * GMP::find_path_Astar(float e, Graph<glm::vec2> * roadmap) {
    //for readability
    using namespace std;
    typedef Node<glm::vec2> * Vert;
    typedef pair<Vert, float> PQ_item;
    
    //init start/goal
    VecPoint verts = *(roadmap->vertices);
    Vert start = verts[0];//TODO: fix this hack IMMEDIATELY
    Vert goal = verts[1];

    //init PQ
    auto cmp = [](PQ_item l, PQ_item r) { return l.second  > r.second; }; //normally <
    priority_queue<PQ_item, vector<PQ_item>, decltype(cmp)> pq(cmp);
    pq.push(PQ_item(start, 0.f));

    // init parent and g_cost (also used as a visited set)
    unordered_map<Vert, Vert> parent;
    unordered_map<Vert, float> g_cost;
    parent[start] = nullptr;
    g_cost[start] = 0.f;

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
            float g_alt = g_cost[cur_v] + glm::distance(adj->data, cur_v->data);

            //if new path is better than current path to adjacent
            if (!g_cost.count(adj) || g_alt < g_cost[adj]) {
                g_cost[adj] = g_alt;
                float fcost = g_alt + e * glm::distance(goal->data, adj->data);
                //it's okay if we get multiple nodes on the PQ with diff fcosts
                //whichever has the lowest will show up first and the later ones will
                //have no effect as they have the same g_cost
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

//move to AI/planner (probably GMP)
void GMP::replan(std::vector<Object *> agents, Gtime::Timer * clock) {
    clock->pause();
    /* PATH PLANNING METHOD */
    for (Object * a : agents) {
        plan_one(a);
    }
    clock->play();
}

void GMP::plan_one(Object * agent) {
    if (agent->ai.has_indy_f()) {
        //todo temporarily add to a->ai.prm->roadmap the start/goal nodes
        //just do an LoS over every node for both
        //this is to fix the one hack
        agent->ai.plan = GMP::find_path_Astar(1.f, agent->ai.prm->roadmap);
        agent->ai.num_done = 0;
    }
}
