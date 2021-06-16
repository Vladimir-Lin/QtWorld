#include <qtworld.h>

N::Bookcase:: Bookcase    (void)
            : SceneObject (    )
{
  Configure ( ) ;
}

N::Bookcase:: Bookcase    ( SceneDestructor * des )
            : SceneObject (                   des )
{
  Configure ( ) ;
}

N::Bookcase::~Bookcase (void)
{
}

void N::Bookcase::Configure(void)
{
  ControlPoint C                              ;
  name    = "Bookcase"                        ;
  minimum = ControlPoint ( -64 , -128 ,  -8 ) ;
  maximum = ControlPoint (  64 ,   64 ,   8 ) ;
  C       = QColor       ( 192 ,  192 , 192 ) ;
  addColor               (   0 ,   C        ) ;
  C       = QColor       (   0 ,    0 ,   0 ) ;
  addColor               (   1 ,   C        ) ;
  C       = QColor       ( 255 ,  255 , 255 ) ;
  addColor               (   2 ,   C        ) ;
}

void N::Bookcase::prepareText(void)
{
  Plane          plane                                        ;
  SceneTexture * t                                            ;
  QSize          s              ( 128 , 64                  ) ;
  QImage       * O = new QImage ( s , QImage::Format_ARGB32 ) ;
  t         = new SceneTexture  ( destructor                ) ;
  t -> node       = 1                                         ;
  t -> create                   ( O , true , false          ) ;
  textures [ 1 ]  = t                                         ;
  t -> original -> fill         ( 0                         ) ;
  plane . O = ControlPoint      (  -64 ,  -128 ,  8         ) ;
  plane . X = ControlPoint      (  128 ,     0 ,  0         ) ;
  plane . Y = ControlPoint      (    0 ,    64 ,  0         ) ;
  plane . T = ControlPoint      (    1 ,     1 ,  1         ) ;
  plane . N = ControlPoint      (    0 ,     0 ,  1         ) ;
  plane . W = 1                                               ;
  plane . H = 1                                               ;
  plane . A = true                                            ;
  addPlane                      ( 1 , plane                )  ;
}

void N::Bookcase::setText (
       QFont   font       ,
       QString text       ,
       int     flags      )
{
  if ( ! textures . contains ( 1 ) ) return                  ;
  QColor c (   0 ,   0 ,   0 , 255 )                         ;
  QRectF r (   0 ,   0 , 128 ,  64 )                         ;
  if ( Variables . contains ( "TextColor" ) )                {
    c = Variables [ "TextColor" ] . value <QColor> ( )       ;
  }                                                          ;
  textures [ 1 ] -> drawText ( c , font , r , flags , text ) ;
}

void N::Bookcase::addBox(double length,double depth)
{
  double v [ 8 ] [ 3 ]                                    ;
  double e      = length / 2                              ;
  double d      = depth  / 2                              ;
  int    index  = Last()                                  ;
  int    last   = index                                   ;
  int    si                                               ;
  GLint faces[6][4] =                                     {
    { 0 , 1 , 2 , 3 }                                     ,
    { 3 , 2 , 6 , 7 }                                     ,
    { 7 , 6 , 5 , 4 }                                     ,
    { 4 , 5 , 1 , 0 }                                     ,
    { 5 , 6 , 2 , 1 }                                     ,
    { 7 , 4 , 0 , 3 }                                   } ;
  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -e              ;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] =  e              ;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -e              ;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] =  e              ;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -d              ;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] =  d              ;
  for (int i=0;i<8;i++)                                   {
    ControlPoint p(v[i][0],v[i][1],v[i][2])               ;
    index = addPoint ( index , p )                        ;
  }                                                       ;
  /////////////////////////////////////////////////////////
  si = Last ( Segment )                                   ;
  SceneSegment F                                          ;
  appendSegments ( si , 6 , F )                           ;
  segments [ si     ] . type = SceneSegment::Loop         ;
  segments [ si     ] . add ( last + 3 )                  ;
  segments [ si     ] . add ( last + 0 )                  ;
  segments [ si     ] . add ( last + 4 )                  ;
  segments [ si     ] . add ( last + 7 )                  ;
  segments [ si + 1 ] . type = SceneSegment::Loop         ;
  segments [ si + 1 ] . add ( last + 5 )                  ;
  segments [ si + 1 ] . add ( last + 6 )                  ;
  segments [ si + 1 ] . add ( last + 2 )                  ;
  segments [ si + 1 ] . add ( last + 1 )                  ;
  segments [ si + 2 ] . type = SceneSegment::Lines        ;
  segments [ si + 2 ] . add ( last + 3 )                  ;
  segments [ si + 2 ] . add ( last + 2 )                  ;
  segments [ si + 3 ] . type = SceneSegment::Lines        ;
  segments [ si + 3 ] . add ( last + 0 )                  ;
  segments [ si + 3 ] . add ( last + 1 )                  ;
  segments [ si + 4 ] . type = SceneSegment::Lines        ;
  segments [ si + 4 ] . add ( last + 4 )                  ;
  segments [ si + 4 ] . add ( last + 5 )                  ;
  segments [ si + 5 ] . type = SceneSegment::Lines        ;
  segments [ si + 5 ] . add ( last + 7 )                  ;
  segments [ si + 5 ] . add ( last + 6 )                  ;
  /////////////////////////////////////////////////////////
  si = Last ( Face )                                      ;
  SceneFace P                                             ;
  this -> faces [ si     ] = P                            ;
  this -> faces [ si + 1 ] = P                            ;
  this -> faces [ si + 2 ] = P                            ;
  this -> faces [ si + 3 ] = P                            ;
  this -> faces [ si + 4 ] = P                            ;
  this -> faces [ si + 5 ] = P                            ;
  for (int i=0;i<6;i++)                                   {
    this->faces [ si + i] . type = SceneFace::Quad        ;
    for (int j=0;j<4;j++)                                 {
      this->faces [ si + i ] . add ( faces[i][j] + last ) ;
    }                                                     ;
  }                                                       ;
}

void N::Bookcase::attache(int side,SceneTexture * texture)
{
  int    w                                         ;
  int    h                                         ;
  double f                                         ;
  double xx = -64                                  ;
  double yy = -64                                  ;
  double ww = 128                                  ;
  double hh = 128                                  ;
  Plane  plane                                     ;
  textures [ texture -> node ] = texture           ;
  w   = texture -> original -> width  ( )          ;
  h   = texture -> original -> height ( )          ;
  if ( w > h )                                     {
    f   = h                                        ;
    f  /= w                                        ;
    ww  = 128.0                                    ;
    hh  = 128.0 * f                                ;
  } else
  if ( w < h )                                     {
    f   = w                                        ;
    f  /= h                                        ;
    hh  = 128.0                                    ;
    ww  = 128.0 * f                                ;
  }                                                ;
  xx  = - ( ww / 2 )                               ;
  yy  = - ( hh / 2 )                               ;
  switch ( side )                                  {
    case NoSide                                    :
    return                                         ;
    case FrontSide                                 :
      plane . O = ControlPoint (  xx ,  yy ,  8 )  ;
      plane . X = ControlPoint (  ww ,   0 ,  0 )  ;
      plane . Y = ControlPoint (   0 ,  hh ,  0 )  ;
      plane . T = ControlPoint (   1 ,   1 ,  1 )  ;
      plane . N = ControlPoint (   0 ,   0 ,  1 )  ;
      plane . W = 1                                ;
      plane . H = 1                                ;
      plane . A = true                             ;
    break                                          ;
    case BackSide                                  :
      plane . O = ControlPoint ( -xx ,  yy ,  8 )  ;
      plane . X = ControlPoint ( -ww ,   0 ,  0 )  ;
      plane . Y = ControlPoint (   0 ,  hh ,  0 )  ;
      plane . T = ControlPoint (   1 ,   1 ,  1 )  ;
      plane . N = ControlPoint (   0 ,   0 , -1 )  ;
      plane . W = 1                                ;
      plane . H = 1                                ;
      plane . A = true                             ;
    return                                         ;
    case LeftSide                                  :
      plane . O = ControlPoint ( -64 , -64 , -8 )  ;
      plane . X = ControlPoint (   0 ,  0  , 16 )  ;
      plane . Y = ControlPoint (   0 , 128 ,  0 )  ;
      plane . T = ControlPoint (   1 ,   1 ,  1 )  ;
      plane . N = ControlPoint (   0 ,   0 , -1 )  ;
      plane . W = 1                                ;
      plane . H = 1                                ;
      plane . A = true                             ;
    return                                         ;
    case RightSide                                 :
      plane . O = ControlPoint (  64 , -64 ,  8 )  ;
      plane . X = ControlPoint (   0 ,  0  ,-16 )  ;
      plane . Y = ControlPoint (   0 , 128 ,  0 )  ;
      plane . T = ControlPoint (   1 ,   1 ,  1 )  ;
      plane . N = ControlPoint (   0 ,   0 , -1 )  ;
      plane . W = 1                                ;
      plane . H = 1                                ;
      plane . A = true                             ;
    return                                         ;
    case TopSide                                   :
    return                                         ;
    case BottomSide                                :
    return                                         ;
  }                                                ;
  addPlane    ( texture -> node , plane          ) ;
}
