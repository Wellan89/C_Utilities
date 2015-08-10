#ifndef DEF_A_STAR_DYNAMIC
#define DEF_A_STAR_DYNAMIC

#include <vector>
#include <deque>
#include <map>
#include "DynamicGraph.h"

// Permet de diminuer la m�moire utilis�e par cette classe (et permet ainsi � cet algorithme de travailler sur des graphes plus grands),
// en contre-partie l'algorithme se r�v�lera l�g�rement plus lent sur des graphes contenant de nombreux liens au co�t nul,
// mais il sera l�g�rement plus rapide sur des graphes sans ce type de lien.
#define A_STAR_DYNAMIC_CLOSED_SET_REMOVAL_OPTIMIZATION

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
#ifndef A_STAR_DYNAMIC_CLOSED_SET_REMOVAL_OPTIMIZATION
		bool alreadyVisited;
#endif

		ASDNodeInfo() : previousNode(Graphe::INVALID_NODE_INDEX()), totalCost(Graphe::INFINITE_COST())
#ifndef A_STAR_DYNAMIC_CLOSED_SET_REMOVAL_OPTIMIZATION
			, alreadyVisited(false)
#endif
			{ }
	};

	std::map<IndexNoeud, ASDNodeInfo> asd;
	const Graphe& g;

	// Le noeud final trouv� par cet algorithme
	IndexNoeud endNode;

#ifdef _DEBUG
	// Le nombre de noeuds explor�s (marqu�s comme visit�s) par cet algorithme
	IndexNoeud nbExploredNodes;
#endif

	// Indique si un lien au co�t n�gatif a �t� d�tect� dans le graphe
	bool negativeLinkFound;

	// R�initialise les informations sur les noeuds
	void reset()
	{
		asd.clear();
		endNode = Graphe::INVALID_NODE_INDEX();
#ifdef _DEBUG
		nbExploredNodes = 0;
#endif
		negativeLinkFound = false;
	}

public:
	AStarDynamic(const Graphe& gr) : g(gr) { reset(); }

	void computeShortestPathFrom(IndexNoeud startNode);

	bool negativeLinkDetected() const
	{
		return negativeLinkFound;
	}
	bool hasFoundPath() const;
	IndexNoeud getFinalNode() const;
	Cout getPathCost();
	std::deque<IndexNoeud> getShortestPath();
	std::vector<IndexNoeud> getReverseShortestPath();

#ifdef _DEBUG
	IndexNoeud getNbExploredNodes() const
	{
		return nbExploredNodes;
	}
#endif
};

#endif
