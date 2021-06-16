#include <qtworld.h>

#ifdef QT_OPENGL_LIB

N::PathRunner:: PathRunner     ( void  )
              : node           ( NULL  )
              , RemoveFunction ( NULL  )
              , Dispatcher     ( NULL  )
              , Deletion       ( false )
              , uuid           ( 0     )
              , DOF            ( 0     )
{
}

N::PathRunner::~PathRunner (void)
{
}

void N::PathRunner::AutoDeletion(void)
{
  if ( ! Deletion ) return                   ;
  if ( Scene::Object == node -> nodeType )   {
    SceneObject * sox = (SceneObject *) node ;
    sox -> runners . remove ( uuid )         ;
    delete this                              ;
  } else
  if ( NULL != RemoveFunction )              {
    QVariants v                              ;
    RemoveFunction ( this , Dispatcher , v ) ;
  }                                          ;
}

int N::PathRunner::step(int after)
{
  return -1 ;
}

bool N::PathRunner::execute(void)
{
  return false ;
}

#endif
