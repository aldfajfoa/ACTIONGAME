#include "PlayScene.h"
#include "Engine/Fbx.h"
#include "Player.h"
#include "Stage.h"
#include "HP.h"
#include "Engine/Input.h"

PlayScene::PlayScene(GameObject* parent)
	:GameObject(parent, "PlayScene")
{
}

void PlayScene::Initialize()
{
	pField_ = Instantiate<Stage>(this);
	Instantiate<Player>(this);
	pField_->SetMap();
	Instantiate<HP>(this);
}

void PlayScene::Release()
{
	pField_ = nullptr;
}
