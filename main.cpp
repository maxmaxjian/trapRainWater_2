#include <iostream>
#include <vector>

using std::vector;

class solution {
  public:
    int trapRainWater(const vector<vector<int>> & heightMap) {
        
    }
};

int main() {
    vector<vector<int>> grid{
        {1,4,3,1,3,2},
        {3,2,1,3,2,4},
        {2,3,3,2,3,1}
    };

    solution soln;
    int mostWater = soln.trapRainWater(grid);
    
}
