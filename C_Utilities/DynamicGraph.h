#ifndef DEF_DYNAMIC_GRAPH
#define DEF_DYNAMIC_GRAPH

#include <deque>
#include <vector>

// Un graphe dynamique est capable de générer un noeud pour chaque indentificateur,
// et chaque noeud peut dynamiquement générer la liste des liens vers ses voisins.
// Un lien contient un noeud (le voisin) et son coût de trajet.

// Fonctions nécessaires de la classe Graphe :
//		Noeud operator[](unsigned int index) const;

// Fonctions nécessaires de la classe Noeud :
//		const std::vector<Lien>& getLinks() const;
//			Note :	std::vector peut être remplacé par tout autre conteneur de List* pouvant être itéré.
// Fonction supplémentaire nécessaire pour les algorithmes A* et DFS :
//		bool isFinal() const;
// Fonction supplémentaire nécessaire pour l'algorithme A* :
//		unsigned int getHeuristic() const;
//			Note :	Cette fonction doit renvoyer la distance minimale estimée du noeud jusqu'à un noeud final.
//					Afin que l'algorithme fourni ici fonctionne, cette fonction doit vérifier :
//						Pour tous noeuds x et y : h(x) <= h(y) + d(x, y).
//					La classe fournie par défaut ici implémente une heuristique nulle :
//					son utilisation revient à effectuer l'algorithme de Dijkstra sur le graphe fourni.

// Fonctions nécessaires de la classe Lien :
//		unsigned int getCost() const;
//		unsigned int getTargetIndex() const;



// Implémentation minimale et efficace en mémoire de ces classes
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
