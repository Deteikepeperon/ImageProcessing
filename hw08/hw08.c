#include <stdio.h>
#include <stdlib.h>

typedef struct {
  unsigned char r;
  unsigned char g;
  unsigned char b;
} RGB;

typedef struct {
  double h;
  double s;
  double v;
} HSV;

void rgb2hsv(RGB rgb, HSV hsv[]);
void hsv2rgb(HSV hsv, RGB rgb[]);
void manipulate_HSV(FILE *fp, RGB rgb[], HSV hsv[], unsigned char gray[], int width, int height, int maxdepth);


int main(int argc, char *argv[])
{
  FILE *fp, *fp2;
  int width, height, maxdepth;
  char buf[128];
  RGB *rgb;
  HSV *hsv;
  unsigned char *gray;


  // 第一引数に低解像度のカラー画像(Low.ppm)，第二引数に高解像度のグレースケール画像(High.pgm)を指定する．
  if (argc != 3)                             goto ARGUMENT_ERR;
  if ((fp  = fopen(argv[1], "rb")) == NULL)  goto FILE_ERR;
  if ((fp2 = fopen(argv[2], "rb")) == NULL)  goto FILE_ERR;

  if ((rgb = (RGB *)malloc(sizeof(RGB) * width * height)) == NULL)                       goto MEMORY_ERR;
  if ((hsv = (HSV *)malloc(sizeof(HSV) * width * height)) == NULL)                       goto MEMORY_ERR;
  if ((gray = (unsigned char *)malloc(sizeof(unsigned char) * width * height)) == NULL)  goto MEMORY_ERR;

  fgets(buf, sizeof(buf), fp);
  fgets(buf, sizeof(buf), fp);
  fscanf(fp, "%d %d\n %d\n", &width, &height, &maxdepth);

  fread(rgb, sizeof(RGB), width * height, fp);

  fgets(buf, sizeof(buf), fp2);
  fgets(buf, sizeof(buf), fp2);
  fscanf(fp2, "%d %d\n %d\n", &width, &height, &maxdepth);

  fread(gray, sizeof(unsigned char), width * height, fp2);

  manipulate_HSV(fp, rgb, hsv, gray, width, height, maxdepth);

  free(rgb);
  free(hsv);
  free(gray);

  fclose(fp2);
  fclose(fp);

  return 0;


  ARGUMENT_ERR:
    printf("コマンドライン引数の数は2つにしてください．\n");
    exit(1);

  FILE_ERR:
    printf("ファイルを開けません．\n");
    exit(1);

  MEMORY_ERR:
    printf("メモリ領域の確保に失敗しました．\n");
    exit(1);
}


// RGBの各値からHSVを求める
void rgb2hsv(RGB rgb, HSV hsv[])
{
  double R, G, B, max, min;


  // 最小値を0.0，最大値を1.0とする（255分率）
  R = (double)rgb.r / 255.0;
  G = (double)rgb.g / 255.0;
  B = (double)rgb.b / 255.0;

  // 最大値・最小値
  if (R >= G && R >= B) {
    max = R;
    min = (G < B) ? G : B;
  }
  else if (G >= R && G >= B) {
    max = G;
    min = (R < B) ? R : B;
  }
  else {
    max = B;
    min = (R < G) ? R : G;
  }

  // Hue（色相）
  if (max == R)      hsv->h  = 60.0 * ((G - B) / (max - min));
  if (max == G)      hsv->h  = 60.0 * ((R - B) / (max - min)) + 120.0;
  if (max == B)      hsv->h  = 60.0 * ((R - G) / (max - min)) + 240.0;
  if (hsv->h < 0.0)  hsv->h += 360.0;
  if (max == min)    NULL;

  // Saturation（彩度）
  if (max == 0.0)    hsv->s = 0.0;
  if (max != 0.0)    hsv->s = (max - min) / max;

  // Value（明度）
  hsv->v = max;
}


// 顕色（HSVからRGBへ変換）
void hsv2rgb(HSV hsv, RGB rgb[])
{
  double R, G, B, HH, f, p, q, t;
  int i = (int)HH;


  HH = ((int)hsv.h / 60) % 6;
  f = hsv.h / 60.0 - HH;
  p = hsv.v * (1.0 - hsv.s);
  q = hsv.v * (1.0 - hsv.s * f);
  t = hsv.v * (1.0 - (1.0 - f) * hsv.s);

  R = G = B = hsv.v;

  switch (i) {
    case 0:
      G = t;
      B = p;
      break;
    case 1:
      R = q;
      B = p;
      break;
    case 2:
      R = p;
      B = t;
      break;
    case 3:
      R = p;
      G = q;
      break;
    case 4:
      R = t;
      G = p;
      break;
    case 5:
      G = p;
      B = q;
      break;
    default:
      break;
  }

  rgb->r = (unsigned char)(R * 255);
  rgb->g = (unsigned char)(G * 255);
  rgb->b = (unsigned char)(B * 255);
}


// HSVによる画質の改善
void manipulate_HSV(FILE *fp, RGB rgb[], HSV hsv[], unsigned char gray[], int width, int height, int maxdepth)
{
  for (int i = 0; i < width * height; i++) {
    rgb2hsv(rgb[i], &hsv[i]);
    hsv[i].v = gray[i] / 255.0;   // 高解像度グレースケール画像の輝度情報を使用
    hsv2rgb(hsv[i], &rgb[i]);
  }

  fp = fopen("manipulateHSV.ppm", "wb");
  fprintf(fp, "P6\n# HSV-manipulated image\n%d %d\n%d\n", width, height, maxdepth);
  fwrite(rgb, sizeof(RGB), width * height, fp);
}