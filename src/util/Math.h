#pragma once

#include <cassert>
#include <SFML/System/Vector2.hpp>

#include <cstdint>
#include <random>

struct Vec2i {
	Vec2i(int x, int y) 
		: 
		x( x ),
		y( y )
	{}

	bool operator==(const Vec2i& other) const
	{
		return x == other.x && y == other.y;
	}

	sf::Vector2i toSF() const
	{
		return sf::Vector2i{ x,y };
	}

	friend std::hash<Vec2i>;
	int32_t x;
	int32_t y;
};

namespace std {
	template <>
	struct hash<Vec2i> {
		std::size_t operator()(const Vec2i& v) const 
		{
			return v.x + (v.y >> 32);
		}
	};
}

class Math {
public:
	static void seed_rng(int seed)
	{
		mt.seed(seed);
	}

	// return uniformly random int between [min, max)
	static int rng(int min, int max)
	{
		std::uniform_int_distribution<int> dist(min, max-1);
		return dist(mt);
	}

	static bool in_bounds(int x, int y, int w, int h)
	{
		return (x >= 0 && x < w && y >= 0 && y < h);
	}

	static bool in_bounds(const Vec2i& p, const Vec2i& d)
	{
		return in_bounds(p.x, p.y, d.x, d.y);
	}

	static bool in_bounds(int index, int min, int max)
	{
		return min <= index && index < max;
	}

	static int to_index(const Vec2i& p, const Vec2i& d)
	{
		return to_index(p.x, p.y, d.x, d.y);
	}

	static int to_index(int x, int y, int w, int h)
	{
		assert(in_bounds(x, y, w, h));

		return (x % w) + (y * w);
	}

	static int to_x(int index, int w, int h) 
	{
		assert(index >= 0);
		assert(index < w*h);
		return index % w;
	}

	static int to_y(int index, int w, int h)
	{
		assert(index >= 0);
		assert(index < w*h);
		return index / w;
	}

private:
	inline static std::mt19937 mt;

};