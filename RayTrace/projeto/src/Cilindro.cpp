#include "cilindro.h"
#include <cmath>

Cilindro::Cilindro(int _indice_textura, float _raio, float _altura, const Ponto_3D& _centro) : Objeto_3D ( _indice_textura )
{
  raio = _raio;
  altura =_altura;
  centro =  _centro;
}


Intersection Cilindro::Intercepta(const Raio& r_vis, IntersectionMode mode, float threshold)
{
  float a, b, c, delta, hmin, hmax, t1, t2, y1, y2;
  Intersection intersection;

  // valores intermediários
  Vetor_3D K = Vetor_3D(r_vis.X0() - centro.X(),
                        r_vis.Y0() - centro.Y(),
                        r_vis.Z0() - centro.Z());


  //calcula dos limites superior e inferior do cilindro
  hmin=centro.Y()-altura/2;
  hmax=centro.Y()+altura/2;

  // montando a equação do 2º grau at2 + bt + c = 0
  a = (r_vis.Dx()*r_vis.Dx())+(r_vis.Dz()*r_vis.Dz());
  b = (2*(K.X()*r_vis.Dx()))+(2*(K.Z()*r_vis.Dz()));
  c = ((K.X()*K.X())+(K.Z()*K.Z())) - raio*raio;

  delta = b*b - 4*a*c;


  if (delta >= 0) {
      t1=(-b - sqrt(delta)) /(2*a);
      t2=(-b + sqrt(delta)) /(2*a);
      y1=K.Y()+t1*r_vis.Dy();
      y2=K.Y()+t2*r_vis.Dy();
      if(((y1>hmin)&&(y1<hmax))&&((y2<hmin)||(y2>hmax))){
        intersection = Intersection(this, t1);
      } else if(((y1<hmin)||(y1>hmax))&&((y2>hmin)&&(y2<hmax))){
        intersection = Intersection(this, t2);
      } else if (((y1>hmin)&&(y1<hmax))&&((y2>hmin)&&(y2<hmax))){
          intersection = Intersection::nearest(
                      Intersection(this, t1),
                      Intersection(this, t2),  threshold);
      }
  }

  return intersection;
}

Vetor_3D Cilindro::normal( const Ponto_3D& ponto ) const
{
    Vetor_3D tmp(ponto - centro);
    Vetor_3D centroNaAlturaDoPonto =  Vetor_3D(centro.X(), ponto.Y(), centro.Z());

    tmp = ponto - centroNaAlturaDoPonto;
    tmp.normaliza();

    return tmp;
}

TexturePoint Cilindro::pontoTextura(const Ponto_3D& ponto) const
{
    float phi, theta;
    Vetor_3D tmp(ponto - centro);

    tmp.normaliza();

    phi  = acosf(tmp.Z());
    theta = tmp.Y();

    return TexturePoint((phi/M_PI), theta);
}
