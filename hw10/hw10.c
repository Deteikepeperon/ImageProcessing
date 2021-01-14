#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
} RGB;

#define THRESHOLD 100   // しきい値

void binariz(unsigned char gray[256][256], int width, int height);
void labeling_gray(FILE *fp_gray, unsigned char gray[256][256], unsigned char gray_lab[256][256], int width, int height, int maxdepth);
void labeling_color(FILE *fp_rgb, unsigned char gray_lab[256][256], RGB rgb_lab[256][256], int width, int height, int maxdepth);
int linking(unsigned char gray[256][256], unsigned char gray_lab[256][256], int i, int j, int label);


int main(int argc, char *argv[])
{
  FILE *fp, *fp_gray, *fp_rgb;
  int width, height, maxdepth;
  char buf[64];
  unsigned char gray[256][256];
  unsigned char gray_lab[256][256];
  RGB rgb_lab[256][256];


  if (argc != 2)                            goto ARGUMENT_ERR;
  if ((fp = fopen(argv[1], "rb")) == NULL)  goto FILE_ERR;

  fgets(buf, sizeof(buf), fp);
  fgets(buf, sizeof(buf), fp);
  fscanf(fp, "%d %d\n%d\n", &width, &height, &maxdepth);

  fread(gray, sizeof(unsigned char), width * height, fp);


  // binariz(gray, width, height);

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      gray[i][j] = (gray[i][j] >= THRESHOLD) ? 255 : 0;
    }
  }


  // labeling_gray(fp_gray, gray, gray_lab, width, height, maxdepth);

  int label, count;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      if (gray[i][j] == 0 && gray_lab[i][j] == 0) {
        label += 7; // 最大階調値 ÷ 最大ラベル数 ≒ 7
        linking(gray, gray_lab, i, j, label);
        count++;
      }
    }
  }
  printf("最大ラベル番号 = %d\n", count);

  fp_gray = fopen("converted_img/gray_labering.pgm", "wb");
  fprintf(fp_gray, "P5\n# Grayscale image\n%d %d\n%d\n", width, height, maxdepth);
  fwrite(gray_lab, sizeof(unsigned char), width * height, fp_gray);


  // labeling_color(fp_rgb, gray_lab, rgb_lab, width, height, maxdepth);

  int color[33][3];

  // 割り当てるカラーをランダム生成
  srand((unsigned int)time(NULL));
  for (int i = 0; i < 33; i++) {
    for (int j = 0; j < 3; j++) {
      color[i][j] = rand() % maxdepth;
    }
  }

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      if (gray_lab[i][j] != 0) {
        rgb_lab[i][j].red   = color[gray_lab[i][j] / 7][0];
        rgb_lab[i][j].green = color[gray_lab[i][j] / 7][1];
        rgb_lab[i][j].blue  = color[gray_lab[i][j] / 7][2];
      }
    }
  }

  fp_rgb = fopen("converted_img/color_labering.pgm", "wb");
  fprintf(fp_rgb, "P6\n# Color image\n%d %d\n%d\n", width, height, maxdepth);
  fwrite(rgb_lab, sizeof(RGB), width * height, fp_rgb);


  fclose(fp);
  fclose(fp_gray);
  fclose(fp_rgb);

  return 0;


  ARGUMENT_ERR:
    printf("コマンドライン引数の数は1つにしてください．\n");
    exit(1);

  FILE_ERR:
    printf("ファイルを開けません．\n");
    exit(1);
}


// 固定しきい値による二値化
void binariz(unsigned char gray[256][256], int width, int height)
{
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      gray[i][j] = (gray[i][j] >= THRESHOLD) ? 255 : 0;
    }
  }
}


// ラベリング（グレー画像）
void labeling_gray(FILE *fp_gray, unsigned char gray[256][256], unsigned char gray_lab[256][256], int width, int height, int maxdepth)
{
  int label, count;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      if (gray[i][j] == 0 && gray_lab[i][j] == 0) {
        label += 7; // 最大階調値 ÷ 最大ラベル数 ≒ 7
        linking(gray, gray_lab, i, j, label);
        count++;
      }
    }
  }
  printf("最大ラベル番号 = %d\n", count);

  fp_gray = fopen("converted_img/gray_labering.pgm", "wb");
  fprintf(fp_gray, "P5\n# Grayscale image\n%d %d\n%d\n", width, height, maxdepth);
  fwrite(gray_lab, sizeof(unsigned char), width * height, fp_gray);
}


// ラベリング（カラー画像）
void labeling_color(FILE *fp_rgb, unsigned char gray_lab[256][256], RGB rgb_lab[256][256], int width, int height, int maxdepth)
{
  int color[33][3];

  // 割り当てるカラーをランダム生成
  srand((unsigned int)time(NULL));
  for (int i = 0; i < 33; i++) {
    for (int j = 0; j < 3; j++) {
      color[i][j] = rand() % maxdepth;
    }
  }

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      if (gray_lab[i][j] != 0) {
        rgb_lab[i][j].red   = color[gray_lab[i][j] / 7][0];
        rgb_lab[i][j].green = color[gray_lab[i][j] / 7][1];
        rgb_lab[i][j].blue  = color[gray_lab[i][j] / 7][2];
      }
    }
  }

  fp_rgb = fopen("converted_img/color_labering.pgm", "wb");
  fprintf(fp_rgb, "P6\n# Color image\n%d %d\n%d\n", width, height, maxdepth);
  fwrite(rgb_lab, sizeof(RGB), width * height, fp_rgb);
}


// 連結領域の判定（8連結，再帰処理）
int linking(unsigned char gray[256][256], unsigned char gray_lab[256][256], int i, int j, int label)
{
  if (i < 0 || i > 255 || j < 0 || j > 255 || gray[i][j] == 255 || gray_lab[i][j] != 0)  return 0;

  gray_lab[i][j] = label;

  return (
    linking(gray, gray_lab, i - 1, j - 1, label) +
    linking(gray, gray_lab, i - 1, j    , label) +
    linking(gray, gray_lab, i - 1, j + 1, label) +
    linking(gray, gray_lab, i    , j - 1, label) +
    linking(gray, gray_lab, i    , j    , label) +
    linking(gray, gray_lab, i    , j + 1, label) +
    linking(gray, gray_lab, i + 1, j - 1, label) +
    linking(gray, gray_lab, i + 1, j    , label) +
    linking(gray, gray_lab, i + 1, j + 1, label)
  );
}