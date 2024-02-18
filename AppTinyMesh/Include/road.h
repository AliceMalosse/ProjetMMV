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

    double width;

    std::vector<Vector> path;

    static double factor; //define the weigth of the slope in point weigth ccalculation in dijktra

public:
    Road();
    Road(int, int);
    Road(int, int, double);

    void SetStart(int);
    void SetFinish(int);
    void SetWidth(double);

    int Start() const;
    int Finish() const;
    double Width() const;
    std::vector<Vector> Path() const;

    std::vector<int> GetConnection(int, int);

    Vector Slope(Vector);
    Vector Gradient(Vector);
    double Value(Vector);

    void FindPath(ScalarField, std::vector<double>, std::vector<bool>);
    void Dijkstra(ScalarField);

    double Weigth(Vector, Vector, Vector);
    int MinimumWeigth(std::vector<double>, std::vector<bool>, ScalarField);
};
