#include "stdafx.h"
#include "system/system.h"
#include "sound_engine.h"
#include "../../MiniEngine/time/Stopwatch.h"

#include "main_processing.h"
#include "constant.h"

//��Ƃ��I�������AGet�֐��ASet�֐���const�Q�ƂɕύX���Ă�������

//�W���Q�O�������t�@�N�^�����O�̒��ߐ؂�
//����ׂ�����
//�E���t�@�N�^�����O
//�E�u�g�D�[���V�F�[�_�[�v�̎���

namespace //constant
{
	const int NUMBER_OF_CONTROLLER = 4; //�R���g���[���[�̍ő�̐�
}



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
	for (int controllerNum = con::FIRST_ELEMENT_ARRAY; controllerNum < NUMBER_OF_CONTROLLER; controllerNum++) {
		g_pad[controllerNum]->Init(controllerNum);
	}

	//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̃C���X�^���X���쐬����B
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();
	SoundEngine::CreateInstance(); //�T�E���h�G���W��
	EffectEngine::CreateInstance(); //�G�t�F�N�g�G���W��

	//�X�g�b�v�E�H�b�`�𐶐�����
	Stopwatch stopWatch;

	MainProcessing* game = NewGO<MainProcessing>(igo::PRIORITY_CLASS, igo::CLASS_NAME_MAIN_PROCESSING);
	
	//////////////////////////////////////
	// ���������s���R�[�h�������̂͂����܂ŁI�I�I
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		//�X�g�b�v�E�H�b�`�̌v���J�n
		stopWatch.Start();

		//�����_�����O�J�n�B
		g_engine->BeginFrame();
		
		//////////////////////////////////////
		// ��������G��`���R�[�h���L�q����B
		//////////////////////////////////////
		
		GameObjectManager::GetInstance()->ExecuteUpdate();
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		//�G�t�F�N�g�G���W���̃A�b�v�f�[�g�B
		EffectEngine::GetInstance()->Update(g_gameTime->GetFrameDeltaTime());

		//�T�E���h�G���W���̃A�b�v�f�[�g
		SoundEngine::GetInstance()->Update();



		//�G�t�F�N�g�̃h���[�B
		EffectEngine::GetInstance()->Draw();

		//////////////////////////////////////
		// �G��`���R�[�h�������̂͂����܂ŁI�I�I
		//////////////////////////////////////
		g_engine->EndFrame();

		//FPS�Œ�̏���
		//�X�s�����b�N���s���B
		int restTime = 0;
		do {
			stopWatch.Stop();
			restTime = STOP_WATCH_CONST_DATA::MILLISECOND_FOR_LOCK_60FPS -
				static_cast<int>(stopWatch.GetElapsedMillisecond());
		} while (restTime > 0);

		//�X�g�b�v�E�H�b�`�̌v���I��
		stopWatch.Stop();

		//�f���^�^�C�����X�g�b�v�E�H�b�`�̌v�����Ԃ���A�v�Z����
		g_gameTime->PushFrameDeltaTime((float)stopWatch.GetElapsed());
	}

	DeleteGO(game);


	//�Q�[���I�u�W�F�N�g�}�l�[�W���[���폜�B
	PhysicsWorld::DeleteInstance();
	GameObjectManager::DeleteInstance();
	EffectEngine::DeleteInstance(); //�G�t�F�N�g�G���W��
	SoundEngine::DeleteInstance(); //�T�E���h�G���W��

	return 0;
}