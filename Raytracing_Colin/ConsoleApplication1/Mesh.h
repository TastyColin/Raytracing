#pragma once
#include "Object.h"
#include <vector>
#include <string>
#include <map>
#include "Bbox.h"
#include "Triangle.h"


class TriangleIndices {
public:
	TriangleIndices(int vtxi = -1, int vtxj = -1, int vtxk = -1, int ni = -1, int nj = -1, int nk = -1, int uvi = -1, int uvj = -1, int uvk = -1) : vtxi(vtxi), vtxj(vtxj), vtxk(vtxk), uvi(uvi), uvj(uvj), uvk(uvk), ni(ni), nj(nj), nk(nk) {
	};
	int vtxi, vtxj, vtxk;
	int uvi, uvj, uvk;
	int ni, nj, nk;
	int faceGroup;
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
	Bbox bbox;
};

