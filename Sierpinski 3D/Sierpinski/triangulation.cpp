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

	//GenerateCube();
	//ReadFile();
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
			glVertex3f(vertices[faces[i].iA].p.getX(), vertices[faces[i].iA].p.getY(), vertices[faces[i].iA].p.getZ());
			glVertex3f(vertices[faces[i].iB].p.getX(), vertices[faces[i].iB].p.getY(), vertices[faces[i].iB].p.getZ());

			glVertex3f(vertices[faces[i].iB].p.getX(), vertices[faces[i].iB].p.getY(), vertices[faces[i].iB].p.getZ());
			glVertex3f(vertices[faces[i].iC].p.getX(), vertices[faces[i].iC].p.getY(), vertices[faces[i].iC].p.getZ());

			glVertex3f(vertices[faces[i].iC].p.getX(), vertices[faces[i].iC].p.getY(), vertices[faces[i].iC].p.getZ());
			glVertex3f(vertices[faces[i].iA].p.getX(), vertices[faces[i].iA].p.getY(), vertices[faces[i].iA].p.getZ());
		}
	}
	else if (renderMode == GL_TRIANGLES)
	{
		for (int i = 0; i < faces.size(); i++)
		{
			Vector3 color = colors[i % 8];
			glColor3f(color.getX(), color.getY(), color.getZ());
			glVertex3f(vertices[faces[i].iA].p.getX(), vertices[faces[i].iA].p.getY(), vertices[faces[i].iA].p.getZ());
			glVertex3f(vertices[faces[i].iB].p.getX(), vertices[faces[i].iB].p.getY(), vertices[faces[i].iB].p.getZ());
			glVertex3f(vertices[faces[i].iC].p.getX(), vertices[faces[i].iC].p.getY(), vertices[faces[i].iC].p.getZ());
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

	QMap<Edge, EdgeInfo> map;
	for (int i = 0; i < faces.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Edge edge(faces[i].GetVIncident(j), faces[i].GetVIncident((j + 1) % 3));
			EdgeInfo edgeInfo(faces[i].GetVIncident((j + 2) % 3), i);

			if (map.contains(edge) == true)
			{
				EdgeInfo edgeInfoMap = map[edge];
				faces[edgeInfoMap.triangleID].SetFAdjacent(edgeInfoMap.adjacentVertex, i);
			}
			else
				map.insert(edge, edgeInfo);
		}
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

	QMap<Edge, EdgeInfo> map;
	for (int i = 0; i < faces.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Edge edge(faces[i].GetVIncident(j), faces[i].GetVIncident((j + 1) % 3));
			EdgeInfo edgeInfo(faces[i].GetVIncident((j + 2) % 3), i);

			if (map.contains(edge) == true)
			{
				EdgeInfo edgeInfoMap = map[edge];
				faces[edgeInfoMap.triangleID].SetFAdjacent(edgeInfoMap.adjacentVertex, i);
			}
			else
				map.insert(edge, edgeInfo);
		}
	}
}

void Triangulation::TriangulationNaive()
{
	vertices.clear();
	faces.clear();

	vertices.push_back(Vertex(Vector3(0.0f, 1.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3(0.5f, -0.5f, 0.0f)));
	vertices.push_back(Vertex(Vector3(-0.5f, -0.5f, 0.0f)));

	hull.push_back(0);
	hull.push_back(1);
	hull.push_back(2);

	// Vertices inside
	vertices.push_back(Vertex(Vector3(0.0f, 0.0f, 0.0f)));

	// Vertices outside
	vertices.push_back(Vertex(Vector3(0.5f, 0.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3(-0.5f, 0.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3(0.0f, -1.0f, 0.0f)));

	CreateFace(0, 1, 2);
	for (int i = 3; i < vertices.size(); i++)
	{
		bool found = false;
		for (int j = 0; j < faces.size(); j++)
		{
			if (IsInFace(vertices[i], faces[j]) == true)
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
}

void Triangulation::CalculateBoundingBox()
{
	Vector3 min = vertices[0].p;
	Vector3 max = vertices[0].p;

	for (int i = 1; i < vertices.size(); ++i)
	{
		if (vertices[i].p.getX() < min.getX())
			min.setX(vertices[i].p.getX());

		if (vertices[i].p.getY() < min.getY())
			min.setY(vertices[i].p.getY());

		if (vertices[i].p.getZ() < min.getZ())
			min.setZ(vertices[i].p.getZ());

		if (vertices[i].p.getX() > max.getX())
			max.setX(vertices[i].p.getX());

		if (vertices[i].p.getY() > max.getY())
			max.setY(vertices[i].p.getY());

		if (vertices[i].p.getZ() > max.getZ())
			max.setZ(vertices[i].p.getZ());
	}
	aabb.SetMinAABB(min - Vector3(0.5f));
	aabb.SetMaxAABB(max + Vector3(0.5f));
}

int Triangulation::CreateFace(int iA, int iB, int iC)
{
	Vector3 a = vertices[1].p - vertices[0].p;
	Vector3 b = vertices[2].p - vertices[1].p;
	Vector3 p = a.CrossProduct(b);
	if (p.getY() > 0.0f)
		faces.push_back(Face(iA, iB, iC));
	else
		faces.push_back(Face(iC, iB, iA));
	return faces.size() - 1;
}

void Triangulation::AddVertexToConvexHull(int s)
{
	Vertex p = GetVertices()[s];

	for (int i = 0; i < hull.size(); i++)
	{
		int s1 = hull[i];
		int s2 = hull[(i + 1) % hull.size()];
		Vertex p1 = GetVertices()[s1];
		Vertex p2 = GetVertices()[s2];

		if (VertexSideLine(p1, p2, p) > 0.0f)
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
	for (int i = 0; i < faces.size(); i++)
	{
		Vertex a = vertices[faces[i].GetFAdjacent((0))];
		Vertex b = vertices[faces[i].GetFAdjacent((1))];
		Vertex c = vertices[faces[i].GetFAdjacent((2))];

		float bX = (float)(a.p.getX() + b.p.getX() + c.p.getX()) / 3.0f;
		float bY = (float)(a.p.getY() + b.p.getY() + c.p.getY()) / 3.0f;
		Vector3 barycenter = Vector3(bX, bY, 0.0f);
		float radius = (barycenter - a.GetPoint()).Magnitude();

		for (int j = 0; j < vertices.size(); j++)
		{
			if (IsInCircumcircle(barycenter, radius, vertices[j]) == false)
			{
				// FlipEdge();
			}
		}
	}
}

void Triangulation::SplitFace(int f, int s)
{
	Face face = faces[f];

	int iC = face.iC;
	face.iC = s;
	vertices[face.iA].adjacentFace = f;
	vertices[face.iB].adjacentFace = f;
	vertices[face.iC].adjacentFace = f;

	int f1 = CreateFace(face.iB, iC, s);
	vertices[faces[f1].iA].adjacentFace = f1;
	vertices[faces[f1].iB].adjacentFace = f1;
	vertices[faces[f1].iC].adjacentFace = f1;
	faces[f1].fA = f;
	face.fB = f1;

	int f2 = CreateFace(face.iA, s, iC);
	vertices[faces[f2].iA].adjacentFace = f2;
	vertices[faces[f2].iB].adjacentFace = f2;
	vertices[faces[f2].iC].adjacentFace = f2;
	faces[f2].fA = f;

	faces[f1].fC = f2;
	faces[f2].fB = f1;
	face.fC = f2;
}

void Triangulation::FlipEdge()
{

}
/* Fonctions utilitaires */

/* Prédicats */
float Triangulation::VertexSideLine(Vertex p1, Vertex p2, Vertex p)
{
	return (p1.p.getX() - p.p.getX()) * (p2.p.getY() - p.p.getY()) - (p1.p.getY() - p.p.getY()) * (p2.p.getX() - p.p.getX());
}

bool Triangulation::IsInFace(Vertex s, Face f)
{
	Vertex p1 = vertices[f.iA];
	Vertex p2 = vertices[f.iB];
	Vertex p3 = vertices[f.iC];

	double area1 = (p2.p.getY() - s.p.getY()) * (p1.p.getX() - s.p.getX()) - (p2.p.getX() - s.p.getX()) * (p1.p.getY() - s.p.getY());
	double area2 = (p3.p.getY() - s.p.getY()) * (p2.p.getX() - s.p.getX()) - (p3.p.getX() - s.p.getX()) * (p2.p.getY() - s.p.getY());
	double area3 = (p1.p.getY() - s.p.getY()) * (p3.p.getX() - s.p.getX()) - (p1.p.getX() - s.p.getX()) * (p3.p.getY() - s.p.getY());

	if (area1 >= 0 && area2 >= 0 && area3 >= 0)
		return true;
	return false;
}

bool Triangulation::IsInCircumcircle(Vector3 barycenter, float radius, Vertex p)
{
	if (pow(p.p.getX() - barycenter.getX(), 2) + pow(p.p.getY() - barycenter.getY(), 2) < pow(radius, 2))
		return true;
	return false;
}
/* Prédicats */


/* Surcharges d'opérateurs */
bool Edge::operator< (const Edge& e) const
{
	return (this->vertexA < e.vertexA && this->vertexB == e.vertexB) ||
		(this->vertexB == e.vertexA && this->vertexA == e.vertexB);
}

bool Edge::operator== (const Edge& e)
{
	if ((this->vertexA == e.vertexA && this->vertexB == e.vertexB) ||
		(this->vertexB == e.vertexA && this->vertexA == e.vertexB))
		return true;
	return false;
}
/* Surcharges d'opérateurs */

/* Itérateurs */
Iterateur_de_faces Triangulation::BeginFace()
{
	Iterateur_de_faces it;
	it.triangulation = this;
	if (faces.size() > 0)
	{
		it.index = 0;
		it.face = &faces[it.index];
	}
	return it;
}

Iterateur_de_faces Triangulation::EndFace()
{
	Iterateur_de_faces it;
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

void Iterateur_de_faces::operator++()
{
	if (index + 1 < triangulation->GetVertices().size())
	{
		index++;
		face = &triangulation->GetFaces()[index];
	}
	else
	{
		index = -1;
		face = nullptr;
	}
}

Face Iterateur_de_faces::operator*()
{
	return *face;
}

void Iterateur_de_sommets::operator++()
{
}

Vertex Iterateur_de_sommets::operator*()
{
	return *sommet;
}
/* Itérateurs */

/* Setteurs & Getteurs */
int Face::GetVIncident(int index)
{
	if (index == 0)
		return iA;
	else if (index == 1)
		return iB;
	return iC;
}

int Face::GetFAdjacent(int index)
{
	if (index == 0)
		return fA;
	else if (index == 1)
		return fB;
	return fC;
}

void Face::SetFAdjacent(int index, int vertexID)
{
	if (index == 0)
		fA = vertexID;
	else if (index == 1)
		fB = vertexID;
	else
		fC = vertexID;
}

int Face::LocalVertexIndex(int s)
{
	if (s == fA)
		return 0;
	else if (s == fB)
		return 1;
	else if (s == fC)
		return 2;
	else
		return -1;
}

void Triangulation::SetGLRenderMode(GLenum m)
{
	renderMode = m;
}
/* Setteurs & Getteurs */
