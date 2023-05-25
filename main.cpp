#include <Novice.h>
#include <array>
#include <string>
#include <memory>
#include <cassert>
#include <imgui.h>
#include <numbers>
#include "Vector3D/Vector3D.h"
#include "Mat4x4/Mat4x4.h"
#include "Camera/Camera.h"
#include "Sphere/Sphere.h"

const std::string kWindowTitle = "LE2A_04_キクタニ_タクマ_タイトル";

void DrawGrid(const Mat4x4& viewProjectionMatrix, const Mat4x4& viewPortmatrix);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	// ライブラリの初期化
	Novice::Initialize(kWindowTitle.c_str(), 1280, 720);

	// キー入力結果を受け取る箱
	std::array<char, 256> keys = { 0 };
	std::array<char, 256> preKeys = { 0 };

	auto camera = std::make_unique<Camera>();
	assert(camera);

	camera->pos = { 0.0f,0.1f, -1.0f };
	camera->rotate = { 0.0f,0.0f,0.0f };
	camera->scale = { 1.0f,1.0f,1.0f };

	std::unique_ptr<Sphere> sphere = std::make_unique<Sphere>();


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();


		// キー入力を受け取る
		std::copy(keys.begin(), keys.end(), preKeys.begin());
		Novice::GetHitKeyStateAll(keys.data());

		///
		/// ↓更新処理ここから
		///
		ImGui::Begin("CameraSet");
		ImGui::DragFloat3("CameraTranslate", &camera->pos.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &camera->rotate.x, 0.01f);
		ImGui::DragFloat3("SphereCenter", &sphere->translation.x, 0.01f);
		ImGui::DragFloat3("SphereRotate", &sphere->worldRoate.x, 0.01f);
		ImGui::DragFloat("SphereRadius", &sphere->radius, 0.01f);
		ImGui::End();

		sphere->Update();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		camera->Update();

		DrawGrid(camera->getViewProjectionMatrix(), camera->getViewPortMatrix());

		sphere->Draw(camera->getViewProjectionMatrix(), camera->getViewPortMatrix(), 0xff);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

void DrawGrid(const Mat4x4& viewProjectionMatrix, const Mat4x4& viewPortmatrix) {
	const float kGridHalfWidth = 2.0f;
	const uint32_t kSubdivision = 10;
	const float kGridEvery = (kGridHalfWidth * 2.0f) / static_cast<float>(kSubdivision);

	Vector3D startLocal = Vector3D(kGridHalfWidth / static_cast<float>(kSubdivision), 0.0f, 0.0f);
	Vector3D endLocal = Vector3D(-kGridHalfWidth / static_cast<float>(kSubdivision), 0.0f, 0.0f);

	Vector3D pos{ 0.0f, 0.0f, (kGridEvery / 2.0f) };

	Mat4x4 worldMat;

	for (uint32_t z = 0; z <= kSubdivision; ++z) {
		worldMat = MakeMatrixAffin({ 1.0f,1.0f,1.0f }, Vector3D(), pos);


		Vector3D screenStartPos = startLocal * (worldMat * viewProjectionMatrix * viewPortmatrix);
		Vector3D screenEndPos = endLocal * (worldMat * viewProjectionMatrix * viewPortmatrix);

		if(pos.z == 0.0f){ Novice::DrawLine(int(screenStartPos.x), int(screenStartPos.y), int(screenEndPos.x), int(screenEndPos.y), 0xff); }
		else { Novice::DrawLine(int(screenStartPos.x), int(screenStartPos.y), int(screenEndPos.x), int(screenEndPos.y), 0xaaaaaaff); }

		pos.z -= (kGridEvery / static_cast<float>(kSubdivision));
	}

	pos = { -(kGridEvery / 2.0f), 0.0f, 0.0f };

	for (uint32_t x = 0; x <= kSubdivision; ++x) {
		worldMat = MakeMatrixAffin({ 1.0f,1.0f,1.0f }, Vector3D(0.0f, std::numbers::pi_v<float> / 2.0f, 0.0f), pos);


		Vector3D screenStartPos = startLocal * (worldMat * viewProjectionMatrix * viewPortmatrix);
		Vector3D screenEndPos = endLocal * (worldMat * viewProjectionMatrix * viewPortmatrix);

		if (pos.x == 0.0f) { Novice::DrawLine(int(screenStartPos.x), int(screenStartPos.y), int(screenEndPos.x), int(screenEndPos.y), 0xff); }
		else { Novice::DrawLine(int(screenStartPos.x), int(screenStartPos.y), int(screenEndPos.x), int(screenEndPos.y), 0xaaaaaaff); }

		pos.x += (kGridEvery / static_cast<float>(kSubdivision));
	}
}
