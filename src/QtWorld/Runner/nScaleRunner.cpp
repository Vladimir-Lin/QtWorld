#include <qtworld.h>

#ifdef QT_OPENGL_LIB

N::ScaleRunner:: ScaleRunner (void)
               : PathRunner  (    )
{
  RemoveFunction = NULL  ;
  Dispatcher     = NULL  ;
  Deletion       = false ;
  uuid           = 0     ;
  DOF            = 0     ;
}

N::ScaleRunner::~ScaleRunner (void)
{
}

int N::ScaleRunner::step(int after)
{
  if ( IsNull(node)     ) return -1 ;
  if ( after >  0       ) return -1 ;
  if ( Start > nTimeNow ) return -1 ;
  if ( Final < nTimeNow )           {
    AutoDeletion ( )                ;
    return -1                       ;
  }                                 ;
  return 0                          ;
}

bool N::ScaleRunner::execute(void)
{
  double       f                        ;
  double       r                        ;
  ControlPoint p                        ;
  ///////////////////////////////////////
  r     = Start . msecsTo ( Final    )  ;
  f     = Start . msecsTo ( nTimeNow )  ;
  f    /= r                             ;
  ///////////////////////////////////////
  p       = interpolate ( S1 , S2 , f ) ;
  p . DOF = ControlPoint::DofScale      ;
  p . t   = 1.0                         ;
  p . r   = 1.0                         ;
  ///////////////////////////////////////
  node -> LockAt   ( "Operate" )        ;
  node -> addDof   ( DOF , p   )        ;
  node -> Dual     (           )        ;
  node -> UnlockAt ( "Operate" )        ;
  ///////////////////////////////////////
  return true                           ;
}

#endif
