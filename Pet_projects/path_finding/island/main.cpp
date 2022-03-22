#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// function to perform BFS in four directions(up-right-down-left)

int bfs(vector<vector<int>>& grid, int row, int col, vector<pair<int, int>>& path) 
{
    int m = grid.size();
    int n = grid[0].size();
    
    /* since the current cell has value = 1
            minimum area we start with is 1*/
    int area = 1;
    
    /* create a queue to be used in BFS*/
    queue<pair<int,int>> q;
    /* push the current cell */
    q.push({row, col});
    
    /* since you have already visited the current cell
        mark it as already visited */
    grid[row][col] = 0;
    /* used for traversing in four directions(up-right-down-left)*/

    //vector<int> dir({1,0,-1,0,1}); //поиграть с этой штуковиной для лучшего построения пути
    //vector<int> dir({-1,1,0,1,-1});
    vector<int> dir({1,1,1,1,1});
    //vector<int> dir({1,0,0,0,1}); 
    //vector<int> dir({-1,0,1,0,-1}); 
    
    /* begin BFS traversal */
    while (!q.empty()) 
    {
        /* pop a cell with containing 1(land) */
        int y = q.front().first, x = q.front().second;
        q.pop();
        
        /* we begin our traversal into four directions from the current cell*/
        for (int i = 0; i < 4; i++) 
        {
            int r = y + dir[i], 
            c = x + dir[i+1];
            /* make traversals only when next cell lies within the matrix and is unvisited*/
            if (r >= 0 && r < m && c >= 0 && c < n && grid[r][c] == 1) 
            {
                /* mark next cell as visited, increase the visited area and push next cells into the queue*/
                grid[r][c] = 0;
                path.push_back(make_pair(r,c));
                area++;
                q.push({r,c});
            }
        }
    }
    /* total number of cells with value 1 visited from the current cell */
    return area;
}
/* function that returns area of largest island */
int largestIsland(vector<vector<int>> grid, vector<pair<int, int>>& path)
{
    int m = grid.size();
    int n = grid[0].size();
    
    /* stores the area of largest consecutive 1's*/
    int maxArea = 0;
    
    /* visit each cell of the matrix*/
    for (int i = 0; i < m; i++) {

        for (int j = 0; j < n; j++) {

            if (grid[i][j] == 1) {
                
                path.push_back(make_pair(i,j));
                maxArea = max(maxArea, bfs(grid, i, j, path));
            }
        }      
    }
      
    return maxArea;    
}

/* main function to implement above function */
// https://www.tutorialcup.com/interview/graph/max-area-of-island.htm

int main()
{
    vector<pair<int, int>> path{};

    vector<vector<int>> grid = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1}, //0
        {1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1}, //1
        {1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1}, //2
        {1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1}, //3
        {1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1}, //4
        {1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1}, //5
        {1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1}, //6
        {1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1}, //7
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, //8
        {1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1}, //9
        {1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1}, //10
        {1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1}, //11
        {1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1}  //12
    }; //0  1  2  3  4  5  6  7  8  9  10 11 12 

    largestIsland(grid, path);

    for(const auto& i : path) {

        cout << i.first << "," << i.second << endl;
    }

    return 0;
}