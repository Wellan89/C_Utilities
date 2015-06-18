#ifndef DEF_FLOYD_WARSHALL
#define DEF_FLOYD_WARSHALL

#include <vector>
#include "Graph.h"

// Algorithme de Floyd-Warshall : détermine toutes les plus courtes distances entre toutes les paires de points.
template<class Graphe = Graph>
class FloydWarshall
{
public:
	typedef typename Graphe::IndexNoeud IndexNoeud;
	typedef typename Graphe::Cout Cout;

protected:
	struct FwPathInfo
	{
		Cout totalCost;

		// Un noeud k par lequel passe le chemin (i, j).
		// Lorsque k = j, ce chemin est un simple lien.
		IndexNoeud middleNode;

		FwPathInfo() : totalCost(Graphe::INFINITE_COST), middleNode(Graphe::INVALID_NODE_INDEX)
			{ }
	};

	std::vector<std::vector<FwPathInfo> > fw;
	const Graphe& g;

	// Réinitialise les informations sur les noeuds
	void reset()
	{
		fw.clear();
		IndexNoeud nodesCount = g.size();
		fw.resize(nodesCount);
		for (IndexNoeud i = 0; i < nodesCount; i++)
			fw[i].resize(nodesCount);
	}

	void appendShortestPath(std::vector<IndexNoeud>& v, IndexNoeud startNode, IndexNoeud endNode) const;

public:
	FloydWarshall(const Graphe& gr) : g(gr) { reset(); }

	void computeShortestPaths();

	bool pathExists(IndexNoeud startNode, IndexNoeud endNode) const;
	Cout getPathCost(IndexNoeud startNode, IndexNoeud endNode) const;
	std::vector<IndexNoeud> getShortestPath(IndexNoeud startNode, IndexNoeud endNode) const
	{
		std::vector<IndexNoeud> v(1, startNode);

		if (pathExists(startNode, endNode))
			appendShortestPath(v, startNode, endNode);

		return v;
	}
};

#endif
