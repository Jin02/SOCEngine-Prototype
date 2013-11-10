#include "TestScene.h"
#include "LambertMaterial.h"

using namespace Rendering;

TestScene::TestScene(void)
{
	camObject = nullptr;
	meshObject = nullptr;
}

TestScene::~TestScene(void)
{

}


void TestScene::OnInitialize()
{
	camObject = new Object;

	rootObjects->Add(camObject);

	Rendering::Camera *camera = camObject->AddComponent<Camera>();	

	fbxsdk_2014_1::FbxManager *manager = fbxsdk_2014_1::FbxManager::Create();
	Rendering::Importer::FBXImporter *importer = new Rendering::Importer::FBXImporter(manager);

	bool success;
	success = importer->Initialize("");
	success = importer->LoadScene("testcarl.fbx");

	meshObject = importer->BuildObject(nullptr);
	rootObjects->Add(meshObject);

	SOC_Vector3 v = SOC_Vector3(0.0, 20.0, 150);
	meshObject->GetTransform()->SetPosition(v);

	camObject->GetTransform()->SetPosition(SOC_Vector3(0, 0, 0));
	camObject->GetTransform()->SetDirection(SOC_Vector3(0, 0, 1));

	camera = camObject->GetComponent<Camera>();
	cameraMgr->SetMainCamera(camera);

	meshObject->Get(1)->GetComponent<Mesh::Mesh>()->GetRenderer()->DeleteMaterial();

	Texture::Texture *texture = textureMgr->AddTexture("Test.jpg");
	Material::LambertMaterial *lambert = new Material::LambertMaterial(texture);
	meshObject->Get(1)->GetComponent<Mesh::Mesh>()->GetRenderer()->SetMaterial(lambert);

	lightObj = new Object;
	rootObjects->Add(lightObj);
	Light::DirectionalLight *light = lightObj->AddComponent<Light::DirectionalLight>();
	lightMgr->AddLight(light);
	lightObj->GetTransform()->SetPosition(SOC_Vector3(500, 500, 500));
	lightObj->GetTransform()->LookAt(meshObject->GetTransform()->GetWorldPosition());
}

void TestScene::OnUpdate(float dt)
{
	SOC_Vector3 eular = meshObject->GetTransform()->GetLocalEulerAngle();
	meshObject->GetTransform()->Rotate(0, 0.000002f, 0.0f);
//	meshObject->GetTransform()->Translate(SOC_Vector3(0, 0, 0.0001));
}

void TestScene::OnRenderPreview()
{
}

void TestScene::OnRenderPost()
{
}

void TestScene::OnDestroy()
{
	camObject->DeleteAllComponent();
	meshObject->DeleteAllComponent();

	Utility::SAFE_DELETE(camObject);
	Utility::SAFE_DELETE(meshObject);
}