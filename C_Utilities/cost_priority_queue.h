#ifndef DEF_COST_PRIORITY_QUEUE
#define DEF_COST_PRIORITY_QUEUE

#include <utility>
#include <vector>
#include <queue>

// File de priorité où à chaque élément est associé un coût :
// Les éléments les plus prioritaires sont ceux avec le coût le plus faible.
template<class EltType, class Cost = int>
class cost_priority_queue
{
protected:
	typedef std::pair<EltType, Cost> elt_cost_pair;
	class cost_priority_queue_comp
	{
	public:
		// left est moins prioritaire que right <=> left a un coût supérieur à right
		bool operator()(const elt_cost_pair& left, const elt_cost_pair& right) const
		{
			return (left.second > right.second);
		}
	};

	cost_priority_queue_comp cmp;
	std::priority_queue<elt_cost_pair, std::vector<elt_cost_pair>, cost_priority_queue_comp> pq;

public:
	cost_priority_queue() : cmp(), pq(cmp) { }

	bool empty() const
	{
		return pq.empty();
	}
	void push(const EltType& elt, const Cost& cost)
	{
		pq.push(elt_cost_pair(elt, cost));
	}
	const EltType& top() const
	{
		return pq.top().first;
	}
	const Cost& top_cost() const
	{
		return pq.top().second;
	}
	void pop()
	{
		pq.pop();
	}
};

#endif
