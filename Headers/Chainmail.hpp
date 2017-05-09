#pragma once

#include <vector>
#include <deque>
#include <unordered_map>
#include <algorithm>

#pragma warning(push, 0)
#pragma warning(disable: 4055)
#include <glad/glad.h>
#pragma warning(disable: 4201)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma warning(pop)

enum NeighborIndex {
	TOP = 1,
	LEFT = 2,
	RIGHT = 3,
	BOTTOM = 4,
	FRONT = 5,
	BACK = 6
};

struct Cuboid {
	glm::vec3 min;
	glm::vec3 max;
};

struct Element {
	int id;
	glm::vec3 origin;
	glm::vec3 pos;
	bool updated;
	std::vector<int> neighbors;
};

class Chainmail
{
public:
	Chainmail();
	virtual ~Chainmail();

	void applyMove(int id, glm::vec3 t);
	void propogate();
	void relax();
	void generateRegions();
	void endFrame();

	std::vector<Element> elements;
	
	// Waiting list of elements to be processed.
	// The first value is the sponsor, the second is the element id.
	std::deque<glm::ivec2> waiting;

	// Each region is shared between two indices, where the vec2
	// is (minIndex, maxIndex) to avoid doubling.
	std::unordered_map<glm::ivec2, Cuboid> regions;

	int objectDimension;
	int spaceDimension;
	double aMin; // compression factor
	double aMax; // stretch factor
	double b;	 // shear factor

private:
	bool inBounds(vec3 pos, vec3 minBound, vec3 maxBound);
};

