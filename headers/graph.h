#ifndef GRAPH
#define GRAPH

#include <iostream>
#include <string>
#include <list>
#include <tuple>
#include <algorithm>
#include <queue>
#include "vertex.h"


template <class T, class T2>
class Graph {
  protected:
    typedef std::list<std::tuple<Vertex<T>*, Vertex<T>*, T2>> EdgeList;
    std::list<Vertex<T>*> vertexes;
    EdgeList edges;
    void (*displayFunc)(std::list<Vertex<T>*>) = nullptr;

  public:
    Graph() {
    }

    void addVertex(Vertex<T>* vertex) {
        vertexes.push_back(vertex);
    }

    void removeVertex(Vertex<T>* vertex) {
      vertexes.remove(vertex);
    }

    void setDisplayFunction(void (*f)(std::list<Vertex<T>*>)) {
      displayFunc = f;
    }

    std::list<Vertex<T>*> getAllVertexes() {
      return vertexes;
    }

    std::list<std::tuple<Vertex<T>*, Vertex<T>*, T2>> getAllEdges() {
      return edges;
    }

    void addEdge(Vertex<T>* to, Vertex<T>* from, T2 value) {
      edges.push_back(std::make_tuple(to, from, value));
      to->addVertexConnected(from);
      from->addVertexConnected(to);
    }

    bool isVertexOnGraph(Vertex<T>* vertex) {
      return (std::find(vertexes.begin(), vertexes.end(), vertex) != vertexes.end());
    }

    void sortEdges(bool (*f)(Vertex<T>*, Vertex<T>*)) {
        auto itr = vertexes.begin();
        Vertex<T>* ver = nullptr;
        while(itr != vertexes.end()) {
        ver = *itr;
        ver->sort(f);
        itr++;
      }
    }

    Vertex<T>* getVertexWithSameContent(Vertex<T>* v) {
      auto itr = vertexes.begin();
      Vertex<T>* ver = nullptr;
      while(itr != vertexes.end()) {
        ver = *itr;
        if (ver->getContent() == v->getContent()) {
          return ver;
        }
        itr++;
      }
      return nullptr;
    }

	/*
		Busca em profundidade (DFS) (Criação do labirinto)
	*/
    Graph<T, T2>* depthSearchTree() {
        Graph<T, T2>* newGraph = new Graph<T, T2>();
        copyVertex(newGraph);
        newGraph->resetVertex();
        Vertex<T>* initialVertex = vertexes.front();
        depthSearchRecursivity(initialVertex, newGraph);
        return newGraph;
    }
	
    /*
      Busca em largura (BFS) (Resolução do labirinto)
      Obs.: Se a distancia fosse relativa seria possivel utilizar dijkstra
    */
    std::list<Vertex<T>*> breadthSearchPath(Vertex<T>* objectiveVertex) {
      std::list<Vertex<T>*> queue;
      Vertex<T>* initialVertex = vertexes.front();
      queue.push_front(initialVertex);
      auto itr = queue.begin();
      while(itr != queue.end()) {
        Vertex<T>* actualVertex = *itr;
        actualVertex->checkVisited(true);
        if (displayFunc != nullptr) {
          displayFunc(vertexes);
        }
        if (objectiveVertex->getContent() == actualVertex->getContent()) {
          return getPathList(objectiveVertex);
        }
        std::list<Vertex<T>*> actualVertexEdges = actualVertex->getEdges();
        auto itr2 = actualVertexEdges.begin();
        while (itr2 != actualVertexEdges.end()) {
          Vertex<T>* childVertex = *itr2;
          if (!childVertex->getVisited()) {
            if (childVertex->getContent() != actualVertex->getContent()) {
              childVertex->setVectorVertex(actualVertex);
            }
            queue.push_back(childVertex);
          }
          itr2++;
        }
        itr++;
      }
      return getPathList(objectiveVertex);
    }

    private:
    /*
       Pega o roteamente de cada node pré-calculado
    */
    std::list<Vertex<T>*> getPathList(Vertex<T>* objectiveVertex) {
    	std::list<Vertex<T>*> pathList;
    	Vertex<T>* vvt = objectiveVertex;
    	while (vvt != nullptr) {
    		pathList.push_back(vvt);
    		vvt = vvt->getVectorVertex();
    	}
    	return pathList;
    }
	
    /*
       Função recursiva da (DFS)
    */
    void depthSearchRecursivity(Vertex<T>* ve, Graph<T, T2>* g) {
        ve->checkVisited(true);
        std::list<Vertex<T>*> vertexEdges = ve->getEdges();
        auto itr = vertexEdges.begin();
        Vertex<T>* ver = nullptr;
        while(itr != vertexEdges.end()) {
        ver = *itr;
        if (!ver->getVisited()) {
          EdgeList edgeList = this->getAllEdges();
          auto it = edgeList.begin();
          while (it != edgeList.end()) {
            std::tuple<Vertex<T>*, Vertex<T>*, T2> ed = *it;
            Vertex<T>* edv1 = std::get<0>(ed);
            Vertex<T>* edv2  = std::get<1>(ed);
            if ((ve == edv1 && ver == edv2)) {
              g->addEdge(g->getVertexWithSameContent(edv1), g->getVertexWithSameContent(edv2), std::get<2>(ed));
            }
            it++;
          }
          depthSearchRecursivity(ver, g);
        }
        itr++;
      }
    }

    void copyVertex(Graph<T, T2>* g) {
      auto itr = vertexes.begin();
      Vertex<T>* ver = nullptr;
      while(itr != vertexes.end()) {
        ver = *itr;
        g->addVertex(new Vertex<T>(ver->getContent()));
        itr++;
      }
    }
	
    void resetVertex() {
      auto itr = vertexes.begin();
      Vertex<T>* ver = nullptr;
      while(itr != vertexes.end()) {
        ver = *itr;
        ver->clearEdgesList();
		    ver->checkVisited(false);
        itr++;
      }
    }
	
};

#endif
