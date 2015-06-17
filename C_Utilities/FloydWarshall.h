 #ifndef DEF_FLOYD_WARSHALL
#define DEF_FLOYD_WARSHALL

#include <vector>
#include "Graph.h"

// Algorithme de Floyd-Warshall : détermine toutes les plus courtes distances entre toutes les paires de points.
template<class Graphe = Graph>
class FloydWarshall
{
protected:
	struct FwPathInfo
	{
		unsigned int totalCost;

		// Un noeud k par lequel passe le chemin (i, j).
		// Lorsque k = j, ce chemin est un simple lien.
		unsigned int middleNode;

		FwPathInfo() : totalCost((unsigned int)(-1)), middleNode((unsigned int)(-1))
			{ }
	};

	std::vector<std::vector<FwPathInfo> > fw;
	const Graphe& g;

	// Réinitialise les informations sur les noeuds
	void reset()
	{
		fw.clear();
		unsigned int nodesCount = g.size();
		fw.resize(nodesCount);
		for (unsigned int i = 0; i < nodesCount; i++)
			fw[i].resize(nodesCount);
	}

	void appendShortestPath(std::vector<unsigned int>& v, unsigned int startNode, unsigned int endNode) const;

public:
	FloydWarshall(const Graphe& gr) : g(gr) { reset(); }

	void computeShortestPaths();

	bool pathExists(unsigned int startNode, unsigned int endNode) const;
	unsigned int getPathCost(unsigned int startNode, unsigned int endNode) const;
	std::vector<unsigned int> getShortestPath(unsigned int startNode, unsigned int endNode) const
	{
		std::vector<unsigned int> v;
		v.push_back(startNode);
		appendShortestPath(v, startNode, endNode);
		return v;
	}
};

#endif
