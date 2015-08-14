#ifndef DEF_BFS_SHORTEST_PATH
#define DEF_BFS_SHORTEST_PATH

#include <vector>
#include <deque>
#include <map>
#include "Graph.h"

// Algorithme du plus court chemin suivant un parcours en largeur : pour un graphe où toutes les arêtes ont un coût positif,
// parcours le graphe en largeur et trouve tous les chemins les plus courts à partir d'un point donné.
template<class Graphe = Graph<> >
class BFS_ShortestPath
{
public:
	typedef typename Graphe::IndexNoeud IndexNoeud;
	typedef typename Graphe::Cout Cout;

protected:
	// Informations sur un noeud
	struct BFSNodeInfo
	{
		IndexNoeud previousNode;
		Cout totalCost;

		BFSNodeInfo() : previousNode(Graphe::INVALID_NODE_INDEX), totalCost(Graphe::INFINITE_COST)
			{ }
	};

	std::vector<BFSNodeInfo> bfs;
	const Graphe& g;

	// Indique si un lien au coût négatif a été détecté dans le graphe
	bool negativeLinkFound;

	// Réinitialise les informations sur les noeuds
	void reset()
	{
		bfs.clear();
		bfs.resize(g.size());
		negativeLinkFound = false;
	}

public:
	BFS_ShortestPath(const Graphe& gr) : g(gr) { reset(); }

	void computeShortestPathsFrom(IndexNoeud startNode)
	{
		// Réinitialise les informations sur les noeuds
		reset();

		// Indique le coût du premier noeud et l'ajoute à la liste des noeuds à parcourir
		std::deque<IndexNoeud> nodesToSee;
		bfs[startNode].totalCost = 0;
		nodesToSee.push_back(startNode);

		while (!nodesToSee.empty())
		{
			IndexNoeud node = nodesToSee.front();
			nodesToSee.pop_front();

			Cout nodeTotalCost = bfs[node].totalCost;

			const auto& links = g[node].getLinks();
			for (auto it = links.begin(); it != links.end(); ++it)
			{
				IndexNoeud targetNode = it->getTargetIndex();

				Cout linkCost = it->getCost();
				if (linkCost < 0)
				{
					// On a trouvé une arête avec un coût négatif : on quitte ici.
					reset();
					negativeLinkFound = true;
					return;
				}

				Cout newCost = nodeTotalCost + linkCost;
				if (newCost < bfs[targetNode].totalCost)
				{
					bfs[targetNode].previousNode = node;
					bfs[targetNode].totalCost = newCost;
					nodesToSee.push_back(targetNode);
				}
			}
		}
	}

	bool negativeLinkDetected() const
	{
		return negativeLinkFound;
	}
	bool canReachNode(IndexNoeud node) const
	{
		return (bfs[node].totalCost != Graphe::INFINITE_COST);
	}
	Cout getCostTo(IndexNoeud node) const
	{
		return bfs[node].totalCost;
	}
	std::deque<IndexNoeud> getShortestPathTo(IndexNoeud endNode) const
	{
		std::deque<IndexNoeud> l;
		while (endNode != Graphe::INVALID_NODE_INDEX)
		{
			l.push_front(endNode);
			endNode = bfs[endNode].previousNode;
		}
		return l;
	}
	std::vector<IndexNoeud> getReverseShortestPathTo(IndexNoeud endNode) const
	{
		std::vector<IndexNoeud> l;
		while (endNode != Graphe::INVALID_NODE_INDEX)
		{
			l.push_back(endNode);
			endNode = bfs[endNode].previousNode;
		}
		return l;
	}
};

#endif
