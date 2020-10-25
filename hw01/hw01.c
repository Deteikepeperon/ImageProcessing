#include <stdio.h>
#include <stdlib.h>

typedef struct {
  unsigned char red;
  unsigned char blue;
  unsigned char green;
} RGB;

int main(int argc, char *argv[]) {
  int i;
  int width, height, bright, img_size;
  char firstline[100];
  char secondline[100];

  RGB *rgb;
  unsigned char *gray;


  // 画像ファイルの読み込み
  FILE* fp = fopen(argv[1], "rb");            // 実行時に第一引数として指定 (./hw01 pepper.ppm)

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
  rgb = (RGB *)malloc(sizeof(RGB) * img_size);
  gray = (unsigned char *)malloc(sizeof(unsigned char) * img_size);
  fread(rgb, sizeof(RGB), img_size, fp);


  // R
  for (i = 0; i < img_size; i++) {
    gray[i] = rgb[i].red;
  }

  fp = fopen("red.ppm", "wb");
  fprintf(fp, "P5\n# Grayscale image\n%d %d\n%d\n", width, height, bright);
  fwrite(gray, sizeof(unsigned char), img_size, fp);


  // G
  for (i = 0; i < img_size; i++) {
    gray[i] = rgb[i].green;
  }

  fp = fopen("green.ppm", "wb");
  fprintf(fp, "P5\n# Grayscale image\n%d %d\n%d\n", width, height, bright);
  fwrite(gray, sizeof(unsigned char), img_size, fp);


  // B
  for (i = 0; i < img_size; i++) {
    gray[i] = rgb[i].blue;
  }

  fp = fopen("blue.ppm", "wb");
  fprintf(fp, "P5\n# Grayscale image\n%d %d\n%d\n", width, height, bright);
  fwrite(gray, sizeof(unsigned char), img_size, fp);


  // RGBの平均
  for (i = 0; i < img_size; i++) {
    gray[i] = (rgb[i].red + rgb[i].green + rgb[i].blue) / 3;
  }

  fp = fopen("avarage.ppm", "wb");
  fprintf(fp, "P5\n# Grayscale image\n%d %d\n%d\n", width, height, bright);
  fwrite(gray, sizeof(unsigned char), img_size, fp);

  // 輝度Y
  for (i = 0; i < img_size; i++) {
    gray[i] = rgb[i].red * 0.299 + rgb[i].green * 0.587 + rgb[i].blue * 0.114;
  }

  fp = fopen("bright.ppm", "wb");
  fprintf(fp, "P5\n# Grayscale image\n%d %d\n%d\n", width, height, bright);
  fwrite(gray, sizeof(unsigned char), img_size, fp);


  fclose(fp);

  return 0;
}
