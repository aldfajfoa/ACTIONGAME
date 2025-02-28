#include "OverScene.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Engine/Image.h"

OverScene::OverScene(GameObject* parent)
	:GameObject(parent, "OverScene"), hImage_(-1)
{
	hImage_ = Image::Load("OVER.png");
	assert(hImage_ >= 0);
}

void OverScene::Update()
{
	if (Input::IsKey(DIK_T))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
}

void OverScene::Draw()
{
	Image::SetTransform(hImage_, transform_);
	Image::Draw(hImage_);
}

void OverScene::Release()
{
}
