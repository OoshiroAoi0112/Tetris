#include "SceneManager.h"
#include "TitleScene.h"
#include "GameMainScene.h"
#include "RankingScene.h"
#include "EndScene.h"
#include "DxLib.h"

/********************************
*マクロ定義
********************************/

/********************************
*型定義
********************************/

/********************************
*変数宣言
********************************/
GAME_MODE game_mode;        //ゲームモード情報
GAME_MODE next_mode;        //次のゲームモード
int read_error;             //読み込みエラー確認

/********************************
*プロトタイプ宣言
********************************/

/********************************
*シーン管理機能：初期化処理
* 引　数：ゲームモード情報
* 戻り値：なし
********************************/
void SceneManager_Initialize(GAME_MODE mode)
{
	read_error = D_NORMALITY;
	switch (mode)
	{
		case E_TITLE:
			read_error = TitleScene_Initialize();          //タイトル画面の初期化
			break;
		case E_GAMEMAIN:
			read_error = GameMainScene_Initialize();       //ゲームメイン画面の初期化
			break;
		case E_RANKING:
			if (game_mode == E_GAMEMAIN)
			{
				Set_RankingMode(RANKING_INPUT_MODE);       //ランキング入力モードで起動
				Set_RankingScore(Get_Score());             //スコアの取得
			}
			else
			{
				Set_RankingMode(RANKING_DISP_MODE);        //ランキング描画モードで起動
			}
			read_error = RankingScene_Initialize();        //ランキング画面の初期化
			break;
		case E_END:
		default:
			EndScene_Initialize();                         //エンド画面の初期化
			break;
	}
	game_mode = mode;
	next_mode = game_mode;
}

/********************************
*シーン管理機能：更新処理
* 引　数：なし
* 戻り値：なし
********************************/
void SceneManager_Update(void)
{
	//前フレームとゲームモードが違っていたらシーンを切り替える
	if (game_mode != next_mode)
	{
		SceneManager_Initialize(next_mode);
	}

	//各画面の更新処理
	switch (game_mode)
	{
		case E_TITLE:
			TitleScene_Update();
			break;
		case E_GAMEMAIN:
			GameMainScene_Update();
			break;
		case E_RANKING:
			RankingScene_Update();
			break;
		case E_END:
		default:
			EndScene_Update();
			break;
	}
}

/********************************
*シーン管理機能：描画処理
* 引　数：なし
* 戻り値：なし
********************************/
void SceneManager_Draw(void)
{
	//各画面の描画処理
	switch (game_mode)
	{
		case E_TITLE:
			TitleScene_Draw();
			break;
		case E_GAMEMAIN:
			GameMainScene_Draw();
			break;
		case E_RANKING:
			RankingScene_Draw();
			break;
		case E_END:
		default:
			EndScene_Draw();
			break;
	}
}

/********************************
*シーン管理機能：シーン変更処理
* 引　数：変更するゲームモード
* 戻り値：なし
********************************/
void Change_Scene(GAME_MODE mode)
{
	next_mode = mode;
}

/********************************
*シーン管理機能：エラーチェック処理
* 引　数：なし
* 戻り値：エラー情報
********************************/
int ErrorCheck(void)
{
	if (Get_EndTime() == TRUE)
	{
		read_error = D_ERROR;
	}
	return read_error;
}

