# TP - Modélisation des mondes virtuels

Le code de base est disponible ici : https://github.com/eric-galin/TinyMesh
Cette version a été compiler sous Windows en suivant les instructions de compilation rappeler ci-dessous. L'IDE utilisée est QtCreator. Les erreurs de compilation rencontrées ont été corriger pour les exécutables de Qt uniquement.  

## Instruction de compilation pour Windows
- First, you must install Qt6 (https://www.qt.io/download, use the open source version). Install the msvc2019_64 compiler only (*Note: other compilers can work too*).
- Set the "QTDIR64" environment variable to your Qt installation folder (example: "E:\Code\Qt\6.3.0\msvc2019_64")
- Set the "GLEW_DIR" environment variable to the path "XXX/TinyMesh/Libs/", with XXX the path to your repository.
- Run the script TinyMesh/Script/AppTinyMeshDeploy.bat

Finally, open the Visual Studio solution files with Visual Studio 2022 Community, or open the QtCreatorProject.pro with QtCreator. Compile/Build and execute, everything should work.
*Note: For other IDE, you will have to use the provided CMakeLists.txt to generate the solution files yourself.*

### Résolution d'une erreur de compilation
A cause d'une erreur inattendue de Window, j'ai du faire quelques ajustement pour pourvoir compiler le projet fourni. J'ai commenter les lignes du fichier QtCreatorProject.pro qui permettaient de copier le dossier de Shader dans le dossier de build, et ai fait la copie manuellement. Pour des raisons que j'ignore, Window générait une erreur lors de l'appel d'une des fonctions.
