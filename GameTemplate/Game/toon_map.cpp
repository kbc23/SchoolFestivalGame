#include "stdafx.h"
//#include "toon_map.h"
//
//void ToonMap::Init()
//{
//    constexpr const wchar_t* test = L"Assets/shader/toonMap.dds";
//
//    //テクスチャを読み込みます
//    DirectX::CreateDDSTextureFromFile(
//        g_d3dDevice, // D3Dデバイス
//        test,  //テクスチャのファイルパス
//        nullptr,   //nullptrでいい
//        &m_srv  //シェーダーリソースビュー
//    );
//}
//
//void ToonMap::SetShaderResourceView()
//{
//    //ピクセルシェーダーにテクスチャを設定します。
//    ID3D11ShaderResourceView* srvArray[]{
//            m_srv
//    };
//    g_d3dDeviceContext->PSSetShaderResources(4, 1, &m_srv);
//}