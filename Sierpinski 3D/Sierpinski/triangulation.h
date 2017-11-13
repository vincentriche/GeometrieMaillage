#ifndef MESH_H
#define MESH_H

#include <Windows.h>
#include <iostream>
#include <time.h>
#include <GL/gl.h>
#include <QVector>
#include <QVector3D>

#include "aabb.h"

class FacesIterator;
class VerticesIterator;

class Vertex
{
private:
	Vector3 p;
	int adjacentFace;
	bool isVoronoi;

public:
	Vertex() : p(0.0) { isVoronoi = false; }
	Vertex(Vector3 p) : p(p) { isVoronoi = false; }
	Vertex(Vector3 p, int adjacentFace) : p(p), adjacentFace(adjacentFace) { isVoronoi == false; }

	Vector3& Point() { return p; }
	int& AdjacentFace() { return adjacentFace; }
	bool& IsVoronoi() { return isVoronoi; }
};

struct Edge
{
private:
	int vertexA;
	int vertexB;

public:
	Edge() {}
	Edge(int a, int b) : vertexA(a), vertexB(b) {}

	bool operator< (const Edge& e) const
	{
		return (this->vertexA < e.vertexA && this->vertexB == e.vertexB) ||
			(this->vertexB == e.vertexA && this->vertexA == e.vertexB);
	}

	bool operator == (const Edge& e)
	{
		if ((this->vertexA == e.vertexA && this->vertexB == e.vertexB) ||
			(this->vertexB == e.vertexA && this->vertexA == e.vertexB))
			return true;
		return false;
	}
};

class Face
{
private:
	int iA, iB, iC;
	int fA, fB, fC;

public:
	Face()
		: iA(-1), iB(-1), iC(-1), fA(-1), fB(-1), fC(-1) {}
	Face(int iA, int iB, int iC)
		: iA(iA), iB(iB), iC(iC), fA(-1), fB(-1), fC(-1) {}

	int VertexIndex(int s) const;
	int FaceIndex(int f) const;
	void VertexIndex(int s, int i);
	void FaceIndex(int f, int i);
	int LocalVertexIndex(int s);
	int LocalFaceIndex(int f);

	bool Face::operator ==(const Face &b) const
	{
		return (iA == b.iA && iB == b.iB && iC == b.iC && fA == b.fA && fB == b.fB && fC == b.fC);
	}
};

class Triangulation
{
private:
	QVector<Vertex> pointsList;
	QVector<Vertex> vertices;
	QVector<Face> faces;
	QVector<int> facesModified;
	QVector<Vertex> voronoisVertices;
	AABB aabb;
	GLenum renderMode;
	Color color;
	
public:
	bool isDelaunay = false;
	bool isVoronoi = false;
	bool isCrust = false;

	Triangulation();
	void draw();

	/* Fonctions principales */
	void TriangulationNaive();
	void DelaunayLawson();
	void DelaunayLawsonIncremental();
	void Voronoi();
	void AddVoronoi();

	/* Fonctions utilitaires */
	void GeneratePoints();
	void ReadPointsFile(const char* filename);
	void ReadOffFile();
	void GenerateCube();
	void AddVertex(Vertex v);
	void CalculateBoundingBox();
	int CreateFace(int iA, int iB, int iC);
	int FindFace(Vertex v);
	void AddVertexToConvexHull(int s);
	void SplitFace(int f, int s);
	void FlipEdge(int f, int s);

	/* Prédicats */
	double VertexSideLine(Vector3 p1, Vector3 p2, Vector3 p);
	bool IsInFace(Face f, Vector3 p);
	bool IsInCircumcircle(int f, Vector3 s);
	bool isTrianglesConvex(int fA, int fB);


	/* Surcharges d'opérateurs */
	FacesIterator BeginFace();
	FacesIterator EndFace();
	Vertex BeginVertex();
	Vertex EndVertex();

	/* Getteurs / Setteurs */
	void SetGLRenderMode(GLenum m);
	void ToggleVoronoi() { isVoronoi = !isVoronoi; }
	void ToggleCrust() { isCrust = !isCrust; }
	GLenum GetGLRenderMode() { return renderMode; }
	QVector<Vertex> Vertices() { return vertices; }
	QVector<Face> Faces() { return faces; }
	AABB AABB() { return aabb; }
};

/* Itérateurs */
class FacesIterator
{
public:
	Triangulation* triangulation;
	Face* face;
	int index;

	FacesIterator() {}
	FacesIterator(Triangulation& t) : triangulation(&t) {}
	FacesIterator(Triangulation& t, int n) : triangulation(&t), index(n) {}
	void operator++();
	Face operator*();
};

class VerticesIterator
{
private:
	Triangulation* triangulation;
	Vertex* sommet;
	int index;
public:
	VerticesIterator() {}
	VerticesIterator(Triangulation& t) : triangulation(&t) {}
	VerticesIterator(Triangulation& t, int n) : triangulation(&t), index(n) {}
	void operator++();
	Vertex operator*();
};

class FacesCirculator
{
private:
	Triangulation* triangulation;
	int currentVertex;
	int currentFace;
public:
	FacesCirculator(Triangulation* t) : triangulation(t) {}
	FacesCirculator(Triangulation* t, int s) : triangulation(t), currentVertex(s)
	{
		currentFace = triangulation->Vertices()[currentVertex].AdjacentFace();
	}
	const int operator*() { return currentFace; }
	const FacesCirculator& operator++()
	{
		Face& face = triangulation->Faces()[currentFace];
		int localVertex = face.LocalVertexIndex(currentVertex);
		currentFace = face.FaceIndex((localVertex + 1) % 3);
		return *this;
	}
	const FacesCirculator& operator--()
	{
		Face& face = triangulation->Faces()[currentFace];
		int localVertex = face.LocalVertexIndex(currentVertex);
		int index = localVertex == 0 ? 2 : localVertex - 1;
		currentFace = face.FaceIndex(index);
		return *this;
	}
};
#endif // MESH_H
