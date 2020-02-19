#pragma once
#include "Object.h"
#include <vector>
#include <string>
#include <map>
#include "Bbox.h"
#include "Triangle.h"


class TriangleIndices {
public:
	TriangleIndices(int vtxi = -1, int vtxj = -1, int vtxk = -1, int ni = -1, int nj = -1, int nk = -1, int uvi = -1, int uvj = -1, int uvk = -1)
	{
		vtx[0] = vtxi; vtx[1] = vtxj; vtx[2] = vtxk;
		uv[0] = uvi; vtx[1] = uvj; uv[2] = uvk;
		n[0] = ni; n[1] = nj; n[2] = nk;
	};
	int vtx[3];
	int uv[3];
	int n[3];
	int faceGroup;
};


struct Node {
	Bbox bbox =Bbox();
	int i_first = 0;
	int nb_triangle = 0;
	Node* right_child = NULL;
	Node* left_child = NULL;
};


class Mesh : public Object
{
public:
	Mesh(const char* obj, double scaling, const Vector3& offset, const Material& mat);
	~Mesh();

	void GetIntersection(const Ray&, IntersectionObject& intersection) const;

	void add_texture(const char* filename);
	void readOBJ(const char* obj);

	std::vector<TriangleIndices> indices;
	std::vector<Vector3> vertices;
	std::vector<Vector3> normals;
	std::vector<Vector3> uvs; // Vector en 3D mais on n'utilise que 2 composantes
	std::vector<Vector3> vertexcolors;

	std::vector<std::vector<unsigned char> > textures;
	std::vector<int> w, h;

private:
	Node* p_node;
	void _ComputeNodes(Node* p_node);
	Bbox _ComputeBbox(int i_first, int nb_triagle);
	void _GetIntersectionRec(const Ray&, IntersectionObject& intersection, Node* p_node) const;
};

