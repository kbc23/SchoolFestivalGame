#include "stdafx.h"
#include "system/system.h"


//�f�B���N�V�������C�g�p�̍\���̂��`����
struct DirectionLight {
	Vector3 ligDirection; //���C�g�̕���
	//HLSL���̒萔�o�b�t�@��float3�^�̕ϐ���16�̔{���̃A�h���X�ɔz�u����邽�߁A
	//C++���ɂ̓p�f�B���O�𖄂߂Ă����B
	float pad;
	Vector3 ligColor;
	float pad1;

	//�\���̂Ɏ��_�̈ʒu��ǉ�����
	Vector3 eyePos; //���_�̈ʒu
};




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

	//���f���E���C�g�̎���

	//�f�B���N�V�������C�g�̃f�[�^���쐬����
	DirectionLight directionLig;
	//���C�g�͎΂ߏォ�瓖�����Ă���B
	directionLig.ligDirection.x = 1.0f;
	directionLig.ligDirection.y = -1.0f;
	directionLig.ligDirection.z = -1.0f;
	//���K������B
	directionLig.ligDirection.Normalize();
	//���C�g�̃J���[�̐ݒ�
	directionLig.ligColor.x = 5.0f;
	directionLig.ligColor.y = 5.0f;
	directionLig.ligColor.z = 5.0f;

	//���_�̈ʒu��ݒ�
	directionLig.eyePos = g_camera3D->GetPosition();

	//3D���f�������[�h���邽�߂̏���ݒ肷��
	//���f���̏��������邽�߂̏���ݒ�
	ModelInitData modelInitData;
	//tkm�t�@�C���̃t�@�C���p�X��ݒ�
	modelInitData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	//�g�p����V�F�[�_�[�t�@�C���p�X��ݒ�
	modelInitData.m_fxFilePath = "Assets/shader/model.fx";
	//�f�B���N�V�������C�g�̏���萔�o�b�t�@�Ƃ��ăf�B�X�N���v�^�q�[�v��
	//�o�^���邽�߂Ƀ��f���̏��������Ƃ��ēn���B
	modelInitData.m_expandConstantBuffer = &directionLig;
	modelInitData.m_expandConstantBufferSize = sizeof(directionLig);
	//�����������g���ă��f���\������������������
	Model charaModel;
	charaModel.Init(modelInitData);















	//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̃C���X�^���X���쐬����B
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();
	
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
		




		//���f���E���C�g�̎���					  
							  
		//���f���̃h���[�R�[�������s����
		charaModel.Draw(renderContext);





		//////////////////////////////////////
		//�G��`���R�[�h�������̂͂����܂ŁI�I�I
		//////////////////////////////////////
		g_engine->EndFrame();
	}
	//�Q�[���I�u�W�F�N�g�}�l�[�W���[���폜�B
	GameObjectManager::DeleteInstance();
	return 0;
}

