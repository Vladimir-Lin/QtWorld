#include <qtworld.h>

N::Camera:: Camera    ( void          )
          : SceneNode ( Scene::Camera )
          , FOV       ( 60.0          )
          , mode      ( Perfect       )
          , asObject  ( false         )
{
}

N::Camera:: Camera    ( int mode      )
          : SceneNode ( Scene::Camera )
          , FOV       ( 60.0          )
          , mode      ( mode          )
          , asObject  ( false         )
{
}

N::Camera:: Camera    ( SceneDestructor * des )
          : SceneNode ( Scene::Camera         )
          , FOV       ( 60.0                  )
          , mode      ( Perfect               )
          , asObject  ( false                 )
{
  destructor = des   ;
  JoinDestructor ( ) ;
}

N::Camera::~Camera (void)
{
}

void N::Camera::assignViewport(QRectF & v)
{
  viewport = v                   ;
  switch (mode)                  {
    case Reality                 :
      setWindowPort()            ;
    break                        ;
    case Perfect                 :
      setWindowPort()            ;
    break                        ;
    case Perspective             :
      setWindowPort()            ;
    break                        ;
    case Orthographic            :
      setWindowPort()            ;
    break                        ;
    case Frustum                 :
      setWindowPort()            ;
    break                        ;
    case Fit                     :
      setWindowPort()            ;
    break                        ;
    case Parking                 :
      Dock         ()            ;
    break                        ;
    case Floating                :
      Partial      ()            ;
    break                        ;
    case NodeMatrix              :
      setWindowPort()            ;
    break                        ;
  }                              ;
}

void N::Camera::setViewport(QRectF & area)
{
  QtGL::Viewport ( area ) ;
}

double N::Camera::Factor(void)
{
  double factor               ;
  factor  = viewport.width () ;
  factor /= viewport.height() ;
  return factor               ;
}

double N::Camera::Factor(QRectF & area)
{
  double factor           ;
  factor  = area.width () ;
  factor /= area.height() ;
  return factor           ;
}

N::Camera & N::Camera::operator=(QRectF & v)
{
  viewport = v   ;
  return (*this) ;
}

void N::Camera::setViewport(void)
{
  QtGL::Viewport ( viewport ) ;
}

void N::Camera::setWindowPort(void)
{
  QtGL::Viewport ( viewport ) ;
}

void N::Camera::setFactorPort(void)
{
  QtGL::Viewport ( viewport ) ;
}

void N::Camera::setNearFar(double Near,double Far)
{
  minimum.z = Near ;
  maximum.z = Far  ;
}

void N::Camera::setOrthogonal(void)
{
        setViewport  (               ) ;
  QtGL::MatrixMode   ( GL_PROJECTION ) ;
  QtGL::LoadIdentity (               ) ;
        setOrtho     (               ) ;
  QtGL::MatrixMode   ( GL_MODELVIEW  ) ;
  QtGL::LoadIdentity (               ) ;
}

void N::Camera::setPerspective(double factor)
{
  QtGL::MatrixMode   ( GL_PROJECTION                        ) ;
  QtGL::LoadIdentity (                                      ) ;
  QtGL::Perspective  ( FOV , factor , minimum.z , maximum.z ) ;
  QtGL::LookAt       ( eye    . x , eye    . y , eye    . z   ,
                       center . x , center . y , center . z   ,
                       up     . x , up     . y , up     . z ) ;
  QtGL::MatrixMode   ( GL_MODELVIEW                         ) ;
  QtGL::LoadIdentity (                                      ) ;
}

void N::Camera::setPerspective(void)
{
  setPerspective ( Factor() ) ;
}

void N::Camera::Perfection(void)
{
  double       fov                  ;
  double       w                    ;
  double       fs                   ;
  double       fc                   ;
  ControlPoint V                    ;
  w    = viewport.height()          ;
  w   /= 2                          ;
  fov  = FOV                        ;
  fov /= 2                          ;
  fs   = Math :: fastSine   ( fov ) ;
  fc   = Math :: fastCosine ( fov ) ;
  w   *= fc                         ;
  w   /= fs                         ;
  V    = eye                        ;
  V   -= center                     ;
  up   . normalize ( )              ;
  V    . normalize ( )              ;
  V   *= w                          ;
  V   += center                     ;
  eye  = V                          ;
}

void N::Camera::setCamera(void)
{
  switch ( mode )                          {
    case Reality                           :
      Perfection         (               ) ;
      setViewport        (               ) ;
      setPerspective     (               ) ;
    break                                  ;
    case Perfect                           :
      Perfection         (               ) ;
      setViewport        (               ) ;
      setPerspective     (               ) ;
    break                                  ;
    case Perspective                       :
      setViewport        (               ) ;
      setPerspective     (               ) ;
    break                                  ;
    case Orthographic                      :
      setOrthogonal      (               ) ;
    break                                  ;
    case Frustum                           :
      QtGL::MatrixMode   ( GL_PROJECTION ) ;
      QtGL::LoadIdentity (               ) ;
            setFrustum   (               ) ;
      QtGL::MatrixMode   ( GL_MODELVIEW  ) ;
      QtGL::LoadIdentity (               ) ;
            setViewport  (               ) ;
    break                                  ;
    case Fit                               :
      FitWindow          (               ) ;
      setOrthogonal      (               ) ;
    break                                  ;
    case Parking                           :
      Dock               (               ) ;
      QtGL::MatrixMode   ( GL_PROJECTION ) ;
      QtGL::LoadIdentity (               ) ;
        setOrtho         (               ) ;
      QtGL::MatrixMode   ( GL_MODELVIEW  ) ;
      QtGL::LoadIdentity (               ) ;
    break                                  ;
    case Floating                          :
      setFloating        (               ) ;
    break                                  ;
    case NodeMatrix                        :
      QtGL::MatrixMode   ( GL_PROJECTION ) ;
      QtGL::LoadIdentity (               ) ;
            SetMatrix    (               ) ;
      QtGL::MatrixMode   ( GL_MODELVIEW  ) ;
      QtGL::LoadIdentity (               ) ;
            setViewport  (               ) ;
    break                                  ;
  }                                        ;
}

void N::Camera::Render(void)
{
  switch ( mode )                          {
    case Reality                           :
    break                                  ;
    case Perfect                           :
    break                                  ;
    case Perspective                       :
    break                                  ;
    case Orthographic                      :
    break                                  ;
    case Frustum                           :
    break                                  ;
    case Fit                               :
    break                                  ;
    case Parking                           :
    break                                  ;
    case Floating                          :
    break                                  ;
    case NodeMatrix                        :
    break                                  ;
  }                                        ;
}

void N::Camera::setBounding(void)
{
  switch ( mode )                          {
    case Reality                           :
    break                                  ;
    case Perfect                           :
    break                                  ;
    case Perspective                       :
    break                                  ;
    case Orthographic                      :
    break                                  ;
    case Frustum                           :
    break                                  ;
    case Fit                               :
    break                                  ;
    case Parking                           :
    break                                  ;
    case Floating                          :
    break                                  ;
    case NodeMatrix                        :
    break                                  ;
  }                                        ;
}

void N::Camera::FitWindow(void)
{
  QRectF R = viewport    ;
  int w = R.width () / 2 ;
  int h = R.height() / 2 ;
  minimum.x = -w         ;
  minimum.y = -h         ;
  maximum.x =  w         ;
  maximum.y =  h         ;
}

bool N::Camera::ParkingArea(QRectF & area)
{
  if ( references . count ( ) != 2 ) return false  ;
  int I,L,T,W,H                                    ;
  I = 0                                            ;
  L = references [ ReferenceCorner ] . x           ;
  T = references [ ReferenceCorner ] . y           ;
  W = references [ ReferenceSize   ] . x           ;
  H = references [ ReferenceSize   ] . y           ;
  #define AsItem(KEY,V)                            \
    if ( operating . contains ( KEY ) && operating [ KEY ] ) I = V ;
  AsItem ( AlignCenter      , 1 )                  ;
  AsItem ( AlignTopLeft     , 2 )                  ;
  AsItem ( AlignTopRight    , 3 )                  ;
  AsItem ( AlignLeftBottom  , 4 )                  ;
  AsItem ( AlignRightBottom , 5 )                  ;
  #undef  AsItem
  switch ( I )                                     {
    case 0                                         :
      L += viewport . left   ( )                   ;
      T += viewport . top    ( )                   ;
    break                                          ;
    case 1                                         :
      L += viewport . width  ( ) / 2               ;
      T += viewport . height ( ) / 2               ;
    break                                          ;
    case 2                                         :
      L += viewport . left   ( )                   ;
      T += viewport . top    ( )                   ;
    break                                          ;
    case 3                                         :
      L += viewport . right  ( )                   ;
      T += viewport . top    ( )                   ;
    break                                          ;
    case 4                                         :
      L += viewport . left   ( )                   ;
      T += viewport . bottom ( )                   ;
    break                                          ;
    case 5                                         :
      L += viewport . right  ( )                   ;
      T += viewport . bottom ( )                   ;
    break                                          ;
  }                                                ;
  area . setLeft   ( L )                           ;
  area . setTop    ( T )                           ;
  area . setWidth  ( W )                           ;
  area . setHeight ( H )                           ;
  return true                                      ;
}

void N::Camera::Dock(void)
{
  QRectF area            ;
  if (ParkingArea(area)) {
    setViewport(area)    ;
  } else                 {
    setWindowPort()      ;
  }                      ;
}

void N::Camera::Partial(void)
{
  QRectF area            ;
  if (ParkingArea(area)) {
    setViewport(area)    ;
  } else                 {
    setViewport ()       ;
  }                      ;
}

void N::Camera::setFloating(void)
{
  QRectF area                     ;
  if (ParkingArea(area))          {
    setViewport    (       area ) ;
    setPerspective (Factor(area)) ;
  } else                          {
    setViewport    ( )            ;
    setPerspective ( )            ;
  }                               ;
}

bool N::Camera::atPort(QRectF & area,QPoint pos,QPointF & portPos)
{
  QPointF P                         ;
  bool    inside                    ;
  P.setX(pos.x())                   ;
  P.setY(viewport.bottom()-pos.y()) ;
  inside   = area.contains(P)       ;
  portPos  = P                      ;
  portPos -= area.center()          ;
  return inside                     ;
}

bool N::Camera::EyePos(QRectF & window,QPointF cursor,ControlPoint & pos)
{
  bool ep = false                                       ;
  switch ( mode )                                       {
    case Reality                                        :
      ep = EyePosPerspective  ( window , cursor , pos ) ;
    break                                               ;
    case Perfect                                        :
      ep = EyePosPerspective  ( window , cursor , pos ) ;
    break                                               ;
    case Perspective                                    :
      ep = EyePosPerspective  ( window , cursor , pos ) ;
    break                                               ;
    case Orthographic                                   :
      ep = EyePosOrthographic ( window , cursor , pos ) ;
    break                                               ;
    case Frustum                                        :
      ep = EyePosFrustum      ( window , cursor , pos ) ;
    break                                               ;
    case Fit                                            :
      ep = EyePosFit          ( window , cursor , pos ) ;
    break                                               ;
    case Parking                                        :
      ep = EyePosParking      ( window , cursor , pos ) ;
    break                                               ;
    case Floating                                       :
      ep = EyePosFloating     ( window , cursor , pos ) ;
    break                                               ;
    case NodeMatrix                                     :
    break                                               ;
  }                                                     ;
  return ep                                             ;
}

bool N::Camera::EyePosPerspective(QRectF & window,QPointF cursor,ControlPoint & pos)
{
  pos     = eye ;
  pos . t = 1   ;
  return true   ;
}

bool N::Camera::EyePosOrthographic(QRectF & window,QPointF cursor,ControlPoint & pos)
{
  pos     = eye ;
  pos . t = 1   ;
  return true   ;
}

bool N::Camera::EyePosFrustum(QRectF & window,QPointF cursor,ControlPoint & pos)
{
  pos     = eye ;
  pos . t = 1   ;
  return true   ;
}

bool N::Camera::EyePosFit(QRectF & window,QPointF cursor,ControlPoint & pos)
{
  pos     = eye ;
  pos . t = 1   ;
  return true   ;
}

bool N::Camera::EyePosParking(QRectF & window,QPointF cursor,ControlPoint & pos)
{
  QRectF area                                              ;
  if ( ! ParkingArea ( area ) ) area = window              ;
  //////////////////////////////////////////////////////////
  int    w = area . width  ( )                             ;
  int    h = area . height ( )                             ;
  //////////////////////////////////////////////////////////
  pos . x  = cursor . x ( ) - area . left ( ) - ( w / 2 )  ;
  pos . y  = ( h / 2 )  - cursor . y ( ) + area . top  ( ) ;
  pos . z  = maximum . z                                   ;
  pos . t  = 1                                             ;
  //////////////////////////////////////////////////////////
  pos . x /= w                                             ;
  pos . y /= h                                             ;
  pos . x *= ( maximum . x - minimum . x )                 ;
  pos . y *= ( maximum . y - minimum . y )                 ;
  //////////////////////////////////////////////////////////
  return true                                              ;
}

bool N::Camera::EyePosFloating(QRectF & window,QPointF cursor,ControlPoint & pos)
{
  pos     = eye ;
  pos . t = 1   ;
  return true   ;
}

bool N::Camera::At(ControlPoint & cursor,ControlPoint & plane)
{
  bool ep = false                            ;
  switch ( mode )                            {
    case Reality                             :
      ep = AtPerspective  ( cursor , plane ) ;
    break                                    ;
    case Perfect                             :
      ep = AtPerspective  ( cursor , plane ) ;
    break                                    ;
    case Perspective                         :
      ep = AtPerspective  ( cursor , plane ) ;
    break                                    ;
    case Orthographic                        :
      ep = AtOrthographic ( cursor , plane ) ;
    break                                    ;
    case Frustum                             :
      ep = AtFrustum      ( cursor , plane ) ;
    break                                    ;
    case Fit                                 :
      ep = AtFit          ( cursor , plane ) ;
    break                                    ;
    case Parking                             :
      ep = AtParking      ( cursor , plane ) ;
    break                                    ;
    case Floating                            :
      ep = AtFloating     ( cursor , plane ) ;
    break                                    ;
    case NodeMatrix                          :
    break                                    ;
  }                                          ;
  return ep                                  ;
}

bool N::Camera::AtPerspective(ControlPoint & cursor,ControlPoint & plane)
{
  QRectF  W = viewport               ;
  QPointF ZP ( cursor.x , cursor.y ) ;
  if (!W.contains(ZP)) return false  ;
  return PerspectPos(W,cursor,plane) ;
}

bool N::Camera::AtOrthographic(ControlPoint & cursor,ControlPoint & plane)
{
  QRectF area = viewport                    ;
  return OrthoPos ( area , cursor , plane ) ;
}

bool N::Camera::AtFrustum(ControlPoint & cursor,ControlPoint & plane)
{
  #pragma message("AtFrustum requires to add an AT position calculation")
  return false ;
}

bool N::Camera::AtFit(ControlPoint & cursor,ControlPoint & plane)
{
  QPointF p (cursor.x,cursor.y)           ;
  if (!viewport.contains(p)) return false ;
  plane.x  = p.x()                        ;
  plane.y  = viewport.bottom() - p.y()    ;
  plane.x /= viewport.width () - 1        ;
  plane.y /= viewport.height() - 1        ;
  plane.x *= (maximum.x-minimum.x)        ;
  plane.y *= (maximum.y-minimum.y)        ;
  plane.x += minimum.x                    ;
  plane.y += minimum.y                    ;
  #pragma message("AtFit requires a fix up about z axis value")
  plane.t  = 1                            ;
  return true                             ;
}

bool N::Camera::AtParking(ControlPoint & cursor,ControlPoint & plane)
{
  QRectF        area                   ;
  if (!ParkingArea(area)) return false ;
  return OrthoPos(area,cursor,plane)   ;
}

bool N::Camera::AtFloating(ControlPoint & mouse,ControlPoint & plane)
{
  QRectF       W                         ;
  QRectF       RR                        ;
  ControlPoint cursor                    ;
  cursor.x = mouse.x                     ;
  cursor.y = viewport.bottom() - mouse.y ;
  QPointF ZP ( cursor.x , cursor.y )     ;
  if (!ParkingArea(RR)) return false     ;
  if (!RR.contains(ZP)) return false     ;
  W . setLeft   ( 0              )       ;
  W . setTop    ( 0              )       ;
  W . setWidth  ( RR . width  () )       ;
  W . setHeight ( RR . height () )       ;
  cursor.x = mouse.x - RR.left()         ;
  cursor.y = RR.bottom() - cursor.y      ;
  return PerspectPos(W,cursor,plane)     ;
}

bool N::Camera::PerspectPos(QRectF & W,ControlPoint & cursor,ControlPoint & plane)
{
  ControlPoint P                                   ;
  ControlPoint C                                   ;
  ControlPoint E                                   ;
  ControlPoint V                                   ;
  ControlPoint X                                   ;
  ControlPoint Y                                   ;
  ControlPoint FX                                  ;
  ControlPoint FY                                  ;
  double       L                                   ;
  double       Half = W.height()                   ;
  double       fov  = FOV / 2                      ;
  double       cos  = Math::fastCosine (fov)       ;
  double       sin  = Math::fastSine   (fov)       ;
  double       R                                   ;
  double       RL                                  ;
  double       RX                                  ;
  double       RY                                  ;
  //////////////////////////////////////////////////
  V         = eye                                  ;
  V        -= center                               ;
  L         = V . length   (       )               ;
  V         . normalize    (       )               ;
  E         = eye                                  ;
  Y         = up                                   ;
  Y         . normalize    (       )               ;
  X         = crossProduct ( Y , V )               ;
  X         . normalize    (       )               ;
  C         = W.center     (       )               ;
  P         = cursor                               ;
  P        -= C                                    ;
  P . y     = - P . y                              ;
  Half     /= 2                                    ;
  R         = L / cos                              ;
  RL        = R * sin                              ;
  RL       /= Half                                 ;
  RX        = RL * P.x                             ;
  RY        = RL * P.y                             ;
  FX        = X                                    ;
  FY        = Y                                    ;
  FX       *= RX                                   ;
  FY       *= RY                                   ;
  FX       += FY                                   ;
  FX       += center                               ;
  plane     = FX                                   ;
  plane . t = 1                                    ;
  return true                                      ;
}

bool N::Camera::OrthoPos(QRectF & area,ControlPoint & cursor,ControlPoint & plane)
{
  ControlPoint p                                           ;
  double       w = area . width  ( )                       ;
  double       h = area . height ( )                       ;
  //////////////////////////////////////////////////////////
  p     . x    = cursor . x - area . left ( ) - ( w / 2 )  ;
  p     . y    = ( h / 2 )  - cursor . y + area . top  ( ) ;
  p     . z    = 0                                         ;
  p     . t    = 1                                         ;
  //////////////////////////////////////////////////////////
  plane      = p                                           ;
  plane . x /= w                                           ;
  plane . y /= h                                           ;
  plane . x *= ( maximum . x - minimum . x )               ;
  plane . y *= ( maximum . y - minimum . y )               ;
  p          = plane                                       ;
  //////////////////////////////////////////////////////////
  if ( p . x < minimum . x ) return false                  ;
  if ( p . y < minimum . y ) return false                  ;
  if ( p . x > maximum . x ) return false                  ;
  if ( p . y > maximum . y ) return false                  ;
  return true                                              ;
}

double N::Camera::FocusPoint(double pixels,double length,double distance)
{
  double fp = pixels ;
  fp *= distance     ;
  fp /= 2            ;
  fp /= length       ;
  return fp          ;
}

double N::Camera::Distance(double pixels,double length,double fp)
{
  double d = length ;
  d *= fp           ;
  d /= pixels       ;
  return d          ;
}

double N::Camera::ObjectLength(double pixels,double distance,double fp)
{
  double D = pixels ;
  D *= distance     ;
  D /= fp           ;
  return D          ;
}

double N::Camera::ObjectPixels(double length,double distance,double fp)
{
  double P = length ;
  P *= fp           ;
  P /= distance     ;
  return P          ;
}

double N::Camera::CcdFov(double M,double F)
{
  long double angle      ;
  angle  = M             ;
  angle /= 2             ;
  angle /= F             ;
  angle  = ::atan(angle) ;
  angle *= 360.0         ;
  angle /= M_PI          ;
  return angle           ;
}
