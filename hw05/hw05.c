#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fixed_threshold(FILE *fp, int width, int height, int maxdepth, unsigned char gray[], unsigned char binariz[]);
void concentration_average(FILE *fp, int width, int height, int maxdepth, unsigned char gray[], unsigned char binariz[]);
void ordered_dither(FILE *fp, int width, int height, int maxdepth, unsigned char gray[], unsigned char dither[]);


int main(int argc, char *argv[])
{
  FILE *fp;                                   // ファイル構造体のポインタ変数
  int width, height, maxdepth;                // 幅， 高さ， 最大階調値
  char buf[64];                               // 形式とコメントの読み飛ばし用
  unsigned char *gray, *binariz, *dither;     // グレイスケール画像データ， 二値化画像データ， ディザ画像データ


  if ((fp = fopen(argv[1], "rb")) == NULL)  goto FILE_ERR;
  if (argc != 2)                            goto ARGUMENT_ERR;

  fgets(buf, sizeof(buf), fp);
  fgets(buf, sizeof(buf), fp);
  fscanf(fp, "%d %d\n%d\n", &width, &height, &maxdepth);

  if ((gray    = (unsigned char *)malloc(sizeof(unsigned char) * width * height)) == NULL)  goto MEMORY_ERR;
  if ((binariz = (unsigned char *)malloc(sizeof(unsigned char) * width * height)) == NULL)  goto MEMORY_ERR;
  if ((dither  = (unsigned char *)malloc(sizeof(unsigned char) * width * height)) == NULL)  goto MEMORY_ERR;

  fread(gray, sizeof(unsigned char), width * height, fp);

  fixed_threshold(fp, width, height, maxdepth, gray, binariz);
  concentration_average(fp, width, height, maxdepth, gray, binariz);
  ordered_dither(fp, width, height, maxdepth, gray, dither);

  free(gray);
  free(binariz);
  free(dither);

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


// 固定しきい値による二値化（しきい値：128）
void fixed_threshold(FILE *fp, int width, int height, int maxdepth, unsigned char gray[], unsigned char binariz[])
{
  for (int i = 0; i < width * height; i++)  binariz[i] = (gray[i] >= 128) ? 255 : 0;

  fp = fopen("fixed_binariz.pgm", "wb");
  fprintf(fp, "P5\n# Grayscale image\n%d %d\n%d\n", width, height, maxdepth);
  fwrite(binariz, sizeof(unsigned char), width * height, fp);
}


// 濃度の平均値をしきい値とした二値化
void concentration_average(FILE *fp, int width, int height, int maxdepth, unsigned char gray[], unsigned char binariz[])
{
  int hist[256];
  int sum = 0, average = 0;
  memset(hist, 0, sizeof(hist));
  memset(binariz, 0, sizeof(unsigned char));

  // 輝度値集計
  for (int i = 0; i < width * height; i++)  hist[gray[i]]++;

  // 濃度の平均値を求める
  for (int i = 0; i <= maxdepth; i++) {
    sum += i * hist[i];
    average = sum / (width * height);
  }
  printf("濃度の平均値：%d\n", average);


  for (int i = 0; i < width * height; i++)  binariz[i] = (gray[i] >= average) ? 255 : 0;

  fp = fopen("average_binariz.pgm", "wb");
  fprintf(fp, "P5\n# Grayscale image\n%d %d\n%d\n", width, height, maxdepth);
  fwrite(binariz, sizeof(unsigned char), width * height, fp);
}


// Bayerマトリクスを用いたディザ画像生成（組織的ディザ法)
void ordered_dither(FILE *fp, int width, int height, int maxdepth, unsigned char gray[], unsigned char dither[])
{
  int bayer_matrix[] = {0, 8, 2, 10,      // ディザマトリクス
                        12, 4, 14, 6,
                        3, 11, 1, 9,
                        15, 7, 13, 5};

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      dither[i * height + j] = (gray[i * height + j] >= bayer_matrix[(i % 4) * 4 + (j % 4)] * 16 + 8) ? 255 : 0;
    }
  }

  fp = fopen("dither.pgm", "wb");
  fprintf(fp, "P5\n# Grayscale image\n%d %d\n%d\n", width, height, maxdepth);
  fwrite(dither, sizeof(unsigned char), width * height, fp);
}
