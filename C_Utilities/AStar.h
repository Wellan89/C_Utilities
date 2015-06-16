#ifndef DEF_A_STAR
#define DEF_A_STAR

#include <vector>
#include <deque>
#include "Graph.h"

// Algorithme A* : parcours le graphe depuis un point de départ en suivant une heuristique,
// et s'arrête dès qu'un point final est trouvé.
template<class Graphe = Graph>
class AStar
{
protected:
	struct ASNodeInfo
	{
		unsigned int previousNode;
		unsigned int totalCost;
		bool alreadyVisited;

		ASNodeInfo() : previousNode((unsigned int)(-1)), totalCost((unsigned int)(-1)), alreadyVisited(false)
			{ }
	};

	std::vector<ASNodeInfo> as;
	const Graphe& g;

	// Le noeud final trouvé par cet algorithme
	unsigned int endNode;

	// Réinitialise les informations sur les noeuds
	void reset()
	{
		as.clear();
		as.resize(g.size());
		endNode = (unsigned int)(-1);
	}

public:
	AStar(const Graphe& gr) : g(gr) { reset(); }

	void computeShortestPathFrom(unsigned int startNode);

	bool hasFoundPath() const;
	unsigned int getFinalNode() const;
	unsigned int getPathCost() const;
	std::deque<unsigned int> getShortestPath() const;
	std::vector<unsigned int> getReverseShortestPath() const;
};

#endif
