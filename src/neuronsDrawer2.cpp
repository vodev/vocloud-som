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

/*
    fprintf(gnuplot, "set terminal pngcairo  transparent enhanced font \"arial,10\" size 1700, 700\n");
    fprintf(gnuplot, "set output '%s'\n", NAME.c_str() );
    fprintf(gnuplot, "set terminal png x333333 xffffff\n");
    fprintf(gnuplot, "set nokey\n");
    fprintf(gnuplot, "set bmargin  7\n");
    fprintf(gnuplot, "set xrange [ -1 : %d ] noreverse nowriteback\n",SOM_SIZE + 1);
    fprintf(gnuplot, "set yrange [ -1 : %d ] noreverse nowriteback\n",SOM_SIZE + 1);
    fprintf(gnuplot, "plot \".umatrix.som\" using 1:2:3 with points palette\n");*/

    int multi = 0;

   fprintf(gnuplot, "set terminal pngcairo  transparent enhanced font \"arial,10\" size 1700, 700\n");
    fprintf(gnuplot, "set output '%s'\n", NAME.c_str() );
    //fprintf(gnuplot, "set terminal png x333333 xffffff\n");
    fprintf(gnuplot, "set terminal png xffffff x000000\n");
    //fprintf(gnuplot, "set terminal png size 320,320\n");
   // fprintf(gnuplot, "set size 1, 1\n");
    fprintf(gnuplot, "set nokey\n");
    fprintf(gnuplot, "set view map\n");
    //fprintf(gnuplot, "set xtics border in scale 0,0 mirror norotate  offset character 0, 0, 0\n");
    //fprintf(gnuplot, "set ytics border in scale 0,0 mirror norotate  offset character 0, 0, 0\n");
    //fprintf(gnuplot, "set ztics border in scale 0,0 nomirror norotate  offset character 0, 0, 0\n");
    //fprintf(gnuplot, "set nocbtics\n");
    if (multi)fprintf(gnuplot, "set multiplot\n");
    fprintf(gnuplot, "set xrange [ -1 : %d ] noreverse nowriteback\n",SOM_SIZE_X);
    fprintf(gnuplot, "set yrange [ -1 : %d ] noreverse nowriteback\n",SOM_SIZE_Y);
    //duha//fprintf(gnuplot, "set palette model HSV\n");
    //fprintf(gnuplot, "set palette rgb 3, 2, 2\n");
    fprintf(gnuplot, "set palette rgb -30,-31,-32\n");
    fprintf(gnuplot, "set bmargin  7\n");
    fprintf(gnuplot, "splot \".temp/umatrix.som\" using 1:2:3 with image\n");

    if (multi)fprintf(gnuplot, "splot \".temp/a\" using 1:2:3 with points palette\n");
    if (multi)fprintf(gnuplot, "unset multiplot\n");
   /*
set view map
set xtics border in scale 0,0 mirror norotate  offset character 0, 0, 0 autojustify
set ytics border in scale 0,0 mirror norotate  offset character 0, 0, 0 autojustify
set ztics border in scale 0,0 nomirror norotate  offset character 0, 0, 0 autojustify
set nocbtics
set rtics axis in scale 0,0 nomirror norotate  offset character 0, 0, 0 autojustify
set title "Heat Map generated from a file containing Z values only"
set xrange [ -0.500000 : 4.50000 ] noreverse nowriteback
set yrange [ -0.500000 : 4.50000 ] noreverse nowriteback
set cblabel "Score"
set cbrange [ 0.00000 : 5.00000 ] noreverse nowriteback
set palette rgbformulae -7, 2, -7
splot '-' matrix with image*/


    REP(i,SOM_SIZE_X) delete[] arr[i];
    delete[] arr;

    fclose(gnuplot);



  return 0;
}
