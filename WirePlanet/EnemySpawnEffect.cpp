#include "EnemySpawnEffect.h"

#include"CharacterFactory.h"
#include"CharacterController.h"
#include"Enemy.h"
#include"GraphicController2D.h"

using namespace std;

const int First_Fase_Time(0);
const int Second_Fase_Time(10);
const int Third_Fase_Time(10);
const int Forth_Fase_Time(10);
const int Fifth_Fase_Time(10);

const string Graph_File("spawn_hall.png");

EnemySpawnEffect::EnemySpawnEffect() :Effect(), cnt(0), param(0)
{
}


EnemySpawnEffect::~EnemySpawnEffect()
{
	DeleteGraph(ghandle);
}

int EnemySpawnEffect::Init(){
	ghandle = LoadGraph(Graph_File.c_str());
	if (ghandle == NULL){ return -1; }
	GetGraphSize(ghandle, &g_size.x, &g_size.y);
	spawn_enemy = static_pointer_cast<Enemy>(CharacterFactory::GetInstance().CreateObject(ENEMY, enemy_iid));
	if (spawn_enemy == nullptr){
		DeleteMe();
		return -1;
	}
	spawn_enemy->SetBelongingPlanet(myplanet);
	spawn_enemy->SetTarget(target);
	spawn_enemy->SetTR(t, r);
	if (size_w == 0){size_w = (int)(spawn_enemy->GetActionSize()* 1.4141356);}
	d_ext = (double)size_w / (double)g_size.x;
	return 0;
}

void EnemySpawnEffect::Update(){
	++cnt;
	if (cnt == First_Fase_Time + Second_Fase_Time + Third_Fase_Time){
		CharacterController::GetInstance().ResistEnemy(spawn_enemy);
	}
	if (cnt > First_Fase_Time + Second_Fase_Time + Third_Fase_Time + Forth_Fase_Time + Fifth_Fase_Time){ DeleteMe(); }
}

void EnemySpawnEffect::Draw(){
	int d_rota = (int)GetDrawRotation();
	if (cnt < First_Fase_Time){
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(255 * (1.0 - (double)cnt / 15.0)));
		gc2d::DrawCircle((int)position.x,(int)position.y,size_w / 2,GetColor(0,0,0),false);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	else if (cnt < First_Fase_Time+Second_Fase_Time){
		int l_cnt = cnt - First_Fase_Time;
		double rate = cos(DX_PI/2.0 * (double)l_cnt / (double)Second_Fase_Time);
//		DrawCircle((int)position.x, (int)position.y, (int)((double)size_w / 2.0 * rate), GetColor(0, 0, 0), false);
		gc2d::DrawCircle((int)position.x, (int)position.y, (int)((double)size_w / 2.0 * rate), GetColor(0, 0, 0), false);
	}
	else if (cnt < First_Fase_Time + Second_Fase_Time+Third_Fase_Time){
		int l_cnt = cnt - First_Fase_Time + Second_Fase_Time;
		double rate = sin(DX_PI / 2.0 * l_cnt / Third_Fase_Time);
//		DrawRotaGraph2F((float)position.x, (float)position.y, g_size.x / 2.0f, g_size.y / 2.0f, d_ext * rate, GetDrawRotation(), ghandle, true, false);
		gc2d::DrawRotaGraphF((float)position.x, (float)position.y, d_ext * rate, GetDrawRotation(), ghandle, true, false);
	}
	else if (cnt < First_Fase_Time + Second_Fase_Time + Third_Fase_Time+Forth_Fase_Time){
//		DrawRotaGraph2F((float)position.x, (float)position.y, g_size.x / 2.0f, g_size.y / 2.0f, d_ext, GetDrawRotation(), ghandle, true, false);
		gc2d::DrawRotaGraphF((float)position.x, (float)position.y, d_ext, GetDrawRotation(), ghandle, true, false);
	}
	else if (cnt < First_Fase_Time + Second_Fase_Time + Third_Fase_Time + Forth_Fase_Time+Fifth_Fase_Time){
		int l_cnt = cnt - First_Fase_Time + Second_Fase_Time + Third_Fase_Time + Forth_Fase_Time;
		double rate = cos(DX_PI / 2.0 * l_cnt / Fifth_Fase_Time);
//		DrawRotaGraph2F((float)position.x, (float)position.y, g_size.x / 2.0f, g_size.y / 2.0f, d_ext * rate, GetDrawRotation(), ghandle, true, false);
		gc2d::DrawRotaGraphF((float)position.x, (float)position.y, d_ext * rate, GetDrawRotation(), ghandle, true, false);
	}
}

void EnemySpawnEffect::SetSpawnEnemyInitID(const string& eiid){ enemy_iid = eiid; }
