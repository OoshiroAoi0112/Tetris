/**************************************************
*プログラミング実習（テトリス編）
**************************************************/
#include "DxLib.h"
#include "FreamControl.h"
#include "SceneManager.h"
#include "InputControl.h"

/********************************
*マクロ定義
********************************/
#define SCREEN_HEIGHT    (720)
#define SCREEN_WIDTH     (1200)
#define SCREEN_COLORBIT  (32)
#define FONT_SIZE        (20)

/********************************
*型定義
********************************/

/********************************
*グローバル変数宣言
********************************/

/********************************
*プロトタイプ宣言
********************************/

/********************************
*プログラムの開始
********************************/

int WINAPI WinMain(_In_ HINSTANCE hInstace, _In_opt_ HINSTANCE hPrevInstance, _In_
	LPSTR lpCmdLine, _In_ int nShowCmd)
{
	//タイトルの変更
	SetMainWindowText("テトリス");

	//ウィンドウモードで起動
	ChangeWindowMode(TRUE);

	//ウィンドウサイズの決定
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_COLORBIT);

	//Dxライブラリの初期化処理
	if (DxLib_Init() == -1)
	{
		//エラー終了
		return -1;
	}

	//フレーム制御機能初期化処理
	FreamControl_Initialize();

	//シーン管理機能初期化処理
	SceneManager_Initialize(E_TITLE);

	//入力制御機能初期化処理
	InputControl_Initialize();

	//描画先画面を裏にする
	SetDrawScreen(DX_SCREEN_BACK);

	//文字サイズを設定
	SetFontSize(FONT_SIZE);

	//ゲームループ
	while (ProcessMessage() == 0 && GetExitButton() != TRUE && ErrorCheck() ==
		D_NORMALITY)
	{
		//フレーム制御機能更新処理
		FreamControl_Update();

		//画面の初期化
		ClearDrawScreen();

		//入力制御機能更新処理
		InputControl_Update();

		//シーン管理機能更新処理
		SceneManeger_Update();

		//シーン管理機能描画処理
		SceneManager_Draw();

		//裏画面の内容を表画面に反映
		ScreenFlip();
	}
	//Dxライブラリ使用の終了処理
	DxLib_End();

	//ソフトの終了
	return 0;
}
