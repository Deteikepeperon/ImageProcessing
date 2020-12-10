#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void quantization(FILE *fp, int width, int height, int maxdepth, unsigned char gray[], unsigned char quantized[]);
void calcPSNR(FILE *fp, int width, int height, int maxdepth, unsigned char gray[], unsigned char quantized[]);


int main(int argc, char *argv[])
{
  FILE *fp;                                   // ファイル構造体のポインタ変数
  int width, height, maxdepth;                // 幅， 高さ， 最大階調値
  char buf[64];                               // 形式とコメントの読み飛ばし用
  unsigned char *gray, *quantized;            // グレイスケール画像データ， 量子化画像データ


  if ((fp = fopen(argv[1], "rb")) == NULL)  goto FILE_ERR;
  if (argc != 2)                            goto ARGUMENT_ERR;

  fgets(buf, sizeof(buf), fp);
  fgets(buf, sizeof(buf), fp);
  fscanf(fp, "%d %d\n%d\n", &width, &height, &maxdepth);

  if ((gray      = (unsigned char *)malloc(sizeof(unsigned char) * width * height)) == NULL)  goto MEMORY_ERR;
  if ((quantized = (unsigned char *)malloc(sizeof(unsigned char) * width * height)) == NULL)  goto MEMORY_ERR;

  fread(gray, sizeof(unsigned char), width * height, fp);
  quantization(fp, width, height, maxdepth, gray, quantized);
  calcPSNR(fp, width, height, maxdepth, gray, quantized);

  free(gray);
  free(quantized);

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


// 8[bit/pix]の画像を4[bit/pix]にする線形量子化プログラム
void quantization(FILE *fp, int width, int height, int maxdepth, unsigned char gray[], unsigned char quantized[])
{
  for (int i = 0; i < width * height; i++) {
    if (gray[i] / 16 == 0)  quantized[i] = 0;
    if (gray[i] / 16 == 15) quantized[i] = 255;

    quantized[i] = (gray[i] / 16) * 16;
  }


  fp = fopen("quantized.pgm", "wb");
  fprintf(fp, "P5\n# Grayscale image\n%d %d\n%d\n", width, height, maxdepth);
  fwrite(quantized, sizeof(unsigned char), width * height, fp);
}


// PSNR値の計算
void calcPSNR(FILE *fp, int width, int height, int maxdepth, unsigned char gray[], unsigned char quantized[])
{
  double MSE;     // 平均二乗誤差 「((元画像の輝度 - 変換後の輝度) ^ 2)の総和 / 画素数」
  double PSNR;    // 変換後の画像がどれ位劣化したかを評価する指標 「10 * log_10 (MAX ^ 2 / MSE)」
  double diff, sum = 0.0;


  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      diff = gray[i * height + j] - quantized[i * height + j];
      sum += pow(diff, 2.0);
    }
  }

  MSE = sum / (width * height);
  PSNR = 10.0 * log10(maxdepth * maxdepth / MSE);

  printf("\nPSNR値：%f\n", PSNR);
}