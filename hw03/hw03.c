#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  unsigned char data;
} GRAY;


int main(int argc, char *argv[]) {
  int width, height, bright, img_size;
  char firstline[100], secondline[100];

  GRAY gray[256][256];

  FILE *fp, *gp;


  // 画像ファイルの読み込み
  fp = fopen(argv[1], "rb");

  if (fp == NULL) {
    printf("ファイル %s を開けません.\n", argv[1]);
    exit(1);
  } else if (argc < 2) {
    printf("引数が足りません.\n");
    exit(1);
  } else if (argc > 2) {
    printf("指定できる画像ファイルは1つです.\n");
    exit(1);
  }

  // ヘッダ部の読み込み
  fgets(firstline, sizeof(firstline), fp);
  fgets(secondline, sizeof(secondline), fp);
  fscanf(fp, "%d %d\n%d\n", &width, &height, &bright);

  // データ部の読み込み
  img_size = width * height;
  fread(gray, sizeof(GRAY), img_size, fp);


  // 濃度度数分布表を生成 ＆ gnulotでヒストグラムを描画
  int hist[256];
  int max_frequency;
  int i, x, y;

  memset(hist, 0, sizeof(hist));

  if ((gp = popen("gnuplot -persist", "w")) == NULL) {
    printf("gnuplotを開けません.");
    exit(1);
  }
  fprintf(gp,"set boxwidth 1\n");
  fprintf(gp, "set xrange [%d:%d]\n", 0, 256);
  fprintf(gp, "set yrange [%d:%d]\n", 0, 3500);
  fprintf(gp, "plot '-' with boxes\n");

  printf("\n濃度度数分布表を出力しています...\n\n");

  for (x = 0; x < width; x++) {
    for (y = 0; y < height; y++) {
      hist[gray[x][y].data]++;
    }
  }

  max_frequency = 0;

  for (i = 0; i < 256; i++) {
    if(hist[i] > max_frequency) {
      max_frequency = hist[i];
    }
    printf("濃度 %3d ：頻度 %d\n", i, hist[i]);
    fprintf(gp, "%d\t%d\n", i, hist[i]);
  }
  fprintf(gp, "e\n");
  printf("\n頻度の最大値 = %d\n\n", max_frequency);


  pclose(gp);
  fclose(fp);

  return 0;
}