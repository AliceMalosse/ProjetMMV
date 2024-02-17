#ifndef __Qte__
#define __Qte__

#include <QtWidgets/qmainwindow.h>
#include "realtime.h"
#include "meshcolor.h"
#include "height-map.h"

QT_BEGIN_NAMESPACE
	namespace Ui { class Assets; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT
private:
  Ui::Assets* uiw;           //!< Interface

  MeshWidget* meshWidget;   //!< Viewer

  MeshColor meshColor;		//!< Mesh.
  MeshColor meshColor2;
  MeshColor FieldMesh;
  MeshColor RoadMesh;

  ScalarField field;

public:
  MainWindow();
  ~MainWindow();
  void CreateActions();
  void UpdateGeometry(bool, QString, MeshColor&);

public slots:
  void editingSceneLeft(const Ray&);
  void editingSceneRight(const Ray&);

  void BoxMeshExample();
  void SphereImplicitExample();
  void Box2Mesh();
  void GridMesh();
  void ScalarFieldMesh();
  void HeightFieldMesh();

  void AddRoadMesh();
  void UpdateRoad();

  void ResetCamera();
  void UpdateMaterial();
};

#endif
