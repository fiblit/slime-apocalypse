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
Chainmail::Chainmail(Mesh * mesh) {
	for (Vertex v : mesh->vertices) {
		Element e;
		e.id = this->elements.size();
		e.origin = v.Position;
		e.pos = v.Position;
		elements.push_back(e);
        /*
        vector<Vertex> vertices;
        vector<GLuint> indices;
        for (int i = 0; i < mesh->indices.size(); i++) {
            vertices.push_back(mesh->vertices[i]);
            indices.push_back(mesh->indices[i]);
        }
        this->mesh = new Mesh(vertices, indices);
        */
	}

	for (int i = 0; i < mesh->indices.size(); i += 3) {
		int idx1 = mesh->indices[i];
		int idx2 = mesh->indices[i+1];
		int idx3 = mesh->indices[i+2];

		if (elements[idx1].neighbors.find(idx2) == elements[idx1].neighbors.end()) {
			elements[idx1].neighbors.insert(idx2);
			elements[idx2].neighbors.insert(idx1);
		}
		if (elements[idx2].neighbors.find(idx3) == elements[idx2].neighbors.end()) {
			elements[idx2].neighbors.insert(idx3);
			elements[idx3].neighbors.insert(idx2);
		}
		if (elements[idx1].neighbors.find(idx3) == elements[idx1].neighbors.end()) {
			elements[idx1].neighbors.insert(idx3);
			elements[idx3].neighbors.insert(idx1);
		}
	}
}



Chainmail::~Chainmail() {}

// Moves an element and readies its neighbors for propogation
void Chainmail::applyMove(int id, vec3 t) {
	this->elements[id].pos += t;
	this->elements[id].updated = true;

	for (int i : this->elements[id].neighbors)
		if (!this->elements[i].updated)
			waiting.push_back(ivec2(id, i));
}

Mesh * Chainmail::updateMesh() {
    vector<Vertex> newVertices;
    for (int i = 0; i < elements.size(); i++) {
        Vertex v = {};
        v.Position = elements[i].pos;

        //DUMMY. TODO: FIX
        v.Normal = vec3(0, 1, 0);
        //FIXFIXFIX

        newVertices.push_back(v);
    }
    mesh->updateVertices(newVertices);
    return mesh;
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
	for (Element e : this->elements) {
		e.pos += dt*centroids[e.id];
	}
}

// Reverts state variables at the end of the frame
void Chainmail::endFrame() {
	for (Element e : this->elements)
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