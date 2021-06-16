#include <qtworld.h>

N::FlatUniverse:: FlatUniverse ( void )
                : Universe     (      )
{
  name = "Flat" ;
  Configure ( ) ;
}

N::FlatUniverse::~FlatUniverse (void)
{
}

void N::FlatUniverse::Configure (void)
{
  Camera * C = new Camera()                                       ;
  C-> mode   = Camera::Parking                                    ;
  C-> FOV    = 45                                                 ;
  C-> eye    = ControlPoint (400.0,400.0,800.0)                   ;
  C-> center = ControlPoint (  0.0,  0.0,  0.0)                   ;
  C-> up     = ControlPoint (  0.0,  1.0,  0.0)                   ;
  C-> setNearFar            ( -10000 , 10000  )                   ;
  C-> references[Camera::ReferenceCorner] = ControlPoint( 0, 0,0) ;
  C-> references[Camera::ReferenceSize  ] = ControlPoint(10,10,0) ;
  C-> minimum.x =   0                                             ;
  C-> minimum.y =   0                                             ;
  C-> maximum.x = 200                                             ;
  C-> maximum.y = 200                                             ;
  C-> operating [Camera::AlignTopLeft] = true                     ;
  addCamera        ( 0 , C )                                      ;
  setCurrentCamera ( 0     )                                      ;
  JoinDestructor   ( C     )                                      ;
}

void N::FlatUniverse::setAlign(int align)
{
  nDropOut ( IsNull(CurrentCamera) )                              ;
  for (int i=Camera::AlignCenter;i<=Camera::AlignRightBottom;i++) {
    CurrentCamera->operating[i] = false                           ;
  }                                                               ;
  CurrentCamera->operating[align] = true                          ;
}

void N::FlatUniverse::setArea(QRect Port,QRectF Region)
{
  Camera * C = CurrentCamera                     ;
  nDropOut ( IsNull(C) )                         ;
  QPointF       RC = Region.center()             ;
  ControlPoint TL(Port.x    (),Port.y     (),0)  ;
  ControlPoint WH(Port.width(),Port.height(),0)  ;
  ControlPoint EP(RC.x(),RC.y(),1000.0)          ;
  ControlPoint EC(RC.x(),RC.y(),   0.0)          ;
  C-> references[Camera::ReferenceCorner] = TL   ;
  C-> references[Camera::ReferenceSize  ] = WH   ;
  C-> eye       = EP                             ;
  C-> center    = EC                             ;
  C-> minimum.x = Region . left   ()             ;
  C-> minimum.y = Region . top    ()             ;
  C-> maximum.x = Region . right  ()             ;
  C-> maximum.y = Region . bottom ()             ;
}

bool N::FlatUniverse::mouseDoubleClickEvent(QWidget * widget,QMouseEvent * event)
{
  return Universe::mouseDoubleClickEvent(widget,event) ;
}

bool N::FlatUniverse::mouseMoveEvent(QWidget * widget,QMouseEvent * event)
{
  Universe::mouseMoveEvent(widget,event)    ;
  if (!isActivated(MouseLeft)) return false ;
  ControlPoint cursor = Users[MouseMove ]   ;
  ControlPoint press  = Users[MousePress]   ;
  Camera * C = (Camera*)CurrentCamera       ;
  ControlPoint FX                           ;
  ControlPoint FY                           ;
  if (!C->At(cursor,FX)) return false       ;
  if (!C->At(press ,FY)) return false       ;
  return Moving(FY,FX)                      ;
}

bool N::FlatUniverse::mousePressEvent(QWidget * widget,QMouseEvent * event)
{
  return Universe::mousePressEvent(widget,event) ;
}

bool N::FlatUniverse::mouseReleaseEvent(QWidget * widget,QMouseEvent * event)
{
  return Universe::mouseReleaseEvent(widget,event) ;
}

bool N::FlatUniverse::Moving(ControlPoint & from,ControlPoint & to)
{
  return false ;
}

bool N::FlatUniverse::Menu(QWidget * widget,QPointF pos)
{
  return false ;
}
