#include <QTextStream>
#include <QColor>
#include <QFile>
#include <QMap>

#include "triangulation.h"
#include <utilities.h>
using namespace std;

Triangulation::Triangulation()
{
	srand(time(NULL));
	color.InitColors();
	renderMode = GL_LINES;
}

void Triangulation::draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(1.0f);
	glBegin(renderMode);
	if (renderMode == GL_LINES)
	{
		for (int i = 0; i < faces.size(); i++)
		{
			Vector3 c = color.GetColor(i);
			glColor4f(c.getX(), c.getY(), c.getZ(), 1.0f);
			glVertex3f(vertices[faces[i].VertexIndex(0)].Point().getX(), vertices[faces[i].VertexIndex(0)].Point().getY(), vertices[faces[i].VertexIndex(0)].Point().getZ());
			glVertex3f(vertices[faces[i].VertexIndex(1)].Point().getX(), vertices[faces[i].VertexIndex(1)].Point().getY(), vertices[faces[i].VertexIndex(1)].Point().getZ());

			glVertex3f(vertices[faces[i].VertexIndex(1)].Point().getX(), vertices[faces[i].VertexIndex(1)].Point().getY(), vertices[faces[i].VertexIndex(1)].Point().getZ());
			glVertex3f(vertices[faces[i].VertexIndex(2)].Point().getX(), vertices[faces[i].VertexIndex(2)].Point().getY(), vertices[faces[i].VertexIndex(2)].Point().getZ());

			glVertex3f(vertices[faces[i].VertexIndex(2)].Point().getX(), vertices[faces[i].VertexIndex(2)].Point().getY(), vertices[faces[i].VertexIndex(2)].Point().getZ());
			glVertex3f(vertices[faces[i].VertexIndex(0)].Point().getX(), vertices[faces[i].VertexIndex(0)].Point().getY(), vertices[faces[i].VertexIndex(0)].Point().getZ());
		}
	}
	else if (renderMode == GL_TRIANGLES)
	{
		for (int i = 0; i < faces.size(); i++)
		{
			Vector3 c = color.GetColor(i);
			glColor4f(c.getX(), c.getY(), c.getZ(), 0.4f);
			glVertex3f(vertices[faces[i].VertexIndex(0)].Point().getX(), vertices[faces[i].VertexIndex(0)].Point().getY(), vertices[faces[i].VertexIndex(0)].Point().getZ());
			glVertex3f(vertices[faces[i].VertexIndex(1)].Point().getX(), vertices[faces[i].VertexIndex(1)].Point().getY(), vertices[faces[i].VertexIndex(1)].Point().getZ());
			glVertex3f(vertices[faces[i].VertexIndex(2)].Point().getX(), vertices[faces[i].VertexIndex(2)].Point().getY(), vertices[faces[i].VertexIndex(2)].Point().getZ());
		}
	}
	glEnd();

	if (isVoronoi == true)
	{
		glPointSize(10.0);
		glBegin(GL_POINTS);
		for (int i = 0; i < voronoisVertices.size(); i++)
		{
			Vector3 c = color.GetColor(i);
			glColor4f(c.getX(), c.getY(), c.getZ(), 1.0f);
			glVertex3f(voronoisVertices[i].Point().getX(), voronoisVertices[i].Point().getY(), voronoisVertices[i].Point().getZ());
		}
		glEnd();
	}

	if (isCrust == true)
	{
		glLineWidth(2.0f);
		glBegin(GL_LINES);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		for (int i = 0; i < faces.size(); i++)
		{
			Vertex p0 = vertices[faces[i].VertexIndex(0)];
			Vertex p1 = vertices[faces[i].VertexIndex(1)];
			Vertex p2 = vertices[faces[i].VertexIndex(2)];

			if (p0.IsVoronoi() == false && p1.IsVoronoi() == false)
			{
				glVertex3f(p0.Point().getX(), p0.Point().getY(), p0.Point().getZ());
				glVertex3f(p1.Point().getX(), p1.Point().getY(), p1.Point().getZ());
			}

			if (p1.IsVoronoi() == false && p2.IsVoronoi() == false)
			{
				glVertex3f(p1.Point().getX(), p1.Point().getY(), p1.Point().getZ());
				glVertex3f(p2.Point().getX(), p2.Point().getY(), p2.Point().getZ());
			}

			if (p2.IsVoronoi() == false && p0.IsVoronoi() == false)
			{
				glVertex3f(p2.Point().getX(), p2.Point().getY(), p2.Point().getZ());
				glVertex3f(p0.Point().getX(), p0.Point().getY(), p0.Point().getZ());
			}
		}
		glEnd();
	}
}

/* Fonctions principales */
/*
	Triangulation Naive à partir d'un fichier de points : Initilisation des premières faces et ajout des points.

	Pas de convex hull. On crée un quad qui sert de bounding box pour contenir tous les futures points.
*/
void Triangulation::NaiveFileTriangulation(QString f)
{
	isOffFile = false;
	filename = f;
	pointsList.clear();
	vertices.clear();
	faces.clear();

	ReadPointsFile(filename);

	float offset = 2.0f;
	Vector3 min = aabb.GetMinAABB();
	Vector3 max = aabb.GetMaxAABB();
	vertices.push_back(Vertex(Vector3(min.getX() - offset, min.getY() - offset, 0.0f)));
	vertices.push_back(Vertex(Vector3(min.getX() - offset, max.getY() + offset, 0.0f)));
	vertices.push_back(Vertex(Vector3(max.getX() + offset, min.getY() - offset, 0.0f)));
	CreateFace(0, 1, 2);
	vertices.push_back(Vertex(Vector3(max.getX() + offset, max.getY() + offset, 0.0f)));
	CreateFace(1, 3, 2);

	faces[0].FaceIndex(2, 1);
	faces[1].FaceIndex(1, 0);

	for (int i = 0; i < pointsList.size(); i++)
		AddVertex(pointsList[i]);
	DelaunayLawson();
}

/*
	Triangulation Naive : Initilisation des premières faces et ajout des points.

	Pas de convex hull. On crée un quad qui sert de bounding box pour contenir tous les futures points.
	On crée une BB et les points seront ajouté par l'utilisateur avec la souris seulement dedans.
*/
void Triangulation::NaiveTriangulation()
{
	isOffFile = false;
	pointsList.clear();
	vertices.clear();
	faces.clear();

	float offset = 2.0f;
	Vector3 min = aabb.GetMinAABB();
	Vector3 max = aabb.GetMaxAABB();
	vertices.push_back(Vertex(Vector3(min.getX() - offset, min.getY() - offset, 0.0f)));
	vertices.push_back(Vertex(Vector3(min.getX() - offset, max.getY() + offset, 0.0f)));
	vertices.push_back(Vertex(Vector3(max.getX() + offset, min.getY() - offset, 0.0f)));
	CreateFace(0, 1, 2);
	vertices.push_back(Vertex(Vector3(max.getX() + offset, max.getY() + offset, 0.0f)));
	CreateFace(1, 3, 2);

	faces[0].FaceIndex(2, 1);
	faces[1].FaceIndex(1, 0);
}

/*
	Passage en triangulation de Delaunay par Lawson.
*/
void Triangulation::DelaunayLawson()
{
	if (isOffFile == true)
		return;

	facesModified.clear();
	for (int i = 0; i < faces.size(); i++)
		facesModified.push_back(i);

	while (facesModified.empty() == false)
	{
		int f = facesModified[0];
		Face face = faces[f];
		facesModified.pop_front();
		for (int j = 0; j < 3; j++)
		{
			int fB = face.FaceIndex(j);
			if (fB == -1 || IsTrianglesConvex(f, fB) == false)
				continue;

			Face faceB = faces[fB];
			Vector3 v = vertices[faceB.VertexIndex(faceB.LocalFaceIndex(f))].Point();
			if (IsInCircumcircle(f, v) == true)
			{
				FlipEdge(f, fB);
				isDelaunay = false;
			}
		}
	}
	isDelaunay = true;
	if (isVoronoi == true)
		Voronoi();
}

/*
	Passage en triangulation de Delaunay quand on ajoute un nouveau point (Delaunay Incrémental).
*/
void Triangulation::DelaunayLawsonIncremental()
{
	if (isOffFile == true)
		return;

	while (facesModified.empty() == false)
	{
		int f = facesModified[0];
		Face face = faces[f];
		facesModified.pop_front();
		for (int j = 0; j < 3; j++)
		{
			int fB = face.FaceIndex(j);
			if (fB == -1 || IsTrianglesConvex(f, fB) == false)
				continue;

			Face faceB = faces[fB];
			Vector3 v = vertices[faceB.VertexIndex(faceB.LocalFaceIndex(f))].Point();
			if (IsInCircumcircle(f, v) == true)
			{
				FlipEdge(f, fB);
			}
		}
	}
	isDelaunay = true;
	if (isVoronoi == true)
		Voronoi();
}

/*
	Calcul des centres de voronois sur une triangulation de Delaunay.
*/
void Triangulation::Voronoi()
{
	if (isVoronoi == false || isOffFile == true)
		return;

	if (isDelaunay == false)
		DelaunayLawson();

	voronoisVertices.clear();
	for (int i = 0; i < faces.size(); i++)
	{
		Vector3 p0 = vertices[faces[i].VertexIndex(0)].Point();
		Vector3 p1 = vertices[faces[i].VertexIndex(1)].Point();
		Vector3 p2 = vertices[faces[i].VertexIndex(2)].Point();

		double d = 2 * (p0.getX() * (p1.getY() - p2.getY()) +
			p1.getX() * (p2.getY() - p0.getY()) +
			p2.getX() * (p0.getY() - p1.getY()));

		double p0XY = pow(p0.getX(), 2) + pow(p0.getY(), 2);
		double p1XY = pow(p1.getX(), 2) + pow(p1.getY(), 2);
		double p2XY = pow(p2.getX(), 2) + pow(p2.getY(), 2);

		double uX = 1 / d * (p0XY * (p1.getY() - p2.getY()) +
			p1XY * (p2.getY() - p0.getY()) +
			p2XY * (p0.getY() - p1.getY()));
		double uY = 1 / d * (p0XY * (p2.getX() - p1.getX()) +
			p1XY * (p0.getX() - p2.getX()) +
			p2XY * (p1.getX() - p0.getX()));

		voronoisVertices.push_back(Vector3(uX, uY, 0.0f));
	}
	isVoronoi = true;
}

/*
	Ajoute les centres de voronois à la triangulation et repasse en triangulation de Delaunay.
*/
void Triangulation::AddVoronoi()
{
	if (voronoisVertices.size() == 0 || isOffFile == true)
		return;

	for (int i = 0; i < voronoisVertices.size(); i++)
	{
		AddVertex(voronoisVertices[i]);
		vertices[vertices.size() - 1].IsVoronoi() = true;
	}
	DelaunayLawsonIncremental();
}
/* Fonctions principales */


/* Fonctions utilitaires */
/*
	Lit le fichier en paramètre et remplit le Qvector de points à traiter.
*/
void Triangulation::ReadPointsFile(QString filename)
{
	pointsList.clear();

	if (filename == NULL)
		return;

	QFile inputFile(filename);
	if (inputFile.open(QIODevice::ReadOnly))
	{
		QTextStream in(&inputFile);
		// Première ligne
		QString line = in.readLine();
		while (!line.isNull())
		{
			line = in.readLine();
			QStringList  fields = line.split(" ");
			if (fields[0] > 0 && fields[1] > 0)
				pointsList.push_back(Vertex(Vector3(fields[0].toDouble(), fields[1].toDouble(), 0.0)));

		}
	}
	inputFile.close();
	CalculateBoundingBox(pointsList);
}

/*
	Lit le fichier Off en paramètre et en remplit les Qvector de faces et vertices.
*/
void Triangulation::ReadOffFile(QString filename)
{
	isOffFile = true;
	QFile inputFile(filename);
	if (inputFile.open(QIODevice::ReadOnly))
	{
		QTextStream in(&inputFile);
		int cpt_vertices = 0, cpt_triangles = 0;

		// Première ligne
		QString line = in.readLine();
		line = in.readLine();
		QStringList  fields = line.split(" ");
		cpt_vertices = fields[0].toInt();
		cpt_triangles = fields[1].toInt();

		// Vertices
		for (int i = 0; i < cpt_vertices; i++)
		{
			QString line = in.readLine();
			QStringList  fields = line.split(" ");
			vertices.push_back(Vertex(Vector3(fields[0].toDouble(), fields[1].toDouble(), fields[2].toDouble())));
		}

		// Triangles
		for (int i = 0; i < cpt_triangles; i++)
		{
			QString line = in.readLine();
			QStringList  fields = line.split(" ");
			faces.push_back(Face(fields[1].toInt(), fields[2].toInt(), fields[3].toInt()));
		}
	}
	inputFile.close();
	CalculateBoundingBox(vertices);
}

/*
	Sauvegarde la triangulation dans un fichier Off.
*/
void Triangulation::SaveOffFile()
{
	QFile inputFile("queen.off");

	QString namefile;
	if (filename.isEmpty())
		namefile = "Files\TriangulationCustom.off";
	else
	{
		QStringList  fields = filename.split(".");
		namefile = fields[0] + ".off";
	}

	QFile file(namefile);
	if (file.open(QIODevice::ReadWrite)) {
		QTextStream stream(&file);

		stream << "OFF" << endl;
		stream << vertices.size() << " " << faces.size() << endl;

		for (int i = 0; i < vertices.size(); i++)
			stream << vertices[i].Point().getX() << " " << vertices[i].Point().getY() << " " << vertices[i].Point().getZ() << endl;


		for (int i = 0; i < faces.size(); i++)
			stream << 3 << " " << faces[i].VertexIndex(0) << " " << faces[i].VertexIndex(1) << " " << faces[i].VertexIndex(2) << endl;
	}
	file.close();
}

/*
	Clear all Qvectors and reset booleans to false.
*/
void Triangulation::Reset()
{
	pointsList.clear();
	vertices.clear();
	faces.clear();
	facesModified.clear();
	voronoisVertices.clear();
	isDelaunay = false;
	isVoronoi = false;
	isCrust = false;
	isOffFile = false;
}

/*
	Génère un cube 2D.
*/
void Triangulation::GenerateCube()
{
	// Vertices face avant
	vertices.push_back(Vertex(Vector3(0.0f, 0.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3(0.0f, 1.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3(1.0f, 1.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3(1.0f, 0.0f, 0.0f)));

	// Vertices face arrière
	vertices.push_back(Vertex(Vector3(0.0f, 0.0f, 1.0f)));
	vertices.push_back(Vertex(Vector3(0.0f, 1.0f, 1.0f)));
	vertices.push_back(Vertex(Vector3(1.0f, 1.0f, 1.0f)));
	vertices.push_back(Vertex(Vector3(1.0f, 0.0f, 1.0f)));


	// Face avant
	faces.push_back(Face(0, 1, 2));
	faces.push_back(Face(2, 3, 0));

	// Face arrière
	faces.push_back(Face(7, 6, 4));
	faces.push_back(Face(4, 5, 7));

	// Face droite
	faces.push_back(Face(0, 4, 5));
	faces.push_back(Face(5, 1, 0));

	// Face gauche
	faces.push_back(Face(3, 2, 6));
	faces.push_back(Face(6, 7, 3));

	// Face dessus
	faces.push_back(Face(1, 5, 6));
	faces.push_back(Face(6, 2, 1));

	// Face dessous
	faces.push_back(Face(0, 3, 7));
	faces.push_back(Face(7, 4, 0));
}

/*
	Calcule la bounding box de la liste de points à traiter.
*/
void Triangulation::CalculateBoundingBox(QVector<Vertex> list)
{
	Vector3 min = list[0].Point();
	Vector3 max = list[0].Point();

	for (int i = 1; i < list.size(); ++i)
	{
		if (list[i].Point().getX() < min.getX())
			min.setX(list[i].Point().getX());

		if (list[i].Point().getY() < min.getY())
			min.setY(list[i].Point().getY());

		if (list[i].Point().getZ() < min.getZ())
			min.setZ(list[i].Point().getZ());

		if (list[i].Point().getX() > max.getX())
			max.setX(list[i].Point().getX());

		if (list[i].Point().getY() > max.getY())
			max.setY(list[i].Point().getY());

		if (list[i].Point().getZ() > max.getZ())
			max.setZ(list[i].Point().getZ());
	}
	aabb.GetMinAABB() = min;
	aabb.GetMaxAABB() = max;
}

/*
	Ajoute un vertex à la triangulation, met à jours l'enveloppe convexe si le point est en dehors.
*/
void Triangulation::AddVertex(Vertex v)
{
	int i = vertices.size();
	vertices.push_back(v);
	bool found = false;
	for (int j = 0; j < faces.size(); j++)
	{
		if (IsInFace(faces[j], vertices[i].Point()) == true)
		{
			found = true;
			SplitFace(j, i);
			break;
		}
	}
	if (found == false)
	{
		AddVertexToConvexHull(i);
	}
}

/*
	Ajoute un point dans l'enveloppe convexe et la met à jour.

	A FAIRE
*/
void Triangulation::AddVertexToConvexHull(int s)
{
	// TODO
}

/*
	Crée une face avec les indices des vertices.
*/
int Triangulation::CreateFace(int iA, int iB, int iC)
{
	Vector3 a = vertices[1].Point() - vertices[0].Point();
	Vector3 b = vertices[2].Point() - vertices[1].Point();
	Vector3 p = Vector3::CrossProduct(a, b);
	if (p.getY() > 0.0f)
		faces.push_back(Face(iA, iB, iC));
	else
		faces.push_back(Face(iC, iB, iA));
	return faces.size() - 1;
}

/*
	Retourne la face dont le vertices en paramètre fait partit.
*/
int Triangulation::FindFace(Vertex v)
{
	for (int i = 0; i < faces.size(); i++)
	{
		if (IsInFace(faces[i], v.Point()) == true)
			return i;
	}
	return -1;
}

/*
	Divise une face en trois nouvelles faces à partir du vertice.
*/
void Triangulation::SplitFace(int f, int s)
{
	Face face = faces[f];

	Face faceA = Face(face.VertexIndex(1), s, face.VertexIndex(0));
	int idFaceA = f;
	faces[idFaceA] = faceA;

	Face faceB = Face(face.VertexIndex(2), s, face.VertexIndex(1));
	int idFaceB = faces.size();
	faces.push_back(faceB);

	Face faceC = Face(face.VertexIndex(0), s, face.VertexIndex(2));
	int idFaceC = faces.size();
	faces.push_back(faceC);

	faces[idFaceA].FaceIndex(0, idFaceC);
	faces[idFaceA].FaceIndex(1, face.FaceIndex(2));
	faces[idFaceA].FaceIndex(2, idFaceB);

	faces[idFaceB].FaceIndex(0, idFaceA);
	faces[idFaceB].FaceIndex(1, face.FaceIndex(0));
	faces[idFaceB].FaceIndex(2, idFaceC);

	faces[idFaceC].FaceIndex(0, idFaceB);
	faces[idFaceC].FaceIndex(1, face.FaceIndex(1));
	faces[idFaceC].FaceIndex(2, idFaceA);

	if (face.FaceIndex(0) >= 0)
	{
		int idFace0 = face.FaceIndex(0);
		faces[idFace0].FaceIndex(faces[idFace0].LocalFaceIndex(f), idFaceB);
	}

	if (face.FaceIndex(1) >= 0)
	{
		int idFace1 = face.FaceIndex(1);
		faces[idFace1].FaceIndex(faces[idFace1].LocalFaceIndex(f), idFaceC);
	}

	if (face.FaceIndex(2) >= 0)
	{
		int idFace2 = face.FaceIndex(2);
		faces[idFace2].FaceIndex(faces[idFace2].LocalFaceIndex(f), idFaceA);
	}

	facesModified.push_back(idFaceA);
	facesModified.push_back(idFaceB);
	facesModified.push_back(idFaceC);
}

/*
	Retourne l'edge entre les deux faces en paramètres.
*/
void Triangulation::FlipEdge(int fA, int fB)
{
	Face faceA = faces[fA];
	Face faceB = faces[fB];

	int localFaceA = faceA.LocalFaceIndex(fB);
	int localFaceB = faceB.LocalFaceIndex(fA);

	int idVertex0 = faceA.VertexIndex(localFaceA);
	int idVertex1 = faceB.VertexIndex(localFaceB);
	int idVertex2 = faceA.VertexIndex((localFaceA + 1) % 3);
	int idVertex3 = faceB.VertexIndex((localFaceB + 1) % 3);

	Face newFaceA(idVertex0, idVertex2, idVertex1);
	Face newFaceB(idVertex1, idVertex3, idVertex0);

	int idFace0 = faceA.FaceIndex((localFaceA + 2) % 3);
	int idFace1 = faceB.FaceIndex((localFaceB + 1) % 3);
	int idFace2 = faceA.FaceIndex((localFaceA + 1) % 3);
	int idFace3 = faceB.FaceIndex((localFaceB + 2) % 3);

	newFaceA.FaceIndex(0, idFace1);
	newFaceA.FaceIndex(1, fB);
	newFaceA.FaceIndex(2, idFace0);

	newFaceB.FaceIndex(0, idFace2);
	newFaceB.FaceIndex(1, fA);
	newFaceB.FaceIndex(2, idFace3);

	faces[fA] = newFaceA;
	faces[fB] = newFaceB;
	facesModified.push_back(fA);
	facesModified.push_back(fB);

	int faceIdA = idFace2;
	if (faceIdA >= 0)
	{
		int localIndexA = faces[faceIdA].LocalFaceIndex(fA);
		faces[faceIdA].FaceIndex(localIndexA, fB);
	}

	int faceIdB = idFace1;
	if (faceIdB >= 0)
	{
		int localIndexB = faces[faceIdB].LocalFaceIndex(fB);
		faces[faceIdB].FaceIndex(localIndexB, fA);
	}
}
/* Fonctions utilitaires */


/* Prédicats */
/*
	Permet de dire de quel côté se trouve un vertice par rapport à une droite.
*/
double Triangulation::VertexSideLine(Vector3 p1, Vector3 p2, Vector3 p)
{
	return (p.getX() - p1.getX()) * (p2.getY() - p1.getY()) - (p.getY() - p1.getY()) * (p2.getX() - p1.getX());
}

/*
	Permet de dire si un vertice est dans une face.
*/
bool Triangulation::IsInFace(Face f, Vector3 p)
{
	Vector3 p0 = vertices[f.VertexIndex(0)].Point();
	Vector3 p1 = vertices[f.VertexIndex(1)].Point();
	Vector3 p2 = vertices[f.VertexIndex(2)].Point();

	// Calculs des aires signées pour chaque arêtes du triangle et le pixel
	double area01PX = 0.5 * ((p1.getX() - p0.getX()) * (p.getY() - p0.getY()) - (p.getX() - p0.getX()) * (p1.getY() - p0.getY()));
	double area12PX = 0.5 * ((p2.getX() - p1.getX()) * (p.getY() - p1.getY()) - (p.getX() - p1.getX()) * (p2.getY() - p1.getY()));
	double area20PX = 0.5 * ((p0.getX() - p2.getX()) * (p.getY() - p2.getY()) - (p.getX() - p2.getX()) * (p0.getY() - p2.getY()));

	// Si pixel dans le triangle (aires signées positives), couleur du triangle
	if (area01PX >= 0.0 && area12PX >= 0.0 && area20PX >= 0.0)
		return true;
	else
		return false;
}

/*
	Permet de dire si un vertice est dans le cercle circonscrit à la face.
*/
bool Triangulation::IsInCircumcircle(int f, Vector3 s)
{
	Face face = faces[f];
	Vector3 p = vertices[face.VertexIndex((0))].Point();
	Vector3 q = vertices[face.VertexIndex((1))].Point();
	Vector3 r = vertices[face.VertexIndex((2))].Point();

	float m[3][3];
	m[0][0] = q.getX() - p.getX();
	m[1][0] = q.getY() - p.getY();
	m[2][0] = pow(m[0][0], 2) + pow(m[1][0], 2);

	m[0][1] = r.getX() - p.getX();
	m[1][1] = r.getY() - p.getY();
	m[2][1] = pow(m[0][1], 2) + pow(m[1][1], 2);

	m[0][2] = s.getX() - p.getX();
	m[1][2] = s.getY() - p.getY();
	m[2][2] = pow(m[0][2], 2) + pow(m[1][2], 2);

	float d = m[0][0] * ((m[1][1] * m[2][2]) - (m[2][1] * m[1][2])) -
		m[0][1] * (m[1][0] * m[2][2] - m[2][0] * m[1][2]) +
		m[0][2] * (m[1][0] * m[2][1] - m[2][0] * m[1][1]);

	if (-d >= 0.0f)
		return true;
	return false;
}

/*
	Permet de dire si deux triangles sont convexes.
*/
bool Triangulation::IsTrianglesConvex(int fA, int fB)
{
	Face faceA = faces[fA];
	Face faceB = faces[fB];

	int localFaceA = faceA.LocalFaceIndex(fB);
	int localFaceB = faceB.LocalFaceIndex(fA);

	if (localFaceA == -1 || localFaceB == -1)
		return false;

	int idVertex0 = faceA.VertexIndex(localFaceA);
	int idVertex1 = faceB.VertexIndex(localFaceB);
	int idVertex2 = faceA.VertexIndex((localFaceA + 1) % 3);
	int idVertex3 = faceB.VertexIndex((localFaceB + 1) % 3);

	Vertex sA = vertices[idVertex0];
	Vertex sB = vertices[idVertex1];
	Vertex s = vertices[idVertex2];
	if (VertexSideLine(sA.Point(), sB.Point(), s.Point()) <= 0.0f)
		return false;

	sA = vertices[idVertex2];
	sB = vertices[idVertex3];
	s = vertices[idVertex1];
	if (VertexSideLine(sA.Point(), sB.Point(), s.Point()) <= 0.0f)
		return false;

	sA = vertices[idVertex1];
	sB = vertices[idVertex0];
	s = vertices[idVertex3];
	if (VertexSideLine(sA.Point(), sB.Point(), s.Point()) <= 0.0f)
		return false;

	sA = vertices[idVertex3];
	sB = vertices[idVertex2];
	s = vertices[idVertex0];
	if (VertexSideLine(sA.Point(), sB.Point(), s.Point()) <= 0.0f)
		return false;

	return true;
}
/* Prédicats */


/* Itérateurs */
FacesIterator Triangulation::BeginFace()
{
	FacesIterator it;
	it.triangulation = this;
	if (faces.size() > 0)
	{
		it.index = 0;
		it.face = &faces[it.index];
	}
	return it;
}

FacesIterator Triangulation::EndFace()
{
	FacesIterator it;
	it.triangulation = this;
	if (faces.size() > 0)
	{
		it.index = faces.size() - 1;
		it.face = &faces[it.index];
	}
	return it;
}

Vertex Triangulation::BeginVertex()
{
	if (vertices.size() > 0)
		return vertices[0];
	return Vertex(0.0);
}

Vertex Triangulation::EndVertex()
{
	if (vertices.size() > 0)
		return vertices[vertices.size() - 1];
	return Vertex(0.0);
}

void FacesIterator::operator++()
{
	if (index + 1 < triangulation->Vertices().size())
	{
		index++;
		face = &triangulation->Faces()[index];
	}
	else
	{
		index = -1;
		face = nullptr;
	}
}

Face FacesIterator::operator*()
{
	return *face;
}

void VerticesIterator::operator++()
{
}

Vertex VerticesIterator::operator*()
{
	return *sommet;
}
/* Itérateurs */


/* Setteurs & Getteurs */
int Face::VertexIndex(int s) const
{
	if (s == 0)
		return iA;
	else if (s == 1)
		return iB;
	else if (s == 2)
		return iC;
	else
		return -1;
}

int Face::FaceIndex(int f) const
{
	if (f == 0)
		return fA;
	else if (f == 1)
		return fB;
	else if (f == 2)
		return fC;
	else
		return -1;
}

void Face::VertexIndex(int s, int i)
{
	if (s == 0)
		iA = i;
	else if (s == 1)
		iB = i;
	else
		i = iC;
}

void Face::FaceIndex(int f, int i)
{
	if (f == 0)
		fA = i;
	else if (f == 1)
		fB = i;
	else
		fC = i;
}

int Face::LocalVertexIndex(int s)
{
	if (s == iA)
		return 0;
	else if (s == iB)
		return 1;
	else if (s == iC)
		return 2;
	else
		return -1;
}

int Face::LocalFaceIndex(int f)
{
	if (f == fA)
		return 0;
	else if (f == fB)
		return 1;
	else if (f == fC)
		return 2;
	else
		return -1;
}

void Triangulation::SetGLRenderMode(GLenum m)
{
	renderMode = m;
}
/* Setteurs & Getteurs */
