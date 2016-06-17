#include "cone.h"
#include <cmath>

Cone::Cone(int _indice_textura, float _raio, float _altura, const Ponto_3D& _centro) : Objeto_3D ( _indice_textura )
{
  raio = _raio;
  altura =_altura;
  centro =  _centro;
}


Intersection Cone::Intercepta(const Raio& r_vis, IntersectionMode mode, float threshold)
{
  float a, b, c, delta, hmin, hmax, t1, t2, y1, y2, raioCone, angulo, z1, z2;
  Ponto_3D centroBase;
  Intersection intersection;

  // valores intermediários
  Vetor_3D K = Vetor_3D(r_vis.X0() - centro.X(),
                        r_vis.Y0() - centro.Y(),
                        r_vis.Z0() - centro.Z());
  angulo = atan(raio/altura);

  //calculo do centro da base
  centroBase=centro;
  centroBase.Y(centroBase.Y()-altura);

  //calculo dos limites superior e inferior do Cone
  hmin=centroBase.Y();
  hmax=centro.Y();

//  float tangente = cos(angulo);
//  float g = r_vis.Y0() - centroBase.Y();
//  float w = raio - (tangente*g);

//  a = (r_vis.Dx()*r_vis.Dx())+(r_vis.Dz()*r_vis.Dz()) - ((tangente*tangente)*(r_vis.Dy()*r_vis.Dy()));
//  b = 2*(((K.X()*r_vis.Dx()))+((K.Z()*r_vis.Dz()) ) + tangente*r_vis.Dy()*g);
//  c = ((K.X()*K.X())+(K.Z()*K.Z())) - (w*w);

    //função do raio

    //raioCone =(centro.Y()-r_vis.Dy())*tan(angulo);

  // montando a equação do 2º grau at2 + bt + c = 0
  a = (r_vis.Dx()*r_vis.Dx())+(r_vis.Dz()*r_vis.Dz());
  b = (2*(K.X()*r_vis.Dx()))+(2*(K.Z()*r_vis.Dz()));
  c = ((K.X()*K.X())+(K.Z()*K.Z())) - raio*raio;

 delta = b*b - 4*a*c;
  if(delta>=0){
      t1=(-b - sqrt(delta)) /(2*a);
      t2=(-b + sqrt(delta)) /(2*a);
      y1=K.Y()+t1*r_vis.Dy();
      y2=K.Y()+t2*r_vis.Dy();
  }
  //função do raio

           raioCone =(centro.Y()-y2)*tan(angulo);

           raioCone =(centro.Y()-y1)*tan(angulo);



    c = ((K.X()*K.X())+(K.Z()*K.Z())) - raioCone*raioCone;
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

Vetor_3D Cone::normal( const Ponto_3D& ponto ) const
{
    Vetor_3D tmp(ponto - centro);
    Vetor_3D centroNaAlturaDoPonto =  Vetor_3D(centro.X(), ponto.Y(), centro.Z());

    tmp = ponto - centroNaAlturaDoPonto;
    tmp.normaliza();

    return tmp;

}

TexturePoint Cone::pontoTextura(const Ponto_3D& ponto) const
{
    float phi, theta;
    Vetor_3D tmp(ponto - centro);

    tmp.normaliza();

    phi  = acosf(tmp.Z());
    theta = tmp.Y();

    return TexturePoint((phi/M_PI), theta);
}
