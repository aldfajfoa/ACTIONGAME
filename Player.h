#pragma once
#include "Engine/GameObject.h"
#include "Engine/Transform.h"
class FBX;
class BoxCollider;
class Stage;
class HP;
class Arrow;
class Timer;

const int PBOX_NUM = 5;
const int ARROW_NUM = 5;

class Player :
	public GameObject
{
	//�萔
	const int P_RIGHT_ANGLE = 90;
	const int P_LEFT_ANGLE = 360 - P_RIGHT_ANGLE;
	const float P_JUMP_HEIGHT = 1.0f * 10.0f;//�W�����v�̍���
	const float GRAVITY = 9.0f / 650.0f;    //�d�͉����x
	const float P_MOVE_SPEED = 0.05;//�ړ����x

	//�|�C���^
	BoxCollider* PboxColl_[PBOX_NUM];
	Arrow* arrows_[ARROW_NUM];
	Stage* stage_;
	HP* hp_;
	Timer* timer_;
	Timer* ArrowInterval_;

	//�n���h��
	int hPlayer_;

	//���̑��̕ϐ�
	bool isRight_;//�E�������ǂ���
	float jumpSpeed_;//�W�����v�̍���
	int Ground_;//�n��
	float CameraStopGround_;//�����艺�ɂ����ƃJ�������v���C���[��ǂ�Ȃ��Ȃ�
	float DeathGround_;//�����܂ŗ�������Q�[���I�[�o�[
	bool isLeftMove_, isRightMove_, isJump_;//���E�ړ��ƃW�����v���ł��邩�̔���
	bool isPdamage_[4];//���̂ǂꂩ��true�̊Ԃ̓_���[�W���󂯂�
	bool isfirst_;//�_���[�W���󂯂�u���b�N�ɐG�ꂽ�u�Ԃ��ǂ���
	int currentArrow_;
	float Dist_;
	int Dir_;

	//�֐�
	void MovePlayer();//�v���C���[�ړ�
	void DiscernBrock(const int& x, const int& y, const Dir& dir);
public:

	Player(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	//�����ɓ�������
	//�����FpTarget ������������
	void OnCollision(GameObject* pTarget) override;

	void SetGround(const float& ground) { Ground_ = ground; }
	void SetPositionXY(const float& x, const float& y) { transform_.position_ = { x,y,0 }; }//�v���C���[�̃|�W�V����
	//CameraStopGround_��ݒ肵���������DeathGround_�̐ݒ������
	void SetCameraStopandDeathGround_(const float& Ground) { CameraStopGround_ = Ground;
		                                                     DeathGround_ = CameraStopGround_ - 20;}
	void SetisLeftMove(bool isLeftMove) { isLeftMove_ = isLeftMove; }
	void SetisRightMove(bool isRightMove) { isRightMove_ = isRightMove; }
	void setDistandDir(float dist, float dir) {
		Dist_ = dist;
		Dir_ = dir;
	}

	XMFLOAT3 GetPosition() { return transform_.position_; }
	bool GetisRight_() { return isRight_; }
	int GetARROW_NUM() { return ARROW_NUM; }
	Arrow* Getarrows(const int& index) { return arrows_[index]; }
	BoxCollider* GetCenterBoxColl() { return PboxColl_[CENTER]; }//�v���C���[�̒��S�����蔻����擾
	BoxCollider* GetPBoxColl(Dir dir) { return PboxColl_[dir]; }

	void RotPlayer(const bool& isRight);//�v���C���[�̌�����ς���
	void BlowawayPlayer();//�v���C���[�𐁂���΂�
};

