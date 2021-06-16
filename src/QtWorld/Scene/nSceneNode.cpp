#include <qtworld.h>

N::SceneNode:: SceneNode   ( void )
             : Node        (      )
             , SceneBox    (      )
             , CacheLayout ( 0    )
             , CacheState  ( 0    )
             , destructor  ( NULL )
             , gui         ( NULL )
{
  addType ( Scene::Root ) ;
}

N::SceneNode:: SceneNode   ( int Type )
             : Node        (     Type )
             , SceneBox    (          )
             , CacheLayout ( 0        )
             , CacheState  ( 0        )
             , destructor  ( NULL     )
             , gui         ( NULL     )
{
  addType ( Scene::Root ) ;
  addType ( Type        ) ;
}

N::SceneNode::~SceneNode (void)
{
  if ( NULL != destructor ) {
    (*destructor) -= this   ;
  }                         ;
}

void N::SceneNode::suicide(void)
{
  delete this ;
}

typedef struct      {
  N::SceneNode * me ;
} ClassStructure    ;

void N::SceneNode::ObjectPointer(QByteArray & pointer)
{
  pointer . resize ( sizeof(ClassStructure) )                 ;
  ClassStructure * cs = (ClassStructure *) pointer . data ( ) ;
  cs -> me = this                                             ;
}

void N::SceneNode::setProperties(void)
{
  QtGL::Enable ( oglenables ) ;
}

void N::SceneNode::setProperty(GLenum pname,bool enable)
{
  oglenables [ pname ] = enable ;
}

void N::SceneNode::setFunction(int id,bool enable)
{
  properties [ id ] = enable ;
}

bool N::SceneNode::isFunction(int id)
{
  if ( ! properties . contains ( id ) ) return false ;
  return properties [ id ]                           ;
}

bool N::SceneNode::hasFunction(QString key)
{
  return Value ( key ) . toBool ( ) ;
}

void N::SceneNode::setCache(int size)
{
  Cache . resize ( size ) ;
}

void N::SceneNode::setCacheLayout(int type)
{
  CacheLayout = type ;
}

void * N::SceneNode::cache(void)
{
  return (void *) Cache . data ( ) ;
}

int N::SceneNode::cacheLayout(void)
{
  return CacheLayout ;
}

void N::SceneNode::setCacheState(TUID state)
{
  CacheState = state ;
}

TUID N::SceneNode::cacheState(void)
{
  return CacheState ;
}

bool N::SceneNode::JoinDestructor(SceneNode * node)
{
  nKickOut ( IsNull(destructor) , false ) ;
  (*destructor) += node                   ;
  node -> destructor = destructor         ;
  return true                             ;
}

bool N::SceneNode::JoinDestructor(QImage * image)
{
  nKickOut ( IsNull(destructor) , false ) ;
  (*destructor) += image                  ;
  return true                             ;
}

bool N::SceneNode::JoinDestructor(void)
{
  nKickOut ( IsNull(destructor) , false ) ;
  (*destructor) += this                   ;
  return true                             ;
}

void N::SceneNode::LockAt(int index)
{
  NodeMutex     [ index ] . lock   ( ) ;
  isMutexLocked [ index ] = true       ;
}

void N::SceneNode::UnlockAt(int index)
{
  NodeMutex     [ index ] . unlock ( ) ;
  isMutexLocked [ index ] = false      ;
}

bool N::SceneNode::isLocked(int index)
{
  if ( ! isMutexLocked . contains ( index ) ) return false ;
  return isMutexLocked [ index ]                           ;
}

void N::SceneNode::LockAt(qint64 index)
{
  NodeMutex     [ index ] . lock   ( ) ;
  isMutexLocked [ index ] = true       ;
}

void N::SceneNode::UnlockAt(qint64 index)
{
  NodeMutex     [ index ] . unlock ( ) ;
  isMutexLocked [ index ] = false      ;
}

bool N::SceneNode::isLocked(qint64 index)
{
  if ( ! isMutexLocked . contains ( index ) ) return false ;
  return isMutexLocked [ index ]                           ;
}

void N::SceneNode::LockAt(QString index)
{
  NodeMutex     [ index ] . lock   ( ) ;
  isSutexLocked [ index ] = true       ;
}

void N::SceneNode::UnlockAt(QString index)
{
  NodeMutex     [ index ] . unlock ( ) ;
  isSutexLocked [ index ] = false      ;
}

bool N::SceneNode::isLocked(QString index)
{
  if ( ! isSutexLocked . contains ( index ) ) return false ;
  return isSutexLocked [ index ]                           ;
}
