#include "Block.h"
#include "DxLib.h"
#include "InputControl.h"

/********************************
*マクロ定義
********************************/
#define FIELD_HEIGHT                (21)
#define FIELD_WDTH                  (12)
#define BLOCK_TROUT_SIZE            (4)
#define BLOCK_SIZE                  (36)
#define BLOCK_TYPE_MAX              (7)
#define BLOCK_NEXT_POS_X            (700)
#define BLOCK_NEXT_POS_Y            (500)
#define BLOCK_STOCK_POS_X           (500)
#define BLOCK_STOCK_POS_Y           (350)
#define DROP_BLOCK_INIT_X           (4)
#define DROP_BLOCK_INIT_Y           (-1)
#define DROP_SPEED                  (60)
#define TURN_CROCKWICE              (0)
#define TURN_ANITICROCKWICE         (1)

/********************************
*マクロ定義
********************************/
enum BLOCK_STATE
{
	E_BLOCK_EMPTY,              //空ブロック
	E_BLOCK_LIGHT_BLUE,         //水色
	E_BLOCK_YELLOW_GREEN,       // 黄緑
	E_BLCOK_YELLOW,             //黄色
	E_BLOCK_ORANGE,             //オレンジ
	E_BLOCK_BLUE,               //青
	E_BLOCK_PINK,               //ピンク
	E_BLOCK_RED,                //赤
	E_BLOCK_GLAY,               //灰色
	E_BLOCK_WALL,               //壁
	E_BLOCK_IMAGE_MAX,
};
/********************************
*定数定義
********************************/
const int C_BLOCK_TABLE[BLOCK_TYPE_MAX][BLOCK_TROUT_SIZE][BLOCK_TROUT_SIZE] =
{
	{
		{0,0,0,0},
		{0,1,1,0},
		{0,1,1,0},
		{0,0,0,0}
	},

	{
		{0,0,0,0},
		{0,0,0,0},
		{2,2,2,2},
		{0,0,0,0}
	},

	{
		{0,0,0,0},
		{3,0,0,0},
		{3,3,3,0},
		{0,0,0,0}
	},

	{
		{0,0,0,0},
		{0,0,0,4},
		{0,4,4,4},
		{0,0,0,0}
	},

	{
		{0,0,0,0},
		{0,5,5,0},
		{0,0,5,5},
		{0,0,0,0}
	},

	{
		{0,0,0,0},
		{0,6,6,0},
		{6,6,0,0},
		{0,0,0,0}
	},

	{
		{0,0,0,0},
		{0,7,0,0},
		{7,7,7,0},
		{0,0,0,0}
	},
};

/********************************
*変数宣言
********************************/
int BlockImage[E_BLOCK_IMAGE_MAX];                             //ブロック画像
BLOCK_STATE Field[FIELD_HEIGHT][FIELD_WDTH];                   //フィールド配列
BLOCK_STATE Next[BLOCK_TROUT_SIZE][BLOCK_TROUT_SIZE];          //待機状態のブロック
BLOCK_STATE Stock[BLOCK_TROUT_SIZE][BLOCK_TROUT_SIZE];         //ストックのブロック
BLOCK_STATE DropBlock[BLOCK_TROUT_SIZE][BLOCK_TROUT_SIZE];     //落ちるブロック
int DropBlock_X;         //落ちるブロックのx座標
int DropBlock_Y;         //落ちるブロックのY座標

int WaitTime;             //待機時間
int Stock_Flg;           //ストックフラグ
int Generate_Flg;        //生成フラグ
int DeleteLine;          //消したラインの数
int SoundEffect[3];      //SE

/********************************
*プロトタイプ宣言
********************************/
void create_field(void);           //フィールドの生成処理
void create_block(void);           //ブロックの生成処理
void move_block(void);             //ブロック移動処理
void change_block(void);           //ストック交換処理
void turn_block(int clockwise);    //ブロック回転処理
int check_overlap(int x, int y);   //範囲外チェック処理
void lock_block(int x, int y);     //着地したブロックを固定済みに変更する処理
void check_line(void);             //ブロックの横一列確認処理

/********************************
*ブロック機能：初期化処理
* 引数：なし
*戻り値：エラー情報(-1:異常,それ以外:正常)
********************************/
int Block_Initialize(void)
{
	int ret = 0;
	int i = 0;
	
	//ブロック画像の読み込み
	ret = LoadDivGraph("images/block.png", E_BLOCK_IMAGE_MAX, 10, 1, BLOCK_SIZE,
		BLOCK_SIZE, BlockImage);

	//SEの読み込み
	SoundEffect[0] = LoadSoundMem("sounds/SE3.mp3");
	SoundEffect[1] = LoadSoundMem("sounds/SE4.mp3");
	SoundEffect[2] = LoadSoundMem("sounds/SE5.wav");

	//音量の調整
	ChangeVolumeSoundMem(150, SoundEffect[0]);
	ChangeVolumeSoundMem(150, SoundEffect[1]);
	ChangeVolumeSoundMem(130, SoundEffect[2]);

	//フィールドの生成
	create_field();

	//ブロック生成
	create_block();
	create_block();

	//待機時間の初期化
	WaitTime = 0;
	//ストックフラグの初期化
	Stock_Flg = FALSE;
	//生成フラグの初期化
	Generate_Flg = TRUE;
	//消したラインの数の初期化
	DeleteLine = 0;

	//エラーチェック
	for (i = 0; i < 3; i++)
	{
		if (SoundEffect[i] == -1)
		{
			ret = -1;
			break;
		}
	}
	return ret;
}


/********************************
*ブロック機能：更新処理
* 引数：なし
*戻り値：なし
********************************/
void Block_Update(void)
{
	//ブロックの移動処理
	move_block();

	//ブロックのストック
	if((GetButtonDown(XINPUT_BUTTON_LEFT_SHOULDER)==TRUE) || 
		(GetButtonDown(XINPUT_BUTTON_RIGHT_SHOULDER) == TRUE))
	{
		//生成可能であれば
		if (Generate_Flg == TRUE)
		{
			change_block();     //ストック交換処理
			//ブロックの回転を正位置にする
		}
	}
	//ブロックの回転(反時計回り)
	if ((GetButtonDown(XINPUT_BUTTON_A) == TRUE) ||
		(GetButtonDown(XINPUT_BUTTON_Y) == TRUE))
	{
		turn_block(TURN_ANITICROCKWICE);
	}
	//ブロックの回転(時計回り)
	if ((GetButtonDown(XINPUT_BUTTON_B) == TRUE) ||
		(GetButtonDown(XINPUT_BUTTON_X) == TRUE))
	{
		turn_block(TURN_CROCKWICE);
	}
	//落下処理
	WaitTime++;   //カウンタの更新
	if (WaitTime > DROP_SPEED)
	{
		if (check_overlap(DropBlock_X, DropBlock_Y + 1) == TRUE)
		{
			DropBlock_Y++;
		}
		else
		{
			//ブロックの固定
			lock_block(DropBlock_X, DropBlock_Y);
			//ブロックの消去とブロックを下す処理
			check_line();
			//新しいブロックの生成
			create_block();
		}
		//カウンタの初期化
		WaitTime = 0;
	}

}

























