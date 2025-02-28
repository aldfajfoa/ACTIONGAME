#include "Gool.h"
#include "Player.h"
#include "Timer.h"
#include "Engine/Model.h"
#include "Engine/BoxCollider.h"
#include "Engine/SceneManager.h"

Gool::Gool(GameObject* parent)
	:GameObject(parent, "Gool"), hGool_(-1)
{
}

void Gool::Initialize()
{
	hGool_ = Model::Load("Gool.fbx");
	assert(hGool_ >= 0);

	GboxColl_ = new BoxCollider({ 0.2,0,0 }, { 0.6,1,0.2 });
	AddCollider(GboxColl_);

	player_ = (Player*)GetParent()->GetParent()->FindGameObject<Player>();
	timer_ = Instantiate<Timer>(this);
}

void Gool::Update()
{
	//プレイヤーと当たったらクリア画面へ
	if (GboxColl_->IsHit(player_->GetCenterBoxColl())) 
	{
		if (timer_->TimeElapsed(1))
		{
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeScene(SCENE_ID_CLEAR);
		}
	}
}

void Gool::Draw()
{
	Model::SetTransform(hGool_, transform_);
	Model::Draw(hGool_);
}

void Gool::Release()
{
	SAFE_RELEASE(player_);
	timer_ = nullptr;
	GboxColl_ = nullptr;
}
