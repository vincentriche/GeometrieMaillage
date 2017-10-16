#ifndef MESH_H
#define MESH_H

#include <Windows.h>
#include <iostream>
#include <time.h>
#include <GL/gl.h>
#include <QVector>
#include <QVector3D>

#include "aabb.h"

class Iterateur_de_faces;
class Iterateur_de_sommets;

class Vertex
{
public:
	Vector3 p;
	int adjacentFace;
	Vertex() : p(0.0) {}
	Vertex(Vector3 p) : p(p) {}
	Vertex(Vector3 p, int adjacentFace) : p(p), adjacentFace(adjacentFace) {}

	Vector3 GetPoint() { return p; }
	int GetAdjacentFace() { return adjacentFace; }
};

struct Edge
{
	int vertexA;
	int vertexB;
	Edge() {}
	Edge(int a, int b) : vertexA(a), vertexB(b) {}

	bool operator< (const Edge& e) const;

	bool operator == (const Edge&);
};

struct EdgeInfo
{
	int adjacentVertex;
	int triangleID;
	EdgeInfo() {}
	EdgeInfo(int a, int b) : adjacentVertex(a), triangleID(b) {}
};

class Face
{
public:
	int iA, iB, iC; // sommets incidents
	int fA, fB, fC; // faces adjacentes
	int adjacentVertex;
	Face()
		: iA(-1), iB(-1), iC(-1), fA(-1), fB(-1), fC(-1) {}
	Face(int iA, int iB, int iC)
		: iA(iA), iB(iB), iC(iC), fA(-1), fB(-1), fC(-1) {}

	int GetVIncident(int index);
	int GetFAdjacent(int index);
	void SetFAdjacent(int index, int vertexID);
	int WorldVertexIndex(int s);
	int WorldFaceIndex(int f);
	int LocalVertexIndex(int s);
	int LocalFaceIndex(int f);
};

class Triangulation
{
private:
	QVector<Vertex> vertices;
	QVector<Face> faces;
	QVector<int> hull;
	GLenum renderMode;
	AABB aabb;

public:
	Triangulation();
	void draw();
	void ReadFile();
	void GenerateCube();
	void TriangulationNaive();
	void CalculateBoundingBox();
	int CreateFace(int iA, int iB, int iC);
	void AddVertexToConvexHull(int s);
	void DelaunayLawson();
	void SplitFace(int f, int s);
	void FlipEdge(int f, int s);
	/* Fonctions utilitaires */

	/* Prédicats */
	bool IsInFace(Vertex s, Face f);
	bool IsInCircumcircle(int f, int s);
	float VertexSideLine(Vertex p1, Vertex p2, Vertex p); 
	int FindFace(int s);


	/* Surcharges d'opérateurs */
	Iterateur_de_faces BeginFace();
	Iterateur_de_faces EndFace();
	Vertex BeginVertex();
	Vertex EndVertex();
	void IncidentFaces(Vertex& v);

	/* Getteurs / Setteurs */
	void SetGLRenderMode(GLenum m);
	GLenum GetGLRenderMode() { return renderMode; }
	QVector<Vertex> GetVertices() { return vertices; }
	QVector<Face> GetFaces() { return faces; }
	AABB GetAABB() { return aabb; }
};

/* Itérateurs */
class Iterateur_de_faces
{
public:
	Triangulation* triangulation;
	Face* face;
	int index;

	Iterateur_de_faces() {}
	Iterateur_de_faces(Triangulation& t) : triangulation(&t) {}
	Iterateur_de_faces(Triangulation& t, int n) : triangulation(&t), index(n) {}
	void operator++();
	Face operator*();
};

class Iterateur_de_sommets
{
private:
	Triangulation* triangulation;
	Vertex* sommet;
	int index;
public:
	Iterateur_de_sommets() {}
	Iterateur_de_sommets(Triangulation& t) : triangulation(&t) {}
	Iterateur_de_sommets(Triangulation& t, int n) : triangulation(&t), index(n) {}
	void operator++();
	Vertex operator*();
};

class Circulateur_de_faces
{
private:
	Triangulation* triangulation;
	int currentVertex;
	int currentFace;
public:
	Circulateur_de_faces(Triangulation* t) : triangulation(t) {}
	Circulateur_de_faces(Triangulation* t, int s) : triangulation(t), currentVertex(s)
	{
		currentFace = triangulation->GetVertices()[currentVertex].GetAdjacentFace();
	}
	const int operator*() { return currentFace; }
	const Circulateur_de_faces& operator++()
	{
		Face& face = triangulation->GetFaces()[currentFace];
		int localVertex = face.LocalVertexIndex(currentVertex);
		currentFace = face.GetFAdjacent((localVertex + 1) % 3);
		return *this;
	}
	const Circulateur_de_faces& operator--()
	{
		Face& face = triangulation->GetFaces()[currentFace];
		int localVertex = face.LocalVertexIndex(currentVertex);
		int index = localVertex == 0 ? 2 : localVertex - 1;
		currentFace = face.GetFAdjacent(index);
		return *this;
	}
};
#endif // MESH_H
