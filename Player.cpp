#include "Player.h"
#include "Stage.h"
#include "Arrow.h"
#include "PlayScene.h"
#include "Timer.h"
#include "HP.h"
#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Engine/Camera.h"
#include "Engine/BoxCollider.h"
#include "Engine/SceneManager.h"
#include "Engine/Debug.h"

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"), hPlayer_(-1), Ground_(0),isLeftMove_(true), isRightMove_(true),isJump_(true), isRight_(true),isfirst_(true)
{
	jumpSpeed_ = 0.0f;
}

void Player::Initialize()
{
	hPlayer_ = Model::Load("Player.fbx");
	assert(hPlayer_ >= 0);
	transform_.rotate_.y = 90;

	//上下左右の当たり判定(ブロックやギミックとの判定用)
	PboxColl_[0] = new BoxCollider({ -0.3, 0.1, 0 }, { 0.3,   1, 0.1 });//左
	PboxColl_[1] = new BoxCollider({  0.5, 0.1, 0 }, { 0.3,   1, 0.1 });//右
	PboxColl_[2] = new BoxCollider({  0.1, 0.7, 0 }, { 0.5, 0.3, 0.1 });//上
	PboxColl_[3] = new BoxCollider({  0.1,-0.5, 0 }, { 0.5,   1, 0.1 });//下
	//中心(敵やゴールとの判定用)
	PboxColl_[4] = new BoxCollider({  0,   0.1, 0 }, { 0.7, 1.2, 0.1 });
	for (int i = 0; i < PBOX_NUM; i++)
	{
		AddCollider(PboxColl_[i]);
	}

	stage_ = GetParent()->FindGameObject<Stage>();
	//vine_ = stage_->FindGameObject<Vine>();
	hp_ = nullptr;
	timer_ = Instantiate<Timer>(this);
	ArrowInterval_ = Instantiate<Timer>(this);
	ArrowInterval_->ResetTime(0);

	for (int a = 0; a < ARROW_NUM; a++)
	{
		arrows_[a] = Instantiate<Arrow>(FindObject("PlayScene"));
	}
}

void Player::Update()
{
	if (hp_ == nullptr)
	{
		hp_ = GetParent()->FindGameObject<HP>();
	}
	MovePlayer();

	if (transform_.position_.y < Ground_) {
		transform_.position_.y = Ground_;
		jumpSpeed_ = 0.0f;
		isJump_ = true;
	}
	
	if (transform_.position_.y > CameraStopGround_)//ここでカメラが止まる
	{
		XMFLOAT3 t = { transform_.position_.x,transform_.position_.y+1,transform_.position_.z };
		XMFLOAT3 p = { transform_.position_.x,transform_.position_.y+1,-10 + transform_.position_.z };
		Camera::SetTarget(t);
		Camera::SetPosition(p);
	}
	else if (transform_.position_.y < DeathGround_)//ゲームオーバー
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_OVER);
	}

	//Fキーで矢を撃つ
	if (ArrowInterval_->NoResetTimeElapsed())
	{
		if (Input::IsKeyDown(DIK_F))
		{
			arrows_[currentArrow_]->SetPosition(transform_.position_);
			arrows_[currentArrow_]->SetArrowDir(isRight_);
			arrows_[currentArrow_]->SetisAlive(true);
			currentArrow_++;
			if (currentArrow_ == ARROW_NUM - 1)
			{
				currentArrow_ = 0;
			}
			ArrowInterval_->ResetTime(1);
		}
	}
	
	BlowawayPlayer();

	//isPdamage_のどれかがtrueだったらダメージを受ける
	if (isPdamage_[LEFT] || isPdamage_[RIGHT] || isPdamage_[TOP] || isPdamage_[UNDER])
	{
		if (isfirst_)//当たった瞬間は即ダメージを受ける
		{
			hp_->DecreaseHP(20);
			isfirst_ = false;
		}
		if (timer_->TimeElapsed(1))//その後は秒間隔でダメージ
		{
			hp_->DecreaseHP(20);
		}
	}
	else
	{
		isfirst_ = true;
		timer_->ResetTime(0);
	}

	isLeftMove_ = true;
	isRightMove_ = true;
	Ground_ = -999;
	for (int dir = 0; dir < 4; dir++)
	{
		isPdamage_[dir] = false;
	}
}

void Player::Draw()
{
	Model::SetTransform(hPlayer_, transform_);
	Model::Draw(hPlayer_);
}

void Player::Release()
{
	SAFE_RELEASE(ArrowInterval_);
	SAFE_RELEASE(timer_);
	SAFE_RELEASE(hp_);
	stage_  = nullptr;

	for (int i = 0; i < PBOX_NUM; i++)
	{
		PboxColl_[i] = nullptr;
	}

	for (int i = 0; i < ARROW_NUM; i++)
	{
		SAFE_RELEASE(arrows_[i]);
	}
}

void Player::RotPlayer(const bool& isRight)
{
	if (isRight_ == false)
	{
		if (transform_.rotate_.y != P_LEFT_ANGLE)
		{
			transform_.rotate_.y += 30;
		}
	}
	else
	{
		if (transform_.rotate_.y != P_RIGHT_ANGLE)
		{
			transform_.rotate_.y -= 30;
		}
	}
}



void Player::BlowawayPlayer()
{
	if (Dist_ > 0)
	{
		if ((Dir_ == -1 && isLeftMove_) || (Dir_ == 1 && isRightMove_))
		{
			transform_.position_.x += 0.2 * Dir_;
		}
		jumpSpeed_ = -sqrtf(GRAVITY * (P_JUMP_HEIGHT / 20));
		Dist_--;
	}
	else if (Dist_ <= 0)
	{
		Dist_ = 0;
		Dir_ = 0;
	}
}

void Player::OnCollision(GameObject* pTarget)
{
	for (int y = 0; y < stage_->Getheight(); y++)
	{
		for (int x = 0; x < stage_->Getwidth(); x++)
		{
			if (stage_->GetboxColl(y, x) != nullptr)//ブロックの当たり判定がnullptr(空白)じゃなかったら当たり判定
			{
				if (PboxColl_[LEFT]->IsHit(stage_->GetboxColl(y, x))) {
					isLeftMove_ = false;//当たったら左に進めなくする
					DiscernBrock(y, x, LEFT);
				}

				if (PboxColl_[RIGHT]->IsHit(stage_->GetboxColl(y, x))) {
					isRightMove_ = false;//当たったら右に進めなくする
					DiscernBrock(y, x, RIGHT);
				}

				if (isJump_) {
					if (PboxColl_[TOP]->IsHit(stage_->GetboxColl(y, x))) {
						jumpSpeed_ = 0;
						isJump_ = false;
					    DiscernBrock(y, x, TOP);
					}
				}

				if (PboxColl_[UNDER]->IsHit(stage_->GetboxColl(y, x))) {
					Ground_ = stage_->GetboxColl(y, x)->Getcenter().y + 0.5;//地面に乗せる
					DiscernBrock(y, x, UNDER);
				}
			}
		}
	}
}

void Player::MovePlayer()
{
	if (Input::IsKey(DIK_LEFT))//左向きにする
	{
		isRight_ = false;
		if (isLeftMove_)//左へ進む
		{
			transform_.position_.x -= P_MOVE_SPEED;
		}
	}

	if (Input::IsKey(DIK_RIGHT))//右向きにする
	{
		isRight_ = true;
		if (isRightMove_)//右へ進む
		{
			transform_.position_.x += P_MOVE_SPEED;
		}
	}

	if (isJump_)
	{
		if (Input::IsKey(DIK_SPACE))//ジャンプ
		{
			jumpSpeed_ = -sqrtf(GRAVITY * P_JUMP_HEIGHT);
		}
	}

	jumpSpeed_ += GRAVITY;
	transform_.position_.y -= jumpSpeed_;

	RotPlayer(isRight_);
}

void Player::DiscernBrock(const int& x, const int& y, const Dir& dir)
{
	switch (stage_->GetBrockID(x, y))
	{
	case 102:
		isPdamage_[dir] = true;
	}
}

