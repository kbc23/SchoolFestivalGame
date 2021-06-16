#include "stdafx.h"
#include "game_camera.h"

GameCamera::GameCamera()
{
    g_camera3D->SetTarget({ 0.0f,0.0f,0.0f });
    g_camera3D->SetPosition({ 0.0f,500.0f,300.0f });
}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{


    return true;
}

void GameCamera::Update()
{

}