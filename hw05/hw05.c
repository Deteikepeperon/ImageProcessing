#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fixed_threshold(FILE *fp, int width, int height, int maxdepth, unsigned char gray[], unsigned char binariz[]);
void discriminant_analysis(FILE *fp, int width, int height, int maxdepth, unsigned char gray[], unsigned char binariz[]);
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
  discriminant_analysis(fp, width, height, maxdepth, gray, binariz);
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

  fp = fopen("binarization_01.pgm", "wb");
  fprintf(fp, "P5\n# Grayscale image\n%d %d\n%d\n", width, height, maxdepth);
  fwrite(binariz, sizeof(unsigned char), width * height, fp);
}


// 判別分析法による二値化（統計処理によりしきい値を決定）
void discriminant_analysis(FILE *fp, int width, int height, int maxdepth, unsigned char gray[], unsigned char binariz[])
{
  memset(binariz, 0, sizeof(unsigned char));

  // 輝度値集計
  int hist[256];
  memset(hist, 0, sizeof(hist));

  for (int i = 0; i < width * height; i++)  hist[gray[i]]++;


  // 判別分析法
  int t = 0;                      // しきい値
  double sb2_max = 0;             // w1 * w2 * (m1 - m2)^2 の最大値

  // 画素数の割合・平均を求める
  for (int i = 0; i <= maxdepth; i++) {
    int w1 = 0, w2 = 0;           // 各クラスの画素数
    long sum1 = 0, sum2 = 0;      // 各クラスの合計値
    double m1 = 0.0, m2 = 0.0;    // 各クラスの平均値

    for (int j = 0; j <= i; j++) {
      w1 += hist[j];
      sum1 += j * hist[j];
    }

    for (int j = i + 1; j < 256; j++) {
      w2 += hist[j];
      sum2 += j * hist[j];
    }

    m1 = (w1 != 0) ? (double)sum1 / w1 : 0;
    m2 = (w2 != 0) ? (double)sum2 / w2 : 0;

    double sb2 = ((double)w1 * w2 * (m1 - m2) * (m1 - m2));

    if (sb2 > sb2_max) {
      sb2_max = sb2;
      t = i;
    }

    for (int i = 0; i < width * height; i++)  binariz[i] = (hist[i] < t) ? 0 : 255;
  }

  fp = fopen("binarization_02.pgm", "wb");
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
