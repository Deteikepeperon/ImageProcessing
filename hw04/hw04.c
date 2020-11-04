#include <stdio.h>
#include <stdlib.h>

void median_fillter(FILE *fp, int width, int height, int maxdepth, unsigned char gray[], unsigned char denoise[]);


int main(int argc, char *argv[])
{
  FILE *fp;                         // ファイル構造体のポインタ変数
  int width, height, maxdepth;      // 幅， 高さ， 最大階調値
  char buf[64];                     // 形式とコメントの読み飛ばし用
  unsigned char *gray, *denoise;    // グレイスケール画像データ， ノイズ除去画像データ


  if ((fp = fopen(argv[1], "rb")) == NULL)  exit(1);
  if (argc != 2)                            exit(1);

  fgets(buf, sizeof(buf), fp);
  fgets(buf, sizeof(buf), fp);
  fscanf(fp, "%d %d\n%d\n", &width, &height, &maxdepth);

  if ((gray    = (unsigned char *)malloc(sizeof(unsigned char) * width * height)) == NULL)  exit(1);
  if ((denoise = (unsigned char *)malloc(sizeof(unsigned char) * width * height)) == NULL)  exit(1);

  fread(gray, sizeof(unsigned char), width * height, fp);
  fclose(fp);

  median_fillter(fp, width, height, maxdepth, gray, denoise);

  free(gray);
  free(denoise);

  return 0;
}


// メディアンフィルタを用いたノイズ除去
void median_fillter(FILE *fp, int width, int height, int maxdepth, unsigned char gray[], unsigned char denoise[])
{
  int neighbor[9];    // 注目画素の近傍領域


  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {

      // 隅4辺の画素は， 注目画素の座標をそのままコピー
      if (i == 0 | i == width - 1 | j == 0 | j == height - 1)  denoise[i * height + j] = gray[i * height + j];

      // 近傍画素の値を代入
      neighbor[0] = gray[(i - 1) * height + (j - 1)];
      neighbor[1] = gray[(i - 1) * height + (j    )];
      neighbor[2] = gray[(i - 1) * height + (j + 1)];
      neighbor[3] = gray[(i    ) * height + (j - 1)];
      neighbor[4] = gray[(i    ) * height + (j    )];
      neighbor[5] = gray[(i    ) * height + (j + 1)];
      neighbor[6] = gray[(i + 1) * height + (j - 1)];
      neighbor[7] = gray[(i + 1) * height + (j    )];
      neighbor[8] = gray[(i + 1) * height + (j -+1)];

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

  if ((fp = fopen("denoise.pgm", "wb")) == NULL)  exit(1);
  fprintf(fp, "P5\n# Grayscale image\n%d %d\n%d\n", width, height, maxdepth);
  fwrite(denoise, sizeof(unsigned char), width * height, fp);
}

