#include <vector>
#include <iostream>
#include <tuple>

#define AVAILABLE 1

/********************************************************************************************************/

namespace path {

	using x_t = int;
	using y_t = int;

	using topology_t = std::vector<std::vector<int>>;
	using point_t = std::pair<x_t, y_t>;
	using search_path_t = std::vector<point_t>;
	using short_path_t = std::vector<std::vector<int>>;
};

/********************************************************************************************************/

class pathFinder {

private:

	path::search_path_t m_searchPath{};
	path::short_path_t m_shortPath{};
	path::topology_t m_poligonTopology{};

	void resizeResultVector();

	bool isSafe(const path::point_t& currentPoint) const;

	bool solvePath(const path::point_t& currentPoint);

	void printOutResults() const;

public:

	pathFinder() = default;

	void solveTopology(const path::topology_t& poligonTopology, const path::point_t& startPoint);
};