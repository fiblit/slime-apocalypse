#include "Chainmail.hpp"

using std::vector;
using glm::ivec2;
using glm::vec2;
using glm::vec3;
using std::min;
using std::max;
/*
Our chainmail deformation used Sarah Gibson's Paper, "3D ChainMail: a Fast Algorithm for Deforming Volumetric Objects"
As a base idea for implementation.
*/
Chainmail::Chainmail(Mesh * mesh, int stacks, int slices, glm::vec3 worldCenter) {
    worldCoordCenter = worldCenter;
    this->vertexLength = mesh->vertices.size();
    for (Vertex v : mesh->vertices) {
        Element e;
        e.id = this->elements.size();
        e.origin = v.Position;
        e.pos = v.Position;
        elements.push_back(e);
    }
    // Add a centroid element
    Element center;
    center.id = this->elements.size();
    center.origin = vec3(0);
    center.pos = vec3(0);
    for (Element & e : this->elements) {
        center.neighbors.insert(e.id);
        e.neighbors.insert(center.id);
    }

    this->elements.push_back(center);

    for (int i = 0; i < mesh->indices.size(); i += 3) {
        int idx1 = mesh->indices[i];
        int idx2 = mesh->indices[i + 1];
        int idx3 = mesh->indices[i + 2];

        if (elements[idx1].neighbors.find(idx2) == elements[idx1].neighbors.end() && glm::length(elements[idx1].pos - elements[idx2].pos) > .003) {
            elements[idx1].neighbors.insert(idx2);
            elements[idx2].neighbors.insert(idx1);
        }
        if (elements[idx2].neighbors.find(idx3) == elements[idx2].neighbors.end() && glm::length(elements[idx3].pos - elements[idx2].pos) > .003) {
            elements[idx2].neighbors.insert(idx3);
            elements[idx3].neighbors.insert(idx2);
        }
        if (elements[idx1].neighbors.find(idx3) == elements[idx1].neighbors.end() && glm::length(elements[idx1].pos - elements[idx3].pos) > .003) {
            elements[idx1].neighbors.insert(idx3);
            elements[idx3].neighbors.insert(idx1);
        }
    }
    generateRegions();
}



Chainmail::~Chainmail() {}

glm::vec3 Chainmail::returnWorldPos() {
    return worldCoordCenter;
}



// Moves an element and readies its neighbors for propogation
void Chainmail::applyMove(int id, vec3 t) {
    applyMove(0, t, 0);
}

void Chainmail::applyMove(int id, vec3 t, double dt) {
    for (int i = 0; i < elements.size()/2; i++) {
        
        int randElement = ((float)rand()) / RAND_MAX * (elements.size()-1);
        while (elements[randElement].updated) {
            randElement = ((float)rand()) / RAND_MAX * (elements.size() - 1);
        }
        elements[randElement].pos += vec3(t[0] * .001, t[1] * .001, t[2] * .001);


        if (elements[randElement].pos.y + worldCoordCenter.y < .003) {
            float delta = (elements[randElement].pos.y + worldCoordCenter.y) - .005;
            //std::cout << delta << "? " << std::endl;
            elements[randElement].pos.y = delta;
        }
        elements[randElement].updated = true;
    }

    for (int i : this->elements[id].neighbors)
        waiting.push_back(ivec2(id, i));
}


//updates the center of the object to ensure the 'center' is the origin;
void Chainmail::updateCenter() {
    glm::vec3 oldCenter = worldCoordCenter;
    glm::vec3 newCenter = glm::vec3(0);
    for (int i = 0; i < elements.size(); i++) {
        newCenter += (worldCoordCenter + elements[i].pos);
    }
    newCenter /= elements.size();
    glm::vec3 delta = oldCenter - newCenter;
    worldCoordCenter = newCenter;
    for (int i = 0; i < elements.size(); i++) {
        elements[i].pos += delta;
    }
}

void Chainmail::returnVertices(vector<vec3> &returnTo) {
    vector<Vertex> newVertices;
    for (int i = 0; i < vertexLength; i++) {
        returnTo.push_back(elements[i].pos + worldCoordCenter);
    }
}

// Propagates the changes from elements moving to their neighbors
void Chainmail::propagate() {
	while (!waiting.empty()) {
		ivec2 v = waiting.front();
		waiting.pop_front();
		Element * sponsor = &this->elements[v.x];
		Element * e = &this->elements[v.y];
        
        if (e->updated) continue;

		if (v.x > v.y)
			v = ivec2(v.y, v.x);
		Cuboid c = this->regions[v]; // this should never throw an error if done right
		vec3 minBounds = sponsor->pos + c.min;

		vec3 maxBounds = sponsor->pos + c.max;
		if (e->pos.x < minBounds.x) {
			e->pos.x = minBounds.x;
			e->updated = true;
		}
		else if (e->pos.x > maxBounds.x) {
			e->pos.x = maxBounds.x;
			e->updated = true;
		}
		if (e->pos.y < minBounds.y) {
			e->pos.y = minBounds.y;
            if (e->pos.y + worldCoordCenter.y < .003) {
                float delta = (e->pos.y + worldCoordCenter.y) - .005;
                e->pos.y = delta;
            }
			e->updated = true;
		}
		else if (e->pos.y > maxBounds.y) {
			e->pos.y = maxBounds.y;
            if (e->pos.y + worldCoordCenter.y < .003) {
                float delta = (e->pos.y + worldCoordCenter.y) - .005;
                e->pos.y = delta;
            }
			e->updated = true;
		}
		if (e->pos.z < minBounds.z) {
			e->pos.z = minBounds.z;
			e->updated = true;
		}
		else if (e->pos.z > maxBounds.z) {
			e->pos.z = maxBounds.z;
			e->updated = true;
		}

		if (e->updated)
			for (int i : this->elements[e->id].neighbors)
                if (!this->elements[i].updated) {
                    waiting.push_back(ivec2(e->id, i));
                }
	}
}

// Brings the state closer to equilibrium
// NOTE: This might be wrong. It's possibly supposed to be sequential,
// but it's currently implemented as a parallel update.
void Chainmail::relax(float dt) {
	// First, generate all relaxation centroids
	vector<vec3> centroids;
	for (Element e : this->elements) {
		vector<vec3> Q;
		for (int i : e.neighbors) {
			Q.push_back(this->elements[i].pos +
						 (e.origin - this->elements[i].origin));
		}
		vec3 c = vec3(0);
		for (vec3 v : Q)
			c += v;
		c /= Q.size();
		centroids.push_back(c);
	}

	// Second, push all elements toward their respective centroid
	for (Element & e : this -> elements) {
		vec3 v = centroids[e.id] - e.pos;
        //v = (e.origin - e.pos);
		e.pos += 2*dt*v;
        if (e.pos.y + worldCoordCenter.y < .003) {
            float delta = (e.pos.y + worldCoordCenter.y) - .005;
            e.pos.y = delta;
        }
	}
}

void Chainmail::simStep(double dt) {
    simStep(0, vec3(0), dt);
}
void Chainmail::simStep(glm::vec3 v, double dt) {
    simStep(0, v, dt);
}
void Chainmail::simStep(int id, glm::vec3 t, double dt) {
    applyMove(id, t, dt);
    applyMove(id, glm::vec3(0, 0, -.3), dt);
    propagate();
    relax(dt);
    updateCenter();
	for (Element & e : this->elements)
		e.updated = false;
	waiting.clear(); // might want a more robust end check than this
}
//Precompute the bounding regions for each element and their neighbors
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
                dX = abs(eN.origin.x - e.origin.x);
                dY = abs(eN.origin.x - e.origin.x);
                dZ = abs(eN.origin.x - e.origin.x);
				c.min = vec3(this->aMin*dX - this->b*(dY + dZ),
							 this->aMin*dY - this->b*(dZ + dX),
							 this->aMin*dZ - this->b*(dX + dY));
				c.max = vec3(this->aMax*dX + this->b*(dY + dZ),
							 this->aMax*dY + this->b*(dZ + dX),
							 this->aMax*dZ + this->b*(dX + dY));

                /*
                if (eN.origin.x < e.origin.x) {
                    c.min.x *= -1;
                    c.max.x *= -1;
                }
                if (eN.origin.y < e.origin.y) {
                    c.min.y *= -1;
                    c.max.y *= -1;
                }
                if (eN.origin.z < e.origin.z) {
                    c.min.z *= -1;
                    c.max.z *= -1;
                }
                */
				this->regions[key] = c;
			}
		}
	}
}