#pragma once
#include "Engine/GameObject.h"

class Timer :
    public GameObject
{
	float currentTime_;//経過時間を保持する変数
public:
	Timer(GameObject* parent);
	void Initialize() override {};
	void Update() override {};
	void Draw() override {};
	void Release() override {};

	void ResetTime(const float& time) { currentTime_ = time; }//タイマーのリセット
	bool TimeElapsed(const float& time);//指定した秒数が経ったらtrueを返す
	//指定した秒数が経ったらtrueを返す
	//勝手にリセットされないタイマー
	bool NoResetTimeElapsed();
};

