#include "Sphere/Sphere.h"
#include <Novice.h>
#include <numbers>

Sphere::Sphere() :
	size(1.0f,0.0f,0.0f),
	scale(1.0f,1.0f,1.0f),
	rotate(0.0f,0.0f,0.0f),
	translation(0.0f,0.0f,10.0f),
	worldMat(),
	kDivision(20),
	spherePosList(0)
{
	for (int y = 0; y <= kDivision; y++) {
		rotate.z = -std::numbers::pi_v<float> / 2.0f;
		for (int z = 0; z <= kDivision; z++) {
			worldMat = MakeMatrixScalar(scale) * MakeMatrixRotateZ(rotate.z) * MakeMatrixRotateY(rotate.y) * MakeMatrixTranslate(translation);
			spherePosList.push_back(size * worldMat);
			rotate.z += (std::numbers::pi_v<float> / static_cast<float>(kDivision));
		}

		rotate.y += (std::numbers::pi_v<float> / static_cast<float>(kDivision)) * 2.0f;
	}
	
}


void Sphere::Update() {
	
}

void Sphere::Draw(const Mat4x4& viewProjectionMatrix, const Mat4x4& viewPortmatrix) {
	auto vpvpMat = viewProjectionMatrix * viewPortmatrix;

	for (auto i = spherePosList.begin(); i != spherePosList.end();) {
		Vector3D startPos = *i * vpvpMat;
		if (++i != spherePosList.rbegin().base()) {
			Vector3D endPos = *(i) * vpvpMat;
			Novice::DrawLine(static_cast<int>(startPos.x), static_cast<int>(startPos.y), static_cast<int>(endPos.x), static_cast<int>(endPos.y), WHITE);
		}
	}
}