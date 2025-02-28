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
	//�㉺���E�̓����蔻��(�u���b�N��M�~�b�N�Ƃ̔���p)
	FboxColl_[0] = new BoxCollider({ -0.5, 0.08, 0 }, { 0.4, 0.8, 0.1 });//��
	FboxColl_[1] = new BoxCollider({  0.5, 0.08, 0 }, { 0.4, 0.8, 0.1 });//�E
	FboxColl_[2] = new BoxCollider({    0,  0.7, 0 }, {   1, 0.4, 0.1 });//��
	FboxColl_[3] = new BoxCollider({    0, -0.5, 0 }, { 0.4, 0.4, 0.1 });//��
	FboxColl_[4] = new BoxCollider({    0,  0.3, 0 }, { 1.1, 1.3, 0.1 });//���S

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
	//�u���b�N�Ƃ̓����蔻��
	for (int y = 0; y < stage_->Getheight(); y++)
	{
		for (int x = 0; x < stage_->Getwidth(); x++)
		{
			if (stage_->GetboxColl(y, x) != nullptr)//�u���b�N�̓����蔻�肪nullptr(��)����Ȃ������瓖���蔻��
			{
				if (FboxColl_[LEFT]->IsHit(stage_->GetboxColl(y, x))) {
					isLeftMove_ = false;//���������獶�ɐi�߂Ȃ�����
					isRight_ = true;
				}

				if (FboxColl_[RIGHT]->IsHit(stage_->GetboxColl(y, x))) {
					isRightMove_ = false;//����������E�ɐi�߂Ȃ�����
					isRight_ = false;
				}

				if (FboxColl_[UNDER]->IsHit(stage_->GetboxColl(y, x))) {
					Ground_ = stage_->GetboxColl(y, x)->Getcenter().y + 0.5;//�n�ʂɏ悹��
				}
			}
		}
	}

	//�i�s�����ɑ��ꂪ���������甽�΂ɐi��
	int x = 0, y = 0;
	bool breakOuterLoop = false; // �O���̃��[�v�𒆒f���邽�߂̃t���O

	for (y = 0; y < stage_->Getheight(); y++)
	{
		for (x = 0; x < stage_->Getwidth(); x++)
		{
			if ((CheckScaffoldColl_->IsHit(stage_->GetboxColl(y, x)))) {//�ǂ����ɓ����������_�Ń��[�v�𒆒f
				breakOuterLoop = true;
				break;
			}
		}
		if (breakOuterLoop)
		{
			break;
		}
	}

	if (y >= stage_->Getheight() - 1 && x >= stage_->Getwidth() - 1)//���[�v�̍Ō�܂łǂ��Ƃ��������Ė��������甽�]
	{
		isRight_ = !isRight_;
	}

	//��Ƃ̓����蔻��
	for (int a = 0; a < player_->GetARROW_NUM(); a++)
	{
		if (FboxColl_[CENTER]->IsHit(player_->Getarrows(a)->GetAboxColl())) {
			player_->Getarrows(a)->ResetArrow();
			FlowerHp_--;
		}
	}

	//�v���C���[�ɓ���������HP�����炵�ăm�b�N�o�b�N
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

	if (isLeftMove_ && isRight_ == false)//���֐i��
	{
		transform_.position_.x -= F_MOVE_SPEED;
	}
	if (isRightMove_ && isRight_)//�E�֐i��
	{
		transform_.position_.x += F_MOVE_SPEED;
	}

	transform_.position_.y -= GRAVITY;

	RotFlower(isRight_);

}
