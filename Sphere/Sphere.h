#pragma once

#include "Vector3D/Vector3D.h"
#include "Mat4x4/Mat4x4.h"
#include <vector>

class Sphere {
public:
	Sphere();
	~Sphere(){}


public:
	void Update();
	void Draw(const Mat4x4& viewProjectionMatrix, const Mat4x4& viewPortmatrix);

private:
	Vector3D size;

	Vector3D scale;
	Vector3D rotate;
	Vector3D translation;

	Mat4x4 worldMat;

	const int kDivision;

	std::vector<Vector3D> spherePosList;

};