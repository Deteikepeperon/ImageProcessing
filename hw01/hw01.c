#include <assert.h>
#include <stdio.h>


typedef struct {
  unsigned char red;
  unsigned char blue;
  unsigned char green;
} RGB;


int main(int argc, char *argv[]) {
  int i;
  int width, height, bright;
  char firstline[100];
  char secondline[100];
  RGB rgb[65536];
  unsigned char gray[65536];


  // 画像ファイルの読み込み
  FILE* fp = fopen(argv[1], "rb");            // 実行時に第一引数として指定 (./hw01 pepper.ppm)

  if (fp == NULL) {
    printf("ファイル %s を開けません.\n", argv[1]);
    return 0;
  }

  if (argc < 2) {
    printf("引数が足りません.\n");
    return 1;
  }
  else if (argc > 2) {
    printf("指定できる画像ファイルは1つです．\n");
    return 1;
  }
  assert(argc == 2);


  // ヘッダ部の読み込み
  fgets(firstline, sizeof(firstline), fp);    // 形式を読み飛ばす
  fgets(secondline, sizeof(secondline), fp);  // コメントを呼び飛ばす

  fscanf(fp, "%d", &width);                   // 画素を取得
  fscanf(fp, "%d", &height);
  fscanf(fp, "%d", &bright);                  // 最大階調値を取得


  // データ部の読み込み
  fread(rgb, sizeof(RGB), width*height, fp);  // まとめて配列に読み込む
  fclose(fp);



  // カラー画像の赤だけを取り出して出力
  for (i = 0; i < width*height; i++) {
    gray[i] = rgb[i].red;
  }

  fp = fopen("red.ppm", "wb");

  fprintf(fp, "P5\n");
  fprintf(fp, "# Created by IrfanView\n");
  fprintf(fp, "%d %d\n", width, height);
  fprintf(fp, "%d\n", bright);
  fwrite(gray, sizeof(gray[0]), sizeof(gray) / sizeof(gray[0]), fp);
  fclose(fp);


  // カラー画像の緑だけを取り出して出力
  for (i = 0; i < width*height; i++) {
    gray[i] = rgb[i].green;
  }

  fp = fopen("green.ppm", "wb");

  fprintf(fp, "P5\n");
  fprintf(fp, "# Created by IrfanView\n");
  fprintf(fp, "%d %d\n", width, height);
  fprintf(fp, "%d\n", bright);
  fwrite(gray, sizeof(gray[0]), sizeof(gray) / sizeof(gray[0]), fp);
  fclose(fp);


  // カラー画像の青だけを取り出して出力
  for (i = 0; i < width*height; i++) {
    gray[i] = rgb[i].blue;
  }

  fp = fopen("blue.ppm", "wb");

  fprintf(fp, "P5\n");
  fprintf(fp, "# Created by IrfanView\n");
  fprintf(fp, "%d %d\n", width, height);
  fprintf(fp, "%d\n", bright);
  fwrite(gray, sizeof(gray[0]), sizeof(gray) / sizeof(gray[0]), fp);
  fclose(fp);


  // RGBの平均を求めて出力
  for (i = 0; i < width*height; i++) {
    gray[i] = (rgb[i].red + rgb[i].green + rgb[i].blue) / 3;
  }

  fp = fopen("avarage.ppm", "wb");

  fprintf(fp, "P5\n");
  fprintf(fp, "# Created by IrfanView\n");
  fprintf(fp, "%d %d\n", width, height);
  fprintf(fp, "%d\n", bright);
  fwrite(gray, sizeof(gray[0]), sizeof(gray) / sizeof(gray[0]), fp);
  fclose(fp);


  // 輝度Yを求めて出力
  for (i = 0; i < width*height; i++) {
    gray[i] = rgb[i].red * 0.299 + rgb[i].green * 0.587 + rgb[i].blue * 0.114;
  }

  fp = fopen("bright.ppm", "wb");

  fprintf(fp, "P5\n");
  fprintf(fp, "# Created by IrfanView\n");
  fprintf(fp, "%d %d\n", width, height);
  fprintf(fp, "%d\n", bright);
  fwrite(gray, sizeof(gray[0]), sizeof(gray) / sizeof(gray[0]), fp);
  fclose(fp);

  return 0;

}
