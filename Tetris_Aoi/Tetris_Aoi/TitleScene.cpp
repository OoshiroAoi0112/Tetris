#include "TitleScene.h"
#include "InputControl.h"
#include "SceneManager.h"
#include "DxLib.h"

/********************************
*マクロ定義
********************************/

/********************************
*型定義
********************************/
enum
{
	E_TITLE_BGM,
	E_TITLE_SE_CURSOR,
	E_TITLE_SE_SELECT,
	E_SOUND_MAX
};

/********************************
*グローバル変数宣言
********************************/
int cursor_number;
int sounds[E_SOUND_MAX];

/********************************
*プロトタイプ宣言
********************************/

/********************************
*タイトル画面：初期化処理
* 引　数：なし
* 戻り値：エラー情報（-1:異常有,-1:正常終了）
********************************/
int TitleScene_Initialize(void)
{
	int ret = 0;

	cursor_number = 0;

	sounds[E_TITLE_BGM] = LoadSoundMem("sounds/BGM041.ogg");
	sounds[E_TITLE_SE_CURSOR] = LoadSoundMem("sounds/SE1.mp3");
	sounds[E_TITLE_SE_SELECT] = LoadSoundMem("sounds/SE2.mp3");

	ChangeVolumeSoundMem(120, sounds[E_TITLE_SE_CURSOR]);
	ChangeVolumeSoundMem(80, sounds[E_TITLE_SE_SELECT]);
	for (int i = 0; i < E_SOUND_MAX; i++)
	{
		if (sounds[i] == -1)
		{
			ret = -1;
			break;
		}
	}return ret;
}

/********************************
*タイトル画面：更新処理
* 引　数：なし
* 戻り値：なし
********************************/
void TitleScene_Update(void)
{
	//BGMが流れてないときに再生
	if (CheckSoundMem(sounds[E_TITLE_BGM]) != TRUE)
	{
		PlaySoundMem(sounds[E_TITLE_BGM], DX_PLAYTYPE_BACK, FALSE);
	}

	if (GetButtonDown(XINPUT_BUTTON_B) == TRUE)
	{
		StopSoundMem(sounds[E_TITLE_BGM]);
		PlaySoundMem(sounds[E_TITLE_SE_SELECT], DX_PLAYTYPE_BACK, FALSE);
		switch (cursor_number)
		{
			case 0:
				Change_Scene(E_GAMEMAIN);
				break;
			case 1:
				Change_Scene(E_RANKING);
				break;
			case 2:
			default:
				Change_Scene(E_END);
				break;
		}
	}
	if (GetButtonDown(XINPUT_BUTTON_DPAD_UP) == TRUE)
	{
		PlaySoundMem(sounds[E_TITLE_SE_CURSOR], DX_PLAYTYPE_NORMAL, FALSE);
		if (cursor_number <= 0)
		{
			cursor_number = 0;
		} 
		else
		{
			cursor_number--;
		}
	}
	if (GetButtonUp(XINPUT_BUTTON_DPAD_DOWN) == TRUE)
	{
		PlaySoundMem(sounds[E_TITLE_SE_CURSOR], DX_PLAYTYPE_NORMAL, FALSE);
		if (cursor_number >= 2)
		{
			cursor_number = 2;
		}
		else
		{
			cursor_number++;
		}
	}
}

/********************************
*タイトル画面：描画処理
* 引　数：なし
* 戻り値：なし
********************************/
void TitleScene_Draw(void)
{
	SetFontSize(50);
	DrawString(100, 100, "テトリス", GetColor(255, 255, 255));
	DrawString(300, 300, "スタート", GetColor(255, 255, 255));
	DrawString(300, 350, "ランキング", GetColor(255, 255, 255));
	DrawString(300, 400, "エンド", GetColor(255, 255, 255));
	SetFontSize(20);

	DrawCircle(275, 325 + (cursor_number * 50), 15, GetColor(255, 0, 0));
}



