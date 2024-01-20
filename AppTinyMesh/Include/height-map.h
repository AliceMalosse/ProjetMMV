#pragma once

#include <vector>
#include <iostream>

#include "mathematics.h"

class Box2
{
protected:
  Vector a,b; //!< Lower and upper vertex.
public:
  //! Empty.
  Box2() {}
  explicit Box2(double);
  explicit Box2(const Vector&, const Vector&);
  explicit Box2(const Vector&, double);

  //! Empty.
  ~Box2() {}

  // Access vertexes
  Vector& operator[] (int);
  Vector operator[] (int) const;

  Vector Vertex(int) const;

  // Comparaison
  bool Inside(const Vector&) const;
  bool Inside(const double&, const double&) const;
  bool Intersect(const Box2&) const;

  friend int operator==(const Box2&, const Box2&);
  friend int operator!=(const Box2&, const Box2&);

  friend std::ostream& operator<<(std::ostream&, const Box2&);

public:
  static const double epsilon; //!< Internal \htmlonly\epsilon;\endhtmlonly for ray intersection tests.
  static const int edge[8]; //!< Edge vertices.
  static const Vector normal[1]; //!< Face normals.
};

//! Returns either end vertex of the box.
inline Vector& Box2::operator[] (int i)
{
  if (i == 0) return a;
  else return b;
}

//! Overloaded.
inline Vector Box2::operator[] (int i) const
{
  if (i == 0) return a;
  else return b;
}

/*!
\brief Returns the k-th vertex of the box.

The returned vector is computed by analysing the first three bits of k as follows:
\code
Vector vertex=Vector((k&1)?b[0]:a[0],(k&2)?b[1]:a[1],(k&4)?b[2]:a[2]);
\endcode
*/
inline Vector Box2::Vertex(int k) const
{
  return Vector((k & 1) ? b[0] : a[0], (k & 2) ? b[1] : a[1], (k & 4) ? b[2] : a[2]);
}



//Une grille est un grand rectangle (Box2) discretiser
class Grid : protected Box2
{
protected :
    int n; //Param de discretisation & taille de l'image (ScalarField)
    std::vector<Vector> vGrid;
    float max_height;
public:
    Grid();
    Grid(int);

    void Load_Grid();

    bool Inside(int, int);
    int Index(int, int);
};


// Class ScalarFields, from Grid
class ScalarField : protected Grid
{
protected:
    std::vector<Vector> scalar;
public:
    ScalarField();

    void Save_Image();

    static double Value(const Vector& p);
    static Vector Gradient(const Vector&);
    static Vector Normalize(const Vector&);
    static Vector Clamp(const Vector&); // TODO
    double GradientNorm(const Vector&) const;
    Vector Laplacian(const Vector&) const;

    void Smooth() const; //TODO
    void Blur() const; //TODO
};


//Class HeightField, from ScalarField
class HeightField : public ScalarField
{
protected:
    static double heightTable[16];

public:
    HeightField();
    // Build the height map
    void LoadHeightMap(std::string);
    // Compute the height for x, y coordinates
    Vector Height(const double&, const double&);

    // Slope Fucntion
    static Vector Slope(const Vector&);
    // Average slope in 8 directions
    Vector AverageSlope(int, int);
};