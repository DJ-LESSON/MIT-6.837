#ifndef __SURFACE_H__
#define __SURFACE_H__

#include "Curve.h"
class Surface :
	public Spline
{
private:
	
public:
	Surface();
	Surface(int);
	~Surface();

	// ���ڻ�ͼ��FOR VISUALIZATION
	void Paint(ArgParser *args) {}

	// ����ʵ����������ת����FOR CONVERTING BETWEEN SPLINE TYPES
	void OutputBezier(FILE *file) {}
	void OutputBSpline(FILE *file) {}

	// ���ڵõ����Ƶ��FOR CONTROL POINT PICKING
	int getNumVertices() { return 0; }
	Vec3f getVertex(int i) { return Vec3f(); }

	// ���ڱ༭������FOR EDITING OPERATIONS
	void moveControlPoint(int selectedPoint, float x, float y) {}
	void addControlPoint(int selectedPoint, float x, float y) {}
	void deleteControlPoint(int selectedPoint) {}

	// ���ڲ��������ε�FOR GENERATING TRIANGLES
	TriangleMesh* OutputTriangles(ArgParser *args) { return NULL; }

	void set(int, Vec3f&) {}
};

class SurfaceOfRevolution :public Surface {
private:
	Curve* curve;
public:
	SurfaceOfRevolution();
	SurfaceOfRevolution(int);
	~SurfaceOfRevolution();
	SurfaceOfRevolution(Curve* s);
	int getNumVertices() { return curve->num_vertices; }
	Vec3f getVertex(int i) { return curve->vertices[i]; }
	void Paint(ArgParser *args);
	void OutputBezier(FILE *file) {}
	void OutputBSpline(FILE *file);
	TriangleMesh* OutputTriangles(ArgParser *args);
	void moveControlPoint(int selectedPoint, float x, float y) { curve->moveControlPoint(selectedPoint, x, y); }
	void addControlPoint(int selectedPoint, float x, float y) { curve->addControlPoint(selectedPoint, x, y); }
	void deleteControlPoint(int selectedPoint) { curve->deleteControlPoint(selectedPoint); }
};

class BezierPatch :public Surface {
private:
	Vec3f vertices[16];
public:
	BezierPatch();
	BezierPatch(int);
	~BezierPatch();
	void set(int, Vec3f&);
	void get(int u, int v, Vec3f& vertex);
	void Paint(ArgParser *args);
	TriangleMesh* OutputTriangles(ArgParser *args);
};

#endif