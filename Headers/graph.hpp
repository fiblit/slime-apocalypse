#ifndef GRAPH_H_GUARD
#define GRAPH_H_GUARD

#include <vector>

template <class T>
class Node {
private:
public:
	T data;
	/* TODO: change to unordered_set for fast lookup, yet easy ---iteration--- <nope> */
	std::vector<Node<T> *> * edges;
    Node(T item, std::vector<Node<T> *> * edges);
    Node(T item, int n, Node<T> * edges);
};

template <class T>
class Graph {
private:
public:
    std::vector<std::pair<Node<T> *, Node<T> *>> * all_edges;//hack, just for the vis
	/* TODO: change to unordered_set for fast lookup, yet easy  ---iteration--- <nope> */
	std::vector<Node<T> *> * vertices;
    Graph();
    void addVertex(Node<T> * v);
    void addDirectedEdge(Node<T> * v, Node<T> * u);
    void addEdge(Node<T> * v, Node<T> * u);
};

#endif // GRAPH_H_GUARD

/* it really bothers me that this all can't go in a .cpp */
template <class T>
inline Graph<T>::Graph() {
    this->vertices = new std::vector<Node<T> *>();
}

template <class T>
inline void Graph<T>::addVertex(Node<T> * v) {
    this->vertices->push_back(v);
}

template <class T>
inline void Graph<T>::addDirectedEdge(Node<T> * v, Node<T> * u) {
    //todo: check that u and v are in vertices
    v->edges->push_back(u);
}

template <class T>
inline void Graph<T>::addEdge(Node<T> * v, Node<T> * u) {
    //todo: check that u and v are in vertices
    this->addDirectedEdge(v, u);
    this->addDirectedEdge(u, v);
    this->all_edges->push_back(std::pair<Node<T> *, Node<T> *>(u, v));
}


template <class T>
inline Node<T>::Node(T item, std::vector<Node<T> *> * edges) {
    this->data = item;
    this->edges = edges;
}

template <class T>
inline Node<T>::Node(T item, int n, Node<T> * edges) {
    this->data = item;
    this->edges = new std::vector<Node<T> *>(n);
    for (int i = 0; i < n; i++)
        this->edges[i] = edges[i];
}
