#ifndef DEF_A_STAR
#define DEF_A_STAR

#include <vector>
#include <deque>
#include "Graph.h"

// Permet de diminuer la mémoire utilisée par cette classe (et permet ainsi à cet algorithme de travailler sur des graphes plus grands),
// en contre-partie l'algorithme se révèlera légèrement plus lent sur des graphes contenant de nombreux liens au coût nul,
// mais il sera légèrement plus rapide sur des graphes sans ce type de lien.
#define A_STAR_CLOSED_SET_REMOVAL_OPTIMIZATION

// Algorithme A* : pour un graphe où toutes les arêtes ont un coût positif,
// parcours le graphe depuis un point de départ en suivant une heuristique, et s'arrête dès qu'un point final est trouvé.
template<class Graphe = Graph<> >
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
#ifndef A_STAR_CLOSED_SET_REMOVAL_OPTIMIZATION
		bool alreadyVisited;
#endif

		ASNodeInfo() : previousNode(Graphe::INVALID_NODE_INDEX()), totalCost(Graphe::INFINITE_COST())
#ifndef A_STAR_CLOSED_SET_REMOVAL_OPTIMIZATION
			, alreadyVisited(false)
#endif
			{ }
	};

	std::vector<ASNodeInfo> as;
	const Graphe& g;

	// Le noeud final trouvé par cet algorithme
	IndexNoeud endNode;

#ifdef _DEBUG
	// Le nombre de noeuds explorés (marqués comme visités) par cet algorithme
	IndexNoeud nbExploredNodes;
#endif

#ifdef A_STAR_CLOSED_SET_REMOVAL_OPTIMIZATION
	// La priorité du dernier noeud validé par l'algorithme : celle-ci est toujours croissante,
	// et permet ainsi de déterminer les noeuds déjà visités simplement par leur priorité.
	Cout currentPriority;
#endif

	// Réinitialise les informations sur les noeuds
	void reset()
	{
		as.clear();
		as.resize(g.size());
		endNode = Graphe::INVALID_NODE_INDEX();
#ifdef _DEBUG
		nbExploredNodes = 0;
#endif
#ifdef A_STAR_CLOSED_SET_REMOVAL_OPTIMIZATION
		currentPriority = 0;
#endif
	}

public:
	AStar(const Graphe& gr) : g(gr)	{ reset(); }

	void computeShortestPathFrom(IndexNoeud startNode);

	bool hasFoundPath() const;
	IndexNoeud getFinalNode() const;
	Cout getPathCost() const;
	std::deque<IndexNoeud> getShortestPath() const;
	std::vector<IndexNoeud> getReverseShortestPath() const;

#ifdef _DEBUG
	IndexNoeud getNbExploredNodes() const
	{
		return nbExploredNodes;
	}
#endif
};

#endif
