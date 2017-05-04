#include "PRM.hpp"
#include "Scenario.hpp"

/* uniformly bin_samps the configuration space to generate nodes for the PRM;
   All sampled points will be non-colliding with the static environment 
*/
VecPoint * PRM::sample_nodes(Cspace2D * cSpace_, float perturb, float bin_dim, int bin_samps, std::pair<float, float> bounds) {
	typedef std::chrono::high_resolution_clock hrclock;
	hrclock::time_point first = hrclock::now();

	std::default_random_engine gen;
	std::uniform_real_distribution<float> std(-0.5f, 0.5f);

	VecPoint * sample = new VecPoint();
	for (int i = 0; i < bin_samps; i++) {
		for (float x = bounds.first+bin_dim/2; x < bounds.second-bin_dim/2; x+=bin_dim) {
            hrclock::duration seed = hrclock::now() - first;
            gen.seed(static_cast<unsigned int>(seed.count()));
			for (float y = bounds.first+bin_dim/2; y < bounds.second-bin_dim/2; y+=bin_dim) {
				glm::vec2 p(
                    std(gen)*bin_dim + x,
                    std(gen)*bin_dim + y
                );
				do {
					p.x += std(gen) * 2 * perturb;
					p.y += std(gen) * 2 * perturb;
					p.x = std::min(std::max(p.x, bounds.first), bounds.second);
					p.y = std::min(std::max(p.y, bounds.first), bounds.second);
				} while (cSpace_->is_collision(p));
                sample->push_back(new Node<glm::vec2> (p, new VecPoint()));
			}
		}
	}

	return sample;
}

/* threshold search to find nearby */
VecPoint * PRM::find_nearest_neighbours(VecPoint * nodes, int targetIdx) {
    float threshold; // meters
    switch (G::SCENARIO) {
    case G::SCENE::DEADEND:
    case G::SCENE::WALL:
        threshold = 3.3f;
        break;
    case G::SCENE::DEFAULT:
    case G::SCENE::NO_BOID:
    case G::SCENE::MAZE:
    default:
        threshold = 5.f;
        break;
    }

	VecPoint * neighbours = new VecPoint();

	glm::vec2 t = (*nodes)[targetIdx]->data;
	for (int i = 0; i < static_cast<int>(nodes->size()); i++) {
		glm::vec2 n = (*nodes)[i]->data;
		// don't consider this node we're looking from

		if (i != targetIdx)
			if (glm::distance(t, n) < threshold)
				// push the close enough node onto the neighbours list
				neighbours->push_back((*nodes)[i]);
	}

	return neighbours;
}

/* connects nearby of each node by Graph edges */
Graph<glm::vec2> * PRM::connect_roadmap(VecPoint * nodes) {
	Graph<glm::vec2> * roadmap = new Graph<glm::vec2>();
	for (int i = 0; i < static_cast<int>(nodes->size()); i++)
		roadmap->add_vertex((*nodes)[i]);

	for (int i = 0; i < static_cast<int>(nodes->size()); i++) {
		VecPoint * nearby = find_nearest_neighbours(nodes, i);
		for (int n = 0; n < static_cast<int>(nearby->size()); n++) {
			if (this->c_space->line_of_sight((*nearby)[n]->data, (*nodes)[i]->data)) {
				// we want directed because we'll be passing over the other side during
				// the course of the outer loop
				roadmap->add_directed_edge((*nearby)[n], (*nodes)[i]);
			}
		}
	}

	return roadmap;
}

/* bin_samps and connects a Pobabilistic Road Map */
PRM::PRM(glm::vec2 start, glm::vec2 goal, Cspace2D * c_space) {
    this->c_space = c_space;

    Node<glm::vec2> * start_node = new Node<glm::vec2>(start, new VecPoint());
    Node<glm::vec2> * goal_node = new Node<glm::vec2>(goal, new VecPoint());

    VecPoint * sample = sample_nodes(this->c_space, .1f, 2.8f, 1, std::make_pair(-10.f, 10.f));

    this->roadmap = connect_roadmap(sample);
}

/* generates a configuartion space given a list of obstacles and agent */
Cspace2D::Cspace2D(std::vector<BoundingVolume *> obs, BoundingVolume * agent) {
    this->c_obs = std::vector<BoundingVolume *>();

    for (BoundingVolume * o : obs) {
        std::vector<BoundingVolume *> ms = agent->minkowskiSum(o);
        this->c_obs.insert(c_obs.end(), ms.begin(), ms.end());
    }
}

/* detects if a point collides with anything in the configuration space */
bool Cspace2D::is_collision(glm::vec2 p) {
    for (BoundingVolume * bv : this->c_obs)
        if (bv->is_collision(p))
            return true;//HIT
    return false;//MISS
}

/* detects if a line segment between glm::vec2 a and glm::vec2 bin_dim collides with the C-space 
 */
bool Cspace2D::line_of_sight(glm::vec2 a, glm::vec2 b) {
	glm::vec2 Lab;
	Lab.x = b.x - a.x;
	Lab.y = b.y - a.y;
	float len2 = glm::dot(Lab, Lab);

	for (BoundingVolume * bv : this->c_obs)
        if (!bv->line_of_sight(a, b, Lab, len2))
            return false;//HIT
    return true;//MISS
}
