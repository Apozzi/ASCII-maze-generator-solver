#include <iostream>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include "headers/vertex.h"
#include "headers/random_alphabet.h"
#include "headers/character_map.h"
#include "headers/graph.h"
#include "headers/termcolor.hpp"
#include <stdio.h>
#include <chrono>
#include <thread>

const int sizex = 69;
const int sizey = 29;
char *positions = new char[sizex * sizey];
char *colormap = new char[sizex * sizey];
typedef Graph<CharacterMap*, CharacterMap*>* GraphMap;
typedef Vertex<CharacterMap*>* VertexMap;
typedef std::tuple<Vertex<CharacterMap*>*, Vertex<CharacterMap*>*, CharacterMap*> EdgeList;
GraphMap mazeGraph;
GraphMap mazeDepthGraph;
VertexMap vertexInitial;
VertexMap vertexFinal;

using namespace std::this_thread;
using namespace std::chrono_literals; 
using std::chrono::system_clock;

bool isFirstVertex(int x, int y) {
	return x == 1 && y == 1;
}

bool isLastVertex(int x, int y) {
	return (x == sizex - 2) && (y == sizey - 2);
}

void init() {
	RandomAlphabet* alphabet = new RandomAlphabet();
	mazeGraph = new Graph<CharacterMap*, CharacterMap*>();	
	for(int iy = 0; iy < sizey; iy++)
	{
		for(int ix = 0; ix < sizex; ix++)
		{
			if (iy % 2 == 1 && ix % 2 == 1) {
				char le = alphabet->getLetter();
				CharacterMap* ch = new CharacterMap(ix, iy);
				ch->setCharacter(le);
				VertexMap vertex = new Vertex<CharacterMap*>(ch);
				mazeGraph->addVertex(vertex);
				positions[iy*sizex + ix] = ' ';
			} else {
				positions[iy*sizex + ix] = '@';
			}
		}
	}

	std::list<VertexMap> vertexes = mazeGraph->getAllVertexes();
	auto itr = vertexes.begin();
	VertexMap vti2 = nullptr;
	VertexMap vti = nullptr;
	CharacterMap* vch = nullptr;
	CharacterMap* vch2 = nullptr;
	while(itr != vertexes.end()) {
		vti = *itr;
		vch = vti->getContent();
			std::list<VertexMap>::iterator itr2 = vertexes.begin();
			while(itr2 != vertexes.end()) {
				vti2 = *itr2;
				vch2 = vti2->getContent();
				CharacterMap* chnew;
				if (vch->getY() == vch2->getY()) {	
					if (vch->getX() == vch2->getX() + 2) {
						chnew = new CharacterMap(vch2->getX() + 1, vch->getY());
						chnew->setCharacter(' ');
						mazeGraph->addEdge(vti, vti2, chnew);
					}
					if (vch->getX() == vch2->getX() - 2) {
						chnew = new CharacterMap(vch2->getX() - 1, vch->getY());
						chnew->setCharacter(' ');
						mazeGraph->addEdge(vti, vti2, chnew);
					}
				}
				if (vch->getX() == vch2->getX()) {
					if (vch->getY() == vch2->getY() + 2) {
						chnew = new CharacterMap(vch->getX(), vch2->getY() + 1);
						chnew->setCharacter(' ');
						mazeGraph->addEdge(vti, vti2, chnew);
		
					}
					if (vch->getY() == vch2->getY() - 2) {
						chnew = new CharacterMap(vch->getX(), vch2->getY() - 1);
						chnew->setCharacter(' ');
						mazeGraph->addEdge(vti, vti2, chnew);
				
					}
				}
				itr2++;
			}
		CharacterMap* vticharmap = vti->getContent();
		if (isLastVertex(vticharmap->getX(), vticharmap->getY())) {
			vertexFinal = vti;
		}
		if (isFirstVertex(vticharmap->getX(), vticharmap->getY())) {
			vertexInitial = vti;
		}
		itr++;	
	}
	mazeGraph->sortEdges([](Vertex<CharacterMap*>* a, Vertex<CharacterMap*>* b){return a->getContent()->getCharacter() < b->getContent()->getCharacter();});
	mazeDepthGraph = mazeGraph->depthSearchTree();

	std::list<EdgeList> mazesEdgesList = mazeDepthGraph->getAllEdges();
	std::list<EdgeList>::iterator mazeEdges = mazesEdgesList.begin();
	while(mazeEdges != mazesEdgesList.end()) {
		EdgeList ed = *mazeEdges;
		CharacterMap* che = std::get<2>(ed);
		positions[che->getY()*sizex + che->getX()] = che->getCharacter();
		mazeEdges++;
	}
}

void display() {
	for(int iy = 0; iy < sizey; iy++)
	{
		for(int ix = 0; ix < sizex; ix++)
		{	
			char character = positions[iy*sizex + ix];
			char colormapColor = colormap[iy*sizex + ix];
			if (character == '@') {
				std::cout << termcolor::on_cyan << termcolor::white;
			} else {
				std::cout << termcolor::on_grey;
			}
			if (colormapColor == 'b') {
				std::cout << termcolor::on_magenta;
			}
			if (colormapColor == 'g') {
				std::cout << termcolor::on_blue;
			}
			if (isFirstVertex(ix, iy)) {
				std::cout << termcolor::on_green;
			}
			if (isLastVertex(ix, iy)) {
				std::cout << termcolor::on_red;
			}
			std::cout << character;
		}
		std::cout << std::endl;
	}
}

void ClearScreen()
    {
	std::cout << termcolor::on_grey << std::endl;
    if (system("CLS")) system("clear");
    }

void routing() {
	std::list<VertexMap> path = mazeDepthGraph->breadthSearchPath(mazeDepthGraph->getVertexWithSameContent(vertexFinal));
	std::list<VertexMap>::iterator pathIt = path.begin();
	CharacterMap* oldcharmap = nullptr;
	while(pathIt != path.end()) {
		CharacterMap* charmap = (*pathIt)->getContent();
		colormap[charmap->getY()*sizex + charmap->getX()] = 'b';
		if (oldcharmap != nullptr) {
			int middleY = (charmap->getY() + oldcharmap->getY())/2;
			int middleX = (charmap->getX() + oldcharmap->getX())/2;
			colormap[middleY*sizex + middleX] = 'b';
		}
		pathIt++;
		oldcharmap = charmap;
	}
}

void messageInstruction() {
	std::cout << termcolor::on_grey << std::endl;
	std::cout << "\n";
	std::cout << "Digite 1 e pressione enter resolver o labirinto \n";
	std::cout << "Digite 2 e para mostrar passo a passo com algoritmo de (BFS) \n";
	std::cout << "Digite 3 e regerar labirinto \n";
	std::cout << "\n";
}

void emptyColorMap() {
   for( int a = 0; a < sizex* sizey; a = a + 1 ) {
      colormap[a] = 'l';
   }
}

void displayPass(std::list<Vertex<CharacterMap*>*> vertexes) {
	ClearScreen();
	CharacterMap* charveold = nullptr;
	std::list<VertexMap>::iterator mazevertexes = vertexes.begin();
	while(mazevertexes != vertexes.end()) {
		VertexMap ve = *mazevertexes;
		if (ve->getVisited() && ve != nullptr) {
			CharacterMap* charmapve = ve->getContent();
			colormap[charmapve->getY()*sizex + charmapve->getX()] = 'g';
		}
		mazevertexes++;
	}
	display();
	sleep_for(10ms);
}

int main(int, char**){
	ClearScreen();
	char l;
	init();
	display();
	messageInstruction();
	while (true) {
		std::cin >> l;
		if (l == '1') {
			ClearScreen();
			mazeDepthGraph->setDisplayFunction(nullptr);
			routing();
			display();
		}
		if (l == '2') {
			mazeDepthGraph->setDisplayFunction(displayPass);
			routing();
			ClearScreen();
			display();
		}
		if (l == '3') {
			emptyColorMap();
			ClearScreen();
			init();
			display();
		}
		messageInstruction();
	}
	return 0;
}
