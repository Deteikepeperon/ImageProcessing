#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PAI 3.1415926535

typedef struct {
  unsigned char data;
} GRAY;


int main(int argc, char *argv[])
{
  FILE *fp;                       // ファイル構造体のポインタ変数
  int width, height, maxdepth;    // 幅， 高さ， 最大階調値
  char buf[64];                   // 形式とコメントの読み飛ばし用
  GRAY gray[256][256],            // グレイスケール画像データ
       shrink[128][128],          // 縮小画像データ
       enlarge[512][512],         // 拡大画像データ
       rotation[350][350];        // 回転画像データ


  if ((fp = fopen(argv[1], "rb")) == NULL)  goto FILE_ERR;
  if (argc != 2)                            goto ARGUMENT_ERR;

  fgets(buf, sizeof(buf), fp);
  fgets(buf, sizeof(buf), fp);
  fscanf(fp, "%d %d\n %d\n", &width, &height, &maxdepth);

  fread(gray, sizeof(GRAY), width * height, fp);


  // 平均操作法による縮小
  unsigned int sx, sy;

  for (int x = 0; x < width; x += 2) {
    for (int y = 0; y < height; y += 2) {
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
  fprintf(fp, "P5\n# Grayscale image\n%d %d\n%d\n", width / 2, height / 2, maxdepth);
  fwrite(shrink, sizeof(GRAY), width * height / 4, fp);


  // 直線補間法による拡大
  unsigned int ex, ey;

  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
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

  for (int x = 0; x < width; x++) {
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

  for (int y = 0; y < height; y++) {
    // 計算対象不足の下端の近似値を計算(2×2画素)
    enlarge[(height - 1) * 2][ey].data     =  gray[height - 1][y].data;
    enlarge[height * 2][ey].data           = (gray[height][y].data +
                                              gray[height - 1][y].data) / 2;
    enlarge[(height - 1) * 2][ey + 1].data = (gray[height - 1][y + 1].data +
                                              gray[height - 1][y].data) / 2;
    enlarge[height * 2][ey + 1].data       = (gray[height][y + 1].data +
                                              gray[height - 1][y].data) / 2;
    ey += 2;
  }

  fp = fopen("enlarge.pgm", "wb");
  fprintf(fp, "P5\n# Grayscale image\n%d %d\n%d\n", width * 2, height * 2, maxdepth);
  fwrite(enlarge, sizeof(GRAY), width * height * 4, fp);


  // アフィン変換による回転
  int move_x = 0;                     // 回転させた画像全体を表示させるための平行移動
  int move_y = 128;
  int deg = 30;                       // 回転角度
  double radian = deg * (PAI / 180);  // 角度をラジアンに変換

  int convert_x1, convert_y1,         // 変換後の座標
      convert_x2, convert_y2;

  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      // 回転
      /*
        convert_x1 = cos30° * x + sin30° * y
        convert_y1 = -sin30° * x + cos30° * y
      */
      convert_x1 = (cos(radian) * x) + (sin(radian) * y);
      convert_y1 = (cos(radian) * y) - (sin(radian) * x);

      // 平行移動
      convert_x2 =  convert_x1 + move_x;
      convert_y2 =  convert_y1 + move_y;

      // アフィン変換後の座標に代入
      rotation[convert_y2][convert_x2].data = gray[y][x].data;
    }
  }

  fp = fopen("rotation.pgm", "wb");
  fprintf(fp, "P5\n# Grayscale image\n%d %d\n%d\n", 350, 350, maxdepth);
  fwrite(rotation, sizeof(GRAY), 350 * 350, fp);


  fclose(fp);

  return 0;


  FILE_ERR:
    printf("ファイルを開けません．\n");
    exit(1);

  ARGUMENT_ERR:
    printf("コマンドライン引数の数は1つにしてください．\n");
    exit(1);
}