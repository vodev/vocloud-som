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

string to_string(int number)
{
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

class IO{ //rename to IO.
  public:
    IO(){full = false;};
    IO(string file, string mode){
        in = fopen( file.c_str(), mode.c_str());
        if (in == NULL)error("bad file "+ file);
        full = true;
    }
    void init(string file, string mode){
        in = fopen( file.c_str(), mode.c_str());
        if (in == NULL)error("bad file "+ file);
        full = true;
    }

    ~IO(){
        if (full) fclose(in);
    }
    bool full;
    FILE * in;
};

IO ** nFiles;
IO ** phpFiles;
int ROWS,COLS,SIZE_X,SIZE_Y,IS_NAME,IS_CLAS;
char * NAME;

int main(int argc, char *argv[]) {

    int it = 0;

    printf("program %s starts.\n", argv[it++] );

    ROWS = atoi(argv[it++]);

    COLS = atoi(argv[it++]);

    SIZE_X = atoi(argv[it++]);

    SIZE_Y = atoi(argv[it++]);

    IS_NAME = atoi(argv[it++]);

    NAME = argv[it++];

    /**FILE_DESCRIPTOR_ERROR**/
   /* nFiles = new IO * [SIZE_X];
    REP(i,SIZE_X) nFiles[i] = new IO [SIZE_Y];

    phpFiles = new IO * [SIZE_X];
    REP(i,SIZE_X) phpFiles[i] = new IO [SIZE_Y];*/

    /*REP(i,SIZE_X)REP(j,SIZE_Y){
      nFiles[i][j].init("result/neurons/" + to_string(i) + "_" + to_string(j) + "/spectra.txt", "a"  );
    }
    REP(i,SIZE_X)REP(j,SIZE_Y){
      phpFiles[i][j].init("result/neurons/" + to_string(i) + "_" + to_string(j) + "/spectra.php", "a"  );
    }*/

    IO data(".temp/data.shuffle","r");
    //if(IS_CLASS) IO classes(".temp/classes.shuffle"); //not needed classes are in ClustersTrain.com

    IO names;
    if(IS_NAME)names.init(".temp/names.shuffle","r");
    IO nn(".temp/ClustersTrain.som","r"); //nearest neuron // class nN nM
    IO som(".temp/code1.map","r");
    char name[200];
    fgets(name, 100, som.in); //som has one line padding (compatibility issue);




    int clas,x, y;
    float f;
    int ** que;
    que = new int * [SIZE_X];
    REP(i,SIZE_X) que[i] = new int [SIZE_Y];
    REP(i,SIZE_X) MM(que[i], 0);

    REP(i,SIZE_X)
    REP(j,SIZE_Y){
      IO nSpec("result/neurons/" + to_string(i) + "_" + to_string(j) + "/n_spectrum.txt", "w" );
      REP(k,COLS){
        fscanf(som.in,"%f",&f);
        if (k==0)fprintf(nSpec.in,"%f",f);
        else fprintf(nSpec.in," %f",f);
      }
      fprintf(nSpec.in,"\n");
    }



REP(i,SIZE_X)
    REP(j,SIZE_Y){
      IO phpFiles("result/neurons/" + to_string(i) + "_" + to_string(j) + "/spectra.html", "a" );

      fprintf(phpFiles.in,"<!DOCTYPE html> \
<html>\
<head>\
\
    <script src=\"../../vendor/jquery-1.11.0.min.js\"></script>\
    <script src=\"../../vendor/dygraph-combined.js\"></script>\
    <script src=\"../../lib/script.js\"></script>\
    <link href=\"../../vendor/normalize.css\" rel=\"stylesheet\" type=\"text/css\">\
    <link href=\"../../vendor/foundation.min.css\" rel=\"stylesheet\" type=\"text/css\">\
    <link href=\"../../lib/style.css\" rel=\"stylesheet\" type=\"text/css\">\
    <title>SOM u-matrix</title>\
</head>\
\
<body>\
    <div class=\"fullWidth row\">\
        <div class=\"fit small-9 columns\">\
            <div id=\"div_g\" class=\"fit\" style=\"position:relative;width:100%%\"></div>\
        </div>\
\
        <div class=\"fit scroll small-3 columns\">\
            <div class=\"header\"> \
                <h3>Neuron x: %d y: %d</h3>\
                <h5><span id='h'>Reference vector</span></h5>\
               \
                <p>&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp<a href=\"../%d_%d/spectra.html\">Y+</a></p>\
                <p><a href=\"../%d_%d/spectra.html\">X-</a>  &nbsp   <a href=\"../../index.html\">HOME</a>  &nbsp      <a href=\"../%d_%d/spectra.html\">X+</a></p> \
                <p>&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp<a href=\"../%d_%d/spectra.html\">Y-</a></p>\
            </div>\
\
            <p class='sp' onclick=\"change(-1, 'Reference Vector', 'n_spectrum.txt')\">Display reference vector</p>\
\
            <p class='sp' onclick=\"change(-1, 'All Associated Spectra', 'spectra.txt' )\">Display all spectra</p>\
            <div class=\"\">\
                <ol id=\"sul\">\
" , i,j,i,min(j+1, SIZE_Y-1), max(i-1,0),j,  min(i+1,SIZE_X-1) ,j , i, max(j-1,0));

    }


    REP(i,ROWS){

      fscanf(nn.in, "%d%d%d", &clas, &x, &y );
      if (IS_NAME) fscanf(names.in, "%s", name);
      else strcpy( name , "no name" );

        IO nFiles("result/neurons/" + to_string(x) + "_" + to_string(y) + "/spectra.txt", "a" );
        IO phpFiles("result/neurons/" + to_string(x) + "_" + to_string(y) + "/spectra.html", "a" );

        REP(j,COLS-1){ //memcpy?
              fscanf(data.in,"%f", &f);
              fprintf(nFiles.in, "%f ", f);
        }
        fscanf(data.in,"%f", &f);

        fprintf(nFiles.in, "%f\n", f);



        if (clas)

        fprintf(phpFiles.in,"<li class=\"sli sli%d\" onclick=\"change(%d,"
               "'Spectrum %s class: %d',"
               "'spectra.txt'"
               ")\">%s class: %d</li>\n",  clas, que[x][y]++, name, clas, name, clas);

        else

        fprintf(phpFiles.in,"<li class=\"sli sli%d\" onclick=\"change(%d,"
               "'Spectrum %s class: x',"
               "'spectra.txt'"
               ")\">%s class: x</li>\n",  clas, que[x][y]++, name,  name);

     /* <li class="sli sli<?php echo "4"?>" onclick="change(0,
    'Spectrum uncpunc/dsunc/ class: 4',
    '<?php echo "neurons/".$_POST["x-coord"]."_".$_POST["y-coord"]."/spectra.txt"  ?>'
			   )">uncpunc/dsunc/ class: 4</li>
    */
    }

      /*<?php
	$url = "neurons/".$_GET["x"]."_".$_GET["y"]."/spectra.php";
	include $url;
    ?>*/
   REP(i,SIZE_X)
    REP(j,SIZE_Y){
      IO phpFiles("result/neurons/" + to_string(i) + "_" + to_string(j) + "/spectra.html", "a" );
fprintf(phpFiles.in," \
</ol>\
</div>\
</div>\
</div>\
</body>\
</html>\
"/*,    i,j*/);
    }

    IO index("result/index.html","w");


    fprintf(index.in, "\
<!DOCTYPE html>\
<html>\
<head>\
    <meta charset=\"utf-8\" />\
    <script src=\"vendor/jquery-1.11.0.min.js\"></script>\
    <link href=\"vendor/normalize.css\" rel=\"stylesheet\" type=\"text/css\">\
    <link href=\"vendor/foundation.min.css\" rel=\"stylesheet\" type=\"text/css\">\
\
    <title>SOM U-matrix</title>\
</head>\
\
<body>\
    <h1>SOM U-matrix</h1>\
    <img alt=\"nnmatrix\" class=\"down\"id=\"nn\" src=\"neurons/nnmatrix.png\" usemap=\"#nnmatrixmap\" height=\"1000\" width=\"1000\">\
    <!--<img alt=\"umatrix\" id=\"uu\" src=\"neurons/umatrix.png\" usemap=\"#umatrixmap\" height=\"446\" width=\"602\" >-->\
\
    <script type=\"text/javascript\">\
	$('#nn').click(function(event)\
{   \
    var sizeMultiplicator = $(this).width() / 1000.0;\n \
    var left = event.offsetX?(event.offsetX):event.pageX \n \
    - $(this).offset().left; \n \
    var top = event.offsetY?(event.offsetY):event.pageY \n \
    - $(this).offset().top; \
\
   left -= 241 * sizeMultiplicator;\
   var left_part = 545.0 / %d.0 * sizeMultiplicator;\
   var numX = Math.floor (( left / left_part)) ;\
\
   top -= 103 * sizeMultiplicator;\
   var top_part = 545.0 / %d.0 * sizeMultiplicator;  \
   var numY = Math.floor(( top / top_part ));\
\
   if(numX == -1) numX = 0; \
   if(numY == -1) numY = 0; \
   if(numX < 0 || numX >= %d || numY < 0 || numY >= %d) return;\
   window.location.href = \"neurons/\" + numX + \"_\" + numY + \"/spectra.html\" ;	\
	\
});\
\
	$('#uu').click(function(event)\
{   \
    var left = event.offsetX?(event.offsetX):event.pageX \n \
    - $(this).offset().left; \n \
    var top = event.offsetY?(event.offsetY):event.pageY \n \
    - $(this).offset().top; \
\
   left -= 96;\
   var left_part = 412 / %d; \
   left-= left_part / 2.0;\
   var numX = Math.floor ( left / left_part);\
\
   top -= 67;\
   var top_part = 221 / %d;  \
   top -= top_part / 2.0; \
   var numY = %d - Math.floor( top / top_part );\
\
   if(numX < 0 || numX >= %d || numY < 0 || numY >= %d) return;\
   window.location.href = \"neurons/\" + numX + \"_\" + numY + \"/spectra.html\" ;	\
	\
});\
    </script>\
</body>\
</html> \
"
, SIZE_X, SIZE_Y, SIZE_X, SIZE_Y , SIZE_X+1, SIZE_Y+1, SIZE_Y-1, SIZE_X, SIZE_Y );
 /*<div style=\"position:fixed;bottom:30px;left:30px;\"> \
<h2>Go to coordinates:</h2>\
<form action= \"display.php\" method=\"get\">\
X: <input style=\"margin=15px;padding=10px;\" type=\"text\" name=\"x\"><br>\
Y: <input style=\"margin=15px;padding=10px;\" type=\"text\" name=\"y\"><br>\
<input type=\"submit\">\
</form>\
</div>\*/

    REP(i,SIZE_X) delete[] que[i];
    delete[] que;





  return 0;
}
