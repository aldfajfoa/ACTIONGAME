#include "Vine.h"
#include "Stage.h"
#include "Player.h"
#include "Timer.h"
#include "Engine/Model.h"
#include "Engine/BoxCollider.h"
#include <sstream> 

Vine::Vine(GameObject* parent)
	:GameObject(parent, "Vine"),hVine_(-1), hSpinyVine_(-1), hLeafVine_(-1),isMove_(true),isUp_(true)
{
	Vines_ = 1;
	addVinesLine_ = 0;
}

void Vine::Initialize()
{
	hVine_ = Model::Load("Vine.fbx");
	assert(hVine_ >= 0);

	vTipColl_ = new BoxCollider({ 0, 0.4, 0 }, { 0.3, 0.2, 0.1 });
	AddCollider(vTipColl_);
	VineColl_ = new BoxCollider({ 0, 0, 0 }, { 0.3, 1, 0.1 });
	AddCollider(VineColl_);

	stage_ = (Stage*)GetParent();
	player_ = (Player*)stage_->GetParent()->FindGameObject<Player>();
	timer_ = Instantiate<Timer>(this);
}

void Vine::Update()
{
	if (isMove_)
	{
		if (isUp_)
		{
			transform_.position_.y += 0.05;
			MoveNum_ += 0.05;
		}
		else
		{
			transform_.position_.y -= 0.05;
			MoveNum_ -= 0.05;
		}
	}

	if (isUp_) {
		if (MoveNum_ >= addVinesLine_)
		{
			Vines_++;
			addVinesLine_++;
			VineColl_->Setcenter({ 0,(float)-0.5 * (Vines_ - 1),0 });
			VineColl_->SetSize({ 0.3,(float)Vines_,0.1 });
		}
	}
	else
	{
		if (MoveNum_ <= addVinesLine_-0.7)
		{

			if (Vines_ > 0)
			{
				Vines_--;
				addVinesLine_--;
				VineColl_->Setcenter({ 0,(float)-0.5 * (Vines_ - 1),0 });
				VineColl_->SetSize({ 0.3,(float)Vines_,0.1 });
			}
			else if(Vines_ == 0)
			{
				if (timer_->TimeElapsed(1))
				{
					isUp_ = true;
				}
			}
		}
	}

	isMove_ = true;
}

void Vine::Draw()
{
	for (int i = 0; i < Vines_; i++)
	{
		Transform tmp = transform_;
		tmp.position_.y -= i;
		Model::SetTransform(hVine_, tmp);
		Model::Draw(hVine_);
	}
}

void Vine::Release()
{
	
}

void Vine::OnCollision(GameObject* pTarget)
{
	for (int y = 0; y < stage_->Getheight(); y++)
	{
		for (int x = 0; x < stage_->Getwidth(); x++)
		{
			if (stage_->GetboxColl(y, x) != nullptr)//ブロックの当たり判定がnullptr(空白)じゃなかったら当たり判定
			{
				if (vTipColl_->IsHit(stage_->GetboxColl(y, x))) 
				{
					if (timer_->TimeElapsed(1))
					{ 
						isMove_ = true;
						if (isUp_){
							isUp_ = false;
						}
						else{
							isUp_ = true;
						}
					}
					else
					{
						isMove_ = false;
					}
				}
			}
		}
	}

	if (VineColl_->IsHit(player_->GetPBoxColl(LEFT)))
	{
		player_->SetisLeftMove(false);
	}
	if (VineColl_->IsHit(player_->GetPBoxColl(RIGHT)))
	{
		player_->SetisRightMove(false);
	}
	if (VineColl_->IsHit(player_->GetPBoxColl(UNDER)))
	{
		if (isUp_)
		{
			player_->SetGround(VineColl_->Getcenter().y - (VineColl_->Getsize().y - 1));
		}
		else if (isUp_ == false)
		{
			player_->SetGround(VineColl_->Getcenter().y - (VineColl_->Getsize().y - 1));
		}
	}

	//プレイヤーの下のBoxと当たってたら乗せる
	if (VineColl_->IsHit(player_->GetPBoxColl(UNDER)))
	{
		player_->SetGround(transform_.position_.y+0.5);
	}
}
