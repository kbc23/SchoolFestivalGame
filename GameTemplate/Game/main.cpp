#include "stdafx.h"
#include "system/system.h"


//�f�B���N�V�������C�g�p�̍\���̂��`����
struct Light {
	Vector3 ligDirection; //���C�g�̕���
	//HLSL���̒萔�o�b�t�@��float3�^�̕ϐ���16�̔{���̃A�h���X�ɔz�u����邽�߁A
	//C++���ɂ̓p�f�B���O�𖄂߂Ă����B
	float pad;
	Vector3 ligColor;
	float pad1;

	//���C�g�\���̂Ƀ|�C���g���C�g�p�̃����o�ϐ���ǉ�����
	Vector3 ptPosition; //�ʒu
	float pad2;			//�p�f�B���O
	Vector3 ptColor;	//�J���[
	float ptRange;		//�e���͈�

	//���C�g�\���̂ɃX�|�b�g���C�g�p�̃����o�ϐ���ǉ�
	Vector3 spPosition;		//�ʒu
	float pad3;				//�p�f�B���O
	Vector3 spColor;		//�J���[
	float spRange;			//�e���͈�
	Vector3 spDirection;	//�ˏo����
	float spAngle;			//�ˏo�p�x

	//�\���̂Ɏ��_�̈ʒu��ǉ�����
	Vector3 eyePos; //���_�̈ʒu
	float pad4;

	Vector3 ambientLight;   // �A���r�G���g���C�g
};

//////////////////////////////////////
//�֐��錾
//////////////////////////////////////
void InitDirectionLight(Light& light);
void InitPointLight(Light& light);
void InitAmbientLight(Light& light);



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
	Light light;
	// �f�B���N�V�������C�g������������
	InitDirectionLight(light);
	// �|�C���g���C�g������������
	//InitPointLight(light);
	// �A���r�G���g���C�g������������
	InitAmbientLight(light);

	//�X�|�b�g���C�g�̃f�[�^������������
	//�������W
	light.spPosition.x = 0.0f;
	light.spPosition.y = 0.0f;
	light.spPosition.z = -50.0f;
	//���C�g�̃J���[
	light.spColor.x = 200.0f;
	light.spColor.y = 10.0f;
	light.spColor.z = 10.0f;
	//���������͎΂߉��ɂ���B
	light.spDirection.x = 1.0f;
	light.spDirection.y = -1.0f;
	light.spDirection.z = 1.0f;
	//���K��
	light.spDirection.Normalize();
	//�ˏo�͈͂�300
	light.spRange = 300.0f;
	//�ˏo�p�x��25�x
	light.spAngle = Math::DegToRad(25.0f);




	//3D���f�������[�h���邽�߂̏���ݒ肷��
	//���f���̏��������邽�߂̏���ݒ�
	ModelInitData modelInitData;
	//tkm�t�@�C���̃t�@�C���p�X��ݒ�
	modelInitData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	//�g�p����V�F�[�_�[�t�@�C���p�X��ݒ�
	modelInitData.m_fxFilePath = "Assets/shader/model.fx";
	//�f�B���N�V�������C�g�̏���萔�o�b�t�@�Ƃ��ăf�B�X�N���v�^�q�[�v��
	//�o�^���邽�߂Ƀ��f���̏��������Ƃ��ēn���B
	modelInitData.m_expandConstantBuffer = &light;
	modelInitData.m_expandConstantBufferSize = sizeof(light);
	//�����������g���ă��f���\������������������
	Model charaModel;
	charaModel.Init(modelInitData);




	//ModelInitData lightModelInitData;
	//lightModelInitData.m_tkmFilePath = "Assets/modelData/light.tkm";

	//// �g�p����V�F�[�_�[�t�@�C���p�X��ݒ肷��
	//lightModelInitData.m_fxFilePath = "Assets/shader/light.fx";
	//lightModelInitData.m_expandConstantBuffer = &light;
	//lightModelInitData.m_expandConstantBufferSize = sizeof(light);

	//// �����������g���ă��f��������������
	//Model lightModel;
	//lightModel.Init(lightModelInitData);










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
		
		//light.ptPosition.x -= g_pad[0]->GetLStickXF();
		//if (g_pad[0]->IsPress(enButtonB)) {
		//	light.ptPosition.y += g_pad[0]->GetLStickXF();
		//}
		//else {
		//	light.ptPosition.z -= g_pad[0]->GetLStickYF();
		//}
		light.spPosition.x -= g_pad[0]->GetLStickXF();
		if (g_pad[0]->IsPress(enButtonB)) {
			light.spPosition.y += g_pad[0]->GetLStickYF();
		}
		else {
			light.spPosition.z -= g_pad[0]->GetLStickYF();
		}

		Quaternion qRotY;
		qRotY.SetRotationY(g_pad[0]->GetRStickXF() * 0.01f);
		qRotY.Apply(light.spDirection);
		Vector3 rotAxis;
		rotAxis.Cross(g_vec3AxisY, light.spDirection);
		Quaternion qRotX;
		qRotX.SetRotation(rotAxis, g_pad[0]->GetLStickYF() * 0.01f);
		qRotX.Apply(light.spDirection);



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

void InitDirectionLight(Light& light)
{
	//���C�g�͎΂ߏォ�瓖�����Ă���B
	light.ligDirection.x = 1.0f;
	light.ligDirection.y = -1.0f;
	light.ligDirection.z = -1.0f;
	//���K������B
	light.ligDirection.Normalize();
	//���C�g�̃J���[�̐ݒ�
	light.ligColor.x = 5.0f;
	light.ligColor.y = 5.0f;
	light.ligColor.z = 5.0f;

	//���_�̈ʒu��ݒ�
	light.eyePos = g_camera3D->GetPosition();
}

void InitPointLight(Light& light)
{
	//�|�C���g���C�g�̏������W��ݒ肷��
	light.ptPosition.x = 0.0f;
	light.ptPosition.y = 50.0f;
	light.ptPosition.z = 50.0f;

	//�|�C���g���C�g�̏����J���[��ݒ肷��
	light.ptColor.x = 15.0f;
	light.ptColor.y = 0.0f;
	light.ptColor.z = 0.0f;

	//�|�C���g���C�g�̉e���͈͂�ݒ肷��
	light.ptRange = 100.0f;
}

void InitAmbientLight(Light& light)
{
	//����
	light.ambientLight.x = 0.8f;
	light.ambientLight.y = 0.8f;
	light.ambientLight.z = 0.8f;
}