#include <iostream>

#include <road.h>

// Constructor
Road::Road(){
    //empty
}

Road::Road(int id_a, int id_b){
    id_start = id_a;
    id_finish = id_b;
    width = 0.1;
    path = {};
}

Road::Road(int id_a, int id_b, double w){
    id_start = id_a;
    id_finish = id_b;
    width = w;
    path = {};
}

// Setter
void Road::SetStart(int a){
    id_start = a;
}

void Road::SetFinish(int b){
    id_finish = b;
}

void Road::SetWidth(double w){
    width = w;
}

// Getter
int Road::Start() const {
    return id_start;
}

int Road::Finish() const {
    return id_finish;
}

double Road::Width() const {
    return width;
}

std::vector<Vector> Road::Path() const {
    return path;
}

// Return the index of the neighboors using M2 neighbooring
std::vector<int> Road::GetConnection(int index, int n){
    int i = index / n;
    int j = index % n;
    std::vector<int> connectivity;
    if(i==0){
        if(j==0){
            connectivity = {index+n, index+n+1,
                            index+1};
        } else if (j==n) {
            connectivity = {index+n,
                            index-1, index+n-1};
        } else {
            connectivity = {index+n, index+n+1,
                            index+1,
                            index-1, index+n-1};
        }
    } else if (i==n) {
        if(j==0){
            connectivity = {index+1, index-n+1,
                            index-n};
        } else if (j==n) {
            connectivity = {index-n, index-n-1,
                            index-1};
        } else {
            connectivity = {index+1, index-n+1,
                            index-n, index-n-1,
                            index-1};
        }
    } else if (j==0) {
        connectivity = {index+n, index+n+1,
                        index+1, index-n+1,
                        index-n};
    } else if (j==n) {
        connectivity = {index+n,
                        index-n, index-n-1,
                        index-1, index+n-1};
    } else {
        connectivity = {index+n, index+n+1,
                        index+1, index-n+1,
                        index-n, index-n-1,
                        index-1, index+n-1};
    }
    return connectivity;
}

// Calculate the Slope - weigth of each edge
Vector Road::Slope(Vector p){
    return Abs(Gradient(p));
}

// Calculate the Gradient
Vector Road::Gradient(Vector p) {
    double Epsilon = 1e-6;
    double x = Value(Vector(p[0] + Epsilon, p[1], p[2])) - Value(Vector(p[0] - Epsilon, p[1], p[2]));
    double y = Value(Vector(p[0], p[1] + Epsilon, p[2])) - Value(Vector(p[0], p[1] - Epsilon, p[2]));
    double z = Value(Vector(p[0], p[1], p[2] + Epsilon)) - Value(Vector(p[0], p[1], p[2] - Epsilon));

    return Vector(x, y, z) * (0.5 / Epsilon);
}

double Road::Value(Vector p)
{
  return Norm(p) - 1.0;
}

double Road::factor = 0.5;

// Calculate a path between start and finish using Dijkstra
void Road::FindPath(ScalarField field, std::vector<double> distGrid, std::vector<bool> exploreGrid){
    path.insert(path.end(), field.Value(id_finish));
    int ind = id_finish;
    while (ind != id_start){
        std::vector<int> connectivity = GetConnection(ind, field.GetN());
        int min = -1;
        for(int c : connectivity){
            if ((c<min || min==-1) && exploreGrid[c]){min = c;}
        }
        ind = min;
        path.insert(path.begin(), field.Value(ind));
    }
    //for(Vector p : path){std::cout<<p<<std::endl;}
}

// Execute the Dijktra algorithme to get the weight of the point between strat and finish point
void Road::Dijkstra(ScalarField field){
    std::cout<<"Running Dijktra algorithm"<<std::endl;
    // Initilisation
    int n = field.GetN();
    std::vector<bool> exploreGrid;
    std::vector<double> distGrid;
    for(int i=0; i<(n+1)*(n+1); i++){
        exploreGrid.push_back(false);
        distGrid.push_back(INT_MAX);
    }
    distGrid[id_start] = 0;

    int k = 0;
    while(exploreGrid[id_finish] == false && k != (n+1)*(n+1)){
        k++;
        int ind = MinimumWeigth(distGrid, exploreGrid, field);
        exploreGrid[ind] = true;
        std::vector<int> connectivity = GetConnection(ind, n);
        for(int i=0; i<connectivity.size(); i++){
            int tmp_ind = connectivity[i];
            double weigth = Weigth(field.Value(tmp_ind), field.Value(id_start), field.Value(id_finish));
            if(!exploreGrid[tmp_ind] && distGrid[ind]!=INT_MAX && (distGrid[ind] + weigth)<distGrid[tmp_ind]){
                distGrid[tmp_ind] = distGrid[ind] + weigth;
            }
        }
    }
    std::cout<<"Dijktra done, compute path"<<std::endl;
    FindPath(field, distGrid, exploreGrid);
}

// Return the weigth of a vector p
    // using his slope and distance from the finish point
double Road::Weigth(Vector p, Vector s, Vector f){
    double max_dist = Norm(f-s);
    return factor*max_dist*Norm(Slope(p)) + (1-factor) * Norm(f-p);
}

// Return the minimal weigth in the whole weigthed matrix named distGrid
int Road::MinimumWeigth(std::vector<double> distGrid, std::vector<bool> exploreGrid, ScalarField field){
    int n = field.GetN();
    double min_w = INT_MAX;
    int ind;
    for(int i=0; i<(n+1)*(n+1); i++){
        if(exploreGrid[i]==false && distGrid[i]<=min_w){
            ind = i;
            min_w = distGrid[i];
        }
    }
    return ind;
}

