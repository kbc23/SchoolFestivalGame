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

	//定数バッファーにポイントライト用の変数を追加
	float3 ptPosition;	 //ポイントライトの位置
	float3 ptColor;		 //ポイントライトのカラー
	float ptRange;		 //ポイントライトの影響範囲

	//スポットライトのデータにアクセスするための変数を追加する
	float3 spPosition;	//スポットライトの位置
	float3 spColor;		//スポットライトのカラー
	float spRange;		//スポットライトの射出範囲
	float3 spDirection;	//スポットライトの射出方向
	float spAngle;		//スポットライトの射出角度

	//視点のデータにアクセスするための変数を定数バッファーに追加する
	float3 eyePos;		 //視点の位置
	float3 ambientLight; //環境光
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

///////////////////////////////////////////
// 関数宣言
///////////////////////////////////////////
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal);
float3 CalcLigFromPointLight(SPSIn psIn);
float3 CalcLigFromDirectionLight(SPSIn psIn);

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
	psIn.worldPos = psIn.pos;
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
	//////////////////////////////
	// ディレクションライトによるLambert拡散反射光とPhong鏡面反射光の計算
	//////////////////////////////

	// ディレクションライトによるライティングを計算する
	float3 directionLig = CalcLigFromDirectionLight(psIn);

	//////////////////////////////
	// ディレクションライトによるLambert拡散反射光の計算
	//////////////////////////////

	//float3 diffDirection = CalcLambertDiffuse(ligDirection, ligColor, psIn.normal);

	
	////ピクセルの法線とライトの方向の内積を計算する
	//float t = dot(psIn.normal,ligDirection);

	////内積の結果が0以下なら0にする
	//if (t < 0.0f) {
	//	t = 0.0f;
	//}

	////ピクセルが受けているライトの光を求める
	//float3 diffuseLig = ligColor * t;


	//////////////////////////////
	// ディレクションライトによるPhong鏡面反射光の計算
	//////////////////////////////

	//float3 specDirection = CalcPhongSpecular(ligDirection, ligColor, psIn.worldPos, psIn.normal);


	////反射ベクトルを求める
	//float3 refVec = reflect(ligDirection, psIn.normal);

	////光が当たったサーフェイスから視点に伸びるベクトルを求める
	//float3 toEye = eyePos - psIn.worldPos;
	////正規化する。
	//toEye = normalize(toEye);

	////鏡面反射の強さを求める
	////dot関数を利用してrefVecとtoEyeの内積を求める。
	//t = dot(refVec, toEye);
	////内積の結果はマイナスになるので、マイナスの場合は０にする。
	//if (t < 0.0f) {
	//	t = 0.0f;
	//}

	////鏡面反射の強さを絞る
	//t = pow(t, 5.0f);

	////鏡面反射光を求める
	//float3 specularLig = ligColor * t;


	//////////////////////////////
	// ポイントライトによるLambert拡散反射光とPhong鏡面反射光の計算
	//////////////////////////////

	// ポイントライトによるライティングを計算する
	float3 pointLig = CalcLigFromPointLight(psIn);

	//// step-7 サーフェイスに入射するポイントライトの光の向きを計算する
	//float3 ligDir = psIn.worldPos - ptPosition;
	////正規化して大きさ１のベクトルにする。
	//ligDir = normalize(ligDir);

	//// step-8 減衰なしのLambert拡散反射光を計算する
	//float3 diffPoint = CalcLambertDiffuse(
	//	ligDir,			//ライトの方向
	//	ptColor,		//ライトのカラー
	//	psIn.normal		//サーフェイスの法線
	//);

	//// step-9 減衰なしのPhong鏡面反射光を計算する
	//float3 specPoint = CalcPhongSpecular(
	//	ligDir,			//ライトの方向
	//	ptColor,		//ライトのカラー
	//	psIn.worldPos,	//サーフェイスのワールド座標
	//	psIn.normal		//サーフェイスの法線
	//);

	//// step-10 距離による影響率を計算する
	////ポイントライトとの距離を計算する。
	//float3 distance = length(psIn.worldPos - ptPosition);
	////影響率を距離に比例して小さくする。
	//float affect = 1.0f - 1.0f / ptRange * distance;
	////影響力がマイナスにならないように補正をかける。
	//if (affect < 0.0f) {
	//	affect = 0.0f;
	//}
	////影響の仕方を指数関数的にする。
	//affect = pow(affect, 3.0f);

	//// step-11 拡散反射光と鏡面反射光に影響率を乗算して影響を弱める
	//diffPoint *= affect;
	//specPoint *= affect;

	// step-12 2つの反射光を合算して最終的な反射光を求める
	//float3 diffuseLig = diffPoint + diffDirection;
	//float3 specularLig = specPoint + specDirection;

	//
	//float3 diffuseLig = diffDirection;
	//float3 specularLig = specDirection;



	// スポットライトによるライティングを計算する
	// ほとんどポイントライトと同じ

	// step-6 サーフェイスに入射するスポットライトの光の向きを計算する
	//ピクセルの座標 - スポットライトの座標を計算
	float3 ligDir = psIn.worldPos - spPosition;
	//正規化
	ligDir = normalize(ligDir);

	// step-7 減衰なしのLambert拡散反射光を計算する
	float3 diffSpotLight = CalcLambertDiffuse(
		ligDir,		//ライトの方向
		spColor,	//ライトのカラー
		psIn.normal	//サーフェイスの法線
	);

	// step-8 減衰なしのPhong鏡面反射光を計算する
	float3 specSpotLight = CalcPhongSpecular(
		ligDir,
		spColor,
		psIn.worldPos,
		psIn.normal
	);

	// step-9 距離による影響率を計算する
	//スポットライトとの距離を計算する。
	float3 distance = length(psIn.worldPos - spPosition);

	//影響率は距離に比例して小さくなっていく。
	float affect = 1.0f - 1.0f / spRange * distance;
	//影響力がマイナスにならないように補正をかける。
	if (affect < 0.0f) {
		affect = 0.0f;
	}
	//影響の仕方を指数関数的にする。
	affect = pow(affect, 3.0f);

	// step-10 影響率を乗算して反射光を弱める
	diffSpotLight *= affect;
	specSpotLight *= affect;

	// step-11 入射光と射出方向の角度を求める
	//内積を求める。
	float angle = dot(ligDir, spDirection);
	//角度を求める。
	angle = acos(angle);

	// step-12 角度による影響率を求める
	//角度に比例して小さくなっていく影響率を計算
	affect = 1.0f - 1.0f / spAngle * angle;
	//影響力がマイナスにならないように補正をかける。
	if (affect < 0.0f) {
		affect = 0.0f;
	}
	//影響の仕方を指数関数的にする。
	affect = pow(affect, 0.5f);

	// step-13 角度による影響率を反射光に乗算して、影響を弱める
	diffSpotLight *= affect;
	specSpotLight *= affect;


	//////////////////////////////
	// 最後の仕上げ
	//////////////////////////////

	

	//拡散反射光と鏡面反射光を足し算して、最終的な光を求める
	//float3 lig = diffuseLig + specularLig;

	////ライトの効果を一律で底上げする
	//lig.x += 0.8f;
	//lig.y += 0.8f;
	//lig.z += 0.8f;

	float3 finalLig = directionLig + pointLig + ambientLight;

	//スポットライトの反射光を最終的な反射光に足し算する
	finalLig += diffSpotLight + specSpotLight;

	float4 finalColor = g_albedo.Sample(g_sampler, psIn.uv);

	//最終出力カラーに光を乗算する
	finalColor.xyz *= finalLig;

	return finalColor;
}

/// <summary>
/// Lambert拡散反射光を計算する
/// </summary>
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
	// ピクセルの法線とライトの方向の内積を計算する
	float t = dot(normal, lightDirection);

	// 内積の値を0以上の値にする
	t = max(0.0f, t);

	// 拡散反射光を計算する
	return lightColor * t;
}

/// <summary>
/// Phong鏡面反射光を計算する
/// </summary>
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal)
{
	// 反射ベクトルを求める
	float3 refVec = reflect(lightDirection, normal);

	// 光が当たったサーフェイスから視点に伸びるベクトルを求める
	float3 toEye = eyePos - worldPos;
	toEye = normalize(toEye);

	// 鏡面反射の強さを求める
	float t = dot(refVec, toEye);

	// 鏡面反射の強さを0以上の数値にする
	t = max(0.0f, t);

	// 鏡面反射の強さを絞る
	t = pow(t, 5.0f);

	// 鏡面反射光を求める
	return lightColor * t;
}

/// <summary>
/// ポイントライトによる反射光を計算
/// </summary>
/// <param name="psIn">ピクセルシェーダーに渡されている引数</param>
float3 CalcLigFromPointLight(SPSIn psIn)
{
	//サーフェイスに入射するポイントライトの光の向きを計算する
	float3 ligDir = psIn.worldPos - ptPosition;
	//正規化して大きさ１のベクトルにする。
	ligDir = normalize(ligDir);

	//減衰なしのLambert拡散反射光を計算する
	float3 diffPoint = CalcLambertDiffuse(
		ligDir,			//ライトの方向
		ptColor,		//ライトのカラー
		psIn.normal		//サーフェイスの法線
	);

	//減衰なしのPhong鏡面反射光を計算する
	float3 specPoint = CalcPhongSpecular(
		ligDir,			//ライトの方向
		ptColor,		//ライトのカラー
		psIn.worldPos,	//サーフェイスのワールド座標
		psIn.normal		//サーフェイスの法線
	);

	//距離による影響率を計算する
	//ポイントライトとの距離を計算する。
	float3 distance = length(psIn.worldPos - ptPosition);
	//影響率を距離に比例して小さくする。
	float affect = 1.0f - 1.0f / ptRange * distance;
	//影響力がマイナスにならないように補正をかける。
	if (affect < 0.0f) {
		affect = 0.0f;
	}
	//影響の仕方を指数関数的にする。
	affect = pow(affect, 3.0f);

	//拡散反射光と鏡面反射光に影響率を乗算して影響を弱める
	diffPoint *= affect;
	specPoint *= affect;

	return diffPoint + specPoint;
}

/// <summary>
/// ディレクションライトによる反射光を計算
/// </summary
/// <param name="psIn">ピクセルシェーダーからの入力。</param>
float3 CalcLigFromDirectionLight(SPSIn psIn)
{
	// ディレクションライトによるLambert拡散反射光を計算する
	float3 diffDirection = CalcLambertDiffuse(ligDirection, ligColor, psIn.normal);

	// ディレクションライトによるPhong鏡面反射光を計算する
	float3 specDirection = CalcPhongSpecular(ligDirection, ligColor, psIn.worldPos, psIn.normal);

	return diffDirection + specDirection;
}