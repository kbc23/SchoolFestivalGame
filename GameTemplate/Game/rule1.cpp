// ��x���s�����玀�S���[��

#include "stdafx.h"
#include "rule1.h"
#include "stage.h"

Rule1::Rule1()
{
}

Rule1::~Rule1()
{
}

/////////////////////////////////////////////////////////////////////////
// ���F�A�F�I�u�W�F�N�g�ɏ���Ă��܂������ɑ���s�\�ɂ���t���O�𗧂Ă�
/////////////////////////////////////////////////////////////////////////

bool Rule1::Start()
{
	m_stage = FindGO<Stage>(igo::CLASS_NAME_STAGE);
	m_stage->stop = true;

	return true;
}