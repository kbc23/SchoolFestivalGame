/*!
 * @brief	シンプルなモデルシェーダー。
 */


////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////
//モデル用の定数バッファ
cbuffer ModelCb : register(b0)
{
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

//ディレクションライト用のデータを受け取る定数バッファーを用意する
cbuffer DirectionLightCb : register(b1)
{
	float3 ligDirection; //ライトの方向
	float3 ligColor;	 //ライトのカラー
	//視点のデータにアクセスするための変数を定数バッファーに追加する
	float3 eyePos;		 //視点の位置
}

////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////
//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn
{
	int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};
//頂点シェーダーへの入力。
struct SVSIn
{
	float4 pos 		: POSITION;		//モデルの頂点座標。
	float3 normal   : NORMAL;
	float2 uv 		: TEXCOORD0;	//UV座標。
	SSkinVSIn skinVert;				//スキン用のデータ。
};
//ピクセルシェーダーへの入力。
struct SPSIn
{
	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
	float3 normal	    : NORMAL;
	float2 uv 			: TEXCOORD0;	//uv座標。
	float3 worldPos		: TEXCOORD1;
};

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。
sampler g_sampler : register(s0);	//サンプラステート。

////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////

/// <summary>
//スキン行列を計算する。
/// </summary>
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
	float4x4 skinning = 0;	
	float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
        w += skinVert.Weights[i];
    }
    
    skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);
	
    return skinning;
}

/// <summary>
/// 頂点シェーダーのコア関数。
/// </summary>
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
	SPSIn psIn;
	float4x4 m;
	if( hasSkin ){
		m = CalcSkinMatrix(vsIn.skinVert);
	}else{
		m = mWorld;
	}

	//ここから下を書くっぽい

	psIn.pos = mul(m, vsIn.pos);
	psIn.worldPos = vsIn.pos;
	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);

	//頂点法線をピクセルシェーダーに渡す
	psIn.normal = mul(m, vsIn.normal);

	psIn.uv = vsIn.uv;

	return psIn;
}

/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, false);
}
/// <summary>
/// スキンありメッシュの頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSSkinMain( SVSIn vsIn ) 
{
	return VSMainCore(vsIn, true);
}
/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
	//ピクセルの法線とライトの方向の内積を計算する
	float t = dot(psIn.normal,ligDirection);

	//内積の結果が0以下なら0にする
	if (t < 0.0f) {
		t = 0.0f;
	}

	//ピクセルが受けているライトの光を求める
	float3 diffuseLig = ligColor * t;

	//反射ベクトルを求める
	float3 refVec = reflect(ligDirection, psIn.normal);

	//光が当たったサーフェイスから視点に伸びるベクトルを求める
	float3 toEye = eyePos - psIn.worldPos;
	//正規化する。
	toEye = normalize(toEye);

	//鏡面反射の強さを求める
	//dot関数を利用してrefVecとtoEyeの内積を求める。
	t = dot(refVec, toEye);
	//内積の結果はマイナスになるので、マイナスの場合は０にする。
	if (t < 0.0f) {
		t = 0.0f;
	}

	//鏡面反射の強さを絞る
	t = pow(t, 5.0f);

	//鏡面反射光を求める
	float3 specularLig = ligColor * t;

	//拡散反射光と鏡面反射光を足し算して、最終的な光を求める
	float3 lig = diffuseLig + specularLig;

	//ライトの効果を一律で底上げする
	lig.x += 0.8f;
	lig.y += 0.8f;
	lig.z += 0.8f;



	//最終
	float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);

	//最終出力カラーに光を乗算する
	albedoColor.xyz *= lig;

	return albedoColor;
}
