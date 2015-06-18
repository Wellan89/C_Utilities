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
public:
	typedef typename Graphe::IndexNoeud IndexNoeud;
	typedef typename Graphe::Cout Cout;

protected:
	struct ASNodeInfo
	{
		IndexNoeud previousNode;
		Cout totalCost;
		bool alreadyVisited;

		ASNodeInfo() : previousNode(Graphe::INVALID_NODE_INDEX), totalCost(Graphe::INFINITE_COST), alreadyVisited(false)
			{ }
	};

	std::vector<ASNodeInfo> as;
	const Graphe& g;

	// Le noeud final trouvé par cet algorithme
	IndexNoeud endNode;

	// Réinitialise les informations sur les noeuds
	void reset()
	{
		as.clear();
		as.resize(g.size());
		endNode = Graphe::INVALID_NODE_INDEX;
	}

public:
	AStar(const Graphe& gr) : g(gr) { reset(); }

	void computeShortestPathFrom(IndexNoeud startNode);

	bool hasFoundPath() const;
	IndexNoeud getFinalNode() const;
	Cout getPathCost() const;
	std::deque<IndexNoeud> getShortestPath() const;
	std::vector<IndexNoeud> getReverseShortestPath() const;
};

#endif
