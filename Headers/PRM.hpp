#ifndef PRM_H_GUARD
#define PRM_H_GUARD

#include "Graph.hpp"
#include "BoundingVolume.hpp"

#include <random>
#include <chrono>
#include <utility>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <string>

// glm: OpenGL mathematics: http://glm.g-truc.net/0.9.8/index.html
#include <glm/glm.hpp>

typedef std::vector<Node<glm::vec2> *> VecPoint;

class Cspace_2D {
private:
public:
    std::vector<BoundingVolume *> bv_obs;
	Cspace_2D(std::vector<BoundingVolume *> obs, BoundingVolume * agent);
	bool isCollision(glm::vec2 a);
	bool lineOfSight(glm::vec2 a, glm::vec2 b);
};

class PRM {
private:
	Cspace_2D * cSpace;
	VecPoint * sampleNodes(Cspace_2D * cSpace);
	VecPoint * findNearestNeighbours(VecPoint * nodes, int targetIdx);
	Graph<glm::vec2> * connectRoadmap(VecPoint * nodes);
public:
	Graph<glm::vec2> * roadmap;
	PRM(glm::vec2 start, glm::vec2 goal, Cspace_2D * cSpace);
	//VecPoint * findPathUCS();
	//VecPoint * findPathAstar(float e);
};

//wtf, why didn't I just use glm::Vec2???
//float dotP(Point a, Point b);
//float distP(Point a, Point b);
//Point subP(Point a, Point b);
//Point addP(Point a, Point b);
//Point scaleP(Point a, float s);

#endif // PRM_H_GUARD