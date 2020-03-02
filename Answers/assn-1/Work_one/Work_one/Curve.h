#ifndef __CURVE_H__
#define __CURVE_H__

#include "Spline.h"
#define CURVE_BEZIER true
#define CURVE_BSPLINE false
class Curve :
	public Spline
{
private:
public:
	// ������Ƶ�
	Vec3f* vertices;
	// ������Ƶ�����
	int num_vertices = 0;
	// �洢���Ƶ������ռ䣬memory_length >= num_vertices
	int memory_length = 0;
	Curve();
	Curve(int);
	~Curve();
	Curve(Spline& s) :Spline(s) {}

	// ���ڻ�ͼ��FOR VISUALIZATION���������߶�Ӧ�Ŀ��Ƶ㲢�����Ƶ����ߣ��������߻�����������ʵ��
	void Paint(ArgParser *args);

	// ����ʵ����������ת����FOR CONVERTING BETWEEN SPLINE TYPES
	void OutputBezier(FILE *file) {}
	void OutputBSpline(FILE *file) {}

	// ���ڵõ����Ƶ��FOR CONTROL POINT PICKING
	int getNumVertices() { return num_vertices; }
	Vec3f getVertex(int i) { return vertices[i]; }

	// ���ڱ༭������FOR EDITING OPERATIONS
	void moveControlPoint(int selectedPoint, float x, float y);
	void addControlPoint(int selectedPoint, float x, float y);
	void deleteControlPoint(int selectedPoint);

	// ���ڲ��������ε�FOR GENERATING TRIANGLES����������ʵ�֡�
	TriangleMesh* OutputTriangles(ArgParser *args) { return NULL; }

	void set(int, Vec3f&);
};


class BezierCurve :public Curve{
private:
	BezierCurve();
public:
	BezierCurve(int n);
	~BezierCurve();
	void Paint(ArgParser * args);
	void OutputBezier(FILE *file);
	void OutputBSpline(FILE *file);
	void addControlPoint(int selectedPoint, float x, float y);
	void deleteControlPoint(int selectedPoint);
	TriangleMesh* OutputTriangles(ArgParser *args);
	BezierCurve(Spline& s) :Curve(s) {}
};

class BSplineCurve :public Curve {
private:
	BSplineCurve();
public:
	BSplineCurve(int n);
	~BSplineCurve();
	void Paint(ArgParser * args);
	void OutputBezier(FILE *file);
	void OutputBSpline(FILE *file);
	void deleteControlPoint(int selectedPoint);
	TriangleMesh* OutputTriangles(ArgParser *args);
	BSplineCurve(Spline& s) :Curve(s) {}
};

#endif