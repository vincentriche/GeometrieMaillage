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
	renderMode = GL_LINES;

	TriangulationNaive();
	CalculateBoundingBox();
}

void Triangulation::draw()
{
	glBegin(renderMode);
	if (renderMode == GL_LINES)
	{
		for (int i = 0; i < faces.size(); i++)
		{
			Vector3 color = colors[i % 8];
			glColor3f(color.getX(), color.getY(), color.getZ());
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
			Vector3 color = colors[i % 8];
			glColor3f(color.getX(), color.getY(), color.getZ());
			glVertex3f(vertices[faces[i].VertexIndex(0)].Point().getX(), vertices[faces[i].VertexIndex(0)].Point().getY(), vertices[faces[i].VertexIndex(0)].Point().getZ());
			glVertex3f(vertices[faces[i].VertexIndex(1)].Point().getX(), vertices[faces[i].VertexIndex(1)].Point().getY(), vertices[faces[i].VertexIndex(1)].Point().getZ());
			glVertex3f(vertices[faces[i].VertexIndex(2)].Point().getX(), vertices[faces[i].VertexIndex(2)].Point().getY(), vertices[faces[i].VertexIndex(2)].Point().getZ());
		}
	}
	glEnd();
}

/* Fonctions utilitaires */
void Triangulation::ReadFile()
{
	QFile inputFile("queen.off");
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
		inputFile.close();
	}
}

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

void Triangulation::TriangulationNaive()
{
	vertices.clear();
	faces.clear();

	vertices.push_back(Vertex(Vector3(3.250f, 9.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3(-4.25f, 5.75f, 0.0f)));
	vertices.push_back(Vertex(Vector3(-3.85f, -8.75, 0.0f)));

	hull.push_back(0);
	hull.push_back(1);
	hull.push_back(2);

	CreateFace(0, 1, 2);

	InsertVertex(Vector3(1.85f, -9.75, 0.0f));
	InsertVertex(Vector3(-2.45f, -5.0f, 0.0f));

}

void Triangulation::InsertVertex(Vertex v)
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

void Triangulation::CalculateBoundingBox()
{
	Vector3 min = vertices[0].Point();
	Vector3 max = vertices[0].Point();

	for (int i = 1; i < vertices.size(); ++i)
	{
		if (vertices[i].Point().getX() < min.getX())
			min.setX(vertices[i].Point().getX());

		if (vertices[i].Point().getY() < min.getY())
			min.setY(vertices[i].Point().getY());

		if (vertices[i].Point().getZ() < min.getZ())
			min.setZ(vertices[i].Point().getZ());

		if (vertices[i].Point().getX() > max.getX())
			max.setX(vertices[i].Point().getX());

		if (vertices[i].Point().getY() > max.getY())
			max.setY(vertices[i].Point().getY());

		if (vertices[i].Point().getZ() > max.getZ())
			max.setZ(vertices[i].Point().getZ());
	}
	aabb.SetMinAABB(min - Vector3(0.5f));
	aabb.SetMaxAABB(max + Vector3(0.5f));
}

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

void Triangulation::AddVertexToConvexHull(int s)
{
	Vertex p = Vertices()[s];

	for (int i = 0; i < hull.size(); i++)
	{
		int s1 = hull[i];
		int s2 = hull[(i + 1) % hull.size()];
		Vertex p1 = Vertices()[s1];
		Vertex p2 = Vertices()[s2];

		if (VertexSideLine(p1.Point(), p2.Point(), p.Point()) > 0.0f)
		{
			CreateFace(s1, s, s2);

			/*
			Circulateur_de_faces circulateur(this, hull[s1]);
			int vertexA = -1;
			int vertexB = -1;
			int c = -1;
			while (vertexA == -1 || vertexB == -1)
			{
				c = *circulateur;
			}
			*/
		}
	}
}

void Triangulation::DelaunayLawson()
{
	bool isDelaunay = false;

	while (isDelaunay == false)
	{
		isDelaunay = true;
		for (int i = 0; i < faces.size(); i++)
		{
			Face f = faces[i];
			for (int j = 0; j < 3; j++)
			{
				int fB = f.FaceIndex(j);

				if (fB == -1 || isTrianglesConvex(i, fB) == false)
					continue;

				Face faceB = faces[fB];
				int localFaceB = faceB.LocalFaceIndex(i);
				int p = faceB.VertexIndex(localFaceB);
				if (IsInCircumcircle(i, p) == true)
				{
					FlipEdge(i, fB);
					isDelaunay = false;
				}
			}
		}
	}
}

void Triangulation::SplitFace(int f, int s)
{
	Face face = faces[f];

	Face faceA = Face(face.VertexIndex(1), s, face.VertexIndex(0));
	Face faceB = Face(face.VertexIndex(2), s, face.VertexIndex(1));
	Face faceC = Face(face.VertexIndex(0), s, face.VertexIndex(2));

	int iA = f;
	faces[iA] = faceA;
	int iB = faces.size();
	faces.push_back(faceB);
	int iC = faces.size();
	faces.push_back(faceC);

	vertices[s].AdjacentFace() = iA;
	if (vertices[face.VertexIndex(1)].AdjacentFace() == iA)
		vertices[face.VertexIndex(1)].AdjacentFace() = iB;
	if (vertices[face.VertexIndex(2)].AdjacentFace() == iA)
		vertices[face.VertexIndex(2)].AdjacentFace() = iC;

	faces[iA].FaceIndex(0) = iC;
	faces[iA].FaceIndex(1) = face.FaceIndex(2);
	faces[iA].FaceIndex(2) = iB;
	if (face.FaceIndex(2) >= 0)
		faces[face.FaceIndex(2)].FaceIndex(faces[face.FaceIndex(2)].LocalFaceIndex(f)) = iA;

	faces[iB].FaceIndex(0) = iA;
	faces[iB].FaceIndex(1) = face.FaceIndex(0);
	faces[iB].FaceIndex(2) = iC;
	if (face.FaceIndex(0) >= 0)
		faces[face.FaceIndex(0)].FaceIndex(faces[face.FaceIndex(0)].LocalFaceIndex(f)) = iB;

	faces[iC].FaceIndex(0) = iB;
	faces[iC].FaceIndex(1) = face.FaceIndex(1);
	faces[iC].FaceIndex(2) = iA;
	if (face.FaceIndex(1) >= 0)
		faces[face.FaceIndex(1)].FaceIndex(faces[face.FaceIndex(1)].LocalFaceIndex(f)) = iC;
}

void Triangulation::FlipEdge(int fA, int fB)
{
	/*
	// Flip
	Face faceA = faces[fA];
	Face faceB = faces[fB];

	//Les indices locaux des sommets adjacents(voisins) aux faces
	int sA = faceA.LocalFaceIndex(fB);
	int sB = faceB.LocalFaceIndex(fA);

	//Voisins de A et B à mettre à jour
	int iFaceK = faceA.FaceIndex((sA + 2) % 3); //Voisin de A
	int iFaceL = faceB.FaceIndex((sB + 2) % 3); //Voisin de B
	Face faceK = faces[iFaceK];
	Face faceL = faces[iFaceL];

	//Les indices locaux des sommets voisins dans les triangles
	int sK = faceK.LocalFaceIndex(fA);
	int sL = faceL.LocalFaceIndex(fB);

	//Les indices globaux avec la fonction WorldVertexIndex
	Face nFaceA(faceA.VertexIndex(sA), faceA.VertexIndex((sA + 1) % 3), faceB.VertexIndex(sB));
	Face nFaceB(faceB.VertexIndex(sB), faceB.VertexIndex((sB + 1) % 3), faceA.VertexIndex(sA));

	//Mise à jour voisin des deux premières faces
	nFaceA.FaceIndex(0) = iFaceL;
	nFaceA.FaceIndex(1) = faceA.FaceIndex(1);
	nFaceA.FaceIndex(2) = faceA.FaceIndex(2);
	nFaceB.FaceIndex(0) = iFaceK;
	nFaceB.FaceIndex(1) = faceB.FaceIndex(1);
	nFaceB.FaceIndex(2) = faceB.FaceIndex(2);

	// Update Voisins faces
	faceK.FaceIndex = (sK, fB);
	faceL.FaceIndex = (sL, fA);
	faces[fA] = nFaceA;
	faces[fB] = nFaceB;
	*/
	
	Face faceA = faces[fA];
	Face faceB = faces[fB];

	int localFaceA = faceA.LocalFaceIndex(fB);
	int localFaceB = faceB.LocalFaceIndex(fA);

	Face newFaceA(faceA.VertexIndex(localFaceA), faceA.VertexIndex((localFaceA + 1) % 3), faceB.VertexIndex(localFaceB));
	Face newFaceB(faceB.VertexIndex(localFaceB), faceB.VertexIndex((localFaceB + 1) % 3), faceA.VertexIndex(localFaceA));

	if (vertices[faceA.VertexIndex((localFaceA + 1) % 3)].AdjacentFace() == fB)
		vertices[faceA.VertexIndex((localFaceA + 1) % 3)].AdjacentFace() = fA;
	if (vertices[faceB.VertexIndex((localFaceB + 1) % 3)].AdjacentFace() == fA)
		vertices[faceB.VertexIndex((localFaceB + 1) % 3)].AdjacentFace() = fB;

	newFaceA.FaceIndex(0) = faceB.FaceIndex((localFaceB + 1) % 3);
	newFaceA.FaceIndex(1) = fB;
	newFaceA.FaceIndex(2) = faceA.FaceIndex((localFaceA + 2) % 3);

	newFaceB.FaceIndex(0) = faceA.FaceIndex((localFaceA + 1) % 3);
	newFaceB.FaceIndex(1) = fA;
	newFaceB.FaceIndex(2) = faceB.FaceIndex((localFaceB + 2) % 3);

	int faceIdA = faceA.FaceIndex((localFaceA + 1) % 3);
	if (faceIdA >= 0)
	{
		int localN = faces[faceIdA].LocalFaceIndex(fA);
		faces[faceIdA].FaceIndex(localN) = fB;
	}

	int faceIdB = faceB.FaceIndex((localFaceB + 1) % 3);
	if (faceIdB >= 0)
	{
		int localN = faces[faceIdB].LocalFaceIndex(fB);
		faces[faceIdB].FaceIndex(localN) = fA;
	}

	faces[fA] = newFaceA;
	faces[fB] = newFaceB;
}
/* Fonctions utilitaires */

/* Prédicats */
double Triangulation::VertexSideLine(Vector3 p1, Vector3 p2, Vector3 p)
{
	return (p.getX() - p1.getX()) * (p2.getY() - p1.getY()) - (p.getY() - p1.getY()) * (p2.getX() - p1.getX());
}

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

bool Triangulation::IsInCircumcircle(int f, int v)
{
	Face face = faces[f];
	Vector3 s = vertices[v].Point();
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

	float d = m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) -
		m[0][1] * (m[1][0] * m[2][2] - m[2][0] * m[1][2]) +
		m[0][2] * (m[1][0] * m[2][1] - m[2][0] * m[1][1]);

	if (d < 0.0f)
		return false;
	return true;
}

bool Triangulation::isTrianglesConvex(int fA, int fB)
{
	Face faceA = faces[fA];
	Face faceB = faces[fB];

	int localFaceA = faceA.LocalFaceIndex(fB);
	int localFaceB = faceB.LocalFaceIndex(fA);

	Vertex sA = vertices[faceA.VertexIndex(localFaceA)];
	Vertex s = vertices[faceA.VertexIndex((localFaceA + 1) % 3)];
	Vertex sB = vertices[faceB.VertexIndex(localFaceB)];
	if (VertexSideLine(sA.Point(), sB.Point(), s.Point()) <= 0.0f)
		return true;

	sA = vertices[faceA.VertexIndex((localFaceA + 1) % 3)];
	s = vertices[faceB.VertexIndex(localFaceB)];
	sB = vertices[faceB.VertexIndex((localFaceB + 1) % 3)];
	if (VertexSideLine(sA.Point(), sB.Point(), s.Point()) <= 0.0f)
		return true;

	sA = vertices[faceB.VertexIndex(localFaceB)];
	s = vertices[faceB.VertexIndex((localFaceB + 1) % 3)];
	sB = vertices[faceA.VertexIndex(localFaceA)];
	if (VertexSideLine(sA.Point(), sB.Point(), s.Point()) <= 0.0f)
		return true;

	sA = vertices[faceB.VertexIndex((localFaceB + 1) % 3)];
	s = vertices[faceA.VertexIndex(localFaceA)];
	sB = vertices[faceA.VertexIndex((localFaceA + 1) % 3)];
	if (VertexSideLine(sA.Point(), sB.Point(), s.Point()) <= 0.0f)
		return true;

	return false;
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

void Triangulation::IncidentFaces(Vertex& v)
{

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

int& Face::VertexIndex(int s)
{
	if (s == 0)
		return iA;
	else if (s == 1)
		return iB;
	else if (s == 2)
		return iC;
}

int& Face::FaceIndex(int f)
{
	if (f == 0)
		return fA;
	else if (f == 1)
		return fB;
	else if (f == 2)
		return fC;
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
