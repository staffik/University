#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define PI 3.14159265

typedef long double Typ;

struct point{Typ x, y, z; struct point * next;};

#define Point struct point

Point * make_Point(Typ X, Typ Y, Typ Z)
{
  Point * nowy = malloc(sizeof(Point));
  nowy->x=X;
  nowy->y=Y;
  nowy->z=Z;
  nowy->next = NULL;
  return nowy;
}

struct vector{Point * first, * last;};

#define Vector struct vector

Vector cube;
int t;

void append(Vector V)
{
  if(cube.first == NULL)
  {
    cube = V;
    return;
  }
  (cube.last)->next = V.first;
  (cube.last) = V.last;
}

void Init(Point * start)
{
  cube.first = start;
  start->next = NULL;
  cube.last = start;
}

Point * copy_Point(Point * it)
{
  Point * res;
  res = make_Point(it->x,it->y,it->z);
  return res;
}

Vector copy_Vector(Vector V)
{
  Vector res;
  (res.first) = copy_Point(V.first);
  Point * it = res.first;
  Point * jt = (V.first)->next;
  while(jt != NULL)
  {
    it->next = copy_Point(jt);
    it = (it->next);
    jt = (jt->next);
  }
  it->next = NULL;
  res.last = it;
  return res;
}

void clear(Vector V)
{
  Point * it = V.first;
  Point * tmp;
  while(it != NULL)
  {
    tmp = it;
    it = it->next;
    free(tmp);
  }
}

void move(Vector V, Typ ox, Typ oy, Typ oz)
{
  Point * it;
  for(it = V.first; it!=NULL; it=(it->next))
  {
    (it->x)+=ox;
    (it->y)+=oy;
    (it->z)+=oz;
  }
}

void rotateA(Vector tmp)
{
  Point * it;
  for(it = tmp.first; it!=NULL; it = (it->next))
  {
    Typ c = it->y;
    it->y = t-it->z;
    it->z = c;
  }
}

void rotateB(Vector tmp)
{
  Point * it;
  for(it = tmp.first; it!=NULL; it = (it->next))
  {
    Typ c = it->x;
    it->x = it->z;
    it->z = t-c;
  }
}

void rotateC(Vector tmp)
{
  Point * it;
  for(it = tmp.first; it!=NULL; it = (it->next))
  {
    Typ c = it->x;
    it->x = t-it->y;
    it->y = c;
  }
}

Vector A(Vector tmp)
{
  Vector res;
  res = copy_Vector(tmp);
  rotateA(res);
  rotateB(res);
  return res;
}

Vector B(Vector tmp)
{
  Vector res;
  res = copy_Vector(tmp);
  rotateC(res);
  int i;
  for(i=0; i<3; i++) rotateB(res);
  return res;
}

Vector C(Vector tmp)
{
  Vector res;
  res = copy_Vector(tmp);
  int i;
  for(i=0; i<2; i++) rotateC(res);
  return res;
}

Vector D(Vector tmp)
{
  Vector res;
  res = copy_Vector(tmp);
  rotateC(res);
  rotateB(res);
  return res;
}

Vector E(Vector tmp)
{
  Vector res;
  res = copy_Vector(tmp);
  int i;
  for(i=0; i<3; i++) rotateA(res);
  for(i=0; i<3; i++) rotateB(res);
  return res;
}

void draw_curve(int n)
{
  Init(make_Point(0,0,0));
  Vector next, tmp;
  int i;
  for(i=1; i<=n; i++)
  {
    tmp = copy_Vector(cube);
    clear(cube);
    cube.first = cube.last = NULL;

    int off = 1<<(i-1);
    t = off-1;

    next = A(tmp);
    move(next,off,off,0);
    append(next);

    next = B(tmp);
    move(next,off,0,0);
    append(next);

    next = B(tmp);
    move(next,0,0,0);
    append(next);

    next = C(tmp);
    move(next,0,off,0);
    append(next);

    next = C(tmp);
    move(next,0,off,off);
    append(next);

    next = D(tmp);
    move(next,0,0,off);
    append(next);

    next = D(tmp);
    move(next,off,0,off);
    append(next);

    next = E(tmp);
    move(next,off,off,off);
    append(next);

    clear(tmp);
    tmp.first = tmp.last = NULL;
  }
}

Point * middle;

void scale(Point * p, Typ u)
{
  p->x *= u;
  p->y *= u;
  p->z *= u;
}

Typ cosinus, sinus;

void turnOX(Point * p)
{
  p->x -= middle->x;
  p->y -= middle->y;
  p->z -= middle->z;
  Typ y = p->y, z = p->z;
  p->y = cosinus*y - sinus*z;
  p->z = sinus*y + cosinus*z;
  p->x += middle->x;
  p->y += middle->y;
  p->z += middle->z;
}

void turnOY(Point * p)
{
  p->x -= middle->x;
  p->y -= middle->y;
  p->z -= middle->z;
  Typ x = p->x, z = p->z;
  p->x = cosinus*x + sinus*z;
  p->z = -sinus*x + cosinus*z;
  p->x += middle->x;
  p->y += middle->y;
  p->z += middle->z;
}

void projection(Point * p, Typ d)
{
  p->x = (d * p->x) / (d + p->z);
  p->y = (d * p->y) / (d + p->z);
}

void transform_cube(int n, Typ s, Typ u, Typ d, int x, int y, int z, Typ alfa, Typ beta)
{
  alfa *= -PI/180.0;
  beta *= PI/180.0;
  Typ edge_len = (Typ) (1<<n)-1;
  Point * it;
  for(it=cube.first; it!=NULL; it = (it->next))
  {
    scale(it,u/edge_len);
  }
  Typ mid = u/2.0;
  middle = make_Point(mid,mid,mid);
  sinus = sin(alfa);
  cosinus = cos(alfa);
  for(it=cube.first; it!=NULL; it = (it->next))
  {
    turnOX(it);
  }
  sinus = sin(beta);
  cosinus = cos(beta);
  for(it=cube.first; it!=NULL; it = (it->next))
  {
    turnOY(it);
  }
  move(cube,(long double)x,(long double)y,(long double)z);
  for(it=cube.first; it!=NULL; it = (it->next))
  {
    projection(it,d);
  }
  move(cube,(s-u)/2.0,(s-u)/2.0,0.0);
}

int main(int argc, char * argv[])
{
  int n, s, u, d, x, y, z;
  Typ alfa, beta;
  sscanf(argv[1], "%d", &n);
  sscanf(argv[2], "%d", &s);
  sscanf(argv[3], "%d", &u);
  sscanf(argv[4], "%d", &d);
  sscanf(argv[5], "%d", &x);
  sscanf(argv[6], "%d", &y);
  sscanf(argv[7], "%d", &z);
  sscanf(argv[8], "%Lf", &alfa);
  sscanf(argv[9], "%Lf", &beta);
  Typ g = 0.4;
  draw_curve(n);
  transform_cube(n,s,u,d,x,y,z,alfa,beta);
  printf("\%%!PS-Adobe-2.0 EPSF-2.0\n%%%%BoundingBox 0 0 %d %d\nnewpath\n", s, s);
  printf("%.2Lf %.2Lf moveto\n", (cube.first)->x, (cube.first)->y);
  Point * it;
  for(it=cube.first; it != NULL; it = (it->next))
  {
    printf("%.2Lf %.2Lf lineto\n", it->x, it->y);
  }
  printf("%.2Lf setlinewidth\nstroke\nshowpage\n%%%%Trailer\n\%%EOF\n", g);
  clear(cube);
  return 0;
}
