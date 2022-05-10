#include <iostream>
#include <vector>
#include <stack>
#include <tuple>

using index_pair_t = std::pair<int, int>;

void resizeVisited(const std::vector<std::vector<int>>& topology, std::vector<std::vector<bool>>& visited) {

	auto xMax = topology[0].size();
	auto yMax = topology.size();

	std::vector<bool> dummy{};

	for(size_t x = 0; x < xMax; x++) {
        dummy.push_back(false);
    }

    for(size_t y = 0; y < yMax; y++) {
        visited.push_back(dummy);
    }

	//std::cout << "x-size: " << visited[0].size() << std::endl;
	//std::cout << "y-size: " << visited.size() << std::endl;
}

bool isSafe(const index_pair_t& currIndexes, const std::vector<std::vector<int>>& topology) {

	int x, y;
	std::tie(x, y) = currIndexes;

	decltype(x) vertMaxIndex = topology.size();
    decltype(y) horizMaxIndex = topology[0].size();

    return (
        x >= 0 && x < horizMaxIndex &&
        y >= 0 && y < vertMaxIndex &&
        topology[x][y] == 1
    );
}

bool isNotVisited(const index_pair_t& currIndexes, const std::vector<std::vector<bool>>& visited) {

	int x, y;
	std::tie(x, y) = currIndexes;

	return visited[x][y] == false;
}

int main() {
	
	std::vector<std::vector<int>> topology {
		// {0, 1, 1, 0, 0},
		// {1, 1, 1, 1, 1},
		// {1, 1, 0, 1, 1},
		// {1, 1, 0, 0, 1},
		// {1, 1, 0, 1, 1},
		{1, 1, 1, 1, 1},
		{1, 0, 0, 1, 1},
		{0, 0, 0, 0, 1},
		{1, 0, 0, 0, 1},
		{1, 1, 0, 0, 1},
	};

	std::vector<std::vector<bool>> visited;

	resizeVisited(topology, visited);

	std::stack<index_pair_t> workingStack{};

	auto startIndex = std::make_pair(0, 0); 
	
	workingStack.push(startIndex);

    const std::vector<index_pair_t> directions {

        {-1, 0},
        {0, -1},
        {1, 0},
        {0, 1},
    };

    std::vector<index_pair_t> result;

	while(!workingStack.empty()) {

		auto flag{false};

		for(const auto& dir : directions) {

			int x, y;
			std::tie(x, y) = startIndex;
			x += dir.first;
			y += dir.second;

			if(isSafe(std::make_pair(x, y), topology) && isNotVisited(std::make_pair(x, y), visited)) {

				workingStack.push(std::make_pair(x, y));
				startIndex = std::make_pair(x, y);
				visited[x][y] = true;
				result.push_back(std::make_pair(x, y));
				flag = true;
				break;
			}
		}
		
		if(!flag) {
			
			workingStack.pop();

			if(!workingStack.empty()) {
				startIndex = workingStack.top();
				result.push_back(startIndex);
			}			
		}
	}

	for(const auto& item : result) {

		std::cout << item.second << "," << item.first << std::endl;
	}
}