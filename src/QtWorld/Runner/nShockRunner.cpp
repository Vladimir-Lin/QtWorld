#include <qtworld.h>

#ifdef QT_OPENGL_LIB

N::ShockRunner:: ShockRunner (void)
               : PathRunner  (    )
{
  rps        = 0.20  ;
  ShockAngle = 15.0  ;
}

N::ShockRunner::~ShockRunner (void)
{
}

int N::ShockRunner::step(int after)
{
  if ( IsNull(node)     ) return -1 ;
  if ( after >  0       ) return -1 ;
  if ( Start > Final    ) return  0 ;
  if ( Start > nTimeNow ) return -1 ;
  if ( Final < nTimeNow )           {
    node -> LockAt   ( "Operate" )  ;
    node -> addDof   ( DOF , R   )  ;
    node -> Dual     (           )  ;
    node -> UnlockAt ( "Operate" )  ;
    AutoDeletion ( )                ;
    return -1                       ;
  }                                 ;
  return 0                          ;
}

bool N::ShockRunner::execute(void)
{
  double       f                       ;
  qint64       r                       ;
  ControlPoint p                       ;
  //////////////////////////////////////
  f     = Start . msecsTo ( nTimeNow ) ;
  f    /= 1000                         ; // seconds
  f    /= rps                          ;
  f    *= 360.0                        ;
  f    += R . r                        ;
  r     = (qint64) f                   ;
  r    /= 360                          ;
  r    *= 360                          ;
  f    -= r                            ;
  f     = Math::fastCosine ( f )       ;
  f    *= ShockAngle                   ;
  //////////////////////////////////////
  p       = R                          ;
  p . DOF = ControlPoint::DofRotate    ;
  p . r   = f                          ;
  //////////////////////////////////////
  node -> LockAt   ( "Operate" )       ;
  node -> addDof   ( DOF , p   )       ;
  node -> Dual     (           )       ;
  node -> UnlockAt ( "Operate" )       ;
  //////////////////////////////////////
  return true                          ;
}

#endif
