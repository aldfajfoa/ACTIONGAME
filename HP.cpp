#include "HP.h"
#include "Timer.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"

HP::HP(GameObject* parent)
	:GameObject(parent, "HP"),hHp_(-1),isDamage_(true)
{
	hHp_ = Image::Load("HP.png");
	assert(hHp_ >= 0);

	currentHp_ = MAX_HP;
	isDamageTime_ = Instantiate<Timer>(this);
}

void HP::Initialize()
{
	transform_.position_ = { HP_POS.x,HP_POS.y,0};
}

void HP::Update()
{
	if (isDamage_ == false)
	{
		if (isDamageTime_->TimeElapsed(1))
		{
			isDamage_ = true;
		}
	}

	//currentHp_�������Ă��炻�̕�HP�摜��؂蔲��
	float percentage = ((currentHp_ / MAX_HP) * 100);//����currentHP_��MAX_HP�̉��p�[�Z���g��
	float DecreasePer = (100 - percentage) / 100;//�c��̊���

	float cutNum = (IMAGE_SIZE.w/2) * DecreasePer;
	Image::SetRect(hHp_, cutNum, 0, IMAGE_SIZE.w - (cutNum*2), IMAGE_SIZE.h);

	if (currentHp_ <= 0)
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_OVER);
	}
}

void HP::Draw()
{
	Image::SetTransform(hHp_, transform_);
	Image::Draw(hHp_);
}

void HP::Release()
{
}
