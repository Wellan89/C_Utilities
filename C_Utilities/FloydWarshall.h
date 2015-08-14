#ifndef DEF_FLOYD_WARSHALL
#define DEF_FLOYD_WARSHALL

#include <vector>
#include "Graph.h"

// Algorithme de Floyd-Warshall : détermine toutes les plus courtes distances entre toutes les paires de points.
template<class Graphe = Graph<> >
class FloydWarshall
{
public:
	typedef typename Graphe::IndexNoeud IndexNoeud;
	typedef typename Graphe::Cout Cout;

protected:
	struct FwPathInfo
	{
		// Un noeud k par lequel passe le chemin (i, j).
		// Lorsque k = j, ce chemin est un simple lien.
		IndexNoeud middleNode;

		Cout totalCost;

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

	void appendShortestPath(std::vector<IndexNoeud>& v, IndexNoeud startNode, IndexNoeud endNode) const
	{
		if (startNode == endNode)
			return;

		IndexNoeud middleNode = fw[startNode][endNode].middleNode;
		if (middleNode == endNode)
		{
			// Il existe un simple lien entre startNode et endNode
			//v.push_back(startNode);	// Cette fonction n'ajoute jamais le premier noeud dans le vecteur
			v.push_back(endNode);
		}
		else
		{
			appendShortestPath(v, startNode, middleNode);
			appendShortestPath(v, middleNode, endNode);
		}
	}

public:
	FloydWarshall(const Graphe& gr) : g(gr) { reset(); }

	void computeShortestPaths()
	{
		// Réinitialise les informations sur les noeuds
		reset();

		// Indique le coût des liens de base
		IndexNoeud nodesCount = g.size();
		for (IndexNoeud i = 0; i < nodesCount; i++)
		{
			fw[i][i].totalCost = 0;
			fw[i][i].middleNode = i;
		}
		for (IndexNoeud node = 0; node < nodesCount; node++)	// Parcours toutes les arêtes du graphe
		{
			const auto& links = g[node].getLinks();
			for (auto it = links.begin(); it != links.end(); ++it)
			{
				IndexNoeud targetNode = it->getTargetIndex();
				Cout linkCost = it->getCost();
				if (linkCost < fw[node][targetNode].totalCost)
				{
					fw[node][targetNode].totalCost = linkCost;
					fw[node][targetNode].middleNode = targetNode;
				}
			}
		}

		// Détermine pour tout noeud k s'il est plus court de passer par ce noeud pour chaque paire de noeuds (i, j)
		for (IndexNoeud k = 0; k < nodesCount; k++)
		{
			for (IndexNoeud i = 0; i < nodesCount; i++)
			{
				if (i == k)
					continue;

				Cout i_k_cost = fw[i][k].totalCost;
				if (i_k_cost == Graphe::INFINITE_COST)
					continue;

				for (IndexNoeud j = 0; j < nodesCount; j++)
				{
					Cout k_j_cost = fw[k][j].totalCost;
					if (k_j_cost == Graphe::INFINITE_COST)
						continue;

					Cout newCost = i_k_cost + k_j_cost;
					if (newCost < fw[i][j].totalCost)
					{
						fw[i][j].totalCost = newCost;
						fw[i][j].middleNode = k;
					}
				}
			}
		}
	}

	bool pathExists(IndexNoeud startNode, IndexNoeud endNode) const
	{
		return (fw[startNode][endNode].totalCost != Graphe::INFINITE_COST);
	}
	Cout getPathCost(IndexNoeud startNode, IndexNoeud endNode) const
	{
		return fw[startNode][endNode].totalCost;
	}
	std::vector<IndexNoeud> getShortestPath(IndexNoeud startNode, IndexNoeud endNode) const
	{
		std::vector<IndexNoeud> v(1, startNode);

		if (pathExists(startNode, endNode))
			appendShortestPath(v, startNode, endNode);

		return v;
	}
};

#endif
