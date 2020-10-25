#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PAI 3.1415926535

typedef struct {
  unsigned char data;
} GRAY;


int main(int argc, char *argv[]) {
  int x, y;
  int width, height, bright, img_size;
  char firstline[100], secondline[100];

  GRAY gray[256][256];
  GRAY shrink[128][128];
  GRAY enlarge[512][512];
  GRAY rotation[350][350];


  // 画像ファイルの読み込み
  FILE* fp = fopen(argv[1], "rb");

  if (fp == NULL) {
    printf("ファイル %s を開けません.\n", argv[1]);
    exit(1);
  } else if (argc < 2) {
    printf("引数が足りません.\n");
    exit(1);
  } else if (argc > 2) {
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
  unsigned int sx, sy;

  for (x = 0; x < width; x += 2) {
    for (y = 0; y < height; y += 2) {
      // 濃度平均を計算(2×2画素)
      shrink[sx][sy].data = (gray[x][y].data +
                             gray[x][y + 1].data +
                             gray[x + 1][y].data +
                             gray[x + 1][y + 1].data) / 4;
      sy++;
    }
    sx++;
    sy = 0;
  }

  fp = fopen("shrink.pgm", "wb");
  fprintf(fp, "P5\n# Grayscale image\n%d %d\n%d\n", width / 2, height / 2, bright);
  fwrite(shrink, sizeof(GRAY), img_size / 4, fp);


  // 直線補間法による拡大
  unsigned int ex, ey;

  for (x = 0; x < width; x++) {
    for (y = 0; y < height; y++) {
      // 隣接画素の近似値を計算(2×2画素)
      enlarge[ex][ey].data         =  gray[x][y].data;
      enlarge[ex][ey + 1].data     = (gray[x][y].data +
                                      gray[x][y + 1].data) / 2;
      enlarge[ex + 1][ey].data     = (gray[x][y].data +
                                      gray[x + 1][y].data) / 2;
      enlarge[ex + 1][ey + 1].data = (gray[x][y].data +
                                      gray[x + 1][y + 1].data) / 2;
      ey += 2;
    }
    ex += 2;
    ey = 0;
  }

  ex = 0;

  for (x = 0; x < width; x++) {
    // 計算対象不足の右端の近似値を計算(2×2画素)
    enlarge[ex][(width - 1) * 2].data     =  gray[x][width - 1].data;
    enlarge[ex][width * 2].data           = (gray[x][width].data +
                                             gray[x][width - 1].data) / 2;
    enlarge[ex + 1][(width - 1) * 2].data = (gray[x + 1][width - 1].data +
                                             gray[x][width - 1].data) / 2;
    enlarge[ex + 1][width * 2].data       = (gray[x + 1][width].data +
                                             gray[x][width - 1].data) / 2;
    ex += 2;
  }

  ex = 0;

  for (y = 0; y < height; y++) {
    // 計算対象不足の下端の近似値を計算(2×2画素)
    enlarge[(height - 1) * 2][ex].data     =  gray[height - 1][y].data;
    enlarge[height * 2][ex].data           = (gray[height][y].data +
                                              gray[height - 1][y].data) / 2;
    enlarge[(height - 1) * 2][ex + 1].data = (gray[height - 1][y + 1].data +
                                              gray[height - 1][y].data) / 2;
    enlarge[height * 2][ex + 1].data       = (gray[height][y + 1].data +
                                              gray[height - 1][y].data) / 2;
    ex += 2;
  }

  fp = fopen("enlarge.pgm", "wb");
  fprintf(fp, "P5\n# Grayscale image\n%d %d\n%d\n", width * 2, height * 2, bright);
  fwrite(enlarge, sizeof(GRAY), img_size * 4, fp);


  // アフィン変換による回転
  int move_x = 40;                   // 回転させた画像全体を表示させるための平行移動
  int move_y = 280;
  int deg = 30;                       // 回転角度
  double radian = deg * (PAI / 180);  // 角度をラジアンに変換

  int convert_x1, convert_y1,         // 変換後の座標
      convert_x2, convert_y2;

  for (x = 0; x < width; x++) {
    for (y = 0; y < height; y++) {
      // 回転
      /*
        convert_x = cos30° * x - sin30° * y
        convert_y = sin30° * x + cos30° * y
      */
      convert_x1 = (cos(deg) * x) + ((-1) * sin(deg) * y);
      convert_y1 = (sin(deg) * x) + (cos(deg) * y);

      // 平行移動
      convert_x2 =  convert_x1 + move_x;
      convert_y2 =  convert_y1 + move_y;

      // アフィン変換後の座標に代入
      rotation[convert_x2][convert_y2].data = gray[x][y].data;
    }
  }

  fp = fopen("rotation.pgm", "wb");
  fprintf(fp, "P5\n# Grayscale image\n%d %d\n%d\n", 350, 350, bright);
  fwrite(rotation, sizeof(GRAY), 350 * 350, fp);


  fclose(fp);

  return 0;
}