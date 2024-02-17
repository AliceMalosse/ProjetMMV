#include <road.h>

// Constructor
Road::Road(){
    //empty
}

Road::Road(int id_a, int id_b){
    id_start = id_a;
    id_finish = id_b;
}

// Setter
void Road::SetStart(int a){
    id_start = a;
}

void Road::SetFinish(int b){
    id_finish = b;
}

// Getter
int Road::Start(){
    return id_start;
}

int Road::Finish(){
    return id_finish;
}

std::vector<int> Road::GetConnection(int index, int n){
    std::vector<int> connectivity = {index+n, index+2*n+1, index+n+1, index+n+2,
                                     index+1, index-n+2, index-n+1, index-2*n+1,
                                     index-n, index-2*n-1, index-n-1, index-n-2,
                                     index-1, index+n-2, index+n-1, index+2*n-1};
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

// Define the vector of M2
std::vector<Vector> Road::connection = {Vector(0, 1, 0),
                                        Vector(1, 2, 0),
                                        Vector(1, 1, 0),
                                        Vector(2, 1, 0),
                                        Vector(1, 0, 0),
                                        Vector(2, -1, 0),
                                        Vector(1, -1, 0),
                                        Vector(1, -2, 0),
                                        Vector(0, -1, 0),
                                        Vector(-1, -2, 0),
                                        Vector(-1, -1, 0),
                                        Vector(-2, -1, 0),
                                        Vector(-1, 0, 0),
                                        Vector(-2, 1, 0),
                                        Vector(-1, 1, 0),
                                        Vector(-1, 2, 0)};

std::vector<double> Road::dist = {1.0, sqrt(5), sqrt(2), sqrt(5),
                                  1.0, sqrt(5), sqrt(2), sqrt(5),
                                  1.0, sqrt(5), sqrt(2), sqrt(5),
                                  1.0, sqrt(5), sqrt(2), sqrt(5)};

double Road::factor = 1.0;

// Calculate a path between start and finish using Dijkstra
void Road::FindPath(){
    // TODO
}

void Road::Dijkstra(ScalarField field){
    // Initilisation
    int n = field.GetN();
    std::vector<bool> exploreGrid;
    std::vector<double> distGrid;
    for(int i=0; i<(n+1)*(n+1); i++){
        exploreGrid[i] = false;
        distGrid[i] = INT_MAX;
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
            distGrid[tmp_ind] = Weigth(field.Value(tmp_ind), field.Value(id_finish));
        }
    }
}

double Road::Weigth(Vector p, Vector f){
    return factor*Norm(Slope(p)) + (1-factor) * Norm(f-p);
}

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





