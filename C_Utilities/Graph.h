#ifndef DEF_GRAPH
#define DEF_GRAPH

#include <vector>

// Un graphe est un tableau de noeuds dont chacun contient la liste des liens vers ses voisins.
// Un lien contient un noeud (le voisin) et son co�t de trajet.

// Fonctions n�cessaires de la classe Graphe :
//		unsigned int size() const;
//		const Noeud* operator[](unsigned int index) const;

// Fonctions n�cessaires de la classe Noeud :
//		unsigned int getIndex() const;
//		const std::vector<Lien*>& getLinks() const;
//			Note :	std::vector peut �tre remplac� par tout autre conteneur de List* pouvant �tre it�r�.

// Fonctions suppl�mentaires n�cessaires pour l'algorithme A* :
//		bool isFinal() const;
//		unsigned int getHeuristic() const;
//			Note :	Cette fonction doit renvoyer la distance minimale estim�e du noeud jusqu'� un noeud final.
//					Afin que l'algorithme fourni ici fonctionne, cette fonction doit v�rifier :
//						Pour tous noeuds x et y : h(x) <= h(y) + d(x, y).
//					La classe fournie par d�faut ici impl�mente une heuristique nulle :
//					son utilisation revient � effectuer l'algorithme de Dijkstra sur le graphe fourni.

// Fonctions n�cessaires de la classe Lien :
//		unsigned int getCost() const;
//		unsigned int getTargetIndex() const;



// Impl�mentation minimale de ces classes
class Graph;
class Link
{
protected:
	unsigned int targetIndex;
	unsigned int cost;

public:
	unsigned int getCost() const		{ return cost; }
	unsigned int getTargetIndex() const	{ return targetIndex; }

	Link(unsigned int _targetIndex, unsigned int _cost)
		: targetIndex(_targetIndex), cost(_cost) { }
};
class Node
{
	friend Graph;

protected:
	unsigned int index;
	std::vector<Link> links;

	bool finalNode;
	unsigned int heuristic;

public:
	unsigned int getIndex() const			{ return index; }
	const std::vector<Link>& getLinks() const	{ return links; }

	bool isFinal() const					{ return finalNode; }
	unsigned int getHeuristic() const		{ return heuristic; }

	Node(unsigned int _index)
		: index(_index), finalNode(false), heuristic((unsigned int)(-1))	{ }
};
class Graph : public std::vector<Node>
{
public:
	Graph(unsigned int nodesNb) : std::vector<Node>()
	{
		reserve(nodesNb);
		for (unsigned int i = 0; i < nodesNb; i++)
			push_back(Node(i));
	}

	void addLink(unsigned int start, unsigned int end, unsigned int cost, bool oneDirection = false)
	{
		(*this)[start].links.push_back(Link(end, cost));
		if (!oneDirection)
			(*this)[end].links.push_back(Link(start, cost));
	}

	void setNodeFinal(unsigned int node, bool isFinal = true)
	{
		(*this)[node].finalNode = isFinal;
	}
	void setNodeHeuristic(unsigned int node, unsigned int heuristic)
	{
		(*this)[node].heuristic = heuristic;
	}
};

#endif
