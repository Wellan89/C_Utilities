#ifndef DEF_A_STAR_DYNAMIC
#define DEF_A_STAR_DYNAMIC

#include <vector>
#include <deque>
#include <map>
#include "DynamicGraph.h"

// Algorithme A* : parcours le graphe depuis un point de départ en suivant une heuristique,
// et s'arrête dès qu'un point final est trouvé.
// Gère aussi les graphes dynamiques.
template<class Graphe = DynamicGraph<> >
class AStarDynamic
{
protected:
	struct ASDNodeInfo
	{
		unsigned int previousNode;
		unsigned int totalCost;
		bool alreadyVisited;

		ASDNodeInfo() : previousNode((unsigned int)(-1)), totalCost((unsigned int)(-1)), alreadyVisited(false)
			{ }
	};

	std::map<unsigned int, ASDNodeInfo> asd;
	const Graphe& g;

	// Le noeud final trouvé par cet algorithme
	unsigned int endNode;

	// Réinitialise les informations sur les noeuds
	void reset()
	{
		asd.clear();
		endNode = (unsigned int)(-1);
	}

public:
	AStarDynamic(const Graphe& gr) : g(gr) { reset(); }

	void computeShortestPathFrom(unsigned int startNode);

	bool hasFoundPath() const;
	unsigned int getFinalNode() const;
	unsigned int getPathCost();
	std::deque<unsigned int> getShortestPath();
	std::vector<unsigned int> getReverseShortestPath();
};

#endif
