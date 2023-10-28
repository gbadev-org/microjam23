#ifndef AWJ_UTILS_H
#define AWJ_UTILS_H

#include "bn_fixed.h"
#include "bn_utility.h"

#include "awj_luts.h"

namespace awj::util
{

template <typename T>
constexpr void copy(const T src[], T dst[], const int N)
{
	for (int n = 0; n < N; n++)
		dst[n] = src[n];
}

template <typename T, int N>
constexpr void copy(const T (&src)[N], T (&dst)[N]) { copy(src, dst, N); }

// shuffle from permutation id - can guarentee a nontrivial shuffle if id is not 0 (or multiple of n!)
template <typename T>
constexpr void shuffle(T data[], const int n, int const id)
{
	auto permute = [&data, &n](const uint8_t* permutation)
	{
		for (int i = 0; i < n; i++)
			if (const int j = permutation[i]; i < j)
				bn::swap(data[i], data[j]);
	};

	switch (n)
	{
	case 0:
	case 1:
		break;
	case 2:
		if (id & 1)
			bn::swap(data[0], data[1]);
		break;
	case 3:
		permute(&awj::luts::permutations3[id][0]);
		break;
	case 4:
		permute(&awj::luts::permutations4[id][0]);
		break;
	case 5:
		permute(&awj::luts::permutations5[id][0]);
		break;
	default:
		break;
	}
}

template <typename T, int N>
constexpr void shuffle(T (&indices)[], int id) { shuffle(indices, N, id); }

constexpr bn::fixed lerp(const bn::fixed alpha, const bn::fixed a, const bn::fixed b)
{
	return (1 -  alpha) * a + alpha * b;
}

// parabola up to y=1 for x=0..1
constexpr bn::fixed parabola(const bn::fixed x) 
{
	return 4 * x * (1 - x);
}

constexpr bn::fixed ease_out_cubic(const bn::fixed x) 
{
	//return x * (2 - x);
	const auto z = (1 - x);
	return 1 - z * z * z;
}

constexpr bn::fixed ease_out_quad(const bn::fixed x) 
{
	//return x * (2 - x);
	const auto z = (1 - x);
	return 1 - z * z;
}

constexpr bn::fixed ease_in_out(const bn::fixed x)
{
	return x * x * (3 - 2 * x);
}

}

#endif
