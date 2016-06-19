#include "Torus.h"
#include <cmath>
#include "Poly34.h"

Torus::Torus(int _indice_textura, float _raioDoAnel, float _raioDoTubo, const Ponto_3D& _centro) : Objeto_3D ( _indice_textura )
{
  raioDoAnel = _raioDoAnel;
  raioDoTubo = _raioDoTubo;
  centro =  _centro;
}

Intersection Torus::Intercepta(const Raio& r_vis, IntersectionMode mode, float threshold)
{
  float a, b, c, d, e, hmin, hmax;
  float Dx, Dy, Dz, Px, Py, Pz, R, r;
  int numRaizes;
  double x[4];
  Vetor_3D D, P;
  Intersection intersection, i1, i2;

  //calcula dos limites superior e inferior do Torus
  hmin=centro.Y()-raioDoTubo;
  hmax=centro.Y()+raioDoTubo;
  R=raioDoAnel;
  r=raioDoTubo;

  D = r_vis.Direcao();
  Dx = r_vis.Dx();
  Dy = r_vis.Dy();
  Dz = r_vis.Dz();
  P = Vetor_3D(r_vis.X0(), r_vis.Y0(), r_vis.Z0());
  Px = r_vis.X0();
  Py = r_vis.Y0();
  Pz = r_vis.Z0();

  a = (D.produtoEscalar(D)*D.produtoEscalar(D)) + 2*(Dx*Dx*Dy*Dy + Dx*Dx*Dz*Dz + Dz*Dz*Dy*Dy);
  b = 4*((D.produtoEscalar(D)*D.produtoEscalar(D)) + Dx*Dx*Dy*Py + Dx*Dx*Dz*Pz + Dy*Dy*Dx*Px + Dy*Dy*Dz*Pz + Dz*Dz*Dx*Px + Dz*Dz*Dy*Py);
  c = 2*( ((R*R)*(Dz*Dz - Dx*Dx - Dy*Dy)) - ((r*r)*D.produtoEscalar(P)) + 3*(D.produtoEscalar(D)*P.produtoEscalar(P)) + 4*(Dx*Px*Dy*Py + Dx*Px*Dz*Pz + Dy*Py*Dz*Pz) + (Px*Px*Dy*Dy + Dx*Dx*Py*Py + Px*Px*Dz*Dz + Py*Py*Dz*Dz + Pz*Pz*Dx*Dx + Dy*Dy*Pz*Pz) );
  d = 4*( ((R*R)*(Dz*Pz - Dx*Px - Dy*Py)) - ((r*r)*D.produtoEscalar(P)) + ((D.produtoEscalar(P)*P.produtoEscalar(P)) + ((Px*Px*Dy*Py) + (Py*Py*Dx*Px) + (Px*Px*Dz*Pz) + (Py*Py*Dz*Pz) + (Pz*Pz*Dx*Px) + (Pz*Pz*Dy*Py))) );
  e = (R*R*R*R) + (r*r*r*r) + 2*(Px*Px*Py*Py + Px*Px*Pz*Pz + Pz*Pz*Py*Py) + (P.produtoEscalar(P)*P.produtoEscalar(P)) + 2*R*R*(Pz*Pz - Px*Px - Py*Py) - 2*r*r*(R*R + P.produtoEscalar(P));

  // Encontra as raizes do polinomio de grau quatro
      b = b/a;
      c = c/a;
      d = d/a;
      e = e/a;
      numRaizes = SolveP4(x, b, c, d, e);
      if(numRaizes > 0){
         if(numRaizes == 4){
               i1 = Intersection::nearest(
                           Intersection(this, x[0]),
                           Intersection(this, x[1]),  threshold);
               i2 = Intersection::nearest(
                           Intersection(this, x[2]),
                           Intersection(this, x[3]),  threshold);
               intersection = Intersection::nearest(
                           i1,
                           i2,  threshold);
         }
         else{
               intersection = Intersection::nearest(
                           Intersection(this, x[0]),
                           Intersection(this, x[1]),  threshold);
         }
      }

  return intersection;
}

Vetor_3D Torus::normal( const Ponto_3D& ponto ) const
{
    Vetor_3D tmp(ponto - centro);
    Vetor_3D centroNaAlturaDoPonto =  Vetor_3D(centro.X(), ponto.Y(), centro.Z());

    tmp = ponto - centroNaAlturaDoPonto;
    tmp.normaliza();

    return tmp;
}

TexturePoint Torus::pontoTextura(const Ponto_3D& ponto) const
{
    float phi, theta;
    Vetor_3D tmp(ponto - centro);

    tmp.normaliza();

    phi  = acosf(tmp.Z());
    theta = tmp.Y();

    return TexturePoint((phi/M_PI), theta);
}
