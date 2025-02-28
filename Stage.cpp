#include "Stage.h"
#include "Player.h"
#include "Gool.h"
#include "Beast.h"
#include "Vine.h"
#include "Flower.h"
#include "Engine/Image.h"
#include "Engine/Model.h"
#include "Engine/CsvReader.h"
#include "Engine/BoxCollider.h"
#include <string>
using std::string;

Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage")
{
	hBack_ = Image::Load("Back.png");
	assert(hBack_ >= 0);
}

void Stage::SetMap()
{
	if (!Map_.empty())
	{
		Map_.clear();
	}
	CsvReader csv;//データを読むクラスのインスタンスを作成
	bool ret = csv.Load("Stage2.csv");
	assert(ret);
	width_ = csv.GetWidth(0);//横幅を取得
	height_ = csv.GetHeight();//縦幅を取得

	Map_.resize(height_, vector<BrockData>(width_));//サイズを変更

	int wholeheight = height_;//全体の縦幅
	height_ = (wholeheight - 1) / 2;//マップの縦幅取得

	for (int h = 0; h < height_; h++)
	{
		for (int w = 0; w < width_; w++)
		{
			Map_[h][w].BrockID = csv.GetInt(w, h);
			switch (Map_[h][w].BrockID)
			{
			case 100:
			case 101:
			case 102:
			{
				Map_[h][w].boxColl = new BoxCollider({ (float)w,(float)-h,0 }, { 1,1,1 });
				AddCollider(Map_[h][w].boxColl);
			}
			break;
			}
		}
	}

	for (int h = 0; h < height_; h++)
	{
		for (int w = 0; w < width_; w++)
		{
			switch (csv.GetInt(w, h + height_ + 1))
			{
			case 0://プレイヤー
			{
				pplayer_ = GetParent()->FindGameObject<Player>();
				pplayer_->SetPositionXY(w, -h);
				pplayer_->SetGround(-h);
				break;
			}
			case 1://ビースト
			{
				Beast *b = Instantiate<Beast>(this);
				b->SetPositionXY(w, -h);
				b->SetGround(-h);
				break;
			}
			case 2://花
			{
				Flower* b = Instantiate<Flower>(this);
				b->SetPositionXY(w, -h);
				b->SetGround(-h);
				break;
			}
			case 20://ゴール
			{
				Gool* g = Instantiate<Gool>(this);
				g->SetPositionXY(w, -h);
				break;
			}
			case 32://ツタ
			{
				Vine* v = Instantiate<Vine>(this);
				v->SetPositionXY(w, -h);
				break;
			}
			break;
			}
		}
	}

	pplayer_->SetCameraStopandDeathGround_(-height_);
}

void Stage::Initialize()
{
	hBrocks_.push_back(Model::Load("Box.fbx"));
	hBrocks_.push_back(Model::Load("Box2.fbx"));
	hBrocks_.push_back(Model::Load("Box3.fbx"));


	for (int i = 0; i < hBrocks_.size(); i++)
	{
		assert(hBrocks_[i] >= 0);
	}
}

void Stage::Draw()
{
	Transform b;
	for (int i = 0; i < 3; i++)
	{
		b.scale_ = { 1.3,1.5,0 };
		Image::SetTransform(hBack_, b);
		Image::Draw(hBack_);
	}

	Transform t;
	
	for (float y = 0; y < height_; y++)
	{
		for (float x = 0; x < width_; x++)
		{
			switch (Map_[y][x].BrockID)
			{
			case 100:
			case 101:
			case 102:
			{
				t.position_ = { x,-y,0 };
				Model::SetTransform(hBrocks_[Map_[y][x].BrockID-100], t);
				Model::Draw(hBrocks_[Map_[y][x].BrockID - 100]);
				break;
			}
			default:
				break;
			}
		}
	}
}

void Stage::Release()
{
	hBrocks_.clear();
	StageboxColl_.clear();
	Map_.clear();

	SAFE_RELEASE(pplayer_);
}
