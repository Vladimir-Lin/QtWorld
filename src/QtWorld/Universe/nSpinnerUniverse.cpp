#include <qtworld.h>

N::SpinnerUniverse:: SpinnerUniverse ( void )
                   : FlatUniverse    (      )
{
  name = "Spinner" ;
  Configure ( )    ;
}

N::SpinnerUniverse::~SpinnerUniverse(void)
{
}

void N::SpinnerUniverse::Configure(void)
{
  Circle C                                ;
  setProperty ( GL_DEPTH_TEST , false )   ;
  Circles [ 0 ] = C                       ;
  Circles [ 1 ] = C                       ;
  Circles [ 2 ] = C                       ;
  Colors  [ 0 ] = QColor(192,192,192, 96) ;
  Colors  [ 1 ] = QColor(192,192,192,128) ;
  Colors  [ 2 ] = QColor(192,192,192,192) ;
  ShiftAngle    = 90                      ;
  FOA           = 30                      ;
  AnglePointer  = NULL                    ;
}

bool N::SpinnerUniverse::Place(void)
{
  SceneObject * ZR = Scene::Circle(Circles[0]           ,Colors[0]    ) ;
  SceneObject * AC = Scene::Arc   (Circles[1],Circles[2],Colors[1],FOA) ;
  ZR-> addSequence ( 0 , SceneObject::RenderFaces                     ) ;
  AC-> addSequence ( 0 , SceneObject::RenderFaces                     ) ;
  addActor         ( 0 , ZR                                           ) ;
  addActor         ( 1 , AC                                           ) ;
  Perform          ( 0 , true                                         ) ;
  Perform          ( 1 , true                                         ) ;
  showAngle        ( ShiftAngle                                       ) ;
  JoinDestructor   ( ZR                                               ) ;
  JoinDestructor   ( AC                                               ) ;
  return true                                                           ;
}

void N::SpinnerUniverse::setDefault(void)
{
  QRect  WR     ( -64 ,-64 , 128 , 128 )           ;
  QRectF VR     ( -64 ,-64 , 128 , 128 )           ;
  Circles [ 0 ] . O = ControlPoint (  0, 0, -1   ) ;
  Circles [ 0 ] . X = ControlPoint ( 64, 0, -1   ) ;
  Circles [ 0 ] . Y = ControlPoint (  0,64, -1   ) ;
  Circles [ 0 ] . N = (int)        ( 64.0*M_PI*4 ) ;
  Circles [ 1 ] . O = ControlPoint (  0, 0,  0   ) ;
  Circles [ 1 ] . X = ControlPoint ( 60, 0,  0   ) ;
  Circles [ 1 ] . Y = ControlPoint (  0,60,  0   ) ;
  Circles [ 1 ] . N = (int)        ( 60.0*M_PI*4 ) ;
  Circles [ 2 ] . O = ControlPoint (  0, 0,  0   ) ;
  Circles [ 2 ] . X = ControlPoint ( 40, 0,  0   ) ;
  Circles [ 2 ] . Y = ControlPoint (  0,40,  0   ) ;
  Circles [ 2 ] . N = (int)        ( 40.0*M_PI*4 ) ;
  setArea  ( WR  , VR             )                ;
  setAlign ( Camera::AlignCenter  )                ;
  Place    (                      )                ;
}

void N::SpinnerUniverse::showAngle(double angle)
{
  if (!Actors.contains(1)) return                    ;
  if (angle<  0.0) angle += 360.0                    ;
  if (angle>360.0) angle -= 360.0                    ;
  SceneObject * actor = (SceneObject *)Actors[1]     ;
  if (IsNull(actor)) return                          ;
  Scene::Arc(actor,Circles[1],Circles[2],FOA,angle)  ;
  int k                                              ;
  HandleArea.clear()                                 ;
  foreach (k,actor->index)                           {
    QPointF P(actor->points[k].x,actor->points[k].y) ;
    HandleArea << P                                  ;
  }                                                  ;
}

bool N::SpinnerUniverse::mousePressEvent(QWidget * widget,QMouseEvent * event)
{
  Universe::mousePressEvent(widget,event)           ;
  Activations[1001] = false                         ;
  if (!isActivated(MouseLeft)) return false         ;
  if (IsNull(AnglePointer )  ) return false         ;
  ControlPoint cursor = Users[MousePress]           ;
  Camera     * C = CurrentCamera                    ;
  ControlPoint FX                                   ;
  if (!C->At(cursor,FX)) return false               ;
  if (FX.lengthSquared()<4.0) return false          ;
  QPointF P ( FX.x , FX.y )                         ;
  QPointF Z (    0 ,    0 )                         ;
  QLineF  L (    Z ,    P )                         ;
  bool is                                           ;
  is  = HandleArea.containsPoint(P,Qt::WindingFill) ;
  Activations[ 1001 ] = is                          ;
  AngleValues[    0 ] = *AnglePointer               ;
  AngleValues[    1 ] = L.angle()                   ;
  AngleValues[    2 ] = AngleValues [ 1 ]           ;
  if (Actors.contains(1))                           {
    SceneObject * actor = (SceneObject *)Actors[1]  ;
    actor->setColor(Colors[2])                      ;
  }                                                 ;
  return true                                       ;
}

bool N::SpinnerUniverse::mouseMoveEvent(QWidget * widget,QMouseEvent * event)
{
  Universe::mouseMoveEvent(widget,event)    ;
  if (!isActivated(MouseLeft)) return false ;
  if (IsNull(AnglePointer )  ) return false ;
  ///////////////////////////////////////////
  ControlPoint cursor = Users[MouseMove ]   ;
  Camera     * C = CurrentCamera            ;
  ControlPoint FX                           ;
  if (!C->At(cursor,FX)) return false       ;
  if (FX.lengthSquared()<4.0) return false  ;
  ///////////////////////////////////////////
  QPointF P ( FX.x , FX.y )                 ;
  QPointF Z (    0 ,    0 )                 ;
  QLineF  L (    Z ,    P )                 ;
  double da                                 ;
  AngleValues[2] = L.angle()                ;
  da = AngleValues[2] - AngleValues[1]      ;
  da = AngleValues[0] - da                  ;
  if (da<  0.0) da += 360.0                 ;
  if (da>360.0) da -= 360.0                 ;
  *AnglePointer = da                        ;
  showAngle ( da + ShiftAngle )             ;
  ///////////////////////////////////////////
  QString m                                 ;
  m = QString("%1").arg(da,3,'f',4,'0')     ;
  QToolTip::showText(QCursor::pos(),m)      ;
  ///////////////////////////////////////////
  return true                               ;
}

bool N::SpinnerUniverse::mouseReleaseEvent(QWidget * widget,QMouseEvent * event)
{
  Universe::mouseReleaseEvent(widget,event)        ;
  if (!Activations[1001]) return false             ;
  Activations[1001] = false                        ;
  if (Actors.contains(1))                          {
    SceneObject * actor = (SceneObject *)Actors[1] ;
    actor->setColor(Colors[1])                     ;
  }                                                ;
  return false                                     ;
}
