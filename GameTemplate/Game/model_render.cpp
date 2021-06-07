#include "stdafx.h"
#include "model_render.h"

ModelRender::ModelRender()
{

}

ModelRender::~ModelRender()
{

}

bool ModelRender::Start()
{


	return true;
}

void ModelRender::Update()
{
	//����������
	if (m_finishInit == false) {
		return;
	}

	//���f���̍��W�X�V
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);

}

////////////////////////////////////////////////////////////
// ������
////////////////////////////////////////////////////////////

void ModelRender::Init(const char* filePath)
{
    //tkm�t�@�C���̃t�@�C���p�X��ێ�
    m_tkmFilePath = filePath;

	//���f���̏�����
	InitModel(filePath);

	//����������
	m_finishInit = true;
}

void ModelRender::InitModel(const char* filePath)
{
	//���C�g�̐ݒ������B
	//�f�B���N�V�������C�g������������
	InitDirectionLight();

	//�|�C���g���C�g������������
	//InitPointLight(light);

	//�X�|�b�g���C�g������������
	//InitSpotLight(light);

	//�A���r�G���g���C�g������������
	InitAmbientLight();

	//3D���f�������[�h���邽�߂̏���ݒ肷��
	//���f���̏��������邽�߂̏���ݒ�
	ModelInitData modelInitData;
	//tkm�t�@�C���̃t�@�C���p�X��ݒ�
	modelInitData.m_tkmFilePath = filePath;
	//�g�p����V�F�[�_�[�t�@�C���p�X��ݒ�
	modelInitData.m_fxFilePath = "Assets/shader/model.fx";
	//�f�B���N�V�������C�g�̏���萔�o�b�t�@�Ƃ��ăf�B�X�N���v�^�q�[�v��
	//�o�^���邽�߂Ƀ��f���̏��������Ƃ��ēn���B
	modelInitData.m_expandConstantBuffer = &m_light;
	modelInitData.m_expandConstantBufferSize = sizeof(m_light);
	//�����������g���ă��f���\������������������
	m_model.Init(modelInitData);
}

//////////////////////////////
// ���C�g���Ƃ̏�����
//////////////////////////////

void ModelRender::InitDirectionLight()
{
	//���C�g�͎΂ߏォ�瓖�����Ă���B
	m_light.dirDirection.x = 1.0f;
	m_light.dirDirection.y = -1.0f;
	m_light.dirDirection.z = -1.0f;
	//���K������B
	m_light.dirDirection.Normalize();
	//���C�g�̃J���[�̐ݒ�i���C�g�̋����j
	m_light.dirColor.x = 1.5f;
	m_light.dirColor.y = 1.5f;
	m_light.dirColor.z = 1.5f;

	//���_�̈ʒu��ݒ�
	m_light.eyePos = g_camera3D->GetPosition();
}


void ModelRender::InitPointLight()
{
	//�|�C���g���C�g�̏������W��ݒ肷��
	m_light.ptPosition.x = 0.0f;
	m_light.ptPosition.y = 50.0f;
	m_light.ptPosition.z = 50.0f;

	//�|�C���g���C�g�̏����J���[��ݒ肷��
	m_light.ptColor.x = 15.0f;
	m_light.ptColor.y = 0.0f;
	m_light.ptColor.z = 0.0f;

	//�|�C���g���C�g�̉e���͈͂�ݒ肷��
	m_light.ptRange = 100.0f;
}

void ModelRender::InitSpotLight()
{
	//�������W
	m_light.spPosition.x = 0.0f;
	m_light.spPosition.y = 50.0f;
	m_light.spPosition.z = -150.0f;
	//���C�g�̃J���[
	m_light.spColor.x = 10.0f;
	m_light.spColor.y = 200.0f;
	m_light.spColor.z = 10.0f;
	//���������͎΂߉��ɂ���B
	m_light.spDirection.x = 1.0f;
	m_light.spDirection.y = -1.0f;
	m_light.spDirection.z = 1.0f;
	//���K��
	m_light.spDirection.Normalize();
	//�ˏo�͈͂�300
	m_light.spRange = 300.0f;
	//�ˏo�p�x��25�x
	m_light.spAngle = Math::DegToRad(25.0f);
}

void ModelRender::InitAmbientLight()
{
	//����
	m_light.ambientLight.x = 0.1f;
	m_light.ambientLight.y = 0.1f;
	m_light.ambientLight.z = 0.1f;
}





void ModelRender::Render(RenderContext& renderContext)
{
	//����������
	if (m_finishInit == false) {
		return;
	}

	//���f���̕`��
	m_model.Draw(renderContext);
}