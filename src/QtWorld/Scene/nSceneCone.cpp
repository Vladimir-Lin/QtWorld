#include <qtworld.h>

N::SceneCone:: SceneCone   ( void )
             : SceneObject (      )
             , Cylinder    (      )
{
  name = "Cone" ;
}

N::SceneCone:: SceneCone   ( SceneDestructor * des )
             : SceneObject ( des                   )
             , Cylinder    (                       )
{
  name = "Cone" ;
}

N::SceneCone::~SceneCone (void)
{
}

void N::SceneCone::renderSmooth(void)
{
  if ( Cache . size ( ) <= 0 ) return                  ;
  #if   defined(Q_OS_ANDROID)
  #elif defined(Q_OS_IOS)
  #else
  QtGL::Enable ( GL_AUTO_NORMAL   )                    ;
  QtGL::Enable ( GL_MAP2_VERTEX_3 )                    ;
  #endif
  colors [ 0 ] . Color4d ( )                           ;
  //////////////////////////////////////////////////////
  int      s = 0                                       ;
  double * d = (double *) cache ( )                    ;
  double * r                                           ;
  for (int z = 0 ; z < 4 ; z++ )                       {
    s = z * 96                                         ;
    r = & d [ s ]                                      ;
    #if   defined(Q_OS_ANDROID)
    #elif defined(Q_OS_IOS)
    #else
    QtGL::Map2d ( GL_MAP2_VERTEX_3                     ,
                  0, 1.0, 3   , 4                      ,
                  0, 1.0, 3*4 , 8                      ,
                  r                                  ) ;
    QtGL::MapGrid2d ( 50 , 0.0 , 1.0 , 50 , 0.0, 1.0 ) ;
    for (int j = 0 ; j <= 50 ; j++ )                   {
      double y = j                                     ;
      y /= 50.0                                        ;
      QtGL::Strips ( )                                 ;
      for (int i = 0 ; i <= 50 ; i++ )                 {
        double x = i                                   ;
        x /= 50.0                                      ;
        QtGL::EvalCoord2d ( x , y )                    ;
      }                                                ;
      QtGL::End    ( )                                 ;
      QtGL::Strips ( )                                 ;
      for (int i = 0 ; i <= 50 ; i++ )                 {
        double x = i                                   ;
        x /= 50.0                                      ;
        QtGL::EvalCoord2d ( y , x )                    ;
      }                                                ;
      QtGL::End    ( )                                 ;
    }                                                  ;
    #endif
  }                                                    ;
  //////////////////////////////////////////////////////
  #if   defined(Q_OS_ANDROID)
  #elif defined(Q_OS_IOS)
  #else
  QtGL::Disable ( GL_MAP2_VERTEX_3 )                   ;
  QtGL::Disable ( GL_AUTO_NORMAL   )                   ;
  #endif
}

void N::SceneCone::renderSurfaces(void)
{
  if ( Cache . size ( ) <= 0 ) return                    ;
  #if   defined(Q_OS_ANDROID)
  #elif defined(Q_OS_IOS)
  #else
  QtGL::Enable ( GL_AUTO_NORMAL   )                      ;
  QtGL::Enable ( GL_MAP2_VERTEX_3 )                      ;
  #endif
  colors [ 0 ] . Color4d ( )                             ;
  ////////////////////////////////////////////////////////
  int      s = 0                                         ;
  double * d = (double *) cache ( )                      ;
  double * r                                             ;
  for (int i = 0 ; i < 4 ; i++ )                         {
    s = i * 96                                           ;
    r = & d [ s ]                                        ;
    #if   defined(Q_OS_ANDROID)
    #elif defined(Q_OS_IOS)
    #else
    QtGL::Map2d     ( GL_MAP2_VERTEX_3                   ,
                      0 , 1.0 , 3     , 4                ,
                      0 , 1.0 , 3 * 4 , 8                ,
                      r                                ) ;
    QtGL::MapGrid2d ( 50 , 0.0 , 1.0 , 50 , 0.0 , 1.0  ) ;
    QtGL::EvalMesh2 ( GL_FILL  , 0 , 50 , 0 , 50       ) ;
    #endif
  }                                                      ;
  ////////////////////////////////////////////////////////
  #if   defined(Q_OS_ANDROID)
  #elif defined(Q_OS_IOS)
  #else
  QtGL::Disable ( GL_MAP2_VERTEX_3 )                     ;
  QtGL::Disable ( GL_AUTO_NORMAL   )                     ;
  #endif
}

void N::SceneCone::Align(void)
{
  double        x [ 16 ]       ;
  double        y [ 16 ]       ;
  double        r [  8 ]       ;
  double        f [  8 ]       ;
  double        k              ;
  ControlPoint V               ;
  ControlPoint X               ;
  ControlPoint Y               ;
  ControlPoint R               ;
  V  = points[1]               ;
  V -= points[0]               ;
  R  = points[2]               ;
  X  = crossProduct ( R , V )  ;
  X  . normalize    (       )  ;
  Y  = crossProduct ( V , X )  ;
  Y  . normalize    (       )  ;
  X  = Y                       ;
  Y  = crossProduct ( V , X )  ;
  Y  . normalize    (       )  ;
  vectors[0]  = V              ;
  vectors[1]  = X              ;
  vectors[2]  = Y              ;
  //////////////////////////////
  k  = 0.5522847498            ;
  #define D(i,XX,YY)           \
    x[i] = XX ; y [i] = YY
  D( 0, 0, 1)                  ;
  D( 1, k, 1)                  ;
  D( 2, 1, k)                  ;
  D( 3, 1, 0)                  ;
  D( 4, 1, 0)                  ;
  D( 5, 1,-k)                  ;
  D( 6, k,-1)                  ;
  D( 7, 0,-1)                  ;
  D( 8, 0,-1)                  ;
  D( 9,-k,-1)                  ;
  D(10,-1,-k)                  ;
  D(11,-1, 0)                  ;
  D(12,-1, 0)                  ;
  D(13,-1, k)                  ;
  D(14,-k, 1)                  ;
  D(15, 0, 1)                  ;
  #undef  D
  //////////////////////////////
  double r1 = paraments[0]     ;
  double r2 = paraments[1]     ;
  for (int i=0;i<=7;i++)       {
    double v = i               ;
    double t                   ;
    v    /= 7                  ;
    t     = 1.0 - v            ;
    r[i]  = t * r1             ;
    r[i] += v * r2             ;
    f[i]  = v                  ;
  }                            ;
  //////////////////////////////
  ControlPoint P[128]          ;
  for (int u=0;u<=7;u++)       {
    double z = r [ u ]         ;
    int    l = u * 16          ;
    V  = vectors [ 0 ]         ;
    V *= f[u]                  ;
    V += points  [ 0 ]         ;
    for (int i=0;i<16;i++)     {
      X  = vectors[1]          ;
      Y  = vectors[2]          ;
      X *= z                   ;
      Y *= z                   ;
      X *= x[i]                ;
      Y *= y[i]                ;
      R  = V                   ;
      R += X                   ;
      R += Y                   ;
      P [ l + i ] = R          ;
    }                          ;
  }                            ;
  //////////////////////////////
  setCache(384*sizeof(double)) ;
  double * d                   ;
  int      w = 0               ;
  d = (double *)cache()        ;
  for (int i=0;i<4;i++)        {
    int si = i * 4             ;
    for (int j=0;j<8;j++)      {
      int ji = j * 16          ;
      for (int k=0;k<4;k++)    {
        int l = si + ji + k    ;
        d[w] = P[l].x ; w++    ;
        d[w] = P[l].y ; w++    ;
        d[w] = P[l].z ; w++    ;
      }                        ;
    }                          ;
  }                            ;
}
