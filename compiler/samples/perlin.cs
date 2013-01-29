
#include "output.hs"
#include "math.hs"

float persistence = 0.5;
int Number_Of_Octaves = 6; 

float Noise1(int x, int y)	
{
  int n = x + y * 57;
  n = (n<<13) ^ n;
  float res = (1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
  return res;
}

float CosineInterpolate(float a, float b, float x)
{
	float ft = x * 3.1415927;
	float f = (1.0 - cosf(ft)) * 0.5;

	float res = a*(1.0-f) + b*f;
   
	return  res;
}

float CubicInterpolate(float v0, float v1, float v2, float v3, float x)
{
	float P = (v3 - v2) - (v0 - v1);
	float Q = (v0 - v1) - P;
	float R = v2 - v0;
	float S = v1;
	return P * (x * x * x) + Q * (x * x) + R * x + S;
}

float SmoothNoise1(int x, int y)
{
  float corners = ( Noise1(x-1, y-1) + Noise1(x+1, y-1) + Noise1(x-1, y+1) + Noise1(x+1, y+1) ) / 16.0;
  float sides   = ( Noise1(x-1, y)  + Noise1(x+1, y)  +Noise1(x, y-1)  +Noise1(x, y+1) ) /  8.0;
  float center  =  Noise1(x, y) / 4.0;
  
  float res = corners + sides + center;  

  return res;
}


float InterpolatedNoise1(float x, float y)
{
  int integer_X    = (int)x;
  float fractional_X = x - integer_X;

  int integer_Y    = (int)y;
  float fractional_Y = y - integer_Y;

  float v1 = SmoothNoise1(integer_X,     integer_Y);
  float v2 = SmoothNoise1(integer_X + 1, integer_Y);
  float v3 = SmoothNoise1(integer_X,     integer_Y + 1);
  float v4 = SmoothNoise1(integer_X + 1, integer_Y + 1); 

  float i1 = CosineInterpolate(v1 , v2 , fractional_X);
  float i2 = CosineInterpolate(v3 , v4 , fractional_X);

  return CosineInterpolate(i1 , i2 , fractional_Y);
}

float PerlinNoise2D(float x, float y)
{
  float total = 0.0;
  float p = persistence;
  int n = Number_Of_Octaves - 1;
   
  float amplitude = 1.0;
  
  for (int i = 0; i < n; ++i)
  {
    float frequency = 1 << i;
    
    total = total + InterpolatedNoise1(x * frequency, y * frequency) * amplitude;
    amplitude = amplitude * p;
  }
  
  return total;
}

int range(float val)
{
  return (int)((val + 1.0)* 127.5);
}

int main()
{
  //float  x = IntNoise(10);

  fprintln("out.txt", "P3");
  fprintln("out.txt", "256 256");
  fprintln("out.txt", "255");
  
  for (int i = 0; i < 256; ++i)
      for (int j = 0; j < 256; ++j)
      {
        int color = range(PerlinNoise2D(i / 16.0, j / 16.0));
        fprint("out.txt", (string)color); 
        fprint("out.txt", " "); 
        fprint("out.txt", (string)color); 
        fprint("out.txt", " "); 
        fprint("out.txt", (string)color); 
        fprint("out.txt", " "); 
      }
}


//0.92940825


