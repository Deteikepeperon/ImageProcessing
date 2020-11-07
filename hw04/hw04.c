#include <stdio.h>
#include <stdlib.h>

void noise_removal(FILE *fp, int width, int height, int maxdepth, unsigned char gray[], unsigned char denoise[]);
void sharpening(FILE *fp, int width, int height, int maxdepth, unsigned char denoise[], unsigned char sharpen[]);


int main(int argc, char *argv[])
{
  FILE *fp;                                   // ファイル構造体のポインタ変数
  int width, height, maxdepth;                // 幅， 高さ， 最大階調値
  char buf[64];                               // 形式とコメントの読み飛ばし用
  unsigned char *gray, *denoise, *sharpen;    // グレイスケール画像データ， ノイズ除去画像データ， 鮮鋭化画像データ


  if ((fp = fopen(argv[1], "rb")) == NULL)  goto FILE_ERR;
  if (argc != 2)                            goto ARGUMENT_ERR;

  fgets(buf, sizeof(buf), fp);
  fgets(buf, sizeof(buf), fp);
  fscanf(fp, "%d %d\n%d\n", &width, &height, &maxdepth);

  if ((gray    = (unsigned char *)malloc(sizeof(unsigned char) * width * height)) == NULL)  goto MEMORY_ERR;
  if ((denoise = (unsigned char *)malloc(sizeof(unsigned char) * width * height)) == NULL)  goto MEMORY_ERR;
  if ((sharpen = (unsigned char *)malloc(sizeof(unsigned char) * width * height)) == NULL)  goto MEMORY_ERR;

  fread(gray, sizeof(unsigned char), width * height, fp);

  noise_removal(fp, width, height, maxdepth, gray, denoise);
  sharpening(fp, width, height, maxdepth, denoise, sharpen);

  free(gray);
  free(denoise);
  free(sharpen);

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


// メディアンフィルタを用いたノイズ除去
void noise_removal(FILE *fp, int width, int height, int maxdepth, unsigned char gray[], unsigned char denoise[])
{
  int neighbor[9];    // 注目画素の近傍領域


  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {

      // 隅4辺の画素は， 注目画素の座標をそのままコピー
      if (i == 0 || i == width - 1 || j == 0 || j == height - 1)  denoise[i * height + j] = gray[i * height + j];

      // 近傍画素の値を代入
      neighbor[0] = gray[(i - 1) * height + (j - 1)];
      neighbor[1] = gray[(i - 1) * height + (j    )];
      neighbor[2] = gray[(i - 1) * height + (j + 1)];
      neighbor[3] = gray[(i    ) * height + (j - 1)];
      neighbor[4] = gray[(i    ) * height + (j    )];
      neighbor[5] = gray[(i    ) * height + (j + 1)];
      neighbor[6] = gray[(i + 1) * height + (j - 1)];
      neighbor[7] = gray[(i + 1) * height + (j    )];
      neighbor[8] = gray[(i + 1) * height + (j + 1)];

      // 近傍画素の値をバブルソート
      for (int k = 0; k < 8; k++) {
        for (int l = k; l < 9; l++) {

          if (neighbor[k] > neighbor[l]) {
            int tmp     = neighbor[k];
            neighbor[k] = neighbor[l];
            neighbor[l] = tmp;
          }
        }
      }

      // 近傍画素の中央値を， 注目画素の新しい値とする
      denoise[i * height + j] = neighbor[4];
    }
  }

  fp = fopen("denoise.pgm", "wb");
  fprintf(fp, "P5\n# Grayscale image\n%d %d\n%d\n", width, height, maxdepth);
  fwrite(denoise, sizeof(unsigned char), width * height, fp);
}


// 8近傍ラプラシアンフィルタを用いた鮮鋭化 (メディアンフィルタ + 鮮鋭化)
void sharpening(FILE *fp, int width, int height, int maxdepth, unsigned char denoise[], unsigned char sharpen[])
{
  int neighbor[9];      // 注目画素の近傍領域


  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {

      // 隅4辺の画素は， 注目画素の座標をそのままコピー
      if (i == 0 || i == width - 1 || j == 0 || j == height - 1)  sharpen[i * height + j] = denoise[i * height + j];

      // 近傍画素の値を代入
      neighbor[0] = denoise[(i - 1) * height + (j - 1)];
      neighbor[1] = denoise[(i - 1) * height + (j    )];
      neighbor[2] = denoise[(i - 1) * height + (j + 1)];
      neighbor[3] = denoise[(i    ) * height + (j - 1)];
      neighbor[4] = (denoise[(i   ) * height + (j    )]) * (-8);
      neighbor[5] = denoise[(i    ) * height + (j + 1)];
      neighbor[6] = denoise[(i + 1) * height + (j - 1)];
      neighbor[7] = denoise[(i + 1) * height + (j    )];
      neighbor[8] = denoise[(i + 1) * height + (j + 1)];

      int laplacian_sum = 0;    // 注目画素の合計

      // 8近傍ラプラシアンを掛けた注目画素の値を合計
      for (int k = 0; k < 9; k++)  laplacian_sum += neighbor[k];

      // フィルタ処理後の値(0以下は0に， 255以上は255にする)
      if ((denoise[i * height + j] - laplacian_sum) <= 0)    sharpen[i * height + j] = 0;
      if ((denoise[i * height + j] - laplacian_sum) >= 255)  sharpen[i * height + j] = 255;

      sharpen[i * height + j] = denoise[i * height + j] - laplacian_sum;
    }
  }

  fp = fopen("sharpening.pgm", "wb");
  fprintf(fp, "P5\n# Grayscale image\n%d %d\n%d\n", width, height, maxdepth);
  fwrite(sharpen, sizeof(unsigned char), width * height, fp);
}

