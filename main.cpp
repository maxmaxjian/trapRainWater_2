#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using std::vector;

class lessthan {
private:
  int height;
public:
  lessthan(int h) : height(h){}
  bool operator()(int val) {
    return val < height;
  }
};

class largerthan {
private:
  int height;
public:
  largerthan(int h) : height(h){}
  bool operator()(int val) {
    return val > height;
  }
};

struct pos {
    size_t x, y;
    pos(size_t xval, size_t yval) : x(xval), y(yval){}

    bool operator==(const pos & other) {
        return x == other.x && y == other.y;
    }

    bool operator!=(const pos & other) {
        return !(*this==other);
    }
};

template<>
struct hash<pos> {
    size_t operator()(const pos & p) {
        return std::hash<size_t>()(p.x)^std::hash<size_t>()(p.y);
    }
};

class solution {
  public:
    int trapRainWater(const vector<vector<int>> & heightMap) {
      int maxht = maxInMat(heightMap);
      vector<vector<vector<int>>> cross;
      for (int i = 2; i <= maxht; ++i)
	cross.push_back(crossCut(heightMap, i));

      int water = 0;
      for (size_t i = 0; i < cross.size(); ++i)
	water += enclosedWater(cross[i]);

      return water;
    }

private:
  int enclosedWater(const vector<vector<int>> & grid) {
      std::unordered_set<pos> cands;
      for (size_t i = 1; i < grid.size()-1; ++i) {
          for (size_t j = 1; j < grid[i].size()-1; ++j)
              if (!grid[i][j])
                  cands.insert(pos(i,j));
      }
  }
  
  int maxInMat(const vector<vector<int>> & grid) {
    vector<int> rowmax;
    for (auto & vec : grid) {
      rowmax.push_back(*std::max_element(vec.begin(), vec.end()));
    }
    return *std::max_element(rowmax.begin(), rowmax.end());
  }

  vector<vector<int>> crossCut(const vector<vector<int>> & map, int hgt) {
    vector<vector<int>> result;
    for (auto vec : map) {
      std::replace_if(vec.begin(), vec.end(), lessthan(hgt), 0);
      std::replace_if(vec.begin(), vec.end(), largerthan(0), 1);
      result.push_back(vec);
      std::for_each(vec.begin(), vec.end(), [](int i){std::cout<<i<<" ";});
      std::cout << std::endl;
    }
    std::cout << std::endl;
    return result;
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
    std::cout << "The volume of rain water can be held is:\n"
	      << mostWater << std::endl;
}
