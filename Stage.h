#pragma once
#include "Engine/GameObject.h"
#include <vector>
using std::vector;
class FBX;
class Player;
class BoxCollider;

class Stage :
    public GameObject
{
	//�|�C���^
	Player* pplayer_;
	vector<BoxCollider*> StageboxColl_;

	//�n���h��
	vector<int> hBrocks_;//�u���b�N�̃n���h�������z��
	int hBack_;

	//���̑��̕ϐ�
	int width_; //�X�e�[�W��
	int height_;//�X�e�[�W����
	vector<vector<BrockData>> Map_;//�}�b�v�̃f�[�^�����z��

public:
	Stage(GameObject* parent);
	void Initialize() override;
	void Update() override {};
	void Draw() override;
	void Release() override;

	void SetMap();//�X�e�[�W�̐���

	int Getwidth() { return width_; }
	int Getheight() { return height_; }
	BoxCollider* GetboxColl(const int& x, const int& y){return Map_[x][y].boxColl; }
	int GetBrockID(const int& x, const int& y) { return Map_[x][y].BrockID; }
};

