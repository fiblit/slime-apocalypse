#include "PRM.hpp"
#include "Scenario.hpp"

/* uniformly samples the configuration space to generate nodes for the PRM;
   All sampled points will be non-colliding with the static environment 
*/
VecPoint * PRM::sampleNodes(Cspace_2D * cSpace_) {
	typedef std::chrono::high_resolution_clock hrclock;
	hrclock::time_point first = hrclock::now();

	std::default_random_engine gen;
	std::uniform_real_distribution<float> std(-0.5f, 0.5f);

    int samplecount;
    float b;
    switch (G::SCENARIO) {
    case G::SCENE::DEFAULT:
    case G::SCENE::NO_BOID:
        samplecount = 1;//this does not work on all maps
        b = 2.8f;
        break;
    case G::SCENE::WALL:
    case G::SCENE::DEADEND:
        samplecount = 4;
        b = 2.8f;
        break;
    case G::SCENE::MAZE:
        samplecount = 5;
        b = 2.8f;
        break;
    }

	VecPoint * sample = new VecPoint();
	for (int i = 0; i < samplecount; i++) {
		for (float x = -10+b/2; x < 10-b/2; x+=b) {
            hrclock::duration seed = hrclock::now() - first;
            gen.seed(static_cast<unsigned int>(seed.count()));
			for (float y = -10+b/2; y < 10-b/2; y+=b) {
				glm::vec2 p(
                    std(gen)*b + x,
                    std(gen)*b + y
                );
				do {
					p.x += std(gen)/5;
					p.y += std(gen)/5;
					p.x = std::min(std::max(p.x, -10.f), 10.f);
					p.y = std::min(std::max(p.y, -10.f), 10.f);
				} while (cSpace_->isCollision(p));
                sample->push_back(new Node<glm::vec2> (p, new VecPoint()));
				//else
				//	i--;
			}
		}
	}

	return sample;
}

/* threshold search to find NNs */
VecPoint * PRM::findNearestNeighbours(VecPoint * nodes, int targetIdx) {
    float threshold; // meters
    switch (G::SCENARIO) {
    case G::SCENE::DEFAULT:
    case G::SCENE::NO_BOID:
    case G::SCENE::MAZE:
        threshold = 5.f;
        break;
    case G::SCENE::DEADEND:
    case G::SCENE::WALL:
        threshold = 3.3f;
        break;
    }

	VecPoint * neighbours = new VecPoint();

	glm::vec2 t = (*nodes)[targetIdx]->data;
	for (int i = 0; i < nodes->size(); i++) {
		glm::vec2 n = (*nodes)[i]->data;
		// don't consider this node we're looking from

		if (i != targetIdx)
			if (glm::distance(t, n) < threshold)
				// push the close enough node onto the neighbours list
				neighbours->push_back((*nodes)[i]);
	}

	return neighbours;
}

/* connects NNs of each node by Graph edges */
Graph<glm::vec2> * PRM::connectRoadmap(VecPoint * nodes) {
	Graph<glm::vec2> * G = new Graph<glm::vec2>();
	for (int i = 0; i < nodes->size(); i++)
		G->addVertex((*nodes)[i]);

	for (int i = 0; i < nodes->size(); i++) {

		VecPoint * NNs = findNearestNeighbours(nodes, i);

		for (int n = 0; n < NNs->size(); n++) {
			if (this->cSpace->lineOfSight((*NNs)[n]->data, (*nodes)[i]->data)) {
				// we want directed because we'll be passing over the other side during
				// the course of the outer loop
				G->addDirectedEdge((*NNs)[n], (*nodes)[i]);
			}
		}
	}

	return G;
}

/* samples and connects a Pobabilistic Road Map */
PRM::PRM(glm::vec2 start, glm::vec2 goal, Cspace_2D * cSpace) {
    this->cSpace = cSpace;

    Node<glm::vec2> * startNode = new Node<glm::vec2>(start, new VecPoint());
    Node<glm::vec2> * goalNode = new Node<glm::vec2>(goal, new VecPoint());

    VecPoint * sample = sampleNodes(this->cSpace);
    sample->insert(sample->begin(), goalNode);
    sample->insert(sample->begin(), startNode);

    this->roadmap = connectRoadmap(sample);
}

/* generates a configuartion space given a list of obstacles and agent */
Cspace_2D::Cspace_2D(std::vector<BoundingVolume *> obs, BoundingVolume * agent) {
    this->bv_obs = std::vector<BoundingVolume *>();

    for (BoundingVolume * o : obs) {
        std::vector<BoundingVolume *> ms = agent->minkowskiSum(o);
        this->bv_obs.insert(bv_obs.end(), ms.begin(), ms.end());
    }
}

/* detects if a point collides with anything in the configuration space */
bool Cspace_2D::isCollision(glm::vec2 p) {
    for (BoundingVolume * bv : this->bv_obs)
        if (bv->isCollision(p))
            return true;//HIT
    return false;//MISS
}

/* detects if a line segment between glm::vec2 a and glm::vec2 b collides with the C-space 
 */
bool Cspace_2D::lineOfSight(glm::vec2 a, glm::vec2 b) {
	glm::vec2 Lab;
	Lab.x = b.x - a.x;
	Lab.y = b.y - a.y;
	float len2 = glm::dot(Lab, Lab);

	for (BoundingVolume * bv : this->bv_obs)
        if (!bv->lineOfSight(a, b, Lab, len2))
            return false;//HIT
    return true;//MISS
}
