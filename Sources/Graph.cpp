//#include "Graph.hpp"
/*
using namespace std;

template <class T>
inline Graph<T>::Graph() {
    this->vertices = new vector<Node<T> *>();
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
}


template <class T>
inline Node<T>::Node(T item, vector<Node<T> *> * edges) {
    this->data = item;
    this->edges = edges;
}

template <class T>
inline Node<T>::Node(T item, int n, Node<T> * edges) {
    this->data = item;
    this->edges = new vector<Node<T> *>(n);
    for (int i = 0; i < n; i++)
        this->edges[i] = edges[i];
}
*/
