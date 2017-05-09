#include "Chainmail.hpp"

using std::vector;
using glm::ivec2;
using glm::vec2;
using glm::vec3;
using std::min;
using std::max;

Chainmail::Chainmail() {}

Chainmail::~Chainmail(){}

// Moves an element and readies its neighbors for propogation
void Chainmail::applyMove(int id, vec3 t) {
	this->elements[id].pos += t;
	this->elements[id].updated = true;
	
	for (int i : this->elements[id].neighbors)
		if (!this->elements[i].updated)
			waiting.push_back(ivec2(id, i));
}

// Propogates the changes from elements moving to their neighbors
void Chainmail::propogate() {

	while (!waiting.empty()) {
		ivec2 v = waiting.front();
		waiting.pop_front();
		Element sponsor = this->elements[v.x];
		Element e = this->elements[v.y];

		if (v.x > v.y)
			v = ivec2(v.y, v.x);
		Cuboid c = this->regions[v]; // this should never throw an error if done right

		vec3 minBounds = sponsor.pos + c.min;
		vec3 maxBounds = sponsor.pos + c.max;

		if (e.pos.x < minBounds.x) {
			e.pos.x = minBounds.x;
			e.updated = true;
		}
		else if (e.pos.x > maxBounds.x) {
			e.pos.x = maxBounds.x;
			e.updated = true;
		}
		if (e.pos.y < minBounds.y) {
			e.pos.y = minBounds.y;
			e.updated = true;
		}
		else if (e.pos.y > maxBounds.y) {
			e.pos.y = maxBounds.y;
			e.updated = true;
		}
		if (e.pos.z < minBounds.z) {
			e.pos.z = minBounds.z;
			e.updated = true;
		}
		else if (e.pos.z > maxBounds.z) {
			e.pos.z = maxBounds.z;
			e.updated = true;
		}

		if (e.updated)
			for (int i : this->elements[e.id].neighbors)
				if (!this->elements[i].updated)
					waiting.push_back(ivec2(e.id, i));
	}

}

// Brings the state closer to equilibrium
void Chainmail::relax() {
	// First, generate all relaxation centroids

	// Second, push all elements toward their respective centroid
}

// Reverts state variables at the end of the frame
void Chainmail::endFrame() {
	for (Element e : this->elements)
		e.updated = false;

	waiting.clear(); // might want a more robust end check than this
}

// Precompute the bounding regions for each element and their neighbors
void Chainmail::generateRegions() {
	for (Element e : this->elements) {
		for (int nId : e.neighbors) {
			int minId = min(e.id, nId);
			int maxId = max(e.id, nId);
			ivec2 key = ivec2(minId, maxId);

			if (this->regions.find(key) == this->regions.end()) {
				Cuboid c;
				Element eN = this->elements[nId];
				double dX, dY, dZ;

				// TODO: test that this logic is correct
				dX = std::abs(e.origin.x - eN.origin.x);
				dY = std::abs(e.origin.y - eN.origin.y);
				dZ = std::abs(e.origin.z - eN.origin.z);

				c.min = vec3(this->aMin*dX - this->b*(dY + dZ),
							 this->aMin*dY - this->b*(dZ + dX),
							 this->aMin*dZ - this->b*(dX + dY));
				c.max = vec3(this->aMax*dX + this->b*(dY + dZ),
							 this->aMax*dY + this->b*(dZ + dX),
							 this->aMax*dZ + this->b*(dX + dY));

				this->regions[key] = c;
			}
		}
	}
}