#pragma once
#include "Engine/GameObject.h"
class Stage;
class BoxCollider;
class HP;
class Arrow;
class Player;

const int FBOX_NUM = 5;

class Flower :
    public GameObject
{
	//�萔
	const int F_RIGHT_ANGLE = 160;
	const int F_LEFT_ANGLE = 360 - F_RIGHT_ANGLE;
	const float GRAVITY = 18.0f / 60.0f; //�d�͉����x
	const int FLOWER_HP = 6;//�Ԃ̗̑�
	const float F_MOVE_SPEED = 0.04;//�ړ����x

	//�|�C���^
	BoxCollider* FboxColl_[FBOX_NUM];//�r�[�X�g�����蔻��
	BoxCollider* CheckScaffoldColl_;//�i�s�����ɑ��ꂪ���邩�ǂ����𒲂ׂ铖���蔻��
	Stage* stage_;
	Player* player_;
	HP* hp_;
	Arrow* arrow_;

	//�n���h��
	int hFlower_;

	//���̑��̕ϐ�
	int Ground_;//�n��
	bool isRight_;//�E�������ǂ���
	bool isLeftMove_, isRightMove_;
	int FlowerHp_;//�Ԃ̗̑�
public:
	Flower(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetGround(const float& ground) { Ground_ = ground; }
	void SetPositionXY(const float& x, const float& y) { transform_.position_ = { x,y,0 }; }

	void RotFlower(const bool& isRight);//�Ԃ̌�����ς���

	//�����ɓ�������
	//�����FpTarget ������������
	void OnCollision(GameObject* pTarget) override;

	void MoveFlower();
};

