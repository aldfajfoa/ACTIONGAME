#include "Flower.h"
#include "Arrow.h"
#include "Player.h"
#include "HP.h"
#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/BoxCollider.h"
#include "Engine/Input.h"

Flower::Flower(GameObject* parent)
	:GameObject(parent, "Flower"), hFlower_(-1), Ground_(0), isRight_(false), isLeftMove_(true), isRightMove_(true)
{
	hFlower_ = Model::Load("Flower.fbx");
	assert(hFlower_ >= 0);
	transform_.rotate_.y = 200;
	FlowerHp_ = FLOWER_HP;
}

void Flower::Initialize()
{
	//上下左右の当たり判定(ブロックやギミックとの判定用)
	FboxColl_[0] = new BoxCollider({ -0.5, 0.08, 0 }, { 0.4, 0.8, 0.1 });//左
	FboxColl_[1] = new BoxCollider({  0.5, 0.08, 0 }, { 0.4, 0.8, 0.1 });//右
	FboxColl_[2] = new BoxCollider({    0,  0.7, 0 }, {   1, 0.4, 0.1 });//上
	FboxColl_[3] = new BoxCollider({    0, -0.5, 0 }, { 0.4, 0.4, 0.1 });//下
	FboxColl_[4] = new BoxCollider({    0,  0.3, 0 }, { 1.1, 1.3, 0.1 });//中心

	for (int i = 0; i < FBOX_NUM; i++)
	{
		AddCollider(FboxColl_[i]);
	}

	CheckScaffoldColl_ = new BoxCollider({ -0.5, -0.5, 0 }, { 0.4, 0.4, 0.1 });
	AddCollider(CheckScaffoldColl_);

	stage_ = (Stage*)GetParent();
	player_ = ((Player*)GetParent()->GetParent()->FindGameObject<Player>());
	hp_ = nullptr;
	arrow_ = nullptr;
}

void Flower::Update()
{
	if (arrow_ == nullptr)
	{
		arrow_ = ((Arrow*)GetParent()->GetParent()->FindGameObject<Arrow>());
	}

	if (hp_ == nullptr)
	{
		hp_ = ((HP*)GetParent()->GetParent()->FindGameObject<HP>());
	}

	if (FlowerHp_ == 0)
	{
		KillMe();
	}

	MoveFlower();
	
	if (transform_.position_.y < Ground_) {
		transform_.position_.y = Ground_;
	}

	isLeftMove_ = true;
	isRightMove_ = true;
	Ground_ = -999;
}

void Flower::Draw()
{
	Model::SetTransform(hFlower_, transform_);
	Model::Draw(hFlower_);
}

void Flower::Release()
{
}

void Flower::RotFlower(const bool& isRight)
{
	if (isRight_ == false)
	{
		if (transform_.rotate_.y != F_LEFT_ANGLE)
		{
			transform_.rotate_.y += 10;
		}
		CheckScaffoldColl_->Setcenter({ -0.5,-0.5, 0 });
	}
	else
	{
		if (transform_.rotate_.y != F_RIGHT_ANGLE)
		{
			transform_.rotate_.y -= 10;
		}
		CheckScaffoldColl_->Setcenter({ 0.5,-0.5, 0 });
	}
}

void Flower::OnCollision(GameObject* pTarget)
{
	//ブロックとの当たり判定
	for (int y = 0; y < stage_->Getheight(); y++)
	{
		for (int x = 0; x < stage_->Getwidth(); x++)
		{
			if (stage_->GetboxColl(y, x) != nullptr)//ブロックの当たり判定がnullptr(空白)じゃなかったら当たり判定
			{
				if (FboxColl_[LEFT]->IsHit(stage_->GetboxColl(y, x))) {
					isLeftMove_ = false;//当たったら左に進めなくする
					isRight_ = true;
				}

				if (FboxColl_[RIGHT]->IsHit(stage_->GetboxColl(y, x))) {
					isRightMove_ = false;//当たったら右に進めなくする
					isRight_ = false;
				}

				if (FboxColl_[UNDER]->IsHit(stage_->GetboxColl(y, x))) {
					Ground_ = stage_->GetboxColl(y, x)->Getcenter().y + 0.5;//地面に乗せる
				}
			}
		}
	}

	//進行方向に足場が無かったら反対に進む
	int x = 0, y = 0;
	bool breakOuterLoop = false; // 外側のループを中断するためのフラグ

	for (y = 0; y < stage_->Getheight(); y++)
	{
		for (x = 0; x < stage_->Getwidth(); x++)
		{
			if ((CheckScaffoldColl_->IsHit(stage_->GetboxColl(y, x)))) {//どこかに当たった時点でループを中断
				breakOuterLoop = true;
				break;
			}
		}
		if (breakOuterLoop)
		{
			break;
		}
	}

	if (y >= stage_->Getheight() - 1 && x >= stage_->Getwidth() - 1)//ループの最後までどことも当たって無かったら反転
	{
		isRight_ = !isRight_;
	}

	//矢との当たり判定
	for (int a = 0; a < player_->GetARROW_NUM(); a++)
	{
		if (FboxColl_[CENTER]->IsHit(player_->Getarrows(a)->GetAboxColl())) {
			player_->Getarrows(a)->ResetArrow();
			FlowerHp_--;
		}
	}

	//プレイヤーに当たったらHPを減らしてノックバック
	if (FboxColl_[LEFT]->IsHit(player_->GetCenterBoxColl())) {
		hp_->DecreaseHP(40);
		player_->setDistandDir(5, -1);
	}
	if (FboxColl_[RIGHT]->IsHit(player_->GetCenterBoxColl())) {
		hp_->DecreaseHP(40);
		player_->setDistandDir(5, 1);
	}
}

void Flower::MoveFlower()
{
	if (Input::IsKey(DIK_U))
	{
		if (isLeftMove_)
		{
			isRight_ = false;
			transform_.position_.x -= 0.05;
		}
	}

	if (Input::IsKey(DIK_I))
	{
		if (isRightMove_)
		{
			isRight_ = true;
			transform_.position_.x += 0.05;
		}
	}

	if (isLeftMove_ && isRight_ == false)//左へ進む
	{
		transform_.position_.x -= F_MOVE_SPEED;
	}
	if (isRightMove_ && isRight_)//右へ進む
	{
		transform_.position_.x += F_MOVE_SPEED;
	}

	transform_.position_.y -= GRAVITY;

	RotFlower(isRight_);

}
