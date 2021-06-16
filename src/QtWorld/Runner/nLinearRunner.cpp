#include <qtworld.h>

#ifdef QT_OPENGL_LIB

N::LinearRunner:: LinearRunner (void)
                : PathRunner   (    )
{
}

N::LinearRunner::~LinearRunner (void)
{
}

int N::LinearRunner::step(int after)
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

bool N::LinearRunner::execute(void)
{
  double       f                       ;
  double       r                       ;
  ControlPoint p1                      ;
  ControlPoint p2                      ;
  ControlPoint p                       ;
  //////////////////////////////////////
  r     = Start . msecsTo ( Final    ) ;
  f     = Start . msecsTo ( nTimeNow ) ;
  f    /= r                            ;
  r     = 1.0 - f                      ;
  //////////////////////////////////////
  p1  = P1                             ;
  p2  = P2                             ;
  p1 *= r                              ;
  p2 *= f                              ;
  p   = p1                             ;
  p  += p2                             ;
  //////////////////////////////////////
  p . DOF = ControlPoint::DofTranslate ;
  p . t   = 1.0                        ;
  p . r   = 1.0                        ;
  //////////////////////////////////////
  node -> LockAt   ( "Operate" )       ;
  node -> addDof   ( DOF , p   )       ;
  node -> Dual     (           )       ;
  node -> UnlockAt ( "Operate" )       ;
  //////////////////////////////////////
  return true                          ;
}

#endif
