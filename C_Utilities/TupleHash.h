#ifndef DEF_TUPLE_HASH
#define DEF_TUPLE_HASH

#include <tuple>

// Code pris d'après :
// http://stackoverflow.com/questions/7110301/generic-hash-for-tuples-in-unordered-map-unordered-set/7115547#7115547

namespace std
{
	namespace
	{
		// Code from boost
		// Reciprocal of the golden ratio helps spread entropy and handles duplicates.
		// See Mike Seymour in magic-numbers-in-boosthash-combine:
		//     http://stackoverflow.com/questions/4948780
		template<class T>
		inline void hash_combine(std::size_t& seed, const T& v)
		{
			seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}

		// Recursive template code derived from Matthieu M.
		template<class Tuple, size_t Index = std::tuple_size<Tuple>::value - 1>
		struct HashValueImpl
		{
			static void apply(size_t& seed, const Tuple& tuple)
			{
				HashValueImpl<Tuple, Index - 1>::apply(seed, tuple);
				hash_combine(seed, std::get<Index>(tuple));
			}
		};

		template<class Tuple>
		struct HashValueImpl<Tuple, 0>
		{
			static void apply(size_t& seed, const Tuple& tuple)
			{
				hash_combine(seed, std::get<0>(tuple));
			}
		};
	}

	template<typename... TT>
	struct hash<std::tuple<TT...> >
	{
		size_t operator()(const std::tuple<TT...>& tt) const
		{
			size_t seed = 0;
			HashValueImpl<std::tuple<TT...> >::apply(seed, tt);
			return seed;
		}
	};

	template<class T1, class T2>
	struct hash<std::pair<T1, T2>>
	{
		size_t operator()(const std::pair<T1, T2>& tt) const
		{
			size_t seed = std::hash<T1>()(tt.first);
			hash_combine(seed, std::hash<T2>()(tt.second));
			return seed;
		}
	};
}

#endif
