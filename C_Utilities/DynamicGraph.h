#ifndef DEF_DYNAMIC_GRAPH
#define DEF_DYNAMIC_GRAPH

#include <deque>
#include <vector>

// Un graphe dynamique est capable de g�n�rer un noeud pour chaque indentificateur,
// et chaque noeud peut dynamiquement g�n�rer la liste des liens vers ses voisins.
// Un lien contient un noeud (le voisin) et son co�t de trajet.

// Fonctions n�cessaires de la classe Graphe :
//		Noeud operator[](unsigned int index) const;

// Fonctions n�cessaires de la classe Noeud :
//		const std::vector<Lien>& getLinks() const;
//			Note :	std::vector peut �tre remplac� par tout autre conteneur de List* pouvant �tre it�r�.
// Fonction suppl�mentaire n�cessaire pour les algorithmes A* et DFS :
//		bool isFinal() const;
// Fonction suppl�mentaire n�cessaire pour l'algorithme A* :
//		unsigned int getHeuristic() const;
//			Note :	Cette fonction doit renvoyer la distance minimale estim�e du noeud jusqu'� un noeud final.
//					Afin que l'algorithme fourni ici fonctionne, cette fonction doit v�rifier :
//						Pour tous noeuds x et y : h(x) <= h(y) + d(x, y).
//					La classe fournie par d�faut ici impl�mente une heuristique nulle :
//					son utilisation revient � effectuer l'algorithme de Dijkstra sur le graphe fourni.

// Fonctions n�cessaires de la classe Lien :
//		unsigned int getCost() const;
//		unsigned int getTargetIndex() const;



// Impl�mentation minimale et efficace en m�moire de ces classes
class DynamicLink;
class EmptyDynamicLinkVectorGenerator
{
	std::vector<DynamicLink> operator()(unsigned int index) const
	{	return std::vector<DynamicLink>();	}
};
class FalseGenerator
{
	bool operator()(unsigned int index) const
	{	return false;						}
};
class NullGenerator
{
	unsigned int operator()(unsigned int index) const
	{	return 0;							}
};
class DynamicLink
{
protected:
	unsigned int targetIndex;
	unsigned int cost;

public:
	unsigned int getTargetIndex() const	{ return targetIndex; }
	unsigned int getCost() const		{ return cost; }

	DynamicLink(unsigned int _targetIndex, unsigned int _cost)
		: targetIndex(_targetIndex), cost(_cost) { }
};
class DynamicNode
{
protected:
	std::vector<DynamicLink> links;
	unsigned int heuristic;
	bool finalNode;

public:
	const std::vector<DynamicLink>& getLinks() const	{ return links; }
	bool isFinal() const								{ return finalNode; }
	unsigned int getHeuristic() const					{ return heuristic; }

	DynamicNode(std::vector<DynamicLink> _links,
		bool _finalNode = false, unsigned int _heuristic = (unsigned int)(-1))
		: links(_links), finalNode(_finalNode), heuristic(_heuristic)
	{ }
};
template<class NodeLinksGenerator = EmptyDynamicLinkVectorGenerator,
	class NodeFinalGenerator = FalseGenerator,
	class NodeHeuristicGenerator = NullGenerator>
class DynamicGraph
{
	const NodeLinksGenerator& linksGen;
	const NodeFinalGenerator& finalGen;
	const NodeHeuristicGenerator& heuristicGen;

public:
	DynamicGraph(const NodeLinksGenerator& linksGenerator,
		const NodeFinalGenerator& finalGenerator,
		const NodeHeuristicGenerator& heuristicGenerator)
		: linksGen(linksGenerator), finalGen(finalGenerator), heuristicGen(heuristicGenerator)
	{ }

	DynamicNode operator[](unsigned int index) const
	{
		return DynamicNode(linksGen(index), finalGen(index), heuristicGen(index));
	}
};

#endif
