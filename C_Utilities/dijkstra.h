#ifndef DEF_DIJKSTRA
#define DEF_DIJKSTRA

#include <deque>
#include <vector>
#include "Graph.h"
#include "cost_priority_queue.h"

// Permet de diminuer la m�moire utilis�e par cette classe (et permet ainsi � cet algorithme de travailler sur des graphes plus grands),
// en contre-partie l'algorithme se r�v�lera l�g�rement plus lent sur des graphes contenant de nombreux liens au co�t nul,
// mais il sera l�g�rement plus rapide sur des graphes sans ce type de lien.
#define DIJKSTRA_CLOSED_SET_REMOVAL_OPTIMIZATION

// Algorithme de Dijkstra : pour un graphe o� toutes les ar�tes ont un co�t positif,
// parcours tout le graphe depuis un point de d�part et d�termine tous les chemins les plus courts depuis ce point.
template<class Graphe = Graph<> >
class Dijkstra
{
public:
	typedef typename Graphe::IndexNoeud IndexNoeud;
	typedef typename Graphe::Cout Cout;

protected:
	struct DjNodeInfo
	{
		IndexNoeud previousNode;
		Cout totalCost;
#ifndef DIJKSTRA_CLOSED_SET_REMOVAL_OPTIMIZATION
		bool alreadyVisited;
#endif

		DjNodeInfo() : previousNode(Graphe::INVALID_NODE_INDEX), totalCost(Graphe::INFINITE_COST)
#ifndef DIJKSTRA_CLOSED_SET_REMOVAL_OPTIMIZATION
			, alreadyVisited(false)
#endif
			{ }
	};

	std::vector<DjNodeInfo> dj;
	const Graphe& g;

	// Indique si un lien au co�t n�gatif a �t� d�tect� dans le graphe
	bool negativeLinkFound;

	// R�initialise les informations sur les noeuds
	void reset()
	{
		dj.clear();
		dj.resize(g.size());
		negativeLinkFound = false;
	}

public:
	Dijkstra(const Graphe& gr) : g(gr) { reset(); }

	void computeShortestPathsFrom(IndexNoeud startNode)
	{
		// R�initialise les informations sur les noeuds
		reset();

		// Indique le co�t du premier noeud et l'ajoute � la liste des noeuds � parcourir
		cost_priority_queue<IndexNoeud, Cout> nodesToSee;
		dj[startNode].totalCost = 0;
		nodesToSee.push(startNode, dj[startNode].totalCost);

#ifdef DIJKSTRA_CLOSED_SET_REMOVAL_OPTIMIZATION
		// Le co�t du dernier chemin valid� par l'algorithme : celui-ci est toujours croissant,
		// et permet ainsi de d�terminer les noeuds d�j� visit�s simplement par leur co�t.
		Cout currentCost = 0;
#endif

		while (!nodesToSee.empty())
		{
			IndexNoeud node = nodesToSee.top();
			nodesToSee.pop();
#ifndef DIJKSTRA_CLOSED_SET_REMOVAL_OPTIMIZATION
			if (dj[node].alreadyVisited)
				continue;

			dj[node].alreadyVisited = true;
			Cout nodeTotalCost = dj[node].totalCost;
#else
			Cout nodeTotalCost = dj[node].totalCost;
			if (nodeTotalCost < currentCost)
				continue;

			currentCost = nodeTotalCost;
#endif

			const auto& links = g[node].getLinks();
			for (auto it = links.begin(); it != links.end(); ++it)
			{
				Cout linkCost = it->getCost();
				if (linkCost < 0)
				{
					// On a trouv� une ar�te avec un co�t n�gatif : on quitte ici.
					reset();
					negativeLinkFound = true;
					return;
				}

				IndexNoeud targetNode = it->getTargetIndex();
				Cout newCost = nodeTotalCost + linkCost;
				if (newCost < dj[targetNode].totalCost)
				{
					dj[targetNode].previousNode = node;
					dj[targetNode].totalCost = newCost;
					nodesToSee.push(targetNode, dj[targetNode].totalCost);
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
		return (dj[node].totalCost != Graphe::INFINITE_COST);
	}
	Cout getCostTo(IndexNoeud node) const
	{
		return dj[node].totalCost;
	}
	std::deque<IndexNoeud> getShortestPathTo(IndexNoeud endNode) const
	{
		std::deque<IndexNoeud> l;
		while (endNode != Graphe::INVALID_NODE_INDEX)
		{
			l.push_front(endNode);
			endNode = dj[endNode].previousNode;
		}
		return l;
	}
	std::vector<IndexNoeud> getReverseShortestPathTo(IndexNoeud endNode) const
	{
		std::vector<IndexNoeud> l;
		while (endNode != Graphe::INVALID_NODE_INDEX)
		{
			l.push_back(endNode);
			endNode = dj[endNode].previousNode;
		}
		return l;
	}
};

#endif
