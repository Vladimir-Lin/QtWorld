#include <qtworld.h>

N::SceneBox:: SceneBox (void)
{
  matrix . set      ( Cpp::Double , 4 , 4 ) ;
  matrix . zerofill (                     ) ;
  double * m = (double *)matrix . array ( ) ;
  m [ 0             ] = 1                   ;
  m [ ( 1 * 4 ) + 1 ] = 1                   ;
  m [ ( 2 * 4 ) + 2 ] = 1                   ;
  m [ ( 3 * 4 ) + 3 ] = 1                   ;
  addType ( Scene::Abstract )               ;
}

N::SceneBox::~SceneBox (void)
{
}

bool N::SceneBox::ForUuid(SUID & u,qint64 & i,UUIDs & U)
{
  if ( i >= U . count ( ) ) return false ;
  u = U [ i ]                            ;
  i++                                    ;
  return true                            ;
}

QVariant & N::SceneBox::setVariable(QString key,QVariant v)
{
  Variables [ key ] = v    ;
  return Variables [ key ] ;
}

QVariant & N::SceneBox::Value(QString key)
{
  if ( ! Variables . contains ( key ) ) {
    Variables [ key ] = QVariant ( )    ;
  }                                     ;
  return Variables [ key ]              ;
}

int N::SceneBox::addType(int objectType)
{
  if ( ! ClassTypes . contains ( objectType ) ) {
    ClassTypes << objectType                    ;
  }                                             ;
  return ClassTypes . count ( )                 ;
}

bool N::SceneBox::containsType(int objectType)
{
  return ClassTypes . contains ( objectType ) ;
}

void N::SceneBox::setUniverse(SUID u)
{
  setVariable ( "UniverseUUID" , u ) ;
}

void N::SceneBox::setParentUuid(SUID u)
{
  setVariable ( "ParentUUID" , u ) ;
}

SUID N::SceneBox::UniverseUuid(void)
{
  return Value ( "UniverseUUID" ) . toULongLong ( ) ;
}

SUID N::SceneBox::ParentUuid(void)
{
  return Value ( "ParentUUID" ) . toULongLong ( ) ;
}

int N::SceneBox::MipSize(int size)
{
  int z = 2                ;
  while ( size > z) z += z ;
  return z                 ;
}

void N::SceneBox::Prepare(void)
{
  minimum . x =  DBL_MAX ;
  minimum . y =  DBL_MAX ;
  minimum . z =  DBL_MAX ;
  maximum . x = -DBL_MAX ;
  maximum . y = -DBL_MAX ;
  maximum . z = -DBL_MAX ;
}

bool N::SceneBox::BoundingBox  (ControlPoint & minp,ControlPoint & maxp)
{
  if ( ! matrices . contains ( 1 ) ) return false                    ;
  ControlPoints p                                                    ;
  ////////////////////////////////////////////////////////////////////
  p [ 0 ] = ControlPoint ( minimum . x , minimum . y , minimum . z ) ;
  p [ 1 ] = ControlPoint ( minimum . x , minimum . y , maximum . z ) ;
  p [ 2 ] = ControlPoint ( minimum . x , maximum . y , minimum . z ) ;
  p [ 3 ] = ControlPoint ( minimum . x , maximum . y , maximum . z ) ;
  p [ 4 ] = ControlPoint ( maximum . x , maximum . y , minimum . z ) ;
  p [ 5 ] = ControlPoint ( maximum . x , maximum . y , maximum . z ) ;
  p [ 6 ] = ControlPoint ( maximum . x , minimum . y , minimum . z ) ;
  p [ 7 ] = ControlPoint ( maximum . x , minimum . y , maximum . z ) ;
  ////////////////////////////////////////////////////////////////////
  for (int i=0;i<8;i++)                                              {
    p [ i     ] . t = 1                                              ;
    p [ i + 8 ]     = matrices [ 1 ] * p [ i ]                       ;
  }                                                                  ;
  ////////////////////////////////////////////////////////////////////
  minp . x =  DBL_MAX                                                ;
  minp . y =  DBL_MAX                                                ;
  minp . z =  DBL_MAX                                                ;
  maxp . x = -DBL_MAX                                                ;
  maxp . y = -DBL_MAX                                                ;
  maxp . z = -DBL_MAX                                                ;
  ////////////////////////////////////////////////////////////////////
  for (int i=8;i<16;i++)                                             {
    if ( p [ i ] . x > maxp . x ) maxp . x = p [ i ] . x             ;
    if ( p [ i ] . y > maxp . y ) maxp . y = p [ i ] . y             ;
    if ( p [ i ] . z > maxp . z ) maxp . z = p [ i ] . z             ;
    if ( p [ i ] . x < minp . x ) minp . x = p [ i ] . x             ;
    if ( p [ i ] . y < minp . y ) minp . y = p [ i ] . y             ;
    if ( p [ i ] . z < minp . z ) minp . z = p [ i ] . z             ;
  }                                                                  ;
  ////////////////////////////////////////////////////////////////////
  return true                                                        ;
}

bool N::SceneBox::contains(ControlPoints & box,ControlPoint & p)
{
  if ( p . x < box [ 0 ] . x ) return false ;
  if ( p . y < box [ 0 ] . y ) return false ;
  if ( p . z < box [ 0 ] . z ) return false ;
  if ( p . x > box [ 1 ] . x ) return false ;
  if ( p . y > box [ 1 ] . y ) return false ;
  if ( p . z > box [ 1 ] . z ) return false ;
  return true                               ;
}

bool N::SceneBox::contains(ControlPoints & box,ControlPoints & p)
{
  CUIDs K = p . keys ( )                ;
  CUID  k                               ;
  foreach ( k , K )                     {
    if ( ! contains ( box , p [ k ] ) ) {
      return false                      ;
    }                                   ;
  }                                     ;
  return true                           ;
}

bool N::SceneBox::isInside(ControlPoints & box,ControlPoints & p)
{
  ControlPoints P                                     ;
  ControlPoint  p1 = p [ 0 ]                          ;
  ControlPoint  p2 = p [ 1 ]                          ;
  P [ 0 ] = ControlPoint ( p1 . x , p1 . y , p1 . z ) ;
  P [ 1 ] = ControlPoint ( p1 . x , p1 . y , p2 . z ) ;
  P [ 2 ] = ControlPoint ( p1 . x , p2 . y , p1 . z ) ;
  P [ 3 ] = ControlPoint ( p1 . x , p2 . y , p2 . z ) ;
  P [ 4 ] = ControlPoint ( p2 . x , p2 . y , p1 . z ) ;
  P [ 5 ] = ControlPoint ( p2 . x , p2 . y , p2 . z ) ;
  P [ 6 ] = ControlPoint ( p2 . x , p1 . y , p1 . z ) ;
  P [ 7 ] = ControlPoint ( p2 . x , p1 . y , p2 . z ) ;
  return contains        ( box    , P               ) ;
}

void N::SceneBox::AddPoint(ControlPoint & point)
{
  if ( point . x > maximum . x ) maximum . x = point . x ;
  if ( point . y > maximum . y ) maximum . y = point . y ;
  if ( point . z > maximum . z ) maximum . z = point . z ;
  if ( point . x < minimum . x ) minimum . x = point . x ;
  if ( point . y < minimum . y ) minimum . y = point . y ;
  if ( point . z < minimum . z ) minimum . z = point . z ;
}

void N::SceneBox::Finish(void)
{
  length . x = maximum . x - minimum . x ;
  length . y = maximum . y - minimum . y ;
  length . z = maximum . z - minimum . z ;
}

void N::SceneBox::GetMatrix(GLenum pname,Matrix & m)
{
  double * a                      ;
  m . set ( Cpp::Double , 4 , 4 ) ;
  a = (double *) m . array ( )    ;
  if ( NULL == a ) return         ;
  QtGL::GetMatrixd ( pname , a )  ;
}

void N::SceneBox::GetMatrix(GLenum pname)
{
  GetMatrix ( pname , matrix ) ;
}

void N::SceneBox::SetMatrix(Matrix & m)
{
  if ( m . Rows    ( ) != 4 ) return    ;
  if ( m . Columns ( ) != 4 ) return    ;
  double * a = (double *) m . array ( ) ;
  if ( NULL            == a ) return    ;
  QtGL::LoadMatrixd ( a )               ;
}

void N::SceneBox::SetMatrix(void)
{
  SetMatrix ( matrix ) ;
}

void N::SceneBox::MultMatrix(Matrix & m)
{
  if ( m . Rows    ( ) != 4 ) return    ;
  if ( m . Columns ( ) != 4 ) return    ;
  double * a = (double *) m . array ( ) ;
  if ( NULL == a            ) return    ;
  QtGL::MultMatrixd ( a )               ;
}

void N::SceneBox::MultMatrix(void)
{
  MultMatrix ( matrix ) ;
}

void N::SceneBox::Transform(void)
{
  CUIDs  K = DOFs . keys ( )              ;
  CUID   k                                ;
  Matrix M                                ;
  MultMatrix ( matrix                   ) ;
  GetMatrix  ( GL_PROJECTION_MATRIX , M ) ;
  matrices [ 0 ] . assign ( M )           ;
  if ( K . count ( ) <= 0 ) return        ;
  foreach ( k , K )                       {
    int dof = DOFs [ k ] . DOF            ;
    switch ( dof )                        {
      case ControlPoint::DofTranslate     :
        DOFs [ k ] . Translate ( )        ;
      break                               ;
      case ControlPoint::DofRotate        :
        DOFs [ k ] . Rotate    ( )        ;
      break                               ;
      case ControlPoint::DofScale         :
        DOFs [ k ] . Scale     ( )        ;
      break                               ;
    }                                     ;
  }                                       ;
}

void N::SceneBox::setFrustum(void)
{
  QtGL::Frustum                 (
    minimum . x , maximum . x   ,
    minimum . y , maximum . y   ,
    minimum . z , maximum . z ) ;
}

void N::SceneBox::setOrtho(void)
{
  QtGL::Ortho                   (
    minimum . x , maximum . x   ,
    minimum . y , maximum . y   ,
    minimum . z , maximum . z ) ;
}

void N::SceneBox::setRect(QRectF & bar)
{
  minimum . x = bar . left   ( ) ;
  minimum . y = bar . top    ( ) ;
  maximum . x = bar . right  ( ) ;
  maximum . y = bar . bottom ( ) ;
}

int N::SceneBox::addDof(int Id,ControlPoint & dof)
{
  DOFs [ Id ] = dof       ;
  return DOFs . count ( ) ;
}

int N::SceneBox::removeDof(int Id)
{
  if ( DOFs . contains ( Id ) ) {
    DOFs . remove ( Id )        ;
  }                             ;
  return DOFs . count ( )       ;
}

bool N::SceneBox::Transform(ControlPoint & S,ControlPoint & T)
{
  ControlPoint C = S                  ;
  ControlPoint O                      ;
  CUIDs        K = DOFs.keys()        ;
  CUID         k                      ;
  if (N::Transform(matrix,C,O)) T = O ;
  if (K.count()<=0) return true       ;
  Matrix M ( Cpp::Double , 4 , 4 )    ;
  C = O                               ;
  foreach (k,K)                       {
    int dof = DOFs[k].DOF             ;
    switch (dof)                      {
      case ControlPoint::DofTranslate :
        O  = C                        ;
        O += DOFs[k]                  ;
        T  = O                        ;
        C  = O                        ;
      break                           ;
      case ControlPoint::DofRotate    :
        N::Rotation ( M , DOFs[k] )   ;
        if (N::Transform(M,C,O))      {
          T  = O                      ;
          C  = O                      ;
        }                             ;
      break                           ;
      case ControlPoint::DofScale     :
        O  = C                        ;
        O *= DOFs[k]                  ;
        T  = O                        ;
        C  = O                        ;
      break                           ;
    }                                 ;
  }                                   ;
  return true                         ;
}

bool N::SceneBox::Multiply(Matrix & result,Matrix & A,Matrix & B)
{
  double  * a = (double *) A      . array ( ) ;
  double  * b = (double *) B      . array ( ) ;
  double  * r = (double *) result . array ( ) ;
  /////////////////////////////////////////////
  float     fa [ 16 ]                         ;
  float     fb [ 16 ]                         ;
  /////////////////////////////////////////////
  for (int i=0;i<16;i++)                      {
    fa [ i ] = (float) a [ i ]                ;
    fb [ i ] = (float) b [ i ]                ;
  }                                           ;
  /////////////////////////////////////////////
  QMatrix4x4 R44 ( fa )                       ;
  QMatrix4x4 M44 ( fb )                       ;
  R44 *= M44                                  ;
  /////////////////////////////////////////////
  float * x = R44 . data ( )                  ;
  for (int i=0;i<16;i++)                      {
    r [ i ] = x [ i ]                         ;
  }                                           ;
  return true                                 ;
}

void N::SceneBox::Dual(void)
{
  double * a                                   ;
  double * b                                   ;
  int      x = 0                               ;
  //////////////////////////////////////////////
  matrices [ 1 ] . set ( Cpp::Double , 4 , 4 ) ;
  matrices [ 2 ] . set ( Cpp::Double , 4 , 4 ) ;
  a = (double *) matrices [ 1 ] . array ( )    ;
  b = (double *) matrices [ 2 ] . array ( )    ;
  ::memset ( a , 0 , sizeof(double) * 16 )     ;
  ::memset ( b , 0 , sizeof(double) * 16 )     ;
  for (int i=0;i<4;i++,x+=5)                   {
    a [ x ] = 1.0                              ;
    b [ x ] = 1.0                              ;
  }                                            ;
  //////////////////////////////////////////////
  if ( DOFs . count ( ) <= 0 ) return          ;
  QMatrix4x4 M44                               ;
  QMatrix4x4 R44                               ;
  CUIDs      U = DOFs . keys ( )               ;
  int        u                                 ;
  M44 . setToIdentity ( )                      ;
  foreach ( u , U )                            {
    switch ( DOFs [ u ] . DOF )                {
      case ControlPoint::DofNone               :
      break                                    ;
      case ControlPoint::DofTranslate          :
        M44 . translate                        (
          DOFs [ u ] . x                       ,
          DOFs [ u ] . y                       ,
          DOFs [ u ] . z                     ) ;
      break                                    ;
      case ControlPoint::DofRotate             :
        M44 . rotate                           (
          DOFs [ u ] . r                       ,
          DOFs [ u ] . x                       ,
          DOFs [ u ] . y                       ,
          DOFs [ u ] . z                     ) ;
      break                                    ;
      case ControlPoint::DofScale              :
        M44 . scale                            (
          DOFs [ u ] . x                       ,
          DOFs [ u ] . y                       ,
          DOFs [ u ] . z                     ) ;
      break                                    ;
    }                                          ;
  }                                            ;
  //////////////////////////////////////////////
  bool inverted = false                        ;
  R44 = M44 . inverted ( &inverted )           ;
  if ( ! inverted ) R44 . setToIdentity ( )    ;
  //////////////////////////////////////////////
  float * A = M44 . data ( )                   ;
  float * B = R44 . data ( )                   ;
  for (int i=0;i<16;i++)                       {
    a [ i ] = A [ i ]                          ;
    b [ i ] = B [ i ]                          ;
  }                                            ;
  //////////////////////////////////////////////
  if ( ! inverted ) matrices . remove ( 2 )    ;
}

bool N::SceneBox::intersects(ControlPoint & p1,ControlPoint & p2)
{
  ControlPoint P1                         ;
  ControlPoint P2                         ;
  bool         collision                  ;
  return intersects ( p1 , p2 , P1 , P2 ) ;
}

bool N::SceneBox::intersects (
       ControlPoint & p1     ,
       ControlPoint & p2     ,
       ControlPoint & P1     ,
       ControlPoint & P2     )
{
  if ( ! matrices . contains ( 2 ) ) return false ;
  ControlPoint Z                                  ;
  ControlPoint V1                                 ;
  double       d                                  ;
  double       r = length . length ( ) / 2        ;
  /////////////////////////////////////////////////
  P1 = matrices [ 2 ] * p1                        ;
  P2 = matrices [ 2 ] * p2                        ;
  /////////////////////////////////////////////////
  Z . x = ( minimum . x + maximum . x ) / 2       ;
  Z . y = ( minimum . y + maximum . y ) / 2       ;
  Z . z = ( minimum . z + maximum . z ) / 2       ;
  Z . t = 1                                       ;
  /////////////////////////////////////////////////
  V1    = P2                                      ;
  V1   -= P1                                      ;
  V1    . normalize ( )                           ;
  d     = Z . distanceToLine ( P1 , V1 )          ;
  if ( d > r ) return false                       ;
  /////////////////////////////////////////////////
  double x                                        ;
  double y                                        ;
  double z                                        ;
  double n                                        ;
  /////////////////////////////////////////////////
  z = V1 . z                                      ;
  if ( z < 0 ) z = -z                             ;
  if ( z > 0.00000000001 )                        {
    n  = maximum . z - P1 . z                     ;
    n /= V1 . z                                   ;
    x  = V1 . x                                   ;
    x *= n                                        ;
    x += P1 . x                                   ;
    y  = V1 . y                                   ;
    y *= n                                        ;
    y += P1 . y                                   ;
    if ( ( x >= minimum . x )                    &&
         ( x <= maximum . x )                    &&
         ( y >= minimum . y )                    &&
         ( y <= maximum . y )                   ) {
      return true                                 ;
    }                                             ;
    n  = minimum . z - P1 . z                     ;
    n /= V1 . z                                   ;
    x  = V1 . x                                   ;
    x *= n                                        ;
    x += P1 . x                                   ;
    y  = V1 . y                                   ;
    y *= n                                        ;
    y += P1 . y                                   ;
    if ( ( x >= minimum . x )                    &&
         ( x <= maximum . x )                    &&
         ( y >= minimum . y )                    &&
         ( y <= maximum . y )                   ) {
      return true                                 ;
    }                                             ;
  }                                               ;
  /////////////////////////////////////////////////
  x = V1 . x                                      ;
  if ( x < 0 ) x = -x                             ;
  if ( x > 0.00000000001 )                        {
    n  = maximum . x - P1 . x                     ;
    n /= V1 . x                                   ;
    y  = V1 . y                                   ;
    y *= n                                        ;
    y += P1 . y                                   ;
    z  = V1 . z                                   ;
    z *= n                                        ;
    z += P1 . z                                   ;
    if ( ( y >= minimum . y )                    &&
         ( y <= maximum . y )                    &&
         ( z >= minimum . z )                    &&
         ( z <= maximum . z )                   ) {
      return true                                 ;
    }                                             ;
    n  = minimum . x - P1 . x                     ;
    n /= V1 . x                                   ;
    y  = V1 . y                                   ;
    y *= n                                        ;
    y += P1 . y                                   ;
    z  = V1 . z                                   ;
    z *= n                                        ;
    z += P1 . z                                   ;
    if ( ( y >= minimum . y )                    &&
         ( y <= maximum . y )                    &&
         ( z >= minimum . z )                    &&
         ( z <= maximum . z )                   ) {
      return true                                 ;
    }                                             ;
  }                                               ;
  /////////////////////////////////////////////////
  y = V1 . y                                      ;
  if ( y < 0 ) y = -y                             ;
  if ( y > 0.00000000001 )                        {
    n  = maximum . y - P1 . y                     ;
    n /= V1 . y                                   ;
    x  = V1 . x                                   ;
    x *= n                                        ;
    x += P1 . x                                   ;
    z  = V1 . z                                   ;
    z *= n                                        ;
    z += P1 . z                                   ;
    if ( ( x >= minimum . x )                    &&
         ( x <= maximum . x )                    &&
         ( z >= minimum . z )                    &&
         ( z <= maximum . z )                   ) {
      return true                                 ;
    }                                             ;
    n  = minimum . y - P1 . y                     ;
    n /= V1 . y                                   ;
    x  = V1 . x                                   ;
    x *= n                                        ;
    x += P1 . x                                   ;
    z  = V1 . z                                   ;
    z *= n                                        ;
    z += P1 . z                                   ;
    if ( ( x >= minimum . x )                    &&
         ( x <= maximum . x )                    &&
         ( z >= minimum . z )                    &&
         ( z <= maximum . z )                   ) {
      return true                                 ;
    }                                             ;
  }                                               ;
  /////////////////////////////////////////////////
  return false                                    ;
}

void N::SceneBox::TextureRect(QRectF & r,double z)
{
  QtGL::Quadrilateral (                                    ) ;
  QtGL::TexCoord2d    (           0.0 ,            0.0     ) ;
  QtGL::Vertex3d      ( r . left  ( ) , r . top    ( ) , z ) ;
  QtGL::TexCoord2d    (           1.0 ,            0.0     ) ;
  QtGL::Vertex3d      ( r . right ( ) , r . top    ( ) , z ) ;
  QtGL::TexCoord2d    (           1.0 ,            1.0     ) ;
  QtGL::Vertex3d      ( r . right ( ) , r . bottom ( ) , z ) ;
  QtGL::TexCoord2d    (           0.0 ,            1.0     ) ;
  QtGL::Vertex3d      ( r . left  ( ) , r . bottom ( ) , z ) ;
  QtGL::End           (                                    ) ;
}

void N::SceneBox::TextureRect(QRectF & r,double z,QSizeF s)
{
  double b = 1.0 - s . height ( )                            ;
  double w =       s . width  ( )                            ;
  QtGL::Quadrilateral (                                    ) ;
  QtGL::TexCoord2d    (           0.0 ,            b       ) ;
  QtGL::Vertex3d      ( r . left  ( ) , r . top    ( ) , z ) ;
  QtGL::TexCoord2d    (           w   ,            b       ) ;
  QtGL::Vertex3d      ( r . right ( ) , r . top    ( ) , z ) ;
  QtGL::TexCoord2d    (           w   ,            1.0     ) ;
  QtGL::Vertex3d      ( r . right ( ) , r . bottom ( ) , z ) ;
  QtGL::TexCoord2d    (           0.0 ,            1.0     ) ;
  QtGL::Vertex3d      ( r . left  ( ) , r . bottom ( ) , z ) ;
  QtGL::End           (                                    ) ;
}

void N::SceneBox::CubeVertex (
       int            side         ,
       ControlPoint & minp         ,
       ControlPoint & maxp         )
{
  switch ( side )                                 {
    case   0                                      : // Back
      QtGL::Vertex3d ( maxp.x , minp.y , minp.z ) ;
      QtGL::Vertex3d ( minp.x , minp.y , minp.z ) ;
      QtGL::Vertex3d ( minp.x , maxp.y , minp.z ) ;
      QtGL::Vertex3d ( maxp.x , maxp.y , minp.z ) ;
    break                                         ;
    case   1                                      : // Front
      QtGL::Vertex3d ( minp.x , minp.y , maxp.z ) ;
      QtGL::Vertex3d ( maxp.x , minp.y , maxp.z ) ;
      QtGL::Vertex3d ( maxp.x , maxp.y , maxp.z ) ;
      QtGL::Vertex3d ( minp.x , maxp.y , maxp.z ) ;
    break                                         ;
    case   2                                      : // Bottom
      QtGL::Vertex3d ( minp.x , minp.y , minp.z ) ;
      QtGL::Vertex3d ( maxp.x , minp.y , minp.z ) ;
      QtGL::Vertex3d ( maxp.x , minp.y , maxp.z ) ;
      QtGL::Vertex3d ( minp.x , minp.y , maxp.z ) ;
    break                                         ;
    case   3                                      : // Right
      QtGL::Vertex3d ( maxp.x , minp.y , maxp.z ) ;
      QtGL::Vertex3d ( maxp.x , minp.y , minp.z ) ;
      QtGL::Vertex3d ( maxp.x , maxp.y , minp.z ) ;
      QtGL::Vertex3d ( maxp.x , maxp.y , maxp.z ) ;
    break                                         ;
    case   4                                      : // Top
      QtGL::Vertex3d ( minp.x , maxp.y , minp.z ) ;
      QtGL::Vertex3d ( minp.x , maxp.y , maxp.z ) ;
      QtGL::Vertex3d ( maxp.x , maxp.y , maxp.z ) ;
      QtGL::Vertex3d ( maxp.x , maxp.y , minp.z ) ;
    break                                         ;
    case   5                                      : // Left
      QtGL::Vertex3d ( minp.x , maxp.y , maxp.z ) ;
      QtGL::Vertex3d ( minp.x , maxp.y , minp.z ) ;
      QtGL::Vertex3d ( minp.x , minp.y , minp.z ) ;
      QtGL::Vertex3d ( minp.x , minp.y , maxp.z ) ;
    break                                         ;
    case   6                                      : // Side lines
      QtGL::Vertex3d ( minp.x , minp.y , minp.z ) ;
      QtGL::Vertex3d ( minp.x , minp.y , maxp.z ) ;
      QtGL::Vertex3d ( maxp.x , minp.y , minp.z ) ;
      QtGL::Vertex3d ( maxp.x , minp.y , maxp.z ) ;
      QtGL::Vertex3d ( maxp.x , maxp.y , minp.z ) ;
      QtGL::Vertex3d ( maxp.x , maxp.y , maxp.z ) ;
      QtGL::Vertex3d ( minp.x , maxp.y , minp.z ) ;
      QtGL::Vertex3d ( minp.x , maxp.y , maxp.z ) ;
    break                                         ;
    case 100                                      : // Back Face
      QtGL::Quadrilateral (                     ) ;
      CubeVertex          (   0 , minp , maxp   ) ;
      QtGL::End           (                     ) ;
    break                                         ;
    case 101                                      : // Front Face
      QtGL::Quadrilateral (                     ) ;
      CubeVertex          (   1 , minp , maxp   ) ;
      QtGL::End           (                     ) ;
    break                                         ;
    case 102                                      : // Bottom Face
      QtGL::Quadrilateral (                     ) ;
      CubeVertex          (   2 , minp , maxp   ) ;
      QtGL::End           (                     ) ;
    break                                         ;
    case 103                                      : // Right Face
      QtGL::Quadrilateral (                     ) ;
      CubeVertex          (   3 , minp , maxp   ) ;
      QtGL::End           (                     ) ;
    break                                         ;
    case 104                                      : // Top Face
      QtGL::Quadrilateral (                     ) ;
      CubeVertex          (   4 , minp , maxp   ) ;
      QtGL::End           (                     ) ;
    break                                         ;
    case 105                                      : // Left Face
      QtGL::Quadrilateral (                     ) ;
      CubeVertex          (   5 , minp , maxp   ) ;
      QtGL::End           (                     ) ;
    break                                         ;
    case 106                                      : // All Faces
      for (int i = 100 ; i <= 105 ; i++ )         {
        CubeVertex        (   i , minp , maxp   ) ;
      }                                           ;
    break                                         ;
    case 200                                      : // Back Lines
      QtGL::Loop          (                     ) ;
      CubeVertex          (   0 , minp , maxp   ) ;
      QtGL::End           (                     ) ;
    break                                         ;
    case 201                                      : // Front Lines
      QtGL::Loop          (                     ) ;
      CubeVertex          (   1 , minp , maxp   ) ;
      QtGL::End           (                     ) ;
    break                                         ;
    case 202                                      : // Bottom Lines
      QtGL::Loop          (                     ) ;
      CubeVertex          (   2 , minp , maxp   ) ;
      QtGL::End           (                     ) ;
    break                                         ;
    case 203                                      : // Right Lines
      QtGL::Loop          (                     ) ;
      CubeVertex          (   3 , minp , maxp   ) ;
      QtGL::End           (                     ) ;
    break                                         ;
    case 204                                      : // Top Lines
      QtGL::Loop          (                     ) ;
      CubeVertex          (   4 , minp , maxp   ) ;
      QtGL::End           (                     ) ;
    break                                         ;
    case 205                                      : // Left Lines
      QtGL::Loop          (                     ) ;
      CubeVertex          (   5 , minp , maxp   ) ;
      QtGL::End           (                     ) ;
    break                                         ;
    case 206                                      : // All Lines
      QtGL::Loop          (                     ) ;
      CubeVertex          (   0 , minp , maxp   ) ;
      QtGL::End           (                     ) ;
      QtGL::Loop          (                     ) ;
      CubeVertex          (   1 , minp , maxp   ) ;
      QtGL::End           (                     ) ;
      QtGL::Lines         (                     ) ;
      CubeVertex          (   6 , minp , maxp   ) ;
      QtGL::End           (                     ) ;
    break                                         ;
  }                                               ;
}

int N::SceneBox::CallDynamic (
      QString     func       ,
      void      * data       ,
      void      * dispatch   ,
      QVariants & values     )
{
  DynamicAssignment da = NULL                 ;
  if ( DynamicFunctions . contains ( func ) ) {
    da = DynamicFunctions [ func ]            ;
    if ( NULL != da )                         {
      return da ( data , dispatch , values )  ;
    }                                         ;
  }                                           ;
  return 0                                    ;
}
