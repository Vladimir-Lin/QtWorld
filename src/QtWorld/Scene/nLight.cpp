#include <qtworld.h>

N::Light:: Light     ( void         )
         : SceneNode ( Scene::Light )
         , lightId   ( -1           )
         , asObject  ( false        )
{
}

N::Light:: Light     ( SceneDestructor * des )
         : SceneNode ( Scene::Light          )
         , lightId   ( -1                    )
         , asObject  ( false                 )
{
  destructor = des   ;
  JoinDestructor ( ) ;
}

N::Light::~Light (void)
{
}

void N::Light::setVector(GLenum pname,ControlPoint & Point)
{
  properties [ pname ] = true  ;
  vectors    [ pname ] = Point ;
}

void N::Light::setValue(GLenum pname,double value)
{
  properties [ pname ] = true  ;
  values     [ pname ] = value ;
}

void N::Light::Enable(void)
{
  if (lightId<0) return              ;
  QtGL::Enable  ( (GLenum) lightId ) ;
}

void N::Light::Disable(void)
{
  if ( lightId < 0 ) return          ;
  QtGL::Disable ( (GLenum) lightId ) ;
}

void N::Light::Place(void)
{
  QList<int> P = properties.keys()    ;
  int        p                        ;
  QtGL::PushMatrix (                ) ;
  foreach (p,P) if (properties[p])    {
    switch (p)                        {
      case GL_AMBIENT                 :
      case GL_DIFFUSE                 :
      case GL_SPECULAR                :
      case GL_POSITION                :
      case GL_SPOT_DIRECTION          :
        if (vectors.contains(p))      {
          vectors[p].Light(lightId,p) ;
        }                             ;
      break                           ;
      case GL_SPOT_EXPONENT           :
      case GL_SPOT_CUTOFF             :
      case GL_CONSTANT_ATTENUATION    :
      case GL_LINEAR_ATTENUATION      :
      case GL_QUADRATIC_ATTENUATION   :
        if (values.contains(p))       {
          GLfloat f[2] = { 0 , 0 }    ;
          f[0] = values[p]            ;
          QtGL::Lightfv(lightId,p,f)  ;
        }                             ;
      break                           ;
    }                                 ;
  }                                   ;
  Transform ( )                       ;
  QtGL::PopMatrix  (                ) ;
}

void N::Light::setAmbient(QColor & color)
{
  ControlPoint C ( color           ) ;
  setVector      ( GL_AMBIENT  , C ) ;
}

void N::Light::setDiffuse(QColor & color)
{
  ControlPoint C ( color           ) ;
  setVector      ( GL_DIFFUSE  , C ) ;
}

void N::Light::setSpecular(QColor & color)
{
  ControlPoint C ( color           ) ;
  setVector      ( GL_SPECULAR , C ) ;
}

void N::Light::setPosition(ControlPoint & Position)
{
  setVector ( GL_POSITION , Position );
}

void N::Light::setDirection(ControlPoint & Direction)
{
  setVector ( GL_SPOT_DIRECTION , Direction ) ;
}

void N::Light::setExponent(double Exponent)
{
  setValue ( GL_SPOT_EXPONENT , Exponent ) ;
}

void N::Light::setCutoff(double Cutoff)
{
  setValue ( GL_SPOT_CUTOFF , Cutoff ) ;
}

void N::Light::setConstant(double Constant)
{
  setValue ( GL_CONSTANT_ATTENUATION , Constant ) ;
}

void N::Light::setLinear(double Linear)
{
  setValue ( GL_LINEAR_ATTENUATION , Linear ) ;
}

void N::Light::setQuadratic(double Quadratic)
{
  setValue ( GL_QUADRATIC_ATTENUATION , Quadratic ) ;
}

void N::Light::Render(void)
{
}
