#include "height-map.h"

#include <random>
#include <QImageWriter>
#include <QImageReader>

// Box2
//****/

const double Box2::epsilon = 1.0e-5; //!< Epsilon value used to check intersections and some round off errors.

const int Box2::edge[8] =
{
  0,1,2,3,
  0,2,1,3,
};

const Vector Box2::normal[1] =
{
  Vector(0.0,0.0,1.0)
};

/*!
\brief Create a box given a center point and the half side length.
\param c Center.
\param r Half side length.
*/
Box2::Box2(const Vector& c, double r)
{
  a = c - Vector(r,r,0.0);
  b = c + Vector(r,r,0.0);
}

/*!
\brief Create a cube centered at the origin and of given half side length.

This is equivalent to:
\code
Box box(Vector(0.0),2.0);  // Simplified constructor Box(2.0);
\endcode
\param r Half side length.
*/
Box2::Box2(double r)
{
  a = -Vector(r,r,0.0);
  b = Vector(r,r,0.0);
}

/*!
\brief Create a box given two opposite corners.

Note that this constructor does not check the coordinates of the two vectors.
Therefore, the coordinates of a should be lower than those of b.

To create the axis aligned bounding box of two vectors a and b in
the general case, one should use:
\code
Box box(Vector::Min(a,b),Vector::Max(a,b));
\endcode
\param a,b End vertices.
*/
Box2::Box2(const Vector& a, const Vector& b)
{
  Box2::a = a;
  Box2::b = b;
}

/*!
\brief Check if vector inside the square
*/
bool Box2::Inside(const Vector& v) const {
    if (a[0]<v[0] && b[0]>v[0] && a[1]<v[1] && b[1]>v[1]){return true;}
    return false;
};

bool Box2::Inside(const double& x, const double& y) const {
    if (a[0]<x && b[0]>x && a[1]<y && b[1]>y){return true;}
    return false;
};

/*!
\brief Check if vector intersect the square
*/
bool Box2::Intersect(const Box2& box) const {
    if (Inside(box.a) | Inside(box.b) |
        Inside(box.a[0], box.b[1]) | Inside(box.b[0], box.a[1])){
        return true;
    }
     return false;
};


// Grid
//****/

Grid::Grid(){
    // Init
    n = 256;
    double r = 1.0;
    a = -Vector(r,r,0.0);
    b = Vector(r,r,0.0);
    Load_Grid();
};

Grid::Grid(int set_n, double r){
    // Init
    n = set_n;
    a = -Vector(r,r,0.0);
    b = Vector(r,r,0.0);
    Load_Grid();
};

void Grid::Load_Grid(){
    int i;
    int j;
    double deltai = abs(a[0]-b[0])/n;
    double deltaj = abs(a[1]-b[1])/n;
    vGrid.resize(n*n);
    for (int index=0; index<(n*n); index++){
        i = index / n;
        j = index % n;
        vGrid[index] = Vector(a[0]+deltai*i, a[1]+deltaj*j, 0.0);
    }
    max_height = 0.0;
};

bool Grid::Inside(int i, int j){
    if (Index(i,j) < vGrid.size()){return true;}
    return false;
};

int Grid::Index(int i, int j) const{
    int index = i*n+j;
    return index;
}

int Grid::GetN() const{
    return n;
}

Vector Grid::Value (int i, int j) const{
    return vGrid[Index(i,j)];
}

Vector Grid::Value(int index) const{
    return vGrid[index];
}

// ScalarField
//***********/

ScalarField::ScalarField(){
    n = 256;
    double r = 1.0;
    a = -Vector(r,r,0.0);
    b = Vector(r,r,0.0);
    Load_Grid();
}

ScalarField::ScalarField(int set_n, double r){
    n = set_n;
    a = -Vector(r,r,0.0);
    b = Vector(r,r,0.0);
    Load_Grid();
}

// note : la fonction compile sans erreur mais ne donne pas de bon resultats
    //la reccuperation des couleurs dans l'image n'est pas la bonne
    //A RE-TRAVAILLER !
void ScalarField::Load_Image(std::string filename){
    std::cout<<"Loading "<<filename<<" ..."<<std::endl;
    // Read the image
    QString FileName = QString::fromStdString(filename);
    QImage image(n, n, QImage::Format_RGB32);
    QImageReader reader;
    reader.setFileName(FileName);
    reader.read(&image);

    // Load content in vGrid
    int u;
    int v;
    for (int index=0; index<n*n; index++){
        u = index/n;
        v = index%n;
        QColor color(image.pixelColor(u,v));
        // Alpha : 255
        // RGB : -1 0 205
        // HSV : 205 205 205
        //color.toHsv();
        float height = color.lightnessF();
        if (height!=0){std::cout<<height<<std::endl;}
        vGrid[index][2] = height;
        //std::cout<<vGrid[index]<<std::endl;

        //D'apres Gimp : vmax ~=30 et vmin = 0

        //maj de la hauteur max
        if (height>max_height){max_height = height;}    }
    std::cout<<"Load success"<<std::endl;
}

void ScalarField::Save_Image(){
    std::cout<<"Saving scalar_field.png ..."<<std::endl;
    // Initialization
    QString imagePath(QStringLiteral("./AppTinyMesh/Data/Result/scalar_field.png"));
    QImage image(n, n, QImage::Format_Grayscale8);
    image.fill(Qt::black);

    // Filling
    QRgb color;
    uint height;
    int u;
    int v;
    float factor = 255 / max_height;
    for (int index=0; index<n*n; index++){
        u = index/n;
        v = index%n;
        height = vGrid[index][1] * factor;
        color = qGray(height, height, height);
        image.setPixel(u, v, color);
    }

    // Writting
    QImageWriter writer(imagePath);
    writer.write(image);
    std::cout<<"Save success"<<std::endl;
}

double ScalarField::ScalarValue(Vector p)
{
  return Norm(p) - 1.0;
}

Vector ScalarField::Gradient(Vector p) {
    double Epsilon = 1e-6;
    double x = ScalarValue(Vector(p[0] + Epsilon, p[1], p[2])) - ScalarValue(Vector(p[0] - Epsilon, p[1], p[2]));
    double y = ScalarValue(Vector(p[0], p[1] + Epsilon, p[2])) - ScalarValue(Vector(p[0], p[1] - Epsilon, p[2]));
    double z = ScalarValue(Vector(p[0], p[1], p[2] + Epsilon)) - ScalarValue(Vector(p[0], p[1], p[2] - Epsilon));

    return Vector(x, y, z) * (0.5 / Epsilon);
}

Vector ScalarField::Normalize(Vector p){
    return p * (1.0 / Norm(p));
}

Vector ScalarField::Clamp(Vector p){
    // TODO
    return p;
}

double ScalarField::GradientNorm(Vector p) {
    return Norm(Gradient(p));
}

Vector ScalarField::Laplacian(Vector p) {
    return Gradient(Gradient(p));
}


void Smooth(){
    //TODO

    // kernel = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
};

void Blur(){
    //TODO

    // kernel = {{1,1,1}, {1,1,1}, {1,1,1}};
};



// HeightField
//***********/

/*!
\brief Constructor.
*/
HeightField::HeightField(){
    n = 256;
    double r = 1.0;
    a = -Vector(r,r,0.0);
    b = Vector(r,r,0.0);
    Load_Grid();
}

HeightField::HeightField(int set_n, double r){
    n = set_n;
    a = -Vector(r,r,0.0);
    b = Vector(r,r,0.0);
    Load_Grid();
}

double HeightField::heightTable[16] = {
    5.0, 5.0, 5.0, 5.0,
    3.0, 3.0, 3.0, 3.0,
    0.0, 0.0, 0.0, 0.0,
    -1.0, -1.0, -1.0, -1.0
};

/*!
\brief Compute the height for p(x,y)

source (slide 10) https://perso.liris.cnrs.fr/eric.galin/M2/Landscapes/1-terrain-models.pdf
*/
Vector HeightField::Height(const double& x, const double& y){
    double u = (x - a[0])/(b[0] - a[0]);
    double v = (y - a[1])/(b[1] - a[1]);

    int nu = int(u*n);
    int nv = int(v*n);

    u = u - nu*(b[0] - a[0])/n;
    v = v - nv*(b[1] - a[1])/n;

    double h = 0;
    if (u+v<1){
        h = (1-u-v)*vGrid[Index(nu,nv)][1]
                +u*vGrid[Index(nu+1, nv)][1]
                +v*vGrid[Index(nu,nv+1)][1];
    }
    else{
        h = (u+v-1)*vGrid[Index(nu+1, nv+1)][1]
           +(1-u)*vGrid[Index(nu+1, nv)][1]
           +(1-v)*vGrid[Index(nu, nv+1)][1];
    }
    Vector point {x, y, h};
    return point;
}

/*!
\brief Compute the slope of the field.
\param p vector point
*/
Vector HeightField::Slope(const Vector& p){
    return Abs(Gradient(p));
}

/*!
\brief Compute the slope of the field.
\param i, j Grid point coord
*/
Vector HeightField::AverageSlope(int i, int j){
    Vector p = Height( (double) i+1, (double) j);
    Vector slope = Slope(p);
    p = Height( (double) i+1, (double) j+1);
    slope += Slope(p);
    p = Height( (double) i, (double) j+1);
    slope += Slope(p);
    p = Height( (double) i-1, (double) j+1);
    slope += Slope(p);
    p = Height( (double) i-1, (double) j);
    slope += Slope(p);
    p = Height( (double) i-1, (double) j-1);
    slope += Slope(p);
    p = Height( (double) i, (double) j-1);
    slope += Slope(p);
    p = Height( (double) i+1, (double) j-1);
    slope += Slope(p);

    slope /= 8;
    return slope;
}
