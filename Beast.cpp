#include "Beast.h"
#include "Stage.h"
#include "Arrow.h"
#include "Player.h"
#include "HP.h"
#include "Engine/Model.h"
#include "Engine/BoxCollider.h"
#include "Engine/SphereCollider.h"

Beast::Beast(GameObject* parent)
	:GameObject(parent, "Beast"), hBeast_(-1), Ground_(0), isLeftMove_(true), isRightMove_(true), isJump_(true), isRight_(false),isChase_(false)
{
	hBeast_ = Model::Load("Beast.fbx");
	assert(hBeast_ >= 0);
	transform_.rotate_.y = -90;
	BeastHp_ = BEAST_HP;
}

void Beast::Initialize()
{
	//�㉺���E�̓����蔻��(�u���b�N��M�~�b�N�Ƃ̔���p)
	BboxColl_[0] = new BoxCollider({ -0.4, 0.08, 0 }, { 0.7, 0.8, 0.1 });//��
	BboxColl_[1] = new BoxCollider({  0.4, 0.08, 0 }, { 0.7, 0.8, 0.1 });//�E
	BboxColl_[2] = new BoxCollider({    0,  0.5, 0 }, {   1, 0.3, 0.1 });//��
	BboxColl_[3] = new BoxCollider({  0.1, -0.5, 0 }, {   1, 0.6, 0.1 });//��
	BboxColl_[4] = new BoxCollider({    0,    0, 0 }, { 1.2, 0.8, 0.1 });//���S

	for (int i = 0; i < BBOX_NUM; i++)
	{
		AddCollider(BboxColl_[i]);
	}

	DetectCircle_ = new SphereCollider({ 0,0,0 }, 5);
	AddCollider(DetectCircle_);

	stage_ = (Stage*)GetParent();
	player_ = ((Player*)GetParent()->GetParent()->FindGameObject<Player>());
	hp_ = nullptr;
	arrow_ = nullptr;
}

void Beast::Update()
{
	if (arrow_ == nullptr)
	{
		arrow_ = ((Arrow*)GetParent()->GetParent()->FindGameObject<Arrow>());
	}

	if (hp_ == nullptr)
	{
		hp_ = ((HP*)GetParent()->GetParent()->FindGameObject<HP>());
	}

	if (BeastHp_ == 0)
	{
		KillMe();
	}

	MoveBeast();

	if (transform_.position_.y < Ground_) {
		transform_.position_.y = Ground_;
		jumpSpeed_ = 0.0f;
		isJump_ = true;
	}

	isLeftMove_ = true;
	isRightMove_ = true;
	isChase_ = false;
	Ground_ = -999;
}

void Beast::Draw()
{
	Model::SetTransform(hBeast_, transform_);
	Model::Draw(hBeast_);
}

void Beast::Release()
{
}

void Beast::RotBeast(const bool& isRight)
{
	if (isRight_ == false)
	{
		if (transform_.rotate_.y != -90)
		{
			transform_.rotate_.y -= 30;
		}
	}
	else
	{
		if (transform_.rotate_.y != 90)
		{
			transform_.rotate_.y += 30;
		}
	}
}

void Beast::OnCollision(GameObject* pTarget)
{
	//�u���b�N�̓����蔻��
	for (int y = 0; y < stage_->Getheight(); y++)
	{
		for (int x = 0; x < stage_->Getwidth(); x++)
		{
			if (stage_->GetboxColl(y, x) != nullptr)//�u���b�N�̓����蔻�肪nullptr(��)����Ȃ������瓖���蔻��
			{
				if (BboxColl_[LEFT]->IsHit(stage_->GetboxColl(y, x))) {
					isLeftMove_ = false;//���������獶�ɐi�߂Ȃ�����
					if (isGround_)
					{
						jumpSpeed_ = -sqrtf(GRAVITY * B_JUMP_HEIGHT);//�W�����v������
						isGround_ = false;
					}
				}

				if (BboxColl_[RIGHT]->IsHit(stage_->GetboxColl(y, x))) {
					isRightMove_ = false;//����������E�ɐi�߂Ȃ�����
					if (isGround_)
					{
						jumpSpeed_ = -sqrtf(GRAVITY * B_JUMP_HEIGHT);//�W�����v������
						isGround_ = false;
					}
				}

				if (isJump_) {
					if (BboxColl_[TOP]->IsHit(stage_->GetboxColl(y, x))) {
						jumpSpeed_ = 0;
						isJump_ = false;
					}
				}

				if (BboxColl_[UNDER]->IsHit(stage_->GetboxColl(y, x))) {
					Ground_ = stage_->GetboxColl(y, x)->Getcenter().y + 0.5;//�n�ʂɏ悹��
					isGround_ = true;
				}
			}
		}
	}

	//��Ƃ̓����蔻��
	for (int a = 0; a < player_->GetARROW_NUM(); a++)
	{
		if (BboxColl_[CENTER]->IsHit(player_->Getarrows(a)->GetAboxColl())) {
			player_->Getarrows(a)->ResetArrow();
			BeastHp_--;
		}
	}

	//�v���C���[�̒ǐՔ͈́@����ɓ���������v���C���[��ǂ�
	if(DetectCircle_->IsHitBoxVsCircle(player_->GetCenterBoxColl(),DetectCircle_))
	{ 
		isChase_ = true;
	}

	//�v���C���[�ɓ���������HP�����炵�ăm�b�N�o�b�N
	if (BboxColl_[LEFT]->IsHit(player_->GetCenterBoxColl())) {
		hp_->DecreaseHP(40);
		player_->setDistandDir(5,-1);
	}
	if (BboxColl_[RIGHT]->IsHit(player_->GetCenterBoxColl())) {
		hp_->DecreaseHP(40);
		player_->setDistandDir(5,1);
	}
}

void Beast::MoveBeast()
{
	if (isChase_)
	{
 		XMFLOAT3 pPos = player_->GetPosition();
		if (transform_.position_.x >= pPos.x)
		{
			isRight_ = false;
		}
		else if (transform_.position_.x <= pPos.x)
		{
			isRight_ = true;
		}
		else
		{
		}
	}
	
	if (isChase_)
	{
		if (isLeftMove_ && isRight_ == false)//���֐i��
		{
			transform_.position_.x -= B_MOVE_SPEED;
		}
		if (isRightMove_ && isRight_)//�E�֐i��
		{
			transform_.position_.x += B_MOVE_SPEED;
		}
	}

	jumpSpeed_ += GRAVITY;
	transform_.position_.y -= jumpSpeed_;

	RotBeast(isRight_);
}