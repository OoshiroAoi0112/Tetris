#pragma once

#include "InputControl.h"

/********************************
*マクロ定義
********************************/


/********************************
*型定義
********************************/


/********************************
*プロトタイプ定義
********************************/
void InputControl_Initialize(void);     //初期化処理
void InputControl_Update(void);         //更新処理

int GetButton(int button);              //押されているか判定処理
int GetButtonDown(int button);          //押した瞬間か判定処理
int GetButtonUp(int button);            //離したか判定処理

int GetExitButton(void);                //ゲーム終了用のボタン判定処理