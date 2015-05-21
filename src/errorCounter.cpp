#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

///restricted to 20 classes!!! ///

using namespace std;
typedef pair<int,int> ii;
typedef vector<int> vi;
typedef vector<ii> vii;
#define MP make_pair
#define PB push_back
#define TR(it,c) for( typeof(c.begin()) it = c.begin(); it != c.end(); ++it )
#define TRR(it,c) for( typeof(c.rbegin()) it = c.rbegin(); it != c.rend(); ++it
#define REP(i,a) for (int i = 0; i < (a); i++)
#define FOR(i,a,b) for (int i = (a); i <= (b); i++)
#define FORD(i,a,b) for (int i = (a); i >= (b); i--)

#define DRI(a) int a; scanf("%d", &a);
#define DRII(a, b) int a, b; scanf("%d %d", &a, &b);
#define RI(a) scanf("%d", &a);
#define RII(a, b) scanf("%d %d", &a, &b);

#define MM(arr, num) memset((arr), (num), sizeof((arr)))

inline bool EQ(double a, double b) { return fabs(a-b) < 1e-9; }

const int INF = 1<<29;
typedef long long ll;
typedef unsigned long long ull;
/*******************************************************/

int error(string s){
    cout << "----------------------------"<< endl;
    cout << "error: " << s << "." << endl;
    cout << "----------------------------"<< endl;
    exit(1);
}

int *** arr ;

int main(int argc, char *argv[]) {

  printf("program %s starts.\n", argv[0] );
  if (argc < 2) error("missing argument rows\n");
  //printf("file path: %s\n", argv[1]);
  int ROWS = atoi(argv[1]);
  if (argc < 3) error("missing argument map_sizex\n");
  //printf("file path: %s\n", argv[1]);
  int MAP_SIZE_X = atoi(argv[2]);

  if (argc < 4) error("missing argument map_sizey\n");
  //printf("file path: %s\n", argv[1]);
  int MAP_SIZE_Y = atoi(argv[3]);

  if (argc < 5) error("missing argument name (all files result)\n");
  //printf("file path: %s\n", argv[1]);
  string NAME = argv[4];

  if (argc < 6) error("missing argument is clas (all files result)\n");
  //printf("file path: %s\n", argv[1]);
  int is_clas = atoi(argv[5]);

  arr = new int ** [MAP_SIZE_X];
  REP(i,MAP_SIZE_X){ arr[i] = new int * [MAP_SIZE_Y]; REP(j,MAP_SIZE_Y) arr[i][j] = new int[20];}


  REP(i,MAP_SIZE_X)
  REP(j,MAP_SIZE_Y)
  REP(k,20)
  arr[i][j][k] = 0;

  FILE * f = fopen( ".temp/ClustersTrain.som" , "r");

  //FILE * rf = fopen( RESULT.c_str() , "w");
  FILE * allrf = fopen( "result/errors.txt" , "a");

  int x,y,z;
  double err;
  int mx,part, all = 0; int incorrect = 0;

  if (is_clas){

      REP(i,ROWS){
        fscanf(f,"%d%d%d", &z,&x,&y);
        if (z < 0) continue; // not classificated.
        if (z >= 20) error("More than 20 classes (0..19) is not supported. ;)");
        arr[x][y][z] ++;
      }

      REP(i,MAP_SIZE_X)
      REP(j,MAP_SIZE_Y){
          part = 0;
          mx = 0;
          REP(k,20){
            mx = max (mx , arr[i][j][k]);
            part += arr[i][j][k];
          }
          all += part;
          incorrect += part - mx;
      }
  }
  else{
     REP(i,ROWS){
        fscanf(f,"%d%d%d", &z,&x,&y);
      }

  }

  fscanf(f, "%lf", &err);


 // fprintf(rf, "all items :%d   incorrect_items: %d  correct_items: %d  correctness rate: %lf percent.",  all, incorrect, all- incorrect, (double)(all- incorrect)/(double)all   );
  fprintf(allrf, "%s ",NAME.c_str());
  if (is_clas)fprintf(allrf, "%lf --- ",(double)(incorrect)/(double)all);
  fprintf(allrf, "quant err: %lf\n", err   );
  cout << "quant error: " << err << endl;

  fclose (f);
  //fclose (rf);
  fclose (allrf);

  return 0;
}

