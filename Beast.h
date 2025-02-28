#pragma once
#include "Engine/GameObject.h"
class BoxCollider;
class SphereCollider;
class Stage;
class Arrow;
class Player;
class HP;

const int BBOX_NUM = 5;

class Beast :
    public GameObject
{
	//�萔
	const float B_JUMP_HEIGHT = 1.0f * 3.0f;//�W�����v�̍���
	const float GRAVITY = 9.0f / 650.0f;    //�d�͉����x
	const int BEAST_HP = 3;//�r�[�X�g�̗̑�
	const float B_MOVE_SPEED = 0.02;//�ړ����x

	//�|�C���^
	BoxCollider* BboxColl_[BBOX_NUM];//�r�[�X�g�����蔻��
	SphereCollider* DetectCircle_;//�v���C���[�����m����͈�
	Stage* stage_;
	Arrow* arrow_;
	Player* player_;
	HP* hp_;

	//�n���h��
	int hBeast_;

	//���̑��̕ϐ�
	float jumpSpeed_;//�W�����v�̍���
	int Ground_;//�n��
	bool isRight_;//�E�������ǂ���
	bool isGround_;
	bool isLeftMove_, isRightMove_, isJump_;//���E�ړ��ƃW�����v���ł��邩�̔���
	int BeastHp_;//�r�[�X�g�̗̑�
	bool isChase_;//�v���C���[��ǂ������邩

	//�֐�
	void MoveBeast();//�r�[�X�g�ړ�

public:
	Beast(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetGround(const float& ground) { Ground_ = ground; }
	void SetPositionXY(const float& x, const float& y) { transform_.position_ = { x,y,0 }; }

	void RotBeast(const bool& isRight);//�r�[�X�g�̌�����ς���

	//�����ɓ�������
	//�����FpTarget ������������
	void OnCollision(GameObject* pTarget) override;
};

