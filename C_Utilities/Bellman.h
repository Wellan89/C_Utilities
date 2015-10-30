#ifndef DEF_BELLMAN
#define DEF_BELLMAN

#include <deque>
#include <vector>
#include <utility>
#include "Graph.h"

// Algorithme de Bellman : pour un graphe sans circuits (i.e. avec un ordre topologique),
// calcule tous les chemins les plus courts depuis un sommmet donn�.
template<class Graphe = Graph<> >
class Bellman
{
public:
	typedef typename Graphe::IndexNoeud IndexNoeud;
	typedef typename Graphe::Cout Cout;

protected:
	struct BmNodeInfo
	{
		IndexNoeud previousNode;
		Cout totalCost;

		BmNodeInfo() : previousNode(Graphe::INVALID_NODE_INDEX), totalCost(Graphe::INFINITE_COST)
		{ }
	};

	std::vector<BmNodeInfo> bm;
	const Graphe& g;

	// Indique si un cycle a �t� d�tect� dans le graphe
	bool cycleFound;

	// D�termine si la somme de deux co�ts d�passe les valeurs maximales ou minimales d'un co�t.
	bool costAddOverflow(Cout a, Cout b)
	{
		if (a >= 0)	// Ce test sera optimis� si Cout est un type non sign�
		{
			// On ne veut pas pouvoir ajouter "b = -1" � "a = Graphe::INFINITE_COST" par exemple.
			// De plus, on �vite d'obtenir un r�sultat �gal � Graphe::INFINITE_COST.
			return (a == Graphe::INFINITE_COST
				|| b >= std::numeric_limits<Cout>::max() - a);
		}
		else
		{
			// On ne veut pas pouvoir ajouter "a = -1" � "b = Graphe::INFINITE_COST" par exemple.
			return (b == Graphe::INFINITE_COST
				|| b < std::numeric_limits<Cout>::min() - a);
		}
	}

	// R�initialise les informations sur les noeuds
	void reset()
	{
		bm.clear();
		bm.resize(g.size());
		cycleFound = false;
	}

public:
	Bellman(const Graphe& gr) : g(gr)	{ reset(); }

	void computeShortestPathsFrom(IndexNoeud startNode)
	{
		// R�initialise les informations sur les noeuds
		reset();

		// Obtient les noeuds du graphe suivant un ordre topologique inverse
		auto orderedNodes = g.getReverseTopologicalyOrderedNodes();

		// D�marre � partir du noeud sp�cifi� (on ignore les noeuds ne pouvant �tre atteints depuis celui-ci)
		auto startIt = find(orderedNodes.rbegin(), orderedNodes.rend(), startNode);

		// Si l'ordre topologique n'a pas pu �tre construit, on quitte ici.
		if (startIt == orderedNodes.rend())
		{
			cycleFound = true;
			return;
		}

		// G�n�re l'ensemble des liens inverse de ce graphe :
		// Pour chaque noeud, l'ensemble des noeuds ayant un lien vers celui-ci et le co�t de ce lien.
		std::vector<std::vector<std::pair<IndexNoeud, Cout> > > incomingLinks(g.size());
		IndexNoeud nodesCount = g.size();
		for (IndexNoeud i = 0; i < nodesCount; i++)
		{
			const auto& links = g[i].getLinks();
			for (auto it = links.begin(); it != links.end(); ++it)
				incomingLinks[it->getTargetIndex()].push_back(std::pair<IndexNoeud, Cout>(i, it->getCost()));
		}

		// Indique le co�t du premier noeud
		bm[startNode].totalCost = 0;

		// Attention : on parcours le vecteur en sens inverse !
		for (auto nodeIt = startIt + 1; nodeIt != orderedNodes.rend(); ++nodeIt)
		{
			IndexNoeud node = (*nodeIt);
			const auto& incLinks = incomingLinks[node];
			for (auto incLinkIt = incLinks.begin(); incLinkIt != incLinks.end(); ++incLinkIt)
			{
				IndexNoeud prevNode = incLinkIt->first;
				Cout linkCost = incLinkIt->second;

				// Evite de d�passer le co�t maximal en calculant le nouveau co�t ici :
				// On ignore ce chemin en cas de d�passement puisqu'on ne peut pas le g�rer.
				if (costAddOverflow(bm[prevNode].totalCost, linkCost))
					continue;

				Cout newCost = bm[prevNode].totalCost + linkCost;
				if (newCost < bm[node].totalCost)
				{
					bm[node].totalCost = newCost;
					bm[node].previousNode = prevNode;
				}
			}
		}
	}

	bool cycleDetected() const
	{
		return cycleFound;
	}
	bool canReachNode(IndexNoeud node) const
	{
		return (bm[node].totalCost != Graphe::INFINITE_COST);
	}
	Cout getCostTo(IndexNoeud node) const
	{
		return bm[node].totalCost;
	}
	std::deque<IndexNoeud> getShortestPathTo(IndexNoeud endNode) const
	{
		std::deque<IndexNoeud> l;
		while (endNode != Graphe::INVALID_NODE_INDEX)
		{
			l.push_front(endNode);
			endNode = bm[endNode].previousNode;
		}
		return l;
	}
	std::vector<IndexNoeud> getReverseShortestPathTo(IndexNoeud endNode) const
	{
		std::vector<IndexNoeud> l;
		while (endNode != Graphe::INVALID_NODE_INDEX)
		{
			l.push_back(endNode);
			endNode = bm[endNode].previousNode;
		}
		return l;
	}
};

#endif
