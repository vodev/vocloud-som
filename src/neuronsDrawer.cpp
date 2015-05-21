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
int ** arr;

int error(string s){
    cout << "----------------------------"<< endl;
    cout << "error: " << s << "." << endl;
    cout << "----------------------------"<< endl;
    exit(1);
}

int main(int argc, char *argv[]) {

   printf("program %s starts.\n", argv[0] );
    if (argc < 2) error("missing argument name (all files result)\n");
    //printf("file path: %s\n", argv[1]);
    string NAME = argv[1];
    if (argc < 3) error("missing argument name (all files result)\n");
    //printf("file path: %s\n", argv[1]);
    int SOM_SIZE_X = atoi(argv[2]);
    if (argc < 4) error("missing argument name (all files result)\n");
    //printf("file path: %s\n", argv[1]);
    int SOM_SIZE_Y = atoi(argv[3]);

    if (argc < 5) error("missing argument name (all files result)\n");
    //printf("file path: %s\n", argv[1]);
    int ROWS = atoi(argv[4]);


    arr = new int * [SOM_SIZE_X];
    REP(i,SOM_SIZE_X) arr[i] = new int [SOM_SIZE_Y];

    FILE * gnuplot = fopen( ".temp/neurons.gnuplot", "w");
    if (gnuplot == NULL) error("bad gnuplot file");
    FILE * draw_by_gnuplot = fopen( ".temp/a", "w");
    if (draw_by_gnuplot == NULL) error("bad draw_by_gnuplot file");
    FILE * data = fopen( ".temp/ClustersTrain.som", "r");
    if (data == NULL) error("bad data file");


    fprintf(gnuplot, "set terminal pngcairo  transparent enhanced font \"arial,10\" size 1700, 700\n");
    fprintf(gnuplot, "set output '%s'\n", NAME.c_str() );
    //fprintf(gnuplot, "set terminal png x333333 xffffff\n");
    fprintf(gnuplot, "set terminal png xffffff x000000\n");
    fprintf(gnuplot, "set nokey\n");
//fprintf(gnuplot, "set palette model HSV\n");
fprintf(gnuplot, "set palette rgb 7,5,15\n");
    fprintf(gnuplot, "set xrange [ -1 : %d ] noreverse nowriteback\n",SOM_SIZE_X );
    fprintf(gnuplot, "set yrange [ -1 : %d ] noreverse nowriteback\n",SOM_SIZE_Y );
    fprintf(gnuplot, "set bmargin  7\n");
    fprintf(gnuplot, "plot \".temp/a\" u 1:2:3 with points palette\n");

    REP(i,SOM_SIZE_X)REP(j,SOM_SIZE_Y) arr[i][j] = 0;
    int c,x,y, div, mod;

    int num = ( min(SOM_SIZE_X,SOM_SIZE_Y) <= 7) ? 9 : 5;


    REP(i,ROWS){
        fscanf(data, "%d%d%d", &c ,&x, &y);
        div = arr[x][y] / num;
        mod = arr[x][y] % num;
        fprintf(draw_by_gnuplot, "%lf %lf %d\n", (double)x +  (double)( mod ) * 0.1, (double)y  +  (double)( div ) * 0.1, c );
        arr[x][y]++;
    }

    REP(i,SOM_SIZE_X) delete[] arr[i];
    delete[] arr;

    fclose(gnuplot);
    fclose(draw_by_gnuplot);
    fclose(data);

  return 0;
}
