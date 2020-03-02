#ifndef __SPLINE_H__
#define __SPLINE_H__
#include "arg_parser.h"
#include "matrix.h"
#include "triangle_mesh.h"
#include <GL/glut.h>

#define pi 3.1415926

class Spline
{
private:
	
public:
	Spline();
	Spline(int);
	~Spline();
	Spline(Spline*);
	// ���ڻ�ͼ��FOR VISUALIZATION
	virtual void Paint(ArgParser *args)=0;

	// ����ʵ����������ת����FOR CONVERTING BETWEEN SPLINE TYPES
	virtual void OutputBezier(FILE *file)=0;
	virtual void OutputBSpline(FILE *file) = 0;

	// ���ڵõ����Ƶ��FOR CONTROL POINT PICKING
	virtual int getNumVertices() = 0;
	virtual Vec3f getVertex(int i) = 0;

	// ���ڱ༭������FOR EDITING OPERATIONS
	virtual void moveControlPoint(int selectedPoint, float x, float y) = 0;
	virtual void addControlPoint(int selectedPoint, float x, float y) = 0;
	virtual void deleteControlPoint(int selectedPoint) = 0;

	// ���ڲ��������ε�FOR GENERATING TRIANGLES
	virtual TriangleMesh* OutputTriangles(ArgParser *args) = 0;

	// �������ô洢��ĺ���
	virtual void set(int, Vec3f&) = 0;

	// ����4�����Ƶ��Ӧ�Ŀ��Ƶ���󣬻��ƶ�Ӧ��Bezier����
	void drawBezier(Matrix m,int tessellation);
	void drawBezierWithDeCasteljau(Matrix m, int tessellation);
	//void recursive(Vec3f *&fs, int i);
	// ����4�����Ƶ��Ӧ�Ŀ��Ƶ���󣬻��ƶ�Ӧ��B��������
	void drawBSpline(Matrix m, int tessellation);
	// Bezier��������
	Matrix *BEZIER_SPLINE_BASIS;
	// BSpline��������
	Matrix *BSPLINE_BASIS;
};

#endif