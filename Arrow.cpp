#include "Arrow.h"
#include "Player.h"
#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/BoxCollider.h"
#include "Timer.h"

Arrow::Arrow(GameObject* parent)
	:GameObject(parent, "Arrow"),hArrow_(-1),isMove_(true),Gravity_(0.006f),dy_(0.04)
{
	hArrow_ = Model::Load("Arrow.fbx");
	assert(hArrow_ >= 0);
}

void Arrow::Initialize()
{
	timer_ = Instantiate<Timer>(this);
	player_ = GetParent()->FindGameObject<Player>();
	stage_ = GetParent()->FindGameObject<Stage>();

	transform_.position_ = player_->GetPosition();
	transform_.scale_ = { 2,2,2 };

	transform_.rotate_.y = 90;
	AboxColl_ = new BoxCollider({ 0.3,0,0 }, { 0.2,0.2,0.2 });
	AddCollider(AboxColl_);
}

void Arrow::Update()
{
	if (isAlive_)
	{
		if (isMove_)
		{
			//重力で少しずつ角度と高度が落ちる
			transform_.position_.y += dy_;
			transform_.rotate_.x += -dy_ * 20;
			dy_ -= Gravity_;

			//プレイヤーの向きで進む方向を変える
			if (PisRight_)
			{
				transform_.position_.x += MOVE_SPEED;
			}
			else
			{
				transform_.position_.x -= MOVE_SPEED;
			}
		}

		ArrowCollision();

		if (isMove_ == false)
		{
			if (timer_->TimeElapsed(1))
			{
				ResetArrow();
			}
		}
	}
}

void Arrow::Draw()
{
	if (isAlive_)
	{
		Model::SetTransform(hArrow_, transform_);
		Model::Draw(hArrow_);
	}
}

void Arrow::Release()
{
}

void Arrow::SetArrowDir(const bool& PisRight)
{
	PisRight_ = PisRight;
	if (PisRight_)
	{
		transform_.rotate_.y = 90;
		AboxColl_->Setcenter({ 0.3,0,0 });
	}
	else
	{
		transform_.rotate_.y = -90;
		AboxColl_->Setcenter({ -0.3,0,0 });
	}
}

void Arrow::ResetArrow()
{
	isAlive_ = false;
	isMove_ = true;
	dy_ = 0.04;
	transform_.rotate_ = { 0,0,0 };
	transform_.position_ = { 0,0,0 };
	timer_->ResetTime(1);
}

void Arrow::ArrowCollision()
{
	for (int y = 0; y < stage_->Getheight(); y++)
	{
		for (int x = 0; x < stage_->Getwidth(); x++)
		{
			if (stage_->GetboxColl(y, x) != nullptr)//ブロックの当たり判定がnullptr(空白)じゃなかったら当たり判定
			{
				if (AboxColl_->IsHit(stage_->GetboxColl(y, x))) {
					isMove_ = false;
				}
			}
		}
	}
}
