#include "qte.h"
#include "box.h"
#include "implicits.h"
#include "height-map.h"
#include "ui_interface.h"
#include "road.h"

#include <QFileDialog>

MainWindow::MainWindow() : QMainWindow(), uiw(new Ui::Assets)
{
	// Chargement de l'interface
    uiw->setupUi(this);

    uiw->addRoad->setEnabled(false);
    uiw->updateRoad->setEnabled(false);
    uiw->spin_StartX->setEnabled(false);
    uiw->spin_StartY->setEnabled(false);
    uiw->spin_FinishX->setEnabled(false);
    uiw->spin_FinishY->setEnabled(false);

	// Chargement du GLWidget
	meshWidget = new MeshWidget;
	QGridLayout* GLlayout = new QGridLayout;
	GLlayout->addWidget(meshWidget, 0, 0);
	GLlayout->setContentsMargins(0, 0, 0, 0);
    uiw->widget_GL->setLayout(GLlayout);

	// Creation des connect
	CreateActions();

	meshWidget->SetCamera(Camera(Vector(10, 0, 0), Vector(0.0, 0.0, 0.0)));
}

MainWindow::~MainWindow()
{
	delete meshWidget;
}

void MainWindow::CreateActions()
{
	// Buttons
    connect(uiw->boxMesh, SIGNAL(clicked()), this, SLOT(BoxMeshExample()));
    connect(uiw->sphereImplicit, SIGNAL(clicked()), this, SLOT(SphereImplicitExample()));
    connect(uiw->box2Mesh, SIGNAL(clicked()), this, SLOT(Box2Mesh()));
    connect(uiw->gridMesh, SIGNAL(clicked()), this, SLOT(GridMesh()));
    connect(uiw->scalarField, SIGNAL(clicked()), this, SLOT(ScalarFieldMesh()));
    connect(uiw->heightField, SIGNAL(clicked()), this, SLOT(HeightFieldMesh()));

    connect(uiw->addRoad, SIGNAL(clicked()), this, SLOT(AddRoadMesh()));
    connect(uiw->updateRoad, SIGNAL(clicked()), this, SLOT(UpdateRoad()));

    connect(uiw->resetcameraButton, SIGNAL(clicked()), this, SLOT(ResetCamera()));
    connect(uiw->wireframe, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));
    connect(uiw->radioShadingButton_1, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));
    connect(uiw->radioShadingButton_2, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));

	// Widget edition
	connect(meshWidget, SIGNAL(_signalEditSceneLeft(const Ray&)), this, SLOT(editingSceneLeft(const Ray&)));
	connect(meshWidget, SIGNAL(_signalEditSceneRight(const Ray&)), this, SLOT(editingSceneRight(const Ray&)));
}

//empty
void MainWindow::editingSceneLeft(const Ray&)
{
}

//empty
void MainWindow::editingSceneRight(const Ray&)
{
}

void MainWindow::BoxMeshExample()
{
	Mesh boxMesh = Mesh(Box(1.0));

	std::vector<Color> cols;
	cols.resize(boxMesh.Vertexes());
    for (size_t i = 0; i < cols.size(); i++)
		cols[i] = Color(double(i) / 6.0, fmod(double(i) * 39.478378, 1.0), 0.0);

	meshColor = MeshColor(boxMesh, cols, boxMesh.VertexIndexes());
    UpdateGeometry(true, "BoxMesh", meshColor);
}

void MainWindow::SphereImplicitExample()
{
  AnalyticScalarField implicit;

  Mesh implicitMesh;
  implicit.Polygonize(31, implicitMesh, Box(2.0));

  std::vector<Color> cols;
  cols.resize(implicitMesh.Vertexes());
  for (size_t i = 0; i < cols.size(); i++)
    cols[i] = Color(0.8, 0.8, 0.8);

  meshColor = MeshColor(implicitMesh, cols, implicitMesh.VertexIndexes());
  UpdateGeometry(true, "SphereMesh", meshColor);
}

void MainWindow::Box2Mesh()
{
    // Test Box2
    Mesh boxMesh = Mesh(Box2(1.0));

    std::vector<Color> cols;
    cols.resize(boxMesh.Vertexes());
    for (size_t i = 0; i < cols.size(); i++)
        cols[i] = Color(0.8, 0.8, 0.8);

    meshColor = MeshColor(boxMesh, cols, boxMesh.VertexIndexes());
    UpdateGeometry(true, "Box2Mesh", meshColor);
}

void MainWindow::GridMesh()
{
    // Test Grid
    Mesh gridMesh = Mesh(Grid(2, 2.0));

    std::vector<Color> cols;
    cols.resize(gridMesh.Vertexes());
    for (size_t i = 0; i < cols.size(); i++)
        cols[i] = Color(0.8, 0.8, 0.8);

    FieldMesh = MeshColor(gridMesh, cols, gridMesh.VertexIndexes());
    UpdateGeometry(true, "GridMesh", FieldMesh);
}

void MainWindow::ScalarFieldMesh()
{
    uiw->addRoad->setEnabled(true);
    //Test ScalarField
    field = ScalarField(255, 10);
    QString filename = QFileDialog::getOpenFileName( 0, tr("Open file : "), "../TinyMesh/AppTinyMesh/Data", tr("Images (*.png *.jpg)") );

    field.Load_Image(filename);
    field.Save_Image("../TinyMesh/AppTinyMesh/Data/scalar_field.png");
    field.GradientNorm();
    field.Laplacian();

    Mesh mesh = Mesh(field);

    std::vector<Color> cols;
    cols.resize(mesh.Vertexes());
    for (size_t i = 0; i < cols.size(); i++){
        double c = field.Value(i)[2];
        cols[i] = Color(c);
    }
    FieldMesh = MeshColor(mesh, cols, mesh.VertexIndexes());
    UpdateGeometry(true, "ScalarFieldMesh", FieldMesh);
}

void MainWindow::HeightFieldMesh()
{
  uiw->addRoad->setEnabled(true);
  // Test HeightField
  Mesh mesh(HeightField(5, 2.0));

  std::vector<Color> cols;
  cols.resize(mesh.Vertexes());
  for (size_t i = 0; i < cols.size(); i++)
    cols[i] = Color(0.8, 0.8, 0.8);

  FieldMesh = MeshColor(mesh, cols, mesh.VertexIndexes());
  UpdateGeometry(true, "HeigthFieldMesh", FieldMesh);
}

void MainWindow::AddRoadMesh(){
    //enable spin
    uiw->updateRoad->setEnabled(true);
    uiw->spin_StartX->setEnabled(true);
    uiw->spin_StartY->setEnabled(true);
    uiw->spin_FinishX->setEnabled(true);
    uiw->spin_FinishY->setEnabled(true);
    //select start
    int start = uiw->spin_StartX->value() * (field.GetN()+1) + uiw->spin_StartY->value();
    //select finish
    int finish = uiw->spin_FinishX->value() * (field.GetN()+1) + uiw->spin_FinishY->value();;

    //draw start red sphere
    Mesh pointMesh = Mesh(Box2(field.Value(start), 0.1));
    std::vector<Color> cols;
    cols.resize(pointMesh.Vertexes());
    for (size_t i = 0; i < cols.size(); i++)
      cols[i] = Color(1.0, 0.0, 0.0, 1.0);
    meshColor = MeshColor(pointMesh, cols, pointMesh.VertexIndexes());
    UpdateGeometry(false, "startSphereMesh", meshColor);

    //draw finish red sphere
    pointMesh = Mesh(Box2(field.Value(finish), 0.1));
    cols.resize(pointMesh.Vertexes());
    for (size_t i = 0; i < cols.size(); i++)
      cols[i] = Color(1.0, 0.0, 0.0, 1.0);
    meshColor2 = MeshColor(pointMesh, cols, pointMesh.VertexIndexes());
    UpdateGeometry(false, "finishSphereMesh", meshColor2);

    //if(start != finish){
        //calculate path
        //Road road(start, finish);
        //road.Dijkstra(field);

        //build road mesh - add mesh constructor

    //}


}

void MainWindow::UpdateRoad(){
    // Add field to view
    Mesh mesh = Mesh(field);
    std::vector<Color> cols;
    cols.resize(mesh.Vertexes());
    for (size_t i = 0; i < cols.size(); i++){
        double c = field.Value(i)[2];
        cols[i] = Color(c);
    }
    FieldMesh = MeshColor(mesh, cols, mesh.VertexIndexes());
    UpdateGeometry(true, "ScalarFieldMesh", FieldMesh);

    // Add road
    AddRoadMesh();
}

void MainWindow::UpdateGeometry(bool clear, QString mesh_name, MeshColor& mesh)
{
    if(clear){meshWidget->ClearAll();}
    meshWidget->AddMesh(mesh_name, mesh);

    uiw->lineEdit->setText(QString::number(mesh.Vertexes()));
    uiw->lineEdit_2->setText(QString::number(mesh.Triangles()));
    UpdateMaterial();
}

void MainWindow::UpdateMaterial()
{
    meshWidget->UseWireframeGlobal(uiw->wireframe->isChecked());

    if (uiw->radioShadingButton_1->isChecked())
		meshWidget->SetMaterialGlobal(MeshMaterial::Normal);
	else
		meshWidget->SetMaterialGlobal(MeshMaterial::Color);
}

void MainWindow::ResetCamera()
{
	meshWidget->SetCamera(Camera(Vector(-10.0), Vector(0.0)));
}
