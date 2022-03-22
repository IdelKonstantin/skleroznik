#include "maze.h"

void pathFinder::resizeResultVector() {

	size_t vertMaxIndex = m_poligonTopology.size();
	size_t horizMaxIndex = m_poligonTopology[0].size();

	std::vector<int> dummy{};

	for(size_t j = 0; j < horizMaxIndex; j++) {
		dummy.push_back(0);
	}

	for(size_t i = 0; i < vertMaxIndex; i++) {
		m_shortPath.push_back(dummy);
	}
}

bool pathFinder::isSafe(const path::point_t& currentPoint) const {

	path::x_t x{};
	path::y_t y{};

	std::tie(x,y) = currentPoint;

	size_t vertMaxIndex = m_poligonTopology.size();
	size_t horizMaxIndex = m_poligonTopology[0].size();

	return (
		x >= 0 && x < horizMaxIndex && 
		y >= 0 && y < vertMaxIndex &&
		m_poligonTopology[x][y] == AVAILABLE
	);
}

bool pathFinder::solvePath(const path::point_t& currentPoint) {

	if(isSafe(currentPoint)) {

		path::x_t x;
		path::y_t y;

		std::tie(x, y) = currentPoint;

		if (m_shortPath[x][y] == 1) {
			return false;
		}

		m_shortPath[x][y] = 1;
		m_searchPath.push_back(std::make_pair(x,y));

		const std::vector<std::pair<int, int>> directions{
			{-1, 0}, 
			{0, -1},
			{0, 1},
			{1, 0}
		};

		for(const auto dir : directions) {

			path::x_t dx;
			path::y_t dy;

			std::tie(dx, dy) = dir;

			if (solvePath(std::make_pair(x + dx, y + dy))) {
				return true;
			}
		}
		return false;
	}
	return false;
}

void pathFinder::printOutResults() const {

	for(const auto& i : m_searchPath) {
		std::cout << i.first << "," << i.second << std::endl;
	}
}

void pathFinder::solveTopology(const path::topology_t& poligonTopology, const path::point_t& startPoint) {

	m_shortPath.clear();
	m_searchPath.clear();

	m_poligonTopology = poligonTopology;

	resizeResultVector();
	solvePath(startPoint);
	printOutResults();
}