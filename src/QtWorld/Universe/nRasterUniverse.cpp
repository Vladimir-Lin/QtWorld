#include <qtworld.h>

N::RasterUniverse:: RasterUniverse(void)
                  : Universe      (    )
{
  name = "Raster" ;
  Configure ( )   ;
}

N::RasterUniverse:: RasterUniverse ( SceneDestructor * des )
                  : Universe       (                       )
{
  destructor = des   ;
  name = "Raster"    ;
  JoinDestructor ( ) ;
  Configure      ( ) ;
}

N::RasterUniverse::~RasterUniverse(void)
{
}

bool N::RasterUniverse::Animation(void)
{
  return Activations[RasterAnimation] ;
}

void N::RasterUniverse::Configure(void)
{
  Camera      * C = new Camera      ( destructor ) ;
  SceneObject * P = new SceneObject ( destructor ) ;
  double    d = 100000                             ;
  C-> mode    = Camera::Fit                        ;
  C-> FOV     = 60                                 ;
  C-> eye     = ControlPoint (1.0,1.0,2.0)         ;
  C-> center  = ControlPoint (0.0,0.0,0.0)         ;
  C-> up      = ControlPoint (0.0,1.0,0.0)         ;
  C-> setNearFar              ( -d , d    )        ;
  P-> addSequence (0,SceneObject::RenderPictures)  ;
  //////////////////////////////////////////////////
  setActivate ( RasterScaling   , true )           ;
  setActivate ( RasterAnimation , true )           ;
  limits      [ RasterObjectId  ] = 0              ;
  //////////////////////////////////////////////////
  addCamera        ( 0 , C    )                    ;
  setCurrentCamera ( 0        )                    ;
  addActor ( limits [ RasterObjectId ] , P    )    ;
  Perform  ( limits [ RasterObjectId ] , true )    ;
  JoinDestructor ( C )                             ;
  JoinDestructor ( P )                             ;
}

void N::RasterUniverse::MoveRasterizer(int NewActorId)
{
  int OldActorId = limits[RasterObjectId]                    ;
  SceneObject * actor = (SceneObject *)Actors [ OldActorId ] ;
  Perform     ( OldActorId , false )                         ;
  removeActor ( OldActorId         )                         ;
  addActor    ( NewActorId , actor )                         ;
  Perform     ( NewActorId , true  )                         ;
  limits [ RasterObjectId  ] = NewActorId                    ;
}

void N::RasterUniverse::Relocation(QRectF viewport)
{
  Universe::Relocation(viewport)                                          ;
  nDropOut ( !Actors.contains(limits[RasterObjectId]) )                   ;
  SceneObject * P = (SceneObject *)Actors[limits[RasterObjectId]]         ;
  double vw = viewport.width () - 2                                       ;
  double vh = viewport.height() - 2                                       ;
  double zz = 0                                                           ;
  if ( Users . contains ( 75000 ) ) zz = Users [ 75000 ] . z              ;
  for (int i=0;i<P->textures.count();i++)                                 {
    ControlPoint   S ( 1 , -1 , 0 , 1 )                                   ;
    SceneTexture * T  = (SceneTexture *) P -> textures [ i ]              ;
    QImage        * I  = T->original                                      ;
    double          w  = I->width ()                                      ;
    double          h  = I->height()                                      ;
    double          fw = vw / w                                           ;
    double          fh = vh / h                                           ;
    double          f  = fw                                               ;
    h   =  h / 2                                                          ;
    w   = -w / 2                                                          ;
    if (Activations[RasterScaling])                                       {
      if (f>fh) f = fh                                                    ;
      h  *= f                                                             ;
      w  *= f                                                             ;
      S   = ControlPoint(f,-f,0,1)                                        ;
    }                                                                     ;
    T -> rasterizers[SceneTexture::RasterPos ] = ControlPoint(w, h,zz,1)  ;
    T -> rasterizers[SceneTexture::RasterZoom] = S                        ;
  }                                                                       ;
}

void N::RasterUniverse::setDimension(QSize size,int bufferSize,QImage::Format fmt)
{
  nDropOut ( !Actors.contains(limits[RasterObjectId]) )                     ;
  nDropOut ( bufferSize < 1      )                                          ;
  SceneObject * P = (SceneObject *)Actors[limits[RasterObjectId]]           ;
  for (int i=0;i<bufferSize;i++)                                            {
    QImage       * I = new QImage       ( size , fmt )                      ;
    SceneTexture * T = new SceneTexture ( destructor )                      ;
    I -> fill ( QColor(0,0,0) )                                             ;
    P -> images   [ i ]  = I                                                ;
    P -> textures [ i ]  = T                                                ;
    P -> pictures [ i ]  = false                                            ;
    P -> textures [ i ] -> setRaster ( I )                                  ;
    if ( NULL != destructor ) (*destructor) += I                            ;
    JoinDestructor ( T )                                                    ;
  }                                                                         ;
  P -> pictures[0] = true                                                   ;
  Users [ 75000 ]  = ControlPoint ( size.width() , size.height() , 0 , -1 ) ;
}

int N::RasterUniverse::Frames(void)
{
  nKickOut ( !Actors.contains(limits[RasterObjectId]) , 0 )       ;
  SceneObject * P = (SceneObject *)Actors[limits[RasterObjectId]] ;
  return P->images.count()                                        ;
}

QImage * N::RasterUniverse::imageAt(int index)
{
  nKickOut ( !Actors.contains(limits[RasterObjectId]) , NULL )    ;
  SceneObject * P = (SceneObject *)Actors[limits[RasterObjectId]] ;
  if (index>=P->images.count()) return NULL                       ;
  return P->images[index]                                         ;
}

bool * N::RasterUniverse::imageDisplay(int index)
{
  nKickOut ( !Actors.contains(limits[RasterObjectId]) , NULL )    ;
  SceneObject * P = (SceneObject *)Actors[limits[RasterObjectId]] ;
  if (index>=P->images.count()) return NULL                       ;
  return &P->pictures[index]                                      ;
}
