#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_set>
#include <functional>

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

    bool operator==(const pos & other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const pos & other) const {
        return !(*this==other);
    }

  void print() const {
    std::cout << "(" << x << "," << y << ")";
  }
};


// struct posHasher {
//   size_t operator()(const pos & p) const {
//     return std::hash<size_t>()(p.x)^std::hash<size_t>()(p.y);
//   }
// };

namespace std {
  template<>
  struct hash<pos> {
    size_t operator()(const pos & p) const {
      return std::hash<size_t>()(p.x)^std::hash<size_t>()(p.y);
    }
  };
}

class solution {
  public:
    int trapRainWater(const vector<vector<int>> & heightMap) {
      int maxht = maxInMat(heightMap);
      vector<vector<vector<int>>> cross;
      for (int i = 1; i <= maxht; ++i)
	cross.push_back(crossCut(heightMap, i));

      int water = 0;
      for (size_t i = 0; i < cross.size(); ++i) {
	// std::cout << i << "th layer:\n";
	water += enclosedWater(cross[i]);
      }

      return water;
    }

private:
  int enclosedWater(const vector<vector<int>> & grid) {
    std::unordered_set<pos> cands;
    for (size_t i = 1; i < grid.size()-1; ++i)
      for (size_t j = 1; j < grid[i].size()-1; ++j)
	if (!grid[i][j])
	  cands.insert(pos(i,j));
    // std::cout << "size of cands = " << cands.size() << std::endl;

    vector<std::unordered_set<pos>> grps;
    while (!cands.empty()) {
      auto it = cands.begin();
      std::unordered_set<pos> curr{*it};
      cands.erase(it);
      auto temp = addNeibs(grid, cands, curr);
      // std::for_each(temp.begin(), temp.end(), [](const pos & p){p.print();});
      // std::cout << std::endl;
      grps.push_back(temp);
      for (auto p : temp)
	cands.erase(p);
      // std::cout << "size of cands = " << cands.size() << std::endl;
    }
    // std::cout << "number of grps = " << grps.size() << std::endl;

    int water = 0;
    for (auto & grp : grps) {
      if (isPond(grid, grp))
	water += grp.size();
    }
    
    return water;
  }

  bool isPond(const vector<vector<int>> & grid, const std::unordered_set<pos> & grps) {
    for (auto it = grps.begin(); it != grps.end(); ++it) {
      pos up(it->x-1, it->y);
      if (grps.find(up) == grps.end() && grid[up.x][up.y] == 0)
	return false;
      pos down(it->x+1, it->y);
      if (grps.find(down) == grps.end() && grid[down.x][down.y] == 0)
	return false;
      pos left(it->x, it->y-1);
      if (grps.find(left) == grps.end() && grid[left.x][left.y] == 0)
	return false;
      pos right(it->x, it->y+1);
      if (grps.find(right) == grps.end() && grid[right.x][right.y] == 0)
	return false;
    }
    return true;
  }

  std::unordered_set<pos> addNeibs(const vector<vector<int>> & grid, std::unordered_set<pos> & cands, const std::unordered_set<pos> & curr) {
    std::unordered_set<pos> result;
    vector<pos> next = getNext(grid, cands, curr);
    if (next.empty())
      result = curr;
    else {
      auto temp = curr;
      temp.insert(next.begin(), next.end());
      auto cpy = cands;
      for (auto p : next)
	cpy.erase(p);
      result = addNeibs(grid, cpy, temp);
    }
    return result;
  }

  vector<pos> getNext(const vector<vector<int>> & grid, const std::unordered_set<pos> & cands, const std::unordered_set<pos> & curr) {
    vector<pos> next;
    for (auto it = curr.begin(); it != curr.end(); ++it) {
      pos left(it->x-1, it->y);
      if (cands.find(left) != cands.end() && curr.find(left) == curr.end() && grid[left.x][left.y] == 0)
	next.push_back(left);
      pos right(it->x+1, it->y);
      if (cands.find(right) != cands.end() && curr.find(right) == curr.end()  && grid[right.x][right.y] == 0)
	next.push_back(right);
      pos up(it->x, it->y-1);
      if (cands.find(up) != cands.end() && curr.find(up) == curr.end() && grid[up.x][up.y] == 0)
	next.push_back(up);
      pos down(it->x, it->y+1);
      if (cands.find(down) != cands.end() && curr.find(down) == curr.end() && grid[down.x][down.y] == 0)
	next.push_back(down);
    }
    
    return next;
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
