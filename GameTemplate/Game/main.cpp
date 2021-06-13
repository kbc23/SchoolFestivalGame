#include "stdafx.h"
#include "system/system.h"

#include "game.h"


//�f�B���N�V�������C�g�p�̍\���̂��`����
//struct Light {
//	//�f�B���N�V�������C�g
//	Vector3 dirDirection; //���C�g�̕���
//	//HLSL���̒萔�o�b�t�@��float3�^�̕ϐ���16�̔{���̃A�h���X�ɔz�u����邽�߁A
//	//C++���ɂ̓p�f�B���O�𖄂߂Ă����B
//	float pad;
//	Vector3 dirColor;
//	float pad1;
//
//	//���C�g�\���̂Ƀ|�C���g���C�g�p�̃����o�ϐ���ǉ�����
//	Vector3 ptPosition; //�ʒu
//	float pad2;			//�p�f�B���O
//	Vector3 ptColor;	//�J���[
//	float ptRange;		//�e���͈�
//
//	//���C�g�\���̂ɃX�|�b�g���C�g�p�̃����o�ϐ���ǉ�
//	Vector3 spPosition;		//�ʒu
//	float pad3;				//�p�f�B���O
//	Vector3 spColor;		//�J���[
//	float spRange;			//�e���͈�
//	Vector3 spDirection;	//�ˏo����
//	float spAngle;			//�ˏo�p�x
//
//	//�\���̂Ɏ��_�̈ʒu��ǉ�����
//	Vector3 eyePos; //���_�̈ʒu
//	float pad4;
//
//	//����
//	Vector3 ambientLight;   // �A���r�G���g���C�g
//	float pad5;
//
//	//�n�ʐF�ƓV���F�A�n�ʂ̖@����ǉ�����
//	Vector3 groundColor;	//�n�ʐF
//	float pad6;
//	Vector3 skyColor;		//�V���F
//	float pad7;
//	Vector3 groundNormal;	//�n�ʂ̖@��
//};

//////////////////////////////////////
//�֐��錾
//////////////////////////////////////
//void InitDirectionLight(Light& light);
//void InitPointLight(Light& light);
//void InitSpotLight(Light& light);
//void InitAmbientLight(Light& light);



///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//////////////////////////////////////
	// �������珉�������s���R�[�h���L�q����B
	//////////////////////////////////////

	//�R���g���[���[�̏�����
	for (int i = 0; i < 4; i++) {
		g_pad[i]->Init(i);
	}



	//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̃C���X�^���X���쐬����B
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();

	Game* game = NewGO<Game>(0);
	
	//////////////////////////////////////
	// ���������s���R�[�h�������̂͂����܂ŁI�I�I
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		//�����_�����O�J�n�B
		g_engine->BeginFrame();
		

		//////////////////////////////////////
		//��������G��`���R�[�h���L�q����B
		//////////////////////////////////////
		
		GameObjectManager::GetInstance()->ExecuteUpdate();
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);




		//////////////////////////////////////
		//�G��`���R�[�h�������̂͂����܂ŁI�I�I
		//////////////////////////////////////
		g_engine->EndFrame();
	}

	DeleteGO(game);


	//�Q�[���I�u�W�F�N�g�}�l�[�W���[���폜�B
	GameObjectManager::DeleteInstance();
	return 0;
}