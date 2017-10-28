#include <iostream>
#include <vector>
#include <fstream>
#include <cassert>

using namespace std;

const unsigned int M = 12;
const unsigned int N = 21;

// Indicates the status of the map. -1 is ocean, 0 empty, 1 residential and 2 commercial
char map[M][N];
vector<int> lockedIndices;

void setTile(int r, int c, char val){
  map[M-1-r][N-1-c] = val;
}

void lockTile(int r, int c, char val){
  setTile(r,c,val);
  int lockidx = (M-1-r)+(N-1-c)*M;
  lockedIndices.push_back(lockidx);
//  cout << "Locking " << r <<", "<<c<<" .. "<<lockidx<<endl;
}

void genMap(){
  int r, c;
  for(r=0;r<M;r++)
    for(c=0;c<N;c++)
      setTile(r, c, '~');

  for(r=0;r<5;r++)
    for(c=0;c<19;c++)
      setTile(r, c, ' ');

  setTile(0, 19, ' ');
  setTile(1, 19, ' ');
  setTile(0, 20, ' ');

  for(r=5;r<8;r++)
    for(c=5;c<17;c++)
      setTile(r, c, ' ');

  setTile(5, 17, ' ');
  setTile(6, 17, ' ');

  for(c=6;c<17;c++)
    setTile(8, c, ' ');

  setTile(9, 13, ' ');
  setTile(9, 14, ' ');
  setTile(9, 15, ' ');
  setTile(10, 14, ' ');

  setTile(9, 6, ' ');
  setTile(9, 7, ' ');
  setTile(9, 8, ' ');
  setTile(10, 6, ' ');
  setTile(10, 7, ' ');
  setTile(11, 7, ' ');

  setTile(5, 4, ' ');
  setTile(5, 0, ' ');
  setTile(5, 1, ' ');
  setTile(6, 0, ' ');
}


void lockSol1(){
  lockTile(3, 0, 'R');
  lockTile(4, 0, 'C');
  lockTile(5, 0, 'R');
  lockTile(6, 0, 'R');
  lockTile(4, 1, 'R');
  lockTile(5, 1, 'R');

  lockTile(9, 6, 'R');
  lockTile(9, 7, 'C');
  lockTile(9, 8, 'R');
  lockTile(10, 6, 'R');
  lockTile(10, 7, 'R');
  lockTile(11, 7, 'R');

  lockTile(0, 20, 'R');
  lockTile(0, 19, 'R');
  lockTile(1, 19, 'R');
  lockTile(0, 18, 'C');
  lockTile(1, 18, 'R');

  lockTile(10, 14, 'R');
  lockTile(9, 13, 'R');
  lockTile(9, 14, 'R');
  lockTile(9, 15, 'R');
  lockTile(8, 13, 'R');
  lockTile(8, 14, 'C');
  lockTile(8, 15, 'R');
  lockTile(8, 16, 'R');
}

void lockSol2(){
  lockTile(3, 1, 'R');
  lockTile(3, 0, 'R');
  lockTile(4, 0, 'C');
  lockTile(5, 0, 'R');
  lockTile(6, 0, 'R');
  lockTile(4, 1, 'R');
  lockTile(5, 1, 'R');
  lockTile(4, 2, 'R');
  lockTile(4, 3, 'R');
  lockTile(5, 4, 'R');
  lockTile(6, 5, 'R');
  lockTile(7, 5, 'R');
  lockTile(8, 6, 'R');

  lockTile(9, 6, 'R');
  lockTile(9, 7, 'C');
  lockTile(9, 8, 'R');
  lockTile(10, 6, 'R');
  lockTile(10, 7, 'R');
  lockTile(11, 7, 'R');

  lockTile(0, 20, 'R');
  lockTile(0, 19, 'R');
  lockTile(1, 19, 'R');
  lockTile(0, 18, 'C');
  lockTile(1, 18, 'R');

  lockTile(10, 14, 'R');
  lockTile(9, 13, 'R');
  lockTile(9, 14, 'R');
  lockTile(9, 15, 'R');
  lockTile(8, 13, 'R');
  lockTile(8, 14, 'C');
  lockTile(8, 15, 'R');
  lockTile(8, 16, 'R');
}

void printMap(ostream& os=cout){
  for(int r=0;r<M;r++){
    for(int c=0;c<N;c++){
      os << map[r][c];
    }
    os << endl;
  }
}

bool nearCommercialOrEmpty(int r, int c){
  //Non-residential tiles or those outside the map are considered near commercial.
  if(r<0 || c<0 || r>=M || c>=N || map[r][c] != 'R') return true;
  //     ?
  //    ???
  //   ??*??
  //    ???
  //     ?
  //Check the 4 tiles that are just one hop away
  if (r>0 && (map[r-1][c  ] == 'C' || map[r-1][c  ] == ' ')) return true;
  if (r<M && (map[r+1][c  ] == 'C' || map[r+1][c  ] == ' ')) return true;
  if (c>0 && (map[r  ][c-1] == 'C' || map[r  ][c-1] == ' ')) return true;
  if (c<N && (map[r  ][c+1] == 'C' || map[r  ][c+1] == ' ')) return true;

  //Check the 4 tiles that are two direct hops away
  if (r>1     && map[r-1][c  ] != '~' && (map[r-2][c  ] == 'C' || map[r-2][c  ] == ' ')) return true;
  if (r<(M-1) && map[r+1][c  ] != '~' && (map[r+2][c  ] == 'C' || map[r+2][c  ] == ' ')) return true;
  if (c>1     && map[r  ][c-1] != '~' && (map[r  ][c-2] == 'C' || map[r  ][c-2] == ' ')) return true;
  if (c<(N-1) && map[r  ][c+1] != '~' && (map[r  ][c+2] == 'C' || map[r  ][c+2] == ' ')) return true;

  //Check the 4 tiles that are one diagonal away
  if (r>0 && c>0 && (map[r-1][c] != '~' || map[r][c-1] != '~') && (map[r-1][c-1] == 'C' || map[r-1][c-1] == ' ')) return true;
  if (r<M && c>0 && (map[r+1][c] != '~' || map[r][c-1] != '~') && (map[r+1][c-1] == 'C' || map[r+1][c-1] == ' ')) return true;
  if (r>0 && c<N && (map[r-1][c] != '~' || map[r][c+1] != '~') && (map[r-1][c+1] == 'C' || map[r-1][c+1] == ' ')) return true;
  if (r<M && c<N && (map[r+1][c] != '~' || map[r][c+1] != '~') && (map[r+1][c+1] == 'C' || map[r+1][c+1] == ' ')) return true;

  return false;
}

/**
 * Evaluates if the indicated tile can be set to residential.
 *
 * Assumes that all non-oceanic tiles dominated by (r,c) have been set and that all non-dominated ones are unassigned.
 * @param r
 * @param c
 * @return true iff making r,c residential wont cause violations with previously assigned tiles.
 */
bool isValidResidential(int r, int c){
  //Let the * be the current tile under consideration. The - tiles are unexplored and the ? ones need consideration.
  //   ?-
  //  ??-
  // ??*-
  //  ?--
  //   --
  if(map[r][c]=='~') return false;
  map[r][c] = 'R';

  if (!nearCommercialOrEmpty(r  , c-2)) return false;
  if (!nearCommercialOrEmpty(r  , c-1)) return false;
  if (!nearCommercialOrEmpty(r-2, c  )) return false;
  if (!nearCommercialOrEmpty(r-1, c-1)) return false;
  if (!nearCommercialOrEmpty(r-1, c  )) return false;
  if (!nearCommercialOrEmpty(r+1, c-1)) return false;
  if (!nearCommercialOrEmpty(r  , c  )) return false;

  return true;
}

bool isValidCommercial(int r, int c){
  return map[r][c] != '~';
}



int count(int tile_idx, char val){
  int sum = 0;
  for (int t=tile_idx;t<M*N;t++){
    int r = t % M;
    int c = t / M;
    sum += map[r][c]==val?1:0;
  }

  return sum;
}

vector<int> emptyLookup;

void generateEmptyLookup(){
  for(int tile_idx=0;tile_idx<M*N;tile_idx++){
    emptyLookup.push_back(count(tile_idx, ' '));
  }
}

unsigned long int counter = 0;

void depthFirstSearch(int tile_idx, int cur_score, int& best_score){
//  if(++counter % 10000000 == 0){
//    cout << "depthFirstSearch(" << tile_idx << ", " << cur_score << ", " << best_score << ")" << endl;
//    cout << "> iteration "<<counter<<endl;
//    printMap();
//
//  }

//  cout << "depthFirstSearch(" << tile_idx << ", " << cur_score << ", " << best_score << ")" << endl;
//  printMap();
  if(tile_idx == M*N) {
    if(cur_score > best_score){
      cout << "Found new best solution with score " << cur_score << endl;
      printMap();
      best_score = cur_score;
    }

//    cout << "Reached leaf" << endl;
    return;
  }

  int r = tile_idx % M;
  int c = tile_idx / M;

  if (cur_score + emptyLookup[tile_idx] < best_score) {
//    cout << "Pruning at tile " << tile_idx << " (" << r << ", " << c << ")" << endl;
//    cout << cur_score << " .. " << countEmpty(tile_idx+1) << " .. " << best_score << endl;
//    printMap();
    return;
  }

//  cout << "> r: " << r << ", c: " << c << endl;
  bool locked = find(lockedIndices.begin(), lockedIndices.end(), tile_idx) != lockedIndices.end();
//  cout<<"> "<<r<<", "<<c<<" ... "<<locked<<" ... "<<map[r][c]<<endl;
//  if(c==1) exit(1);

  if(!locked && isValidResidential(r,c)) {
    map[r][c] = 'R';
    depthFirstSearch(tile_idx + 1, cur_score + 1, best_score);
    map[r][c] = ' ';
  }
  if(!locked && isValidCommercial(r,c)){
    map[r][c] = 'C';
    depthFirstSearch(tile_idx + 1, cur_score, best_score);
    map[r][c] = ' ';
  }
  else {
    depthFirstSearch(tile_idx + 1, cur_score, best_score);
  }
}

int main() {
  genMap();
  lockSol2();
  int cur_score = count(0, 'R');
  generateEmptyLookup();
  int best_score = 140;
  printMap();
  depthFirstSearch(0, cur_score, best_score);
  return 0;
}