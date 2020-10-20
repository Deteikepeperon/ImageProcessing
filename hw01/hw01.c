#include <stdio.h>
#include <stdlib.h>

typedef struct {
  unsigned char red;
  unsigned char blue;
  unsigned char green;
} RGB;



int main(int argc, char *argv[]) {
  int i;
  int width, height, gradation;
  char* c;
  char head[128];
  RGB rgb[256][256];


  /********************************/
  /*** 画像データの読み込み(ヘッダ部) ***/

  //? ファイルは実行時に第一引数として指定したい(argv[1]?)
  FILE *fp = fopen("./pepper.ppm", "rb");    // 画像ファイルの読み込み

  //? コメントは形式と画素の間以外にも書かれる可能性がある
  //! EOFチェックをしていないので無限ループになる可能性あり
  do {                                       // ヘッダ部の先頭に数字が読み込まれるまでループ(形式とコメントをスキップする)
    fgets(head, 256, fp);
  } while (head[0] < '0' || head[0] > '9');

  /*
  strtol(変換対象の文字列, 変換終了した位置, 基数)
  */
  width = strtol(head, &c, 10);              // 見つかった先頭の2つの値(画素)を10進数の数値として読み込む(横×縦ピクセル)
  height = strtol(c, NULL, 10);

  fgets(head, 256, fp);                      // 最大階調値を取得するために1行ファイルから読み込む
  gradation = strtol(head, &c, 10);          // 最大階調数を10進数の数値として読み込む


  /********************************/
  /*** 画像データの読み込み(データ部) ***/

  for (i = 0; i < 128; i++) {
    /*
    fread(データ格納用配列, 1回に読み込むサイズ(バイト数), 読み込む回数, 読み込むファイルポインタ)
    */
    fread(rgb, sizeof(RGB), 256 * 256, fp);
  }
  fclose(fp);


  /*********************/
  /*** 画像データの処理 ***/
  



  /************************/
  /*** 画像データの書き出し ***/

  return 0;
}

