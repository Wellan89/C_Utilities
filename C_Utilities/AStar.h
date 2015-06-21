#ifndef DEF_A_STAR
#define DEF_A_STAR

#include <vector>
#include <deque>
#include "Graph.h"

// Permet de diminuer la m�moire utilis�e par cette classe (et permet ainsi � cet algorithme de travailler sur des graphes plus grands),
// en contre-partie l'algorithme se r�v�lera l�g�rement plus lent sur des graphes contenant de nombreux liens au co�t nul,
// mais il sera l�g�rement plus rapide sur des graphes sans ce type de lien.
#define A_STAR_CLOSED_SET_REMOVAL_OPTIMIZATION

// Algorithme A* : pour un graphe o� toutes les ar�tes ont un co�t positif,
// parcours le graphe depuis un point de d�part en suivant une heuristique, et s'arr�te d�s qu'un point final est trouv�.
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

	// Le noeud final trouv� par cet algorithme
	IndexNoeud endNode;

#ifdef _DEBUG
	// Le nombre de noeuds explor�s (marqu�s comme visit�s) par cet algorithme
	IndexNoeud nbExploredNodes;
#endif

#ifdef A_STAR_CLOSED_SET_REMOVAL_OPTIMIZATION
	// La priorit� du dernier noeud valid� par l'algorithme : celle-ci est toujours croissante,
	// et permet ainsi de d�terminer les noeuds d�j� visit�s simplement par leur priorit�.
	Cout currentPriority;
#endif

	// R�initialise les informations sur les noeuds
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
