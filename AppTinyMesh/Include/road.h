#pragma once

#include <vector>
#include <iostream>

#include <mathematics.h>
#include <height-map.h>

class Road
{
protected:
    int id_start;
    int id_finish;

    std::vector<Vector> path;

    static std::vector<Vector> connection;
    static std::vector<double> dist;
    static double factor; //define the weigth of the slope in point weigth ccalculation in dijktra

public:
    Road();
    Road(int, int);

    void SetStart(int);
    void SetFinish(int);

    int Start();
    int Finish();

    std::vector<int> GetConnection(int, int);

    Vector Slope(Vector);
    Vector Gradient(Vector);
    double Value(Vector);

    void FindPath();
    void Dijkstra(ScalarField);

    double Weigth(Vector, Vector);
    int MinimumWeigth(std::vector<double>, std::vector<bool>, ScalarField);
};
