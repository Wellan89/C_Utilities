#ifndef DEF_A_STAR
#define DEF_A_STAR

#include <vector>
#include <deque>
#include "Graph.h"

// Algorithme A* : parcours le graphe depuis un point de départ en suivant une heuristique,
// et s'arrête dès qu'un point final est trouvé.
template<class Graphe = Graph, class Noeud = Node, class Lien = Link>
class AStar
{
protected:
	struct ASNodeInfo
	{
		unsigned int previousNode;
		unsigned int totalCost;
		unsigned int totalEstimatedCost;
		bool alreadyVisited;

		ASNodeInfo() : previousNode((unsigned int)(-1)), totalCost((unsigned int)(-1)),
			totalEstimatedCost((unsigned int)(-1)), alreadyVisited(false) { }
	};

	std::vector<ASNodeInfo> as;
	const Graphe& g;

	// Le noeud final trouvé par cet algorithme
	unsigned int endNode;

public:
	AStar(const Graphe& gr) : g(gr), endNode((unsigned int)(-1)) { }

	void computeShortestPathFrom(unsigned int startNode);

	bool hasFoundPath() const;
	unsigned int getFinalNode() const;
	unsigned int getPathCost() const;
	std::deque<unsigned int> getShortestPath() const;
	std::vector<unsigned int> getReverseShortestPath() const;
};

#endif
