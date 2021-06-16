#include <qtworld.h>

N::SceneDestructor:: SceneDestructor(void)
{
}

N::SceneDestructor::~SceneDestructor(void)
{
  cleanup ( ) ;
}

void N::SceneDestructor::cleanup(void)
{
  if ( images   . count ( ) > 0 )                     {
    for (int i = 0 ; i < images   . count ( ) ; i++ ) {
      delete images   [ i ]                           ;
    }                                                 ;
    images . clear ( )                                ;
  }                                                   ;
  if ( original . count ( ) > 0 )                     {
    for (int i = 0 ; i < original . count ( ) ; i++ ) {
      delete original [ i ]                           ;
    }                                                 ;
    original . clear ( )                              ;
  }                                                   ;
  if ( nodes    . count ( ) > 0 )                     {
    for (int i = 0 ; i < nodes    . count ( ) ; i++ ) {
      delete nodes    [ i ]                           ;
    }                                                 ;
    nodes  . clear ( )                                ;
  }                                                   ;
}

void N::SceneDestructor::operator += (QImage * image)
{
  nDropOut ( IsNull ( image )            ) ;
  nDropOut ( images . contains ( image ) ) ;
  images << image                          ;
}

void N::SceneDestructor::operator -= (QImage * image)
{
  nDropOut ( IsNull ( image )              ) ;
  nDropOut ( ! images . contains ( image ) ) ;
  int index                                  ;
  index = images . indexOf ( image )         ;
  if ( index < 0 ) return                    ;
  images . takeAt ( index )                  ;
}

void N::SceneDestructor::Append(QImage * image)
{
  nDropOut ( IsNull ( image )              ) ;
  nDropOut ( original . contains ( image ) ) ;
  original << image                          ;
}

void N::SceneDestructor::Remove(QImage * image)
{
  nDropOut ( IsNull ( image )                ) ;
  nDropOut ( ! original . contains ( image ) ) ;
  int index                                    ;
  index = original . indexOf ( image )         ;
  if ( index < 0 ) return                      ;
  original . takeAt ( index )                  ;
  delete image                                 ;
}

void N::SceneDestructor::operator += (SceneNode * node)
{
  nDropOut ( IsNull ( node )           ) ;
  nDropOut ( nodes . contains ( node ) ) ;
  nodes  << node                         ;
  node -> destructor = this              ;
}

void N::SceneDestructor::operator -= (SceneNode * node)
{
  nDropOut ( IsNull ( node )             ) ;
  nDropOut ( ! nodes . contains ( node ) ) ;
  int index                                ;
  index = nodes . indexOf ( node )         ;
  if ( index < 0 ) return                  ;
  nodes . takeAt ( index )                 ;
}

void N::SceneDestructor::addChildren (SceneObject * object)
{
  nDropOut ( IsNull ( object ) )          ;
  UUIDs K = object -> children . keys ( ) ;
  SUID  k                                 ;
  foreach ( k , K )                       {
    SceneObject * c                       ;
    c = object -> children [ k ]          ;
    if ( ! nodes . contains ( c ) )       {
      nodes << c                          ;
    }                                     ;
    c -> destructor = this                ;
  }                                       ;
}

void N::SceneDestructor::addTree(SceneObject * object)
{
  nDropOut ( IsNull ( object ) )          ;
  UUIDs K = object -> children . keys ( ) ;
  SUID  k                                 ;
  if ( ! nodes . contains ( object ) )    {
    nodes  << object                      ;
    object -> destructor = this           ;
  }                                       ;
  foreach ( k , K )                       {
    addTree ( object -> children [ k ] )  ;
  }                                       ;
}

bool N::SceneDestructor::Destroy(SceneNode * node)
{
  nKickOut ( IsNull ( node ) , false ) ;
  int idx = nodes . indexOf ( node   ) ;
  nKickOut ( idx < 0    , false      ) ;
  nodes . takeAt ( idx )               ;
  delete node                          ;
  return true                          ;
}
