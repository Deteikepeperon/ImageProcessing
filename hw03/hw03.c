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


  // 濃度度数分布表を生成＆gnuplotでヒストグラムを描画
  int hist[256];
  int max_freq;
  int i, x, y;

  memset(hist, 0, sizeof(hist));    // 配列を初期化

  if ((gp = popen("gnuplot -persist", "w")) == NULL) {
    printf("gnuplotを開けません.");
    exit(1);
  }

  fprintf(gp,"set boxwidth 1\n");
  fprintf(gp, "set xrange [%d:%d]\n", 0, 256);
  fprintf(gp, "set yrange [%d:%d]\n", 0, 3500);
  fprintf(gp, "plot '-' with boxes\n");


  for (x = 0; x < width; x++) {
    for (y = 0; y < height; y++) {
      hist[gray[x][y].data]++;
    }
  }

  printf("\n%s\n\n", "---- 濃度度数分布表 ----");
  max_freq = 0;

  for (i = 0; i <= bright; i++) {
    if(hist[i] > max_freq) {
      max_freq = hist[i];
    }
    printf("濃度：%3d    頻度：%4d\n", i, hist[i]);
    fprintf(gp, "%d\t%d\n", i, hist[i]);
  }
  fprintf(gp, "e\n");
  printf("\n頻度の最大値 = %d\n\n", max_freq);


  // ヒストグラムを平滑化
  int target_bright = 64;                         // 目標階調
  int target_freq = img_size / target_bright;     // 目標頻度
  int sum_freq = 0;                               // 累積頻度
  int absolute_value = target_freq - sum_freq;    // 目標頻度と蓄積頻度の差の絶対値
  int cor_thick[bright];                          // 補正濃度

  memset(cor_thick, 0, sizeof(cor_thick));

  // 濃度変換表作成
  for (i = 0; i <= bright; i++) {
    /*
      「目標頻度と蓄積頻度の差の絶対値」 と
      「目標頻度と蓄積頻度 と 現在の濃度の頻度 の差の絶対値」 を比較
    */

    // 後者が大きい場合
    if (abs(absolute_value) < abs(absolute_value - hist[i])) {
      sum_freq = 0;                               // 累積頻度を0にする
      cor_thick[i]++;                             // 補正頻度を1加算

      if (i == 0)                                 // 補正濃度が加算されていない場合, 前の補正濃度の値は0
        cor_thick[i - 1] = 0;

      if (cor_thick[i] >= target_bright)          // 加算した値が目標階調数以上ならば, (目標階調数 - 1)に補正
        cor_thick[i] = target_bright - 1;

    // 前者が大きい場合
    } else {
      cor_thick[i] = cor_thick[i - 1];            // 現在の濃度変換表の値を補正濃度の値とする

      if (i == 0)                                 // 補正濃度が加算されていない場合, 前の補正濃度の値は0
        cor_thick[i - 1] = 0;
    }
    sum_freq += hist[i];                          // 蓄積頻度に現在の濃度の頻度を加算
  }


  char smooth[img_size];

  for (i = 0; i < img_size; i++) {
    smooth[i] = cor_thick[gray[x][y].data] * ((bright + 1) / target_bright);
  }

  fp = fopen("smooth.pgm", "wb");
  fprintf(fp, "P5\n# Grayscale image\n%d %d\n%d\n", width, height, bright);
  fwrite(smooth, sizeof(GRAY), img_size, fp);


  pclose(gp);
  fclose(fp);

  return 0;
}