#include "stdafx.h"
//#include "toon_map.h"
//
//void ToonMap::Init()
//{
//    constexpr const wchar_t* test = L"Assets/shader/toonMap.dds";
//
//    //�e�N�X�`����ǂݍ��݂܂�
//    DirectX::CreateDDSTextureFromFile(
//        g_d3dDevice, // D3D�f�o�C�X
//        test,  //�e�N�X�`���̃t�@�C���p�X
//        nullptr,   //nullptr�ł���
//        &m_srv  //�V�F�[�_�[���\�[�X�r���[
//    );
//}
//
//void ToonMap::SetShaderResourceView()
//{
//    //�s�N�Z���V�F�[�_�[�Ƀe�N�X�`����ݒ肵�܂��B
//    ID3D11ShaderResourceView* srvArray[]{
//            m_srv
//    };
//    g_d3dDeviceContext->PSSetShaderResources(4, 1, &m_srv);
//}