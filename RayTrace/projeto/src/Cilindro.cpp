#include "Cilindro.h"
#include <cmath>

Cilindro::Cilindro(int _indice_textura, float _raio, float _altura, const Ponto_3D& _centro) : Objeto_3D ( _indice_textura )
{
  raio = _raio;
  altura =_altura;
  centro =  _centro;
}


//Foley capítulo 15.10, fórmula 15.17
Intersection Cilindro::Intercepta(const Raio& r_vis, IntersectionMode mode, float threshold)
{
  float a, b, c, delta, hmin, hmax, t1, t2, y1, y2;
  Intersection intersection, plano, sides;

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

  Vetor_3D pontoNoPlanoSuperior(centro.X(), hmax, centro.Z());
  Vetor_3D pontoNoPlanoInferior(centro.X(), hmin, centro.Z());
  Vetor_3D normalAosPlanos(0, 1, 0); // A base do cilindro é paralela ao plano XZ

  float naoParalelos = r_vis.Direcao().produtoEscalar(normalAosPlanos);

  if (delta >= 0) {
      t1=(-b - sqrt(delta)) /(2*a);
      t2=(-b + sqrt(delta)) /(2*a);
      y1=K.Y()+t1*r_vis.Dy();
      y2=K.Y()+t2*r_vis.Dy();
      if(((y1 >= hmin)&&(y1 <= hmax)) && ((y2 < hmin)||(y2 > hmax))){
        intersection = Intersection(this, t1);
//        if(naoParalelos){
//          float p1 = normalAosPlanos.produtoEscalar(Vetor_3D(pontoNoPlanoInferior - Vetor_3D(r_vis.Origem().X(), r_vis.Origem().Y(), r_vis.Origem().Z())))/naoParalelos;
//          float p2 = normalAosPlanos.produtoEscalar(Vetor_3D(pontoNoPlanoSuperior - Vetor_3D(r_vis.Origem().X(), r_vis.Origem().Y(), r_vis.Origem().Z())))/naoParalelos;
//          plano = Intersection::nearest(
//                      Intersection(this, p1),
//                      Intersection(this, p2),  threshold);
//          intersection = Intersection::nearest(
//                      plano,
//                      Intersection(this, t1),  threshold);
//        }
      } else if(((y1 <= hmin)||(y1 >= hmax))&&((y2 > hmin)&&(y2 < hmax))){
          intersection = Intersection(this, t2);
//        if(naoParalelos){
//          float p1 = normalAosPlanos.produtoEscalar(Vetor_3D(pontoNoPlanoInferior - Vetor_3D(r_vis.Origem().X(), r_vis.Origem().Y(), r_vis.Origem().Z())))/naoParalelos;
//          float p2 = normalAosPlanos.produtoEscalar(Vetor_3D(pontoNoPlanoSuperior - Vetor_3D(r_vis.Origem().X(), r_vis.Origem().Y(), r_vis.Origem().Z())))/naoParalelos;
//          plano = Intersection::nearest(
//                      Intersection(this, p1),
//                      Intersection(this, p2),  threshold);
//          intersection = Intersection::nearest(
//                      plano,
//                      Intersection(this, t1),  threshold);
//        }
      } else if (((y1>hmin)&&(y1<hmax))&&((y2>hmin)&&(y2<hmax))){
//          if(naoParalelos){
//            float p1 = normalAosPlanos.produtoEscalar(Vetor_3D(pontoNoPlanoInferior - Vetor_3D(r_vis.Origem().X(), r_vis.Origem().Y(), r_vis.Origem().Z())))/naoParalelos;
//            float p2 = normalAosPlanos.produtoEscalar(Vetor_3D(pontoNoPlanoSuperior - Vetor_3D(r_vis.Origem().X(), r_vis.Origem().Y(), r_vis.Origem().Z())))/naoParalelos;
//            plano = Intersection::nearest(
//                        Intersection(this, p1),
//                        Intersection(this, p2),  threshold);
//            sides = Intersection::nearest(
//                        Intersection(this, t2),
//                        Intersection(this, t1),  threshold);
//            intersection = Intersection::nearest(
//                        plano,
//                        sides,  threshold);
//          }
//          else{
              intersection = Intersection::nearest(
                          Intersection(this, t1),
                          Intersection(this, t2),  threshold);
//          }
      }
  }

  return intersection;
}

Vetor_3D Cilindro::normal( const Ponto_3D& ponto ) const
{
    Vetor_3D tmp;

//    if(ponto.Y() == (centro.Y() + altura/2)){
//        tmp = Vetor_3D(0, 1, 0);
//    } else if(ponto.Y() == (centro.Y() - altura/2)) {
//        tmp = Vetor_3D(0, -1, 0);
//    } else {
        Vetor_3D centroNaAlturaDoPonto(centro.X(), ponto.Y(), centro.Z());
        tmp = ponto - centroNaAlturaDoPonto;
        tmp.normaliza();
//    }

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
