#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

void rgb2hsv(RGB rgb[], HSV hsv[]);
void hsv2rgb(RGB rgb[], HSV hsv[]);
void manipulate_HSV(FILE *fp, int width, int height, int maxdepth, RGB rgb[], HSV hsv[], unsigned char gray[]);


int main(int argc, char *argv[])
{
  FILE *fp;
  int width, height, maxdepth;
  char buf[64];
  RGB *rgb;
  HSV *hsv;
  unsigned char *gray;


  if (argc != 3)                            goto ARGUMENT_ERR;
  if ((fp = fopen(argv[1], "rb")) == NULL)  goto FILE_ERR;
  if ((fp = fopen(argv[2], "rb")) == NULL)  goto FILE_ERR;

  fgets(buf, sizeof(buf), fp);
  fgets(buf, sizeof(buf), fp);
  fscanf(fp, "%d %d\n %d\n", &width, &height, &maxdepth);

  if ((rgb = (RGB *)malloc(sizeof(RGB) * width * height)) == NULL)                       goto MEMORY_ERR;
  if ((hsv = (HSV *)malloc(sizeof(HSV) * width * height)) == NULL)                       goto MEMORY_ERR;
  if ((gray = (unsigned char *)malloc(sizeof(unsigned char) * width * height)) == NULL)  goto MEMORY_ERR;

  fread(rgb, sizeof(RGB), width * height, fp);

  free(rgb);
  free(hsv);
  free(gray);

  fclose(fp);

  return 0;


  FILE_ERR:
    printf("ファイルを開けません．\n");
    exit(1);

  ARGUMENT_ERR:
    printf("コマンドライン引数の数は2つにしてください．\n");
    exit(1);

  MEMORY_ERR:
    printf("メモリ領域の確保に失敗しました．\n");
    exit(1);
}


// RGBの各値からHSVを求める
void rgb2hsv(RGB rgb[], HSV hsv[])
{
  double R, G, B, H, S, V, min, max;

  // 最小値を0.0，最大値を1.0とする（255分率）
  R = rgb->r / 255.0;
  G = rgb->g / 255.0;
  B = rgb->b / 255.0;

  H = hsv->h;
  S = hsv->s;
  V = hsv->v;

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
  if (max == R)    H  = 60.0 * ((G - B) / (max - min));
  if (max == G)    H  = 60.0 * ((R - B) / (max - min)) + 120.0;
  if (max == B)    H  = 60.0 * ((R - G) / (max - min)) + 240.0;
  if (H < 0.0)     H += 360.0;
  if (max == min)  NULL;

  // Saturation（彩度）
  if (max == 0)    S = 0;
  if (max != 0)    S = (max - min) / max;

  // Value（明度）
  V = max;
}


// 顕色（HSVからRGBへ変換）
void hsv2rgb(RGB rgb[], HSV hsv[])
{
  double R, G, B, H, S, V, HH, f, p, q, t;
  int i = (int)H;

  R = rgb->r;
  G = rgb->g;
  B = rgb->b;
  H = hsv->h;
  S = hsv->s;
  V = hsv->v;

  R = V, G = V, B = V;
  HH = fmod(H / 60.0, 6);
  f = H / 60.0 - HH;
  p = V * (1.0 - S);
  q = V * (1.0 - S * (HH - i));
  t = V * (1.0 - (1.0 - f) * S);

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
    default:
      G = p;
      B = q;
      break;
  }
}


// HSVによる画質の改善
void manipulate_HSV(FILE *fp, int width, int height, int maxdepth, RGB rgb[], HSV hsv[], unsigned char gray[])
{
  int r, g, b, v;

  //? アルディアンシャーのパクリ（後で考える）
  for (int i = 0; i < width * height; i++) {
    r = rgb[i].r;
    g = rgb[i].g;
    b = rgb[i].b;

    // rgb2hsv(r, g, b, &h, &s, &v);
    v = gray[i];
    // hsv2rgb(h, s, v, &r, &g, &b);

    rgb[i].r = r;
    rgb[i].g = g;
    rgb[i].b = b;
  }
}