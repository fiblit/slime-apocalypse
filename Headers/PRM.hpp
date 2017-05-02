#ifndef PRM_H_GUARD
#define PRM_H_GUARD

#include "graph.hpp"
#include "BoundingVolume.hpp"

#pragma warning(push, 0)
#include <random>
#include <chrono>
#include <utility>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#pragma warning(pop)

typedef std::vector<Node<glm::vec2> *> VecPoint;

class Cspace_2D {
private:
public:
    std::vector<BoundingVolume *> c_obs;
	Cspace_2D(std::vector<BoundingVolume *> obs, BoundingVolume * agent);
	bool is_collision(glm::vec2 a);
	bool line_of_sight(glm::vec2 a, glm::vec2 b);
};

class PRM {
private:
	Cspace_2D * c_space;
	VecPoint * sample_nodes(Cspace_2D * c_space);
	VecPoint * find_nearest_neighbours(VecPoint * nodes, int targetIdx);
	Graph<glm::vec2> * connect_roadmap(VecPoint * nodes);
public:
	Graph<glm::vec2> * roadmap;
	PRM(glm::vec2 start, glm::vec2 goal, Cspace_2D * c_space);
};

#endif // PRM_H_GUARD
