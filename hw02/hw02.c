#include <stdio.h>
#include <stdlib.h>

typedef struct {
  unsigned char data;
} GRAY;

int main(int argc, char *argv[]) {
  int i, j;
  int width, height, bright, img_size;
  char firstline[100], secondline[100];

  GRAY gray[256][256];
  GRAY scale_down[128][128];
  GRAY scale[512][512];


  // 画像ファイルの読み込み
  FILE* fp = fopen(argv[1], "rb");

  if (fp == NULL) {
    printf("ファイル %s を開けません.\n", argv[1]);
    exit(1);
  }
  else if (argc < 2) {
    printf("引数が足りません.\n");
    exit(1);
  }
  else if (argc > 2) {
    printf("指定できる画像ファイルは1つです．\n");
    exit(1);
  }

  // ヘッダ部の読み込み
  fgets(firstline, sizeof(firstline), fp);                // 形式を読み飛ばす
  fgets(secondline, sizeof(secondline), fp);              // コメントを呼び飛ばす
  fscanf(fp, "%d %d\n %d\n", &width, &height, &bright);   // 画素, 最大階調値を取得

  // データ部の読み込み
  img_size = width * height;
  fread(gray, sizeof(GRAY), img_size, fp);


  // 平均操作法による縮小
  unsigned int x1, y1;

  for (i = 0; i < width; i += 2) {
    for (j = 0; j < height; j += 2) {
      // 濃度平均を計算(2×2画素)
      scale_down[x1][y1].data = (gray[i][j].data +
                                 gray[i][j + 1].data +
                                 gray[i + 1][j].data +
                                 gray[i + 1][j + 1].data) / 4;
      y1++;
    }
    x1++;
    y1 = 0;
  }

  fp = fopen("scaled_img.pgm", "wb");
  fprintf(fp, "P5\n# Grayscale image\n%d %d\n%d\n", width / 2, height / 2, bright);
  fwrite(scale_down, sizeof(GRAY), img_size / 4, fp);


  // 直線補間法による拡大
  unsigned int x2, y2;

  for (i = 0; i < width; i++) {
    for (j = 0; j < height; j++) {
      // 隣接画素の近似値を計算(2×2画素)
      scale[x2][y2].data         =  gray[i][j].data;
      scale[x2][y2 + 1].data     = (gray[i][j].data +
                                    gray[i][j + 1].data) / 2;
      scale[x2 + 1][y2].data     = (gray[i][j].data +
                                    gray[i + 1][j].data) / 2;
      scale[x2 + 1][y2 + 1].data = (gray[i][j].data +
                                    gray[i + 1][j + 1].data) / 2;
      y2 += 2;
    }
    x2 += 2;
    y2 = 0;
  }

  x2 = 0;

  for (i = 0; i < width; i++) {
    // 計算対象不足の右端の近似値を計算(2×2画素)
    scale[x2][(width - 1) * 2].data     =  gray[i][width - 1].data;
    scale[x2][width * 2].data           = (gray[i][width].data +
                                           gray[i][width - 1].data) / 2;
    scale[x2 + 1][(width - 1) * 2].data = (gray[i + 1][width - 1].data +
                                           gray[i][width - 1].data) / 2;
    scale[x2 + 1][width * 2].data       = (gray[i + 1][width].data +
                                           gray[i][width - 1].data) / 2;
    x2 += 2;
  }

  x2 = 0;

  for (j = 0; j < height; j++) {
    // 計算不足対象の下端の近似値を計算(2×2画素)
    scale[(height - 1) * 2][x2].data     =  gray[height - 1][i].data;
    scale[height * 2][x2].data           = (gray[height][i].data +
                                            gray[height - 1][i].data) / 2;
    scale[(height - 1) * 2][x2 + 1].data = (gray[height - 1][i + 1].data +
                                            gray[height - 1][i].data) / 2;
    scale[height * 2][x2 + 1].data       = (gray[height][i + 1].data +
                                            gray[height - 1][i].data) / 2;
    x2 += 2;
  }

  fp = fopen("expanded_image.pgm", "wb");
  fprintf(fp, "P5\n# Grayscale image\n%d %d\n%d\n", width * 2, height * 2, bright);
  fwrite(scale, sizeof(GRAY), img_size * 4, fp);


  // アフィン変換による回転



  fclose(fp);

  return 0;
}