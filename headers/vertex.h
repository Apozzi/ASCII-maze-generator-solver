#ifndef VERTEX
#define VERTEX

#include <iostream>
#include <string>
#include <algorithm>
#include <list>

template <class T>
class Vertex {
  protected:
    std::list<Vertex*> edges;
  	Vertex* to;
    bool visited = false;
    T content;

  public:
    Vertex(T content): content(content) {
      to = nullptr;
    }

    void addVertexConnected(Vertex* vertex) {
      edges.push_back(vertex);
    }

    void removeVertexConnected(Vertex* vertex) {
      edges.remove(vertex);
    }
	
	 void setVectorVertex(Vertex* vertex) {
      to = vertex;
    }

    void sort(bool (*f)(Vertex<T>*, Vertex<T>*)) {
      edges.sort(f);
    }

    void clearEdgesList() {
      edges.clear();
    }

    void checkVisited(bool isVisited) {
      visited = isVisited;
    }

    bool getVisited() {
      return visited;
    }

    std::list<Vertex*> getEdges(){
      return edges;
    }

    T getContent() {
      return content;
    }
	
	  Vertex* getVectorVertex() {
      return to;
    }

};

#endif
