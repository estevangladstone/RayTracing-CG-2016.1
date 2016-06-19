#ifndef TORUS_H
#define TORUS_H

#include "Objeto_3d.h"

//* classe Torus #Torus#
class Torus : public Objeto_3D {
  protected:
    float raioDoAnel, raioDoTubo;
  public:
 //* Construtor e Destrutor
    Torus( int _indice_textura, float _raioDoAnel, float _raioDoTubo, const Ponto_3D& _centro );
    ~Torus() {}
 //* interseção e Normal
    Intersection Intercepta(const Raio& r_vis, IntersectionMode mode, float threshold) ;
    Vetor_3D normal( const Ponto_3D& ponto ) const ;
    TexturePoint pontoTextura(const Ponto_3D& ponto) const;
};
#endif // Torus_H
