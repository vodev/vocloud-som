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
#include <omp.h>

using namespace std;
typedef pair<int,int> ii;
typedef vector<int> vi;
typedef vector<double> vd;
typedef vector<ii> vii;
#define MP make_pair
#define PB push_back
#define TR(it,c) for( typeof(c.begin()) it = c.begin(); it != c.end(); ++it )
#define TRR(it,c) for( typeof(c.rbegin()) it = c.rbegin(); it != c.rend(); ++it)
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
class Node;
class SOM;
class Input;

//small vs big
int TRAIN_DATA_NUM;

const string TRAIN_STRING= ".temp/data.shuffle";
const string CLASSES_STRING = ".temp/classes.shuffle";
const string QUE_STRING = ".temp/que.shuffle";

const string REAL_DIVIDER_STRING_TRAIN = ".temp/PNG_divider.som";
const string DIVIDER_STRING_TRAIN = ".temp/ClustersTrain.som";
const string U_MATRIX_FILE = ".temp/umatrix.som";
const string CLUSTER_GRAPH_FILE = ".temp/cg";

/***************************/

int MAP_SIZE_X;  // 10 * 10 neuron layer.
int MAP_SIZE_Y;  // 10 * 10 neuron layer.

int REPETITION;

int INPUT_DIM;//  10/*/ 1997/**/ ;

 //  denormalize to  <-1.0, 1.0>; !!!FIXME ... snad 0..1 .

/***********************************/


double MAP_RADIUS;
double LN_MAP_RADIUS; //TO DO neinicializovane.

double learningRate0;
double ERR;

double  * inputVector;
double ** arr;

int * permutation;
double start_time;

//int id[TRAIN_DATA_NUM];
int ID;
double learningRate, influence;

int error(string s){
    cout << "----------------------------"<< endl;
    cout << "error: " << s << "." << endl;
    cout << "----------------------------"<< endl;
    exit(1);
}

int IMPLEM,PERCENT,VISUALIZE,ERROR,is_clas,WITHOUT_TRAIN, ROWS,COLS, THREAD_NUM, PITER;


void processArguments(int argc, char *argv[]){


    if (argc == 6) WITHOUT_TRAIN = true;
    else if (argc == 11)WITHOUT_TRAIN = false;
    else error( "bad number of parameters");

    int it = 0;

    printf("program %s starts.\n", argv[it++] );
    printf("taking arguments: \n");


    printf("rows: %s\n", argv[it]);
    ROWS = TRAIN_DATA_NUM = atoi(argv[it++]);

    printf("cols: %s\n", argv[it]);
    COLS = INPUT_DIM = atoi(argv[it++]);

    printf("map_size x: %s\n", argv[it]);
    MAP_SIZE_X = atoi(argv[it++]);

    printf("map_size y: %s\n", argv[it]);
    MAP_SIZE_Y = atoi(argv[it++]);

    if(!WITHOUT_TRAIN){

      printf("repetition: %s\n", argv[it]);
      REPETITION = atoi(argv[it++]);

      printf("learningRate0: %s\n", argv[it]);
      learningRate0 = atof(argv[it++]);
    }
    //printf("clas: %s\n", argv[it]);
    is_clas = atoi(argv[it++]);

     if(!WITHOUT_TRAIN){
        THREAD_NUM = atoi( argv[it++]);
        IMPLEM = atoi(argv[it++]);
        PITER = atoi(argv[it++]);
     }



    MAP_RADIUS =  min ( MAP_SIZE_X, MAP_SIZE_Y ) /2 ;
    LN_MAP_RADIUS = log(MAP_RADIUS);

    //IMPLEM = 0; //TODO nacitat z argumentov
    PERCENT = 0;
    VISUALIZE = 1;
    ERROR = 1;

}

class Node{
  public:

    Node(){
      weight = new double[INPUT_DIM+2];
    }
    ~Node(){
      delete[] weight;
    }

    void randInit(){
        REP(i,INPUT_DIM){
            weight[i] = 0.95 + (double)( (rand() % 1000)  )  / 1000.0  ; //TODO sprav testy na delitela ;)  //initialize to random value;  using int 0 - MAX_VAL instead of 0 - 1;
           //cout << weight[i] << endl; usleep(50000);
       }
        //REP(i,PARAM_NUM) probability[i] = 0;
        //probCnt = 0;
    }


    void adjustWeights(){
        REP(i,INPUT_DIM){
            weight[i] += learningRate * influence * (inputVector[i] - weight[i]);
        }
    }

    double getDistance(){
        double distance = 0;
        REP(i,INPUT_DIM){
            distance += (inputVector[i] - weight[i]) * (inputVector[i] - weight[i]);
            //distance += ((double)inputVector[i] - weight[i]) * ((double)inputVector[i] - weight[i]) * ((double)inputVector[i] - weight[i]) * ((double)inputVector[i] - weight[i]);
        }
        //if ( SQRT_DIST || 0) return sqrt(distance);
        //else
        return distance;
    }

    double getDist(Node & x){

        double distance = 0;
        REP(i,INPUT_DIM){
             distance += ( x.weight[i] - weight[i]) * ( x.weight[i] - weight[i]);
        }
        //return sqrt(distance);
        return distance;
    }


    double * weight; //sizeof INPUT_DIM /*$*/

};

Node ** neurons;/*$*/

class Input{ //rename to IO.
  public:

    Input(string file, string mode){
        in = fopen( file.c_str(), mode.c_str());
        if (in == NULL)error("bad input file "+ file);
       // out= fopen( "TestSmall.txt" ,"w");
       // if (out == NULL)error("bad out file");
       first = 1;
    }
    ~Input(){
        fclose(in);
       // fclose(out)

    }
    int getInt(){

        int ret;

        if ( fscanf(in,"%d", &ret) != 1){ return 0;}
        return ret;
    }

    void getInputVector(){
        //fscanf(in,"%d", &ID);
        //if (ID < 256) error("bad input reading!");

        REP(i,INPUT_DIM){
            fscanf(in,"%lf", &inputVector[i]);
        }

    }

    void readWeightVector( Node * n ){

        REP(i,INPUT_DIM){
            fscanf(in,"%lf", &n->weight[i]);
            //cout << "  |" << n->weight[i] << "|  ";
        }

    }



    void writeNum( int num ,char endChar){
        fprintf(in,"%d%c", num, endChar);
    }
    void fwriteNum( double num ,char endChar){
        fprintf(in,"%lf%c", num, endChar);
    }


    int first;
    FILE * in;
    //FILE * out;
};





class SOM{
  public:

    SOM( int n, int r ):iter_num(n),repetitions(r){
        REP(i,MAP_SIZE_X)
        REP(j,MAP_SIZE_Y){
            neurons[i][j].randInit();
        }
    }
    SOM(){}

    SOM(string PATH, int n ):iter_num(n){
      Input in( PATH,"r");

      char buffer[100];
      fgets(buffer, 100, in.in);

      REP(i,MAP_SIZE_X)
      REP(j,MAP_SIZE_Y){
          in.readWeightVector(&neurons[i][j]);
      }

/*      REP(i,MAP_SIZE_X)
      REP(j,MAP_SIZE_Y){
      if (i==0 && j == 0)REP(k,iter_num) cout << neurons[i][j].weight[k] << "  ";

      }
*/
    }

    double processDist(double & mn, int & mnI, int i, int j, int x1, int x2 ){
        double val = neurons[i][j].getDist( neurons[i+x1][j+x2] );
        if (val < mn){ mn = val; mnI = x1 + 2*x2;  }
        return val;
    }

    void dfs(int * gc, int ** gn ,int * visited, int u, int group){
        REP(i, gc[u]){
            if ( !visited[gn[u][i]] ){ visited[gn[u][i]] = group; dfs( gc, gn, visited, gn[u][i], group ); }
        }
    }

    void createUMatrix(){

            Input umatfile(U_MATRIX_FILE, "w");
            double div,val, suma, mn ;
            int mnI;


/***GRAPH*****
            int * gc = new int[MAP_SIZE_X*MAP_SIZE_Y]; //graph
            int * visited = new int[MAP_SIZE_X*MAP_SIZE_Y];
            int ** gn = new int * [MAP_SIZE_X*MAP_SIZE_Y];
            REP(i,MAP_SIZE_X*MAP_SIZE_Y)gn[i] = new int[5];

             memset(gc, 0, sizeof(int)*MAP_SIZE_X*MAP_SIZE_Y );
            memset(visited, 0, sizeof(int)*MAP_SIZE_X*MAP_SIZE_Y );
*/


            double ** uVal = new double * [MAP_SIZE_X];
            REP(i,MAP_SIZE_X) uVal[i] = new double[MAP_SIZE_Y];/*$*/







            REP(i,MAP_SIZE_X)REP(j,MAP_SIZE_Y){  //can be more effective, but it has little inpackt to whole run-time.
                div = 4; suma = 0;
                mn = 1000000;

                if (j == 0) div--;
                else suma += processDist( mn, mnI, i, j,0, -1);//-2
                if (i == 0) div--;
                else suma += processDist( mn, mnI, i, j,-1, 0 );//-1
                if (i+1 == MAP_SIZE_X) div--;
                else suma += processDist( mn, mnI, i, j, 1, 0 );//1
                if (j+1 == MAP_SIZE_Y) div--;
                else suma += processDist( mn, mnI, i, j, 0, 1 );//2

                int u = i*MAP_SIZE_Y + j;
                uVal[i][j] = suma/div;

                 umatfile.writeNum( i, ' ');
                 umatfile.writeNum( j, ' ');
                 double x = uVal[i][j] < 1.0 ? log(uVal[i][j]): log(uVal[i][j]);
                // int x = log(10 * uVal[i][j]);
                 umatfile.fwriteNum( x , '\n');


                 /***GRAPH*****
                switch (mnI){ //create graph
                    case -2 :   gn[ u ][ gc[u]++ ] = u - 1;            gn[ u - 1 ][ gc[u - 1]++ ] = u;                break;// =  i*MAP_SIZE + (j-1)
                    case -1 :   gn[ u ][ gc[u]++ ] = u - MAP_SIZE_Y;     gn[ u - MAP_SIZE_Y ][ gc[u - MAP_SIZE_Y]++ ] = u;  break;// =  (i-1)*MAP_SIZE + j
                    case 1 :    gn[ u ][ gc[u]++ ] = u + MAP_SIZE_Y;     gn[ u + MAP_SIZE_Y ][ gc[u + MAP_SIZE_Y]++ ] = u;  break;// =  i*MAP_SIZE + (j+1)
                    case 2 :    gn[ u ][ gc[u]++ ] = u + 1;            gn[ u+1 ][ gc[u+1]++ ] = u;                    break;// =  (i+1)*MAP_SIZE + j
                }
                */

            }


                /***GRAPH*****/
           /* REP(i,MAP_SIZE_X)
            REP(j,MAP_SIZE_Y){
                mn = 1000000;
                if (j != 0) if ( uVal[i][j-1] < mn){ mn = uVal[i][j]; mnI = 0;}
                if (i != 0) if ( uVal[i-1][j] < mn){ mn = uVal[i][j]; mnI = 1;}
                if (i+1 != MAP_SIZE_X) if ( uVal[i+1][j] < mn){ mn = uVal[i][j]; mnI = 2;}
                if (j+1 != MAP_SIZE_Y) if ( uVal[i][j+1] < mn){ mn = uVal[i][j]; mnI = 3;}

                int u = i*MAP_SIZE_Y + j;

                switch (mnI){ //create graph
                    case 0 :   gn[ u ][ gc[u]++ ] = u - 1;            gn[ u - 1 ][ gc[u - 1]++ ] = u;                break;// =  i*MAP_SIZE + (j-1)
                    case 1 :   gn[ u ][ gc[u]++ ] = u - MAP_SIZE_Y;     gn[ u - MAP_SIZE_Y ][ gc[u - MAP_SIZE_Y]++ ] = u;  break;// =  (i-1)*MAP_SIZE + j
                    case 2 :    gn[ u ][ gc[u]++ ] = u + MAP_SIZE_Y;     gn[ u + MAP_SIZE_Y ][ gc[u + MAP_SIZE_Y]++ ] = u;  break;// =  i*MAP_SIZE + (j+1)
                    case 3 :    gn[ u ][ gc[u]++ ] = u + 1;            gn[ u+1 ][ gc[u+1]++ ] = u;                    break;// =  (i+1)*MAP_SIZE + j
                }
            }



            int clusterNum = 0;
            REP(i,MAP_SIZE_X * MAP_SIZE_Y){
                if(!visited[i]){   dfs(gc , gn, visited,i,(++clusterNum)%15 + 1) ; }
            }



            Input graphFile(CLUSTER_GRAPH_FILE, "w");

            REP(i,MAP_SIZE_X)REP(j,MAP_SIZE_Y){
                graphFile.writeNum( i, ' ');
                graphFile.writeNum( j, ' ');
                graphFile.writeNum( visited[i*MAP_SIZE_Y + j] , '\n');
            }*/


            //destruction

            /***GRAPH*****
            delete[] gc;

            delete[] visited;

            REP(i, MAP_SIZE_X*MAP_SIZE_Y) delete[] gn[i];
            delete[] gn;*/


            REP(i,MAP_SIZE_X) delete[] uVal[i];
            delete[] uVal;


    }






    void train(){

        cout << "Training started!" << endl;

        int iterAll = iter_num * repetitions;
        int timeStep = 1;
        ii bmu;

        int PROBING = iterAll;

        if ( IMPLEM == 1){//1==probing_alg.
            PROBING = (double)iterAll * ((double)PERCENT * 0.01);

        }
        int BFS = double(iterAll) * 0.8;  //0.0 vzdy bfs 1.0 vzdy n*n

        int cnt = 0, cntP = 0;


       // inputVector = arr[ 0 ];
        // cout << "T: " << omp_get_wtime() - start_time << endl;


        //build SOM.
        REP(i,repetitions){

            REP(j,iter_num){

                inputVector = arr[ permutation[j] ];

                if (timeStep <= PROBING){  /*bmu = probingAlgorithm();}*/   bmu = bestMatchingNode();}
                else {bmu = probingAlgorithm();  }




                //cout << "bmu " << bmu.first << " " << bmu.second << endl; usleep(5000);
                if (timeStep <= BFS){cnt++; actualizeNodes(bmu, iterAll, timeStep);}
                else {cntP++; actualizeNodesBFS(bmu, iterAll, timeStep);}

                timeStep++;

                //if (timeStep == 2)break;/*delee*/
                if (timeStep % 100 == 0){ cout << timeStep << " of " << iterAll << endl;
                  //cout << "T: " << omp_get_wtime() - start_time << endl;
                }
            }

            //if (timeStep == 2)break;/*delee*/
        }

        cout << "---------------" << endl;
        cout << cnt << "  " << cntP << endl;
        cout << "---------------" << endl;

        serialize();


    }

    void serialize(){
      Input out(".temp/code1.map","w");
      fprintf(out.in, "padding\n");
      REP(i,MAP_SIZE_X)
      REP(j,MAP_SIZE_Y){
        REP(k,COLS){
          fprintf(out.in," %lf", neurons[i][j].weight[k]);
        }
      }
    }


    void postProcess(){

        Input in(TRAIN_STRING,"r"); //".temp/data.shuffle";
        Input devider(DIVIDER_STRING_TRAIN,"w"); //".temp/ClustersTrain.som";

        if(WITHOUT_TRAIN) { // the column number at beginning
           char buffer[100];
           fgets(buffer, 100, in.in);
        }

        Input * classes;
        if (is_clas) classes = new Input(CLASSES_STRING,"r");

        ii bmu;

        int clasNum,queNum;

        ERR = 0.0;
        cout << "error counting..." << endl;

        REP(j,iter_num){

            if (is_clas) clasNum = classes->getInt();
            else clasNum = 0;
            in.getInputVector();//fill global array

            bmu = bestMatchingNode();



            /******************/ devider.writeNum(clasNum, ' ');
                                devider.writeNum(bmu.first, ' ');devider.writeNum(bmu.second, '\n');
            //addProbability(j,bmu);
        }

        cout << ERR << " <- err sum" << endl;
        cout << iter_num << " <- iter_num " << endl;

        ERR /= iter_num;

        devider.fwriteNum( ERR, '\n' );


        if (is_clas) delete classes;

          /**BEGIN-UMATRIX**//*cout << "Umatrix begin" << endl;

          REP(i,MAP_SIZE+1)REP(j,MAP_SIZE+1)REP(k,INPUT_DIM)if (ucnt[i][j] > 0) umatrix[i][j][k] /= ucnt[i][j];

          int m;
          double dist;

          FOR(i,1,MAP_SIZE){
            m = 0;
            FOR(j,1,MAP_SIZE+1){
                while ( !ucnt[i][j] ){j++;}
                dist = vectorDistance(i,m, i,j);
                dist = sqrt(dist);//fixme pokus :)
                FOR(k, m+1,j){
                    if (k != m+1 && k != j )continue;
                    umatfile.fwriteNum( i - 1, ' ');
                    umatfile.fwriteNum( (double)k - 1.5, ' ');
                    umatfile.fwriteNum( dist , '\n');
                }
                m = j;
            }
          }
          FOR(j,1,MAP_SIZE){
            m = 0;
            FOR(i,1,MAP_SIZE+1){
                while ( !ucnt[i][j] ){i++;}
                dist = vectorDistance(m,j, i,j);
                dist = sqrt(dist); //fixme pokus :)
                FOR(k, m+1,i){
                    if (k != m+1 && k != i )continue;
                    umatfile.fwriteNum( (double)k -1.5, ' ');
                    umatfile.fwriteNum( j -1, ' ');
                    umatfile.fwriteNum( dist , '\n');
                }
                m = i;
            }
          }*/
          /*REP(j,MAP_SIZE)REP(i,MAP_SIZE-1){
                if(j+1 != MAP_SIZE && ucnt[i][j]  && ucnt[i][j+1]){  umatfile.fwriteNum( i, ' '); umatfile.fwriteNum( (double)j + 0.5, ' '); umatfile.fwriteNum( vectorDistance(i,j, i,j+1) , '\n');}
          }*/

          /**END-UMATRIX**///cout << "Umatrix end" << endl;




    }




    ii bestMatchingNode(){
        double dist, min_dist = 9e99;
        ii ret;



        #pragma omp parallel for
        REP(i,MAP_SIZE_X)
        REP(j,MAP_SIZE_Y){

                dist = neurons[i][j].getDistance();
             //   cout << dist << endl; usleep(40000);

                if (dist < min_dist){
                    min_dist = dist;
                    ret = MP(i,j);
                }

        }

        ERR += min_dist;
        //cout << "?. "<< min_dist << endl;
       //usleep(40000);
       // cout << ret.first << ret.second <<endl;
        return ret;
    }

    inline bool isNotLocalMin( int & aX, int & aY, double & dist){
        double d;
        bool ret = false;
        int x = aX, y = aY;
        if (x > 0             ){
            d = neurons[x-1][y].getDistance();
            if (d < dist){  dist = d; aX = x-1; /*aY = y;*/  ret = true; }
        }
        if (x < MAP_SIZE_X - 1){
            d = neurons[x+1][y].getDistance();
            if (d < dist){  dist = d; aX = x+1; /*aY = y;*/  ret = true; }
        }
        if (y > 0             ){
            d = neurons[x][y-1].getDistance();
            if (d < dist){  dist = d; aX = x; aY = y-1;  ret = true; }
        }
        if (y < MAP_SIZE_Y - 1){
            d = neurons[x][y+1].getDistance();
            if (d < dist){  dist = d; aX = x; aY = y+1;  ret = true; }
        }

        return ret;
    }

    ii probingAlgorithm(){



        int rep = PITER;

        int * aX, * aY; double * dist;
        aX = new int[PITER+1];
        aY = new int[PITER+1];
        dist = new double[PITER+1];



        #pragma omp parallel for
        REP(i,rep){
            int r = rand() % ROWS;
            double mn = 100000000.0;
            REP(i2,MAP_SIZE_X)REP(j2,MAP_SIZE_Y){

                if (  (inputVector[r] - neurons[i2][j2].weight[r])*(inputVector[r] - neurons[i2][j2].weight[r]) < mn  ){
                  mn = (inputVector[r] - neurons[i2][j2].weight[r])*(inputVector[r] - neurons[i2][j2].weight[r]);
                  aX[i] = i2; aY[i] = j2;
                }

            }
            //aX[i] = rand() % MAP_SIZE_X;
            //aY[i] = rand() % MAP_SIZE_Y;
            dist[i] = neurons[aX[i]][aY[i]].getDistance();

            while(  isNotLocalMin( aX[i], aY[i], dist[i])  );
        }

        int mn = 0;
        FOR(i,1,rep-1) if (dist[i] < dist[0]){ dist[0] = dist[i]; mn = i;}

        return MP(aX[mn], aY[mn]);



    }

    inline double getTauT(double all, double step){

        //cout << MAP_RADIUS /*tau0*/ * exp(  -step / ( all / LN_MAP_RADIUS )/*lambda*/   ) << ". ." << endl;
        return MAP_RADIUS /*tau0*/ * exp(  -step / ( all / LN_MAP_RADIUS )/*lambda*/   ); //FIXME (double)
    }

    void actualizeNodes(ii bmu, double all, double step){ //! Step cant be "0", starts with "1";
          double tauT = getTauT(all,step);

          learningRate = learningRate0 * exp( (-(double)step)/( (double)all ) );

          #pragma omp parallel for
          REP(i,MAP_SIZE_X)
           REP(j,MAP_SIZE_Y){
              double x =  (bmu.first - i)*(bmu.first - i) + (bmu.second - j)*(bmu.second - j) ;

              if (x < tauT*tauT){
                  influence =  exp (  (- x) / (2*tauT*tauT)    );

                  neurons[i][j].adjustWeights();
              }
          }


    }

     void actualizeNodesBFS(ii bmu, double all, double step){ //! Step cant be "0", starts with "1";
            double tauT = getTauT(all,step);


            learningRate = learningRate0 * exp( (-(double)step)/( (double)all ) );


            //cout << "tt: " << tauT << endl; usleep(50000);

          //  int mFB = 0;


            queue<ii> q;
            queue<int> state;
            double tauTSquare = tauT*tauT;

            influence =  1;
            neurons[bmu.first][bmu.second].adjustWeights();

            if (1 < tauTSquare){
                q.push( MP( bmu.first, bmu.second+1 ) ); state.push(3);
                q.push( MP( bmu.first, bmu.second-1 ) ); state.push(2);
                q.push( MP( bmu.first-1, bmu.second ) ); state.push(1);
                q.push( MP( bmu.first+1, bmu.second ) ); state.push(0);
            }

            ii act;int st;
            double x;



            while ( !q.empty() ){
                act = q.front(); q.pop();
                st = state.front(); state.pop();
                x = (bmu.first - act.first)*(bmu.first - act.first) + (bmu.second - act.second)*(bmu.second - act.second);
                if( act.first < 0 || act.second < 0 || act.first >= MAP_SIZE_X || act.second >= MAP_SIZE_Y)continue;
                if( x >= tauTSquare ) continue;

                influence =  exp (  (- x) / (2.0*tauTSquare)    );
                neurons[act.first][act.second].adjustWeights();

                switch (st){
                    case 0:{
                        q.push( MP( act.first + 1, act.second) ); state.push(0);
                        break;
                    }
                    case 1:{
                        q.push( MP( act.first - 1, act.second) ); state.push(1);
                        break;
                    }
                    case 2:{

                            q.push( MP( act.first - 1, act.second) ); state.push(1);
                            q.push( MP( act.first + 1, act.second) ); state.push(0);
                            q.push( MP( act.first, act.second - 1) ); state.push(2);

                        break;
                    }
                    case 3:{

                            q.push( MP( act.first - 1, act.second) ); state.push(1);
                            q.push( MP( act.first + 1, act.second) ); state.push(0);
                            q.push( MP( act.first, act.second + 1) ); state.push(2);
                        break;
                    }
                }
            }


    }

    int iter_num; //pocet iteracii
    int repetitions;


};

void g_constructor(){
  neurons = new  Node*[MAP_SIZE_X];
  REP(i,MAP_SIZE_X) neurons[i] = new Node[MAP_SIZE_Y];

  arr = new  double*[ROWS];
  REP(i,ROWS) arr[i] = new double[COLS];

  Input in(TRAIN_STRING,"r");
  REP(i,ROWS)REP(j,COLS){
      fscanf(in.in,"%lf", &arr[i][j]);
  }

  permutation = new int[ROWS];
  REP(i,ROWS) permutation[i] = i;
  random_shuffle ( permutation, permutation + ROWS );


  //inputVector = new double[INPUT_DIM];

}
void g_memManagement(){
    REP(i,ROWS) delete[] arr[i];
    delete[] arr;
    inputVector = new double[INPUT_DIM];
}

void g_destructor(){
  REP(i,MAP_SIZE_X) delete[] neurons[i];
  delete[] neurons;
  delete[] permutation;
  delete[] inputVector;

}


int main(int argc, char *argv[]) {


    srand(time(NULL));
    int mode;
   // cout << "For Training Push 0.\nFor Testing  Push 1." << endl;

    //cin >> mode;
    processArguments(argc,argv);
    omp_set_num_threads(THREAD_NUM);

    g_constructor();

    SOM * som;

    if(!WITHOUT_TRAIN){
      som = new SOM( TRAIN_DATA_NUM , REPETITION );

      start_time = omp_get_wtime();
      som->train();
      double runtime = omp_get_wtime() - start_time;
      cout << "threads:" << THREAD_NUM << "  time: " << runtime << "s" << endl;


    }
    else{
      som = new SOM( ".temp/code1.map", TRAIN_DATA_NUM );
    }

    g_memManagement();



    if(VISUALIZE) som->createUMatrix();
    som->postProcess(); //TODO dividers
       // som.serialize();



    g_destructor();
    delete som;

        return 0;
}
