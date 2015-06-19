#ifndef DEF_A_STAR_DYNAMIC
#define DEF_A_STAR_DYNAMIC

#include <vector>
#include <deque>
#include <map>
#include "DynamicGraph.h"

// Algorithme A* : parcours le graphe depuis un point de d�part en suivant une heuristique,
// et s'arr�te d�s qu'un point final est trouv�.
// G�re aussi les graphes dynamiques.
template<class Graphe = DynamicGraph<> >
class AStarDynamic
{
public:
	typedef typename Graphe::IndexNoeud IndexNoeud;
	typedef typename Graphe::Cout Cout;

protected:
	struct ASDNodeInfo
	{
		IndexNoeud previousNode;
		Cout totalCost;
		bool alreadyVisited;

		ASDNodeInfo() : previousNode(Graphe::INVALID_NODE_INDEX()), totalCost(Graphe::INFINITE_COST()), alreadyVisited(false)
			{ }
	};

	std::map<IndexNoeud, ASDNodeInfo> asd;
	const Graphe& g;

	// Le noeud final trouv� par cet algorithme
	IndexNoeud endNode;

	// R�initialise les informations sur les noeuds
	void reset()
	{
		asd.clear();
		endNode = Graphe::INVALID_NODE_INDEX();
	}

public:
	AStarDynamic(const Graphe& gr) : g(gr) { reset(); }

	void computeShortestPathFrom(IndexNoeud startNode);

	bool hasFoundPath() const;
	IndexNoeud getFinalNode() const;
	Cout getPathCost();
	std::deque<IndexNoeud> getShortestPath();
	std::vector<IndexNoeud> getReverseShortestPath();

	size_t getNbExploredNodes() const
	{
		return asd.size();
	}
};

#endif
