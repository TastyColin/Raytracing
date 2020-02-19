#include "pch.h"

#include "Mesh.h"



Mesh::Mesh(const char* obj, double scaling, const Vector3& offset, const Material& mat) :
	Object(mat)
{
	readOBJ(obj);
	for (int i = 0; i < vertices.size(); i++) {
		vertices[i] = scaling * vertices[i] + offset;
	}
	Bbox bbox = _ComputeBbox(0, indices.size());
	p_node = new Node{bbox, 0, (int) indices.size(), NULL, NULL};
	_ComputeNodes(p_node);
}


Bbox Mesh::_ComputeBbox(int i_first, int nb_triangle)
{
	if (nb_triangle == 0)
		return Bbox();
	Vector3 bmax = vertices[indices[i_first].vtx[0]];
	Vector3 bmin = vertices[indices[i_first].vtx[0]];
	for (int i_triangle = i_first; i_triangle < i_first + nb_triangle; ++i_triangle)
	{
		for (int k_vertex = 0; k_vertex < 3; ++k_vertex)
		{
			int i = indices[i_triangle].vtx[0];
			for (int k = 0; k < 3; ++k)
			{
				bmin[k] = fmin(bmin[k], vertices[i][k]);
				bmax[k] = fmax(bmax[k], vertices[i][k]);
			}
			i = indices[i_triangle].vtx[1];
			for (int k = 0; k < 3; ++k)
			{
				bmin[k] = fmin(bmin[k], vertices[i][k]);
				bmax[k] = fmax(bmax[k], vertices[i][k]);
			}
			i = indices[i_triangle].vtx[2];
			for (int k = 0; k < 3; ++k)
			{
				bmin[k] = fmin(bmin[k], vertices[i][k]);
				bmax[k] = fmax(bmax[k], vertices[i][k]);
			}
		}
	}
	Bbox bbox(bmin, bmax);
	return bbox;
}


void Mesh::_ComputeNodes(Node *p_node)
{
	if (p_node->nb_triangle >= 4)
	{
		Node* p_right_child = new Node();
		Node* p_left_child = new Node();
		p_left_child->i_first = p_node->i_first;
		p_right_child->i_first = p_node->i_first + p_node->nb_triangle;
		int k_max;				// Dimension la plus étirée;
		double center_bbox;		// Centre de la dimension la plus étirée;
		p_node->bbox.GetCenterMaxDim(k_max, center_bbox);
		int i_triangle = p_node->i_first;
		while (i_triangle < p_right_child->i_first)
		// Tant que l'indice est plus petit que l'indice du premier de la boite 2
		{
			double center_triangle = (vertices[indices[i_triangle].vtx[0]][k_max] +
				vertices[indices[i_triangle].vtx[1]][k_max] +
				vertices[indices[i_triangle].vtx[2]][k_max])/3;
			if (center_bbox > center_triangle) // alors on insère le triangle dans la boite 1
			{
				p_left_child->nb_triangle++;	// On ajoute un triangle à gauche
				++i_triangle;					// On décale l'indice en cours
			}
			else // alors on insère le triangle dans la boite 2
			{
				p_right_child->nb_triangle++;	// On ajoute un triangle à droite
				p_right_child->i_first--;		// On décrémente le premier élement de droite
				std::swap(indices[i_triangle], indices[p_right_child->i_first]);	// On échange l'indice en cours avec le premier du suivant
			}
		}

		// On ne fait la récursion que si il y a des triangles de part et d'autre du centre
		if (p_left_child->nb_triangle > 0 && p_right_child->nb_triangle > 0)
		{
			p_left_child->bbox = _ComputeBbox(p_left_child->i_first, p_left_child->nb_triangle);
			p_right_child->bbox = _ComputeBbox(p_right_child->i_first, p_right_child->nb_triangle);
			_ComputeNodes(p_left_child);
			_ComputeNodes(p_right_child);
			p_node->left_child = p_left_child;
			p_node->right_child = p_right_child;
		}
		else 
		{
			delete p_left_child;
			delete p_right_child;
		}
	}
}



void delete_nodes(Node* p_node)
{
	if (p_node == NULL)
		return;
	delete_nodes(p_node->left_child);
	delete_nodes(p_node->right_child);
	delete p_node;
}


Mesh::~Mesh()
{
	delete_nodes(p_node);
}




void Mesh::readOBJ(const char* obj) {

	char matfile[255];
	char grp[255];

	FILE* f;
	errno_t err;
	err = fopen_s(&f, obj, "r");

	std::map<std::string, int> groupNames;
	int curGroup = -1;
	while (!feof(f)) {
		char line[255];
		if (!fgets(line, 255, f)) break;

		std::string linetrim(line);
		linetrim.erase(linetrim.find_last_not_of(" \r\t") + 1);
		strcpy_s(line, linetrim.c_str());

		if (line[0] == 'u' && line[1] == 's') {
			sscanf_s(line, "usemtl %[^\n]\n", grp, 255);
			if (groupNames.find(std::string(grp)) != groupNames.end()) {
				curGroup = groupNames[std::string(grp)];
			}
			else {
				curGroup = groupNames.size();
				groupNames[std::string(grp)] = curGroup;
			}
		}
		if (line[0] == 'm' && line[1] == 't' && line[2] == 'l') {
			sscanf_s(line, "mtllib %[^\n]\n", matfile, 255);
		}
		if (line[0] == 'v' && line[1] == ' ') {
			Vector3 vec;
			Vector3 col;
			if (sscanf_s(line, "v %lf %lf %lf %lf %lf %lf\n", &vec[0], &vec[2], &vec[1], &col[0], &col[1], &col[2], 255) == 6) {
				vertices.push_back(vec);
				vertexcolors.push_back(col);
			}
			else {
				sscanf_s(line, "v %lf %lf %lf\n", &vec[0], &vec[2], &vec[1], 255);  // helmet
																				//vec[2] = -vec[2]; //car2
				vertices.push_back(vec);
			}
		}
		if (line[0] == 'v' && line[1] == 'n') {
			Vector3 vec;
			sscanf_s(line, "vn %lf %lf %lf\n", &vec[0], &vec[2], &vec[1], 255); //girl
			normals.push_back(vec);
		}
		if (line[0] == 'v' && line[1] == 't') {
			Vector3 vec;
			sscanf_s(line, "vt %lf %lf\n", &vec[0], &vec[1], 255);
			uvs.push_back(vec);
		}
		if (line[0] == 'f') {
			TriangleIndices t;
			int i0, i1, i2, i3;
			int j0, j1, j2, j3;
			int k0, k1, k2, k3;
			int nn;

			char* consumedline = line + 1;
			int offset;
			t.faceGroup = curGroup;
			nn = sscanf_s(consumedline, "%u/%u/%u %u/%u/%u %u/%u/%u%n", &i0, &j0, &k0, &i1, &j1, &k1, &i2, &j2, &k2, &offset,255);
			if (nn == 9) {
				if (i0 < 0) t.vtx[0] = vertices.size() + i0; else	t.vtx[0] = i0 - 1;
				if (i1 < 0) t.vtx[1] = vertices.size() + i1; else	t.vtx[1] = i1 - 1;
				if (i2 < 0) t.vtx[2] = vertices.size() + i2; else	t.vtx[2] = i2 - 1;
				if (j0 < 0) t.uv[0] = uvs.size() + j0; else	t.uv[0] = j0 - 1;
				if (j1 < 0) t.uv[1] = uvs.size() + j1; else	t.uv[1] = j1 - 1;
				if (j2 < 0) t.uv[2] = uvs.size() + j2; else	t.uv[2] = j2 - 1;
				if (k0 < 0) t.n[0] = normals.size() + k0; else	t.n[0] = k0 - 1;
				if (k1 < 0) t.n[1] = normals.size() + k1; else	t.n[1] = k1 - 1;
				if (k2 < 0) t.n[2] = normals.size() + k2; else	t.n[2] = k2 - 1;

				indices.push_back(t);
			}
			else {
				nn = sscanf_s(consumedline, "%u/%u %u/%u %u/%u%n", &i0, &j0, &i1, &j1, &i2, &j2, &offset, 255);
				if (nn == 6) {
					if (i0 < 0) t.vtx[0] = vertices.size() + i0; else	t.vtx[0] = i0 - 1;
					if (i1 < 0) t.vtx[1] = vertices.size() + i1; else	t.vtx[1] = i1 - 1;
					if (i2 < 0) t.vtx[2] = vertices.size() + i2; else	t.vtx[2] = i2 - 1;
					if (j0 < 0) t.uv[0] = uvs.size() + j0; else	t.uv[0] = j0 - 1;
					if (j1 < 0) t.uv[1] = uvs.size() + j1; else	t.uv[1] = j1 - 1;
					if (j2 < 0) t.uv[2] = uvs.size() + j2; else	t.uv[2] = j2 - 1;
					indices.push_back(t);
				}
				else {
					nn = sscanf_s(consumedline, "%u %u %u%n", &i0, &i1, &i2, &offset, 255);
					if (nn == 3) {
						if (i0 < 0) t.vtx[0] = vertices.size() + i0; else	t.vtx[0] = i0 - 1;
						if (i1 < 0) t.vtx[1] = vertices.size() + i1; else	t.vtx[1] = i1 - 1;
						if (i2 < 0) t.vtx[2] = vertices.size() + i2; else	t.vtx[2] = i2 - 1;
						indices.push_back(t);
					}
					else {
						nn = sscanf_s(consumedline, "%u//%u %u//%u %u//%u%n", &i0, &k0, &i1, &k1, &i2, &k2, &offset,255);
						if (i0 < 0) t.vtx[0] = vertices.size() + i0; else	t.vtx[0] = i0 - 1;
						if (i1 < 0) t.vtx[1] = vertices.size() + i1; else	t.vtx[1] = i1 - 1;
						if (i2 < 0) t.vtx[2] = vertices.size() + i2; else	t.vtx[2] = i2 - 1;
						if (k0 < 0) t.n[0] = normals.size() + k0; else	t.n[0] = k0 - 1;
						if (k1 < 0) t.n[1] = normals.size() + k1; else	t.n[1] = k1 - 1;
						if (k2 < 0) t.n[2] = normals.size() + k2; else	t.n[2] = k2 - 1;
						indices.push_back(t);
					}
				}
			}


			consumedline = consumedline + offset;

			while (true) {
				if (consumedline[0] == '\n') break;
				if (consumedline[0] == '\0') break;
				nn = sscanf_s(consumedline, "%u/%u/%u%n", &i3, &j3, &k3, &offset, 255);
				TriangleIndices t2;
				t2.faceGroup = curGroup;
				if (nn == 3) {
					if (i0 < 0) t2.vtx[0] = vertices.size() + i0; else	t2.vtx[0] = i0 - 1;
					if (i2 < 0) t2.vtx[1] = vertices.size() + i2; else	t2.vtx[1] = i2 - 1;
					if (i3 < 0) t2.vtx[2] = vertices.size() + i3; else	t2.vtx[2] = i3 - 1;
					if (j0 < 0) t2.uv[0] = uvs.size() + j0; else	t2.uv[0] = j0 - 1;
					if (j2 < 0) t2.uv[1] = uvs.size() + j2; else	t2.uv[1] = j2 - 1;
					if (j3 < 0) t2.uv[2] = uvs.size() + j3; else	t2.uv[2] = j3 - 1;
					if (k0 < 0) t2.n[0] = normals.size() + k0; else	t2.n[0] = k0 - 1;
					if (k2 < 0) t2.n[1] = normals.size() + k2; else	t2.n[1] = k2 - 1;
					if (k3 < 0) t2.n[2] = normals.size() + k3; else	t2.n[2] = k3 - 1;
					indices.push_back(t2);
					consumedline = consumedline + offset;
					i2 = i3;
					j2 = j3;
					k2 = k3;
				}
				else {
					nn = sscanf_s(consumedline, "%u/%u%n", &i3, &j3, &offset, 255);
					if (nn == 2) {
						if (i0 < 0) t2.vtx[0] = vertices.size() + i0; else	t2.vtx[0] = i0 - 1;
						if (i2 < 0) t2.vtx[1] = vertices.size() + i2; else	t2.vtx[1] = i2 - 1;
						if (i3 < 0) t2.vtx[2] = vertices.size() + i3; else	t2.vtx[2] = i3 - 1;
						if (j0 < 0) t2.uv[0] = uvs.size() + j0; else	t2.uv[0] = j0 - 1;
						if (j2 < 0) t2.uv[1] = uvs.size() + j2; else	t2.uv[1] = j2 - 1;
						if (j3 < 0) t2.uv[2] = uvs.size() + j3; else	t2.uv[2] = j3 - 1;
						consumedline = consumedline + offset;
						i2 = i3;
						j2 = j3;
						indices.push_back(t2);
					}
					else {
						nn = sscanf_s(consumedline, "%u//%u%n", &i3, &k3, &offset, 255);
						if (nn == 2) {
							if (i0 < 0) t2.vtx[0] = vertices.size() + i0; else	t2.vtx[0] = i0 - 1;
							if (i2 < 0) t2.vtx[1] = vertices.size() + i2; else	t2.vtx[1] = i2 - 1;
							if (i3 < 0) t2.vtx[2] = vertices.size() + i3; else	t2.vtx[2] = i3 - 1;
							if (k0 < 0) t2.n[0] = normals.size() + k0; else	t2.n[0] = k0 - 1;
							if (k2 < 0) t2.n[1] = normals.size() + k2; else	t2.n[1] = k2 - 1;
							if (k3 < 0) t2.n[2] = normals.size() + k3; else	t2.n[2] = k3 - 1;
							consumedline = consumedline + offset;
							i2 = i3;
							k2 = k3;
							indices.push_back(t2);
						}
						else {
							nn = sscanf_s(consumedline, "%u%n", &i3, &offset, 255);
							if (nn == 1) {
								if (i0 < 0) t2.vtx[0] = vertices.size() + i0; else	t2.vtx[0] = i0 - 1;
								if (i2 < 0) t2.vtx[1] = vertices.size() + i2; else	t2.vtx[1] = i2 - 1;
								if (i3 < 0) t2.vtx[2] = vertices.size() + i3; else	t2.vtx[2] = i3 - 1;
								consumedline = consumedline + offset;
								i2 = i3;
								indices.push_back(t2);
							}
							else {
								consumedline = consumedline + 1;
							}
						}
					}
				}
			}

		}


	}
	fclose(f);

}


void Mesh::add_texture(const char* filename) {

	textures.resize(textures.size() + 1);
	w.resize(w.size() + 1);
	h.resize(h.size() + 1);

	FILE* f;
	errno_t err;
	err = fopen_s(&f, filename, "rb");
	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

	w[w.size() - 1] = *(int*)&info[18]; // extract image height and width from header
	h[h.size() - 1] = *(int*)&info[22];

	int size = 3 * w[w.size() - 1] * h[h.size() - 1];
	textures[textures.size() - 1].resize(size); // allocate 3 bytes per pixel
	fread(&textures[textures.size() - 1][0], sizeof(unsigned char), size, f); // read the rest of the data at once
	fclose(f);

	for (int i = 0; i < size; i += 3) {
		std::swap(textures[textures.size() - 1][i], textures[textures.size() - 1][i + 2]);
	}
}


void Mesh::GetIntersection(const Ray& ray, IntersectionObject& intersection) const
{
	intersection.b_intersect = false;
	_GetIntersectionRec(ray, intersection, p_node);
}

void Mesh::_GetIntersectionRec(const Ray& ray, IntersectionObject& intersection, Node* p_node) const
{
	double t_current;
	if (p_node == NULL || !p_node->bbox.DoIntersect(ray, t_current) || (intersection.b_intersect && t_current > intersection.t))
	{
		return;
	}
	if (p_node->left_child == NULL)
	{
		IntersectionObject local_intersection;
		for (int i_triangle = p_node->i_first; i_triangle < p_node->i_first + p_node->nb_triangle; ++i_triangle)
		{
			double alpha, beta, gamma;
			Triangle tri(
				vertices[indices[i_triangle].vtx[0]],
				vertices[indices[i_triangle].vtx[1]],
				vertices[indices[i_triangle].vtx[2]],
				normals[indices[i_triangle].n[0]],
				normals[indices[i_triangle].n[1]],
				normals[indices[i_triangle].n[2]],
				material);
			tri.GetIntersection(ray, local_intersection);
			if (local_intersection.b_intersect && (!intersection.b_intersect || local_intersection.t < intersection.t))
			{
				intersection = local_intersection;
			}
		}
	}
	else
	{
		_GetIntersectionRec(ray, intersection, p_node->left_child);
		_GetIntersectionRec(ray, intersection, p_node->right_child);
	}
}