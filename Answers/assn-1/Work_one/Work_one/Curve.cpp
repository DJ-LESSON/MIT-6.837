#include "Curve.h"



Curve::Curve()
{
}
// �����ƿ��Ƶ�Ϳ��Ƶ����ߣ����߻���������ʵ��
void Curve::Paint(ArgParser *args) {
	glColor4f(1, 0, 0, 1);
	glPointSize(5);
	glBegin(GL_POINTS);
	glVertex3d(vertices[0][0], vertices[0][1], vertices[0][2]);
	glEnd();
	for (int i=1;i<num_vertices;i++)
	{
		glColor4f(0, 0, 1, 0);
		glBegin(GL_LINES);
		glVertex3d(vertices[i-1][0], vertices[i-1][1], vertices[i-1][2]);
		glVertex3d(vertices[i][0], vertices[i][1], vertices[i][2]);
		glEnd();

		glColor4f(1, 0, 0, 1);
		glPointSize(5);
		glBegin(GL_POINTS);
		glVertex3d(vertices[i][0], vertices[i][1], vertices[i][2]);
		glEnd();
	}
	glPointSize(1);
}
Curve::Curve(int num_vervices)
{
	this->num_vertices = num_vervices;
	vertices = new Vec3f[num_vervices*2];
	memory_length = num_vertices * 2;
}
// �ƶ����Ƶ�
void Curve::moveControlPoint(int selectedPoint, float x, float y)
{
	vertices[selectedPoint].Set(x, y, vertices[selectedPoint].z());
}

// ���һ�����Ƶ�
void Curve::addControlPoint(int selectedPoint, float x, float y)
{
	// ����洢���Ƶ�Ŀռ䲻�㣬����������
	if (num_vertices >= memory_length) {
		Vec3f * temp = vertices;
		vertices = new Vec3f[num_vertices*2];
		for (int i=0;i<num_vertices;i++)
		{
			vertices[i] = temp[i];
		}
		memory_length = memory_length * 2;
		delete []temp;
	}
	for (int i = num_vertices - 1; i >= selectedPoint; i--)
		vertices[i + 1] = vertices[i];
	num_vertices++;
	
	vertices[selectedPoint] = Vec3f(x, y, 0);
}
// TODO
// ɾ��һ�����Ƶ㣬����ֻ��Ӧ��B�����Ŀ��Ƶ�ɾ��
void Curve::deleteControlPoint(int selectedPoint)
{
	if (num_vertices <= 4) return;
	for (int i = selectedPoint; i < num_vertices-1; i++) {
		vertices[i] = vertices[i + 1];
	}
	num_vertices--;
}

void Curve::set(int i, Vec3f& v) {
	assert(i < num_vertices);
	vertices[i] = v;
}
Curve::~Curve()
{
	delete[] vertices;
}

BezierCurve::BezierCurve()
{
}

BezierCurve::BezierCurve(int n):Curve(n)
{
}

BezierCurve::~BezierCurve()
{
}

// ����Bezier����
void BezierCurve::Paint(ArgParser *args) {
	// ���ƿ��Ƶ�
	Curve::Paint(args);

	Matrix m;
	for (int i = 0; i <= num_vertices - 4; i += 3) {
		for (int j = i; j < i + 4; j++) {
			m.Set(j - i, 0, vertices[j][0]);
			m.Set(j - i, 1, vertices[j][1]);
			m.Set(j - i, 2, vertices[j][2]);
		}
		//drawBezier(m, args->curve_tessellation);
		drawBezierWithDeCasteljau(m, args->curve_tessellation);
	}
}
void BezierCurve::OutputBezier(FILE * file)
{
	fprintf(file, "\nbezier\n");
	fprintf(file, "num_vertices %d\n", num_vertices);
	for (int i = 0; i < num_vertices; i++) {
		fprintf(file, "%f %f %f\n", vertices[i][0], vertices[i][1], vertices[i][2]);
	}
}
//void BezierCurve::OutputBSpline(FILE * file)
//{
//	fprintf(file, "\nbspline\n");
//	fprintf(file, "num_vertices %d\n", (num_vertices - 1) / 3 *5+3);
//	Matrix m;
//	for (int i = 0; i <= num_vertices - 4; i+=3) {
//		for (int j = i; j < i + 4; j++) {
//			m.Set(j - i, 0, vertices[j][0]);
//			m.Set(j - i, 1, vertices[j][1]);
//			m.Set(j - i, 2, vertices[j][2]);
//		}
//		m = m * (*BEZIER_SPLINE_BASIS);
//		Matrix temp = *BSPLINE_BASIS;
//		temp.Inverse();
//		m = m * (temp);
//		if (i == 0) {
//			fprintf(file, "%f %f %f\n", m.Get(0, 0), m.Get(0, 1), m.Get(0, 2));
//			fprintf(file, "%f %f %f\n", m.Get(0, 0), m.Get(0, 1), m.Get(0, 2));
//			fprintf(file, "%f %f %f\n", m.Get(0, 0), m.Get(0, 1), m.Get(0, 2));
//		}
//		fprintf(file, "%f %f %f\n", m.Get(1, 0), m.Get(1, 1), m.Get(1, 2));
//		fprintf(file, "%f %f %f\n", m.Get(2, 0), m.Get(2, 1), m.Get(2, 2));
//
//		fprintf(file, "%f %f %f\n", m.Get(3, 0), m.Get(3, 1), m.Get(3, 2));
//		fprintf(file, "%f %f %f\n", m.Get(3, 0), m.Get(3, 1), m.Get(3, 2));
//		fprintf(file, "%f %f %f\n", m.Get(3, 0), m.Get(3, 1), m.Get(3, 2));
//	}
//}
void BezierCurve::OutputBSpline(FILE * file)
{
	fprintf(file, "\nbspline\n");
	fprintf(file, "num_vertices %d\n", (num_vertices - 1) / 3 +3);
	Matrix m;
	for (int i = 0; i <= num_vertices - 4; i += 3) {
		for (int j = i; j < i + 4; j++) {
			m.Set(j - i, 0, vertices[j][0]);
			m.Set(j - i, 1, vertices[j][1]);
			m.Set(j - i, 2, vertices[j][2]);
		}
		m = m * (*BEZIER_SPLINE_BASIS);
		Matrix temp = *BSPLINE_BASIS;
		temp.Inverse();
		m = m * (temp);
		fprintf(file, "%f %f %f\n", m.Get(0, 0), m.Get(0, 1), m.Get(0, 2));
		fprintf(file, "%f %f %f\n", m.Get(1, 0), m.Get(1, 1), m.Get(1, 2));
		fprintf(file, "%f %f %f\n", m.Get(2, 0), m.Get(2, 1), m.Get(2, 2));
		fprintf(file, "%f %f %f\n", m.Get(3, 0), m.Get(3, 1), m.Get(3, 2));
	}
}
// Bezier������ӿ��Ƶ㣬��ͬһλ��ͬʱ����������Ƶ㣬��Ӻ�֤����������״����
void BezierCurve::addControlPoint(int selectedPoint, float x, float y)
{
	Curve::addControlPoint(selectedPoint, x, y);
	Curve::addControlPoint(selectedPoint, x, y);
	Curve::addControlPoint(selectedPoint, x, y);
}
// TODO Bezier����ɾ�����Ƶ�
void BezierCurve::deleteControlPoint(int selectedPoint)
{
}
TriangleMesh * BezierCurve::OutputTriangles(ArgParser * args)
{
	TriangleNet* net=NULL;
	float theta = (float)2*pi / args->revolution_tessellation;
	for (int i = 0; i < args->revolution_tessellation; i++) {
		// �������������磬���񲼾ֺ���ֱ���Ϊ1������ֱ���Ϊ С��Bezier��������*ÿС�����ߵ�����߶�����
		TriangleNet* net2 = new TriangleNet(1, (args->curve_tessellation)*(num_vertices - 1) / 3);
		Matrix m;
		for (int i2 = 0; i2 <= num_vertices - 4; i2 += 3) {
			// ���ɿ��Ƶ����
			for (int j = i2; j < i2 + 4; j++) {
				m.Set(j - i2, 0, vertices[j][0]);
				m.Set(j - i2, 1, vertices[j][1]);
				m.Set(j - i2, 2, vertices[j][2]);
			}
			// ���Ƶ���������������˵ñ任����
			m = m * (*BEZIER_SPLINE_BASIS);
			Matrix T, temp;
			float t = 0;
			Vec3f v;
			for (int i3 = 0; i3 <= args->curve_tessellation; i3++)
			{
				// ����T����
				t = (float)i3 / args->curve_tessellation;
				T.Set(0, 0, t*t*t);
				T.Set(0, 1, t*t);
				T.Set(0, 2, t);
				T.Set(0, 3, 1);
				// �任������T�������
				temp = m * T;
				v.Set(temp.Get(0, 0), temp.Get(0, 1), temp.Get(0, 2));
				// ���ԭ�����ϵ���Y����תtheta*i�Ƕȵĵ�
				Matrix::MakeYRotation(theta*i).Transform(v);
				net2->SetVertex(0, (i2 / 3)*(args->curve_tessellation) + i3, v);
				// ���ԭ�����ϵ���Y����תtheta*(i+1)�Ƕȵĵ�
				Matrix::MakeYRotation(theta).Transform(v);
				net2->SetVertex(1, (i2 / 3)*(args->curve_tessellation) + i3, v);
			}
		}
		if (net == NULL) net = net2;
		else {
			net->Merge(*net2);
			delete net2;
		}
	}
	return net;
}
BSplineCurve::BSplineCurve()
{
}

BSplineCurve::BSplineCurve(int n):Curve(n)
{
}

BSplineCurve::~BSplineCurve()
{
}
void BSplineCurve::Paint(ArgParser *args) {
	Curve::Paint(args);
	Matrix m;
	for (int i = 0; i <= num_vertices - 4; i ++) {
		for (int j = i; j < i + 4; j++) {
			m.Set(j - i, 0, vertices[j][0]);
			m.Set(j - i, 1, vertices[j][1]);
			m.Set(j - i, 2, vertices[j][2]);
		}
		glEnd();
		drawBSpline(m, args->curve_tessellation);
	}
}

void BSplineCurve::OutputBezier(FILE * file)
{
	fprintf(file, "\nbezier\n");
	fprintf(file, "num_vertices %d\n", (num_vertices -3)*3+1);
	Matrix m;
	for (int i = 0; i <= num_vertices - 4; i++) {
		for (int j = i; j < i + 4; j++) {
			m.Set(j - i, 0, vertices[j][0]);
			m.Set(j - i, 1, vertices[j][1]);
			m.Set(j - i, 2, vertices[j][2]);
		}
		// Q2 = Q1*S1*(s2)^(-1)
		m = m * (*BSPLINE_BASIS);
		Matrix temp = *BEZIER_SPLINE_BASIS;
		temp.Inverse();
		m = m * (temp);
		if (i == 0) {
			fprintf(file, "%f %f %f\n", m.Get(0, 0), m.Get(0, 1), m.Get(0, 2));
		}
		fprintf(file, "%f %f %f\n", m.Get(1, 0), m.Get(1, 1), m.Get(1, 2));
		fprintf(file, "%f %f %f\n", m.Get(2, 0), m.Get(2, 1), m.Get(2, 2));
		fprintf(file, "%f %f %f\n", m.Get(3, 0), m.Get(3, 1), m.Get(3, 2));
	}
}

void BSplineCurve::OutputBSpline(FILE * file)
{
	fprintf(file, "\nbspline\n");
	fprintf(file, "num_vertices %d\n", num_vertices);
	for (int i = 0; i < num_vertices; i++) {
		fprintf(file, "%f %f %f\n", vertices[i][0], vertices[i][1], vertices[i][2]);
	}
}

void BSplineCurve::deleteControlPoint(int selectedPoint)
{
	Curve::deleteControlPoint(selectedPoint);
}

TriangleMesh * BSplineCurve::OutputTriangles(ArgParser * args)
{
	TriangleNet* net = NULL;
	float theta = (float)2 * pi / args->revolution_tessellation;
	for (int i = 0; i < args->revolution_tessellation; i++) {
		// �������������磬���񲼾ֺ���ֱ���Ϊ1������ֱ���Ϊ С��BSpline��������*ÿС�����ߵ�����߶�����
		TriangleNet* net2 = new TriangleNet(1, (args->curve_tessellation)*(num_vertices - 3));
		Matrix m;
		for (int i2 = 0; i2 <= num_vertices - 4; i2 ++) {
			for (int j = i2; j < i2 + 4; j++) {
				m.Set(j - i2, 0, vertices[j][0]);
				m.Set(j - i2, 1, vertices[j][1]);
				m.Set(j - i2, 2, vertices[j][2]);
			}
			m = m * (*BSPLINE_BASIS);
			Matrix T, temp;
			float t = 0;
			Vec3f v;
			for (int i3 = 0; i3 <= args->curve_tessellation; i3++)
			{
				t = (float)i3 / args->curve_tessellation;
				T.Set(0, 0, t*t*t);
				T.Set(0, 1, t*t);
				T.Set(0, 2, t);
				T.Set(0, 3, 1);
				temp = m * T;
				v.Set(temp.Get(0, 0), temp.Get(0, 1), temp.Get(0, 2));
				Matrix::MakeYRotation(theta*i).Transform(v);
				net2->SetVertex(0, i2*(args->curve_tessellation) + i3, v);
				Matrix::MakeYRotation(theta).Transform(v);
				net2->SetVertex(1, i2*(args->curve_tessellation) + i3, v);
			}
		}
		if (net == NULL) net = net2;
		else net->Merge(*net2);
	}
	return net;
}
