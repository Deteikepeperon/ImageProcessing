#include <stdio.h>
#include <stdlib.h>

typedef struct {
  unsigned char red;
  unsigned char blue;
  unsigned char green;
} RGB;


int main(int argc, char *argv[]) {
  FILE *fp;                       // ファイル構造体のポインタ変数
  int width, height, maxdepth;    // 幅， 高さ， 最大階調値
  char buf[64];                   // 形式とコメントの読み飛ばし用
  RGB *rgb;                       // RGB画像データ
  unsigned char *gray;            // グレイスケール画像データ


  // 画像ファイルの読み込み（実行時に第一引数として指定 例： ./hw01 pepper.ppm)
  if ((fp = fopen(argv[1], "rb")) == NULL)  goto FILE_ERR;      // ファイルを読み込めない場合， プログラムを終了
  if (argc != 2)                            goto ARGUMENT_ERR;  // 第一引数の数が不適切な場合， プログラムを終了

  // ヘッダ部の読み込み
  fgets(buf, sizeof(buf), fp);                                  // 形式を読み飛ばす
  fgets(buf, sizeof(buf), fp);                                  // コメントを呼び飛ばす
  fscanf(fp, "%d %d\n %d\n", &width, &height, &maxdepth);       // 幅， 高さ， 最大階調値を取得

  // データ容量域を動的確保（確保に失敗した場合， プログラムを終了）
  if ((rgb  = (RGB *)malloc(sizeof(RGB) * width * height))                     == NULL)  goto MEMORY_ERR;
  if ((gray = (unsigned char *)malloc(sizeof(unsigned char) * width * height)) == NULL)  goto MEMORY_ERR;

  // データ部の読み込み
  fread(rgb, sizeof(RGB), width * height, fp);


  // R
  for (int i = 0; i < width * height; i++)  gray[i] = rgb[i].red;

  fp = fopen("red.pgm", "wb");
  fprintf(fp, "P5\n# Grayscale image\n%d %d\n%d\n", width, height, maxdepth);
  fwrite(gray, sizeof(unsigned char), width * height, fp);


  // G
  for (int i = 0; i < width * height; i++)  gray[i] = rgb[i].green;

  fp = fopen("green.pgm", "wb");
  fprintf(fp, "P5\n# Grayscale image\n%d %d\n%d\n", width, height, maxdepth);
  fwrite(gray, sizeof(unsigned char), width * height, fp);


  // B
  for (int i = 0; i < width * height; i++)  gray[i] = rgb[i].blue;

  fp = fopen("blue.pgm", "wb");
  fprintf(fp, "P5\n# Grayscale image\n%d %d\n%d\n", width, height, maxdepth);
  fwrite(gray, sizeof(unsigned char), width * height, fp);


  // RGBの平均
  for (int i = 0; i < width * height; i++)  gray[i] = (rgb[i].red + rgb[i].green + rgb[i].blue) / 3;

  fp = fopen("avarage.pgm", "wb");
  fprintf(fp, "P5\n# Grayscale image\n%d %d\n%d\n", width, height, maxdepth);
  fwrite(gray, sizeof(unsigned char), width * height, fp);


  // 輝度Y
  for (int i = 0; i < width * height; i++)  gray[i] = rgb[i].red * 0.299 + rgb[i].green * 0.587 + rgb[i].blue * 0.114;

  fp = fopen("bright.pgm", "wb");
  fprintf(fp, "P5\n# Grayscale image\n%d %d\n%d\n", width, height, maxdepth);
  fwrite(gray, sizeof(unsigned char), width * height, fp);


  free(rgb);
  free(gray);

  fclose(fp);

  return 0;


  FILE_ERR:
    printf("ファイルを開けません．\n");
    exit(1);

  ARGUMENT_ERR:
    printf("コマンドライン引数の数は1つにしてください．\n");
    exit(1);

  MEMORY_ERR:
    printf("メモリ領域の確保に失敗しました．\n");
    exit(1);
}
