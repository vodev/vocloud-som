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

//data 1565 * 1997;


float ** arr;
int * classes;
string * names;
int * permutation;



int error(string s){
    cout << "----------------------------"<< endl;
    cout << "error: " << s << "." << endl;
    cout << "----------------------------"<< endl;
    exit(1);
}

int main(int argc, char *argv[]) {

  string TRAIN_STRING, NAME_STRING, CLASS_STRING;
  int ROWS, COLS;



  printf("program %s starts.\n", argv[0] );
  if (argc < 2) error("missing argument file_path\n");
  //printf("file path: %s\n", argv[1]);
  TRAIN_STRING = argv[1];
  if (argc < 3) error("missing argument rows\n");
  //printf("rows: %s\n", argv[2]);
  ROWS = atoi(argv[2]);
  if (argc < 4) error("missing argument cols\n");
  //printf("cols: %s\n", argv[3]);
  COLS = atoi(argv[3]);

  if (argc < 5) error("missing argument cols\n");
  //printf("cols: %s\n", argv[3]);
  int flag = atoi(argv[4]);

  int temp = 5;
  if (flag & 1){
    NAME_STRING = argv[temp++];
  }
  if (flag & 2){
    CLASS_STRING = argv[temp];
  }

  arr = new  float*[ROWS];
  REP(i,ROWS) arr[i] = new float[COLS];
  classes = new int[ROWS];
  permutation = new int[ROWS];
  names = new string[ROWS];


  FILE * finclas;FILE * finname;FILE * foutclas;FILE * foutque;FILE * foutname;
  FILE * findata = fopen( TRAIN_STRING.c_str() , "r");
   if (findata == NULL)error("bad input file findata");

  if (flag & 2){
    finclas = fopen(CLASS_STRING.c_str(), "r");
    if (finclas == NULL)error("bad input file finclas");
  }
  if (flag & 1){
    finname = fopen(NAME_STRING.c_str(), "r");
    if (finname == NULL)error("bad input file finname");
  }
  string OUT_STRING = ".temp/data.shuffle";
  FILE * foutdata = fopen( OUT_STRING.c_str(), "w");
  if (foutdata == NULL)error("bad input file foutdata");

  string s;
  if (flag & 2){
     s = ".temp/classes.shuffle";
     foutclas = fopen( s.c_str(), "w");
  }
  if (flag & 1){
    s = ".temp/names.shuffle";
    foutname = fopen( s.c_str(), "w");
  }

  foutque = fopen(".temp/que.shuffle", "w");

  //FILE * foutque = fopen("data/fv_dwt_cdf97_10_mean.mat", "r");

  srand(time(NULL));



  /*txt */REP(i,ROWS)REP(j,COLS) fscanf(findata,"%f", &arr[i][j]);
  /*csv *//*REP(i,ROWS)REP(j,COLS) if (j == COLS -1 ) fscanf(findata,"%f", &arr[i][j]);
                                 else fscanf(findata,"%f,", &arr[i][j]);
*/


  if (flag & 2) REP(i,ROWS) fscanf(finclas,"%d", &classes[i]);
  if (flag & 1){
    char buff[150];
    REP(i,ROWS){ fscanf(finname,"%s", buff); names[i] += buff;    }
  }
  REP(i,ROWS) permutation[i] = i;




  random_shuffle ( permutation, permutation + ROWS );



  REP(i,ROWS){

    REP(j,COLS)fprintf(foutdata, "%.5lf ", arr[ permutation[i] ][j]);
    fprintf(foutdata, "\n");

    if (flag & 2) fprintf(foutclas, "%d\n", classes[ permutation[i] ]);

    if (flag & 1) fprintf(foutclas, "%s\n", names[ permutation[i] ].c_str() );
    fprintf(foutque, "%d\n", permutation[i]);

  }

  REP(i,ROWS) delete[] arr[i];
  delete[] arr;
  delete[] classes;
  delete[] names;
  delete[] permutation;

  return 0;
}

