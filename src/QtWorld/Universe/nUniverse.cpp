#include <qtworld.h>

N::Universe:: Universe      ( void            )
            : SceneNode     ( Scene::Universe )
            , OcTree        (                 )
            , CurrentCamera ( NULL            )
            , Unit          ( Physics::Pixel  )
{
  name = "Universe"                      ;
  setButtons  ( 0                      ) ;
  setVariable ( "AutoDeletion" , true  ) ;
  setVariable ( "Hover"        , false ) ;
  setVariable ( "Finalize"     , false ) ;
}

N::Universe::~Universe (void)
{
  AutoDeletion ( ) ;
}

bool N::Universe::ForUuid(SUID & u,qint64 & i,UUIDs & U)
{
  if ( Value ( "Finalize" ) . toBool ( ) ) return false ;
  if ( i >= U . count ( )                ) return false ;
  u = U [ i ]                                           ;
  i++                                                   ;
  return true                                           ;
}

bool N::Universe::ForChild(SUID & u,qint64 & i,UUIDs & U)
{
  if ( Value ( "Finalize" ) . toBool ( ) ) return false ;
  if ( i >= U . count ( )                ) return false ;
  SceneNode * sou = NULL                                ;
  while ( i < U . count ( ) )                           {
    u = U [ i ]                                         ;
    i++                                                 ;
    if ( Actors . contains ( u ) )                      {
      sou = Actors [ u ]                                ;
      if ( NULL != sou )                                {
        if ( Scene::Object == sou -> nodeType )         {
          return true                                   ;
        }                                               ;
      }                                                 ;
    }                                                   ;
  }                                                     ;
  return false                                          ;
}

void N::Universe::suicide(void)
{
  delete this ;
}

typedef struct     {
  N::Universe * me ;
} ClassStructure   ;

void N::Universe::ObjectPointer(QByteArray & pointer)
{
  pointer . resize ( sizeof(ClassStructure) )                 ;
  ClassStructure * cs = (ClassStructure *) pointer . data ( ) ;
  cs -> me = this                                             ;
}

void N::Universe::AutoDeletion(void)
{
  if ( ! Variables . contains ( "AutoDeletion" )   ) return ;
  if ( ! Variables [ "AutoDeletion" ] . toBool ( ) ) return ;
  RemoveObjects ( 0 )                                       ;
}

void N::Universe::RemoveObjects(int what)
{
  QStringList S                                         ;
  QString     s                                         ;
  UUIDs       U                                         ;
  SUID        u                                         ;
  switch ( what )                                       {
    case   0                                            :
      for (int i = 101 ; i <= 106 ; i++ )               {
        RemoveObjects ( i )                             ;
      }                                                 ;
      for (int i = 201 ; i <= 206 ; i++ )               {
        RemoveObjects ( i )                             ;
      }                                                 ;
    break                                               ;
    case 101                                            :
      Activations . clear ( )                           ;
    break                                               ;
    case 102                                            :
      Illuminates . clear ( )                           ;
    break                                               ;
    case 103                                            :
      Performs    . clear ( )                           ;
    break                                               ;
    case 104                                            :
      Users       . clear ( )                           ;
    break                                               ;
    case 105                                            :
      MUIDs       . clear ( )                           ;
    break                                               ;
    case 106                                            :
      Names       . clear ( )                           ;
    break                                               ;
    case 201                                            :
      U = Cameras . keys ( )                            ;
      foreach ( u , U )                                 {
        if ( NULL != destructor )                       {
          Camera * ca = Cameras [ u ]                   ;
          if ( destructor -> nodes . contains ( ca ) )  {
            (*destructor) -= ca                         ;
            delete ca                                   ;
          }                                             ;
        } else                                          {
          delete Cameras [ u ]                          ;
        }                                               ;
      }                                                 ;
      Cameras . clear ( )                               ;
    break                                               ;
    case 202                                            :
      U = Actors . keys ( )                             ;
      foreach ( u , U )                                 {
        if ( NULL != destructor )                       {
          SceneNode * sn = Actors [ u ]                 ;
          if ( destructor -> nodes . contains ( sn ) )  {
            (*destructor) -= sn                         ;
            delete sn                                   ;
          }                                             ;
        } else                                          {
          delete Actors [ u ]                           ;
        }                                               ;
      }                                                 ;
      Actors . clear ( )                                ;
    break                                               ;
    case 203                                            :
      U = Textures . keys ( )                           ;
      foreach ( u , U )                                 {
        if ( NULL != destructor )                       {
          SceneTexture * st = Textures [ u ]            ;
          if ( destructor -> nodes . contains ( st ) )  {
            (*destructor) -= st                         ;
            delete st                                   ;
          }                                             ;
        } else                                          {
          delete Textures [ u ]                         ;
        }                                               ;
      }                                                 ;
      Textures . clear ( )                              ;
    break                                               ;
    case 204                                            :
      U = Lights . keys ( )                             ;
      foreach ( u , U )                                 {
        if ( NULL != destructor )                       {
          Light * lt = Lights [ u ]                     ;
          if ( destructor -> nodes . contains ( lt ) )  {
            (*destructor) -= lt                         ;
            delete lt                                   ;
          }                                             ;
        } else                                          {
          delete Lights [ u ]                           ;
        }                                               ;
      }                                                 ;
      Lights . clear ( )                                ;
    break                                               ;
    case 205                                            :
      U = Runners . keys ( )                            ;
      foreach ( u , U )                                 {
        delete Runners [ u ]                            ;
      }                                                 ;
      Runners . clear ( )                               ;
    break                                               ;
    case 206                                            :
      S = userobjs . keys ( )                           ;
      foreach ( s , S ) if ( NULL != userobjs [ s ] )   {
        delete userobjs [ s ]                           ;
      }                                                 ;
      userobjs . clear ( )                              ;
    break                                               ;
  }                                                     ;
}

int N::Universe::addCamera(SUID uuid,Camera * camera)
{
  Cameras[uuid] = camera    ;
  JoinDestructor ( camera ) ;
  return Cameras.count()    ;
}

int N::Universe::removeCamera(SUID uuid)
{
  if (Cameras.contains(uuid)) {
    Cameras.remove(uuid)      ;
  }                           ;
  return Cameras.count()      ;
}

void N::Universe::setCurrentCamera(SUID uuid)
{
  CurrentCamera = NULL                 ;
  nDropOut ( !Cameras.contains(uuid) ) ;
  CurrentCamera = Cameras [ uuid ]     ;
}

int N::Universe::addLight(SUID uuid,Light * light)
{
  Lights[uuid] = light     ;
  JoinDestructor ( light ) ;
  return Lights.count()    ;
}

int N::Universe::removeLight(SUID uuid)
{
  if (Lights.contains(uuid)) {
    Lights.remove(uuid)      ;
  }                          ;
  return Lights.count()      ;
}

void N::Universe::illuminate(SUID uuid,bool enable)
{
  if (!Lights.contains(uuid)) return    ;
  if (enable)                           {
    if (!Illuminates.contains(uuid))    {
      Illuminates << uuid               ;
    }                                   ;
  } else                                {
    if (Illuminates.contains(uuid))     {
      int index                         ;
      index = Illuminates.indexOf(uuid) ;
      if (index<0) return               ;
      Illuminates.takeAt(index)         ;
    }                                   ;
  }                                     ;
}

int N::Universe::addActor(SUID uuid,SceneNode * actor)
{
  JoinDestructor ( actor ) ;
  Actors[uuid] = actor     ;
  return Actors.count()    ;
}

int N::Universe::removeActor(SUID uuid)
{
  if (Actors.contains(uuid)) {
    Actors.remove(uuid)      ;
  }                          ;
  return Actors.count()      ;
}

void N::Universe::Perform(SUID uuid,bool show)
{
  if (show)                            {
    if (!Performs.contains(uuid))      {
      Performs << uuid                 ;
    }                                  ;
  } else                               {
    if (Performs.contains(uuid))       {
      int index                        ;
      index = Performs.indexOf(uuid)   ;
      if (index<0) return              ;
      Performs.takeAt(index)           ;
    }                                  ;
  }                                    ;
}

bool N::Universe::mouseDoubleClickEvent(QWidget * widget,QMouseEvent * event)
{
  QPointF PX = event->pos()    ;
  ControlPoint p               ;
  p = PX                       ;
  Users[MouseDClick] = p       ;
  setButtons(event->buttons()) ;
  return false                 ;
}

bool N::Universe::mouseMoveEvent(QWidget * widget,QMouseEvent * event)
{
  QPointF PX = event->pos()    ;
  ControlPoint p               ;
  p = PX                       ;
  Users[MouseMove] = p         ;
  setButtons(event->buttons()) ;
  return false                 ;
}

bool N::Universe::mousePressEvent(QWidget * widget,QMouseEvent * event)
{
  QPointF PX = event->pos()    ;
  ControlPoint p               ;
  p = PX                       ;
  Users[MousePress] = p        ;
  setButtons(event->buttons()) ;
  return false                 ;
}

bool N::Universe::mouseReleaseEvent(QWidget * widget,QMouseEvent * event)
{
  QPointF PX = event->pos()    ;
  ControlPoint p               ;
  p = PX                       ;
  Users[MouseRelease] = p      ;
  setButtons(event->buttons()) ;
  return false                 ;
}

bool N::Universe::wheelEvent(QWidget * widget,QWheelEvent * event)
{
  return false ;
}

QVariant N::Universe::inputMethodQuery(QWidget * w,Qt::InputMethodQuery query) const
{
  return QVariant ( ) ;
}

void N::Universe::inputMethodEvent(QWidget * w,QInputMethodEvent * e)
{
}

void N::Universe::keyPressEvent(QWidget * w,QKeyEvent * e)
{
}

void N::Universe::keyReleaseEvent(QWidget * w,QKeyEvent * e)
{
}

void N::Universe::enterEvent(QWidget * w,QEvent * e)
{
}

void N::Universe::leaveEvent(QWidget * w,QEvent * e)
{
}

N::SceneNode * N::Universe::EnabledObject (QString key)
{
  UUIDs U = Performs                                  ;
  SUID  u                                             ;
  foreach ( u , U ) if ( Actors . contains ( u ) )    {
    if ( Actors [ u ] -> Value ( key ) . toBool ( ) ) {
      return Actors [ u ]                             ;
    }                                                 ;
  }                                                   ;
  return NULL                                         ;
}

N::SceneObject * N::Universe::CaptureMouse(void)
{
  UUIDs U = Performs                               ;
  SUID  u                                          ;
  foreach ( u , U ) if ( Actors . contains ( u ) ) {
    SceneNode * snx = Actors [ u ]                 ;
    if ( Scene::Object == snx -> nodeType )        {
      SceneObject * sox = (SceneObject *) snx      ;
      if ( sox -> isMouseGrabbed ( ) ) return sox  ;
    }                                              ;
  }                                                ;
  return NULL                                      ;
}

N::SceneObject * N::Universe::CaptureKeyboard(void)
{
  UUIDs U = Performs                                  ;
  SUID  u                                             ;
  foreach ( u , U ) if ( Actors . contains ( u ) )    {
    SceneNode * snx = Actors [ u ]                    ;
    if ( Scene::Object == snx -> nodeType )           {
      SceneObject * sox = (SceneObject *) snx         ;
      if ( sox -> isKeyboardGrabbed ( ) ) return sox  ;
    }                                                 ;
  }                                                   ;
  return NULL                                         ;
}

void N::Universe::setButtons(Qt::MouseButtons buttons)
{
  Activations [ MouseLeft   ] = IsMask ( buttons , Qt::LeftButton   ) ;
  Activations [ MouseMiddle ] = IsMask ( buttons , Qt::MiddleButton ) ;
  Activations [ MouseRight  ] = IsMask ( buttons , Qt::RightButton  ) ;
}

bool N::Universe::allowDrag(void)
{
  return Value ( "AllowDrag" ) . toBool ( ) ;
}

bool N::Universe::canDrop(void)
{
  return Value ( "AcceptDrop" ) . toBool ( ) ;
}

bool N::Universe::removeDrop(void)
{
  if ( ! canDrop ( ) ) return false ;
  return worldRemoveDrop ( )        ;
}

bool N::Universe::dragEnter(QDragEnterEvent * e)
{
  if ( ! canDrop ( ) ) return false ;
  return worldDragEnter ( e )       ;
}

bool N::Universe::dragMove(QDragMoveEvent * e)
{
  if ( ! canDrop ( ) ) return false ;
  return worldDragMove ( e )        ;
}

bool N::Universe::drop(QDropEvent * e)
{
  if ( ! canDrop ( ) ) return false ;
  return worldDrop ( e )            ;
}

bool N::Universe::worldRemoveDrop(void)
{
  UUIDs  U       = Performs                          ;
  bool   correct = false                             ;
  qint64 i       = 0                                 ;
  SUID   u                                           ;
  ////////////////////////////////////////////////////
  while ( ForChild ( u , i , U ) )                   {
    SceneObject * sox = (SceneObject *) Actors [ u ] ;
    if ( sox -> removeDrop ( ) )                     {
      correct = true                                 ;
    }                                                ;
  }                                                  ;
  ////////////////////////////////////////////////////
  return correct                                     ;
}

bool N::Universe::worldDragEnter(QDragEnterEvent * e)
{
  ControlPoint cr                                          ;
  Camera     * C  = (Camera *) CurrentCamera               ;
  ControlPoint FX                                          ;
  ControlPoint FY                                          ;
  QPointF      p  = e -> posF ( )                          ;
  UUIDs        U  = Performs                               ;
  qint64       i  = 0                                      ;
  SUID         u                                           ;
  //////////////////////////////////////////////////////////
  cr = p                                                   ;
  //////////////////////////////////////////////////////////
  if ( ! C -> EyePos ( Viewport , p  , FX ) ) return false ;
  if ( ! C -> At     (            cr , FY ) ) return false ;
  //////////////////////////////////////////////////////////
  while ( ForChild ( u , i , U ) )                         {
    SceneObject * sox = ChildOf<SceneObject> ( u )         ;
    if ( sox -> dragEnter ( FX , FY , e ) ) return true    ;
  }                                                        ;
  //////////////////////////////////////////////////////////
  return false                                             ;
}

bool N::Universe::worldDragMove(QDragMoveEvent * e)
{
  ControlPoint cr                                          ;
  Camera     * C  = (Camera *) CurrentCamera               ;
  ControlPoint FX                                          ;
  ControlPoint FY                                          ;
  QPointF      p  = e -> posF ( )                          ;
  UUIDs        U  = Performs                               ;
  qint64       i  = 0                                      ;
  SUID         u                                           ;
  //////////////////////////////////////////////////////////
  cr = p                                                   ;
  //////////////////////////////////////////////////////////
  if ( ! C -> EyePos ( Viewport , p  , FX ) ) return false ;
  if ( ! C -> At     (            cr , FY ) ) return false ;
  //////////////////////////////////////////////////////////
  while ( ForChild ( u , i , U ) )                         {
    SceneObject * sox = ChildOf<SceneObject> ( u )         ;
    if ( sox -> dragMove ( FX , FY , e ) ) return true     ;
  }                                                        ;
  //////////////////////////////////////////////////////////
  return false                                             ;
}

bool N::Universe::worldDrop(QDropEvent * e)
{
  ControlPoint cr                                          ;
  Camera     * C  = (Camera *) CurrentCamera               ;
  ControlPoint FX                                          ;
  ControlPoint FY                                          ;
  QPointF      p  = e -> posF ( )                          ;
  UUIDs        U  = Performs                               ;
  qint64       i  = 0                                      ;
  SUID         u                                           ;
  //////////////////////////////////////////////////////////
  cr = p                                                   ;
  //////////////////////////////////////////////////////////
  if ( ! C -> EyePos ( Viewport , p  , FX ) ) return false ;
  if ( ! C -> At     (            cr , FY ) ) return false ;
  //////////////////////////////////////////////////////////
  while ( ForChild ( u , i , U ) )                         {
    SceneObject * sox = ChildOf<SceneObject> ( u )         ;
    if ( sox -> drop ( FX , FY , e ) ) return true         ;
  }                                                        ;
  //////////////////////////////////////////////////////////
  return false                                             ;
}

bool N::Universe::LocalDoubleClick(QWidget * w,QMouseEvent * e)
{
  if ( Performs . count ( ) <= 0 ) return false               ;
  Universe::mouseDoubleClickEvent ( w , e )                   ;
  /////////////////////////////////////////////////////////////
  UUIDs U = Intersected ( Users [ MouseDClick ] )             ;
  bool  b = ( U . count ( ) > 0 )                             ;
  if ( ! b ) return false                                     ;
  /////////////////////////////////////////////////////////////
  ControlPoint press  = Users [ MouseDClick ]                 ;
  Camera     * C      = (Camera *) CurrentCamera              ;
  QPointF      p      = e -> pos ( )                          ;
  bool         meet   = true                                  ;
  ControlPoint FX                                             ;
  ControlPoint FY                                             ;
  /////////////////////////////////////////////////////////////
  if ( ! C -> EyePos ( Viewport , p     , FX ) ) meet = false ;
  if ( ! C -> At     (            press , FY ) ) meet = false ;
  if ( ! meet                                  ) return false ;
  /////////////////////////////////////////////////////////////
  ControlPoint AT                                             ;
  SUID         u                                              ;
  if ( ! isMouseGrabbed ( u ) ) u = Nearest ( FX , FY , AT )  ;
  if ( u <= 0 ) return false                                  ;
  /////////////////////////////////////////////////////////////
  SceneObject * sox = ChildOf<SceneObject> ( u )              ;
  if ( IsNull ( sox ) ) return false                          ;
  return sox -> MouseClicked ( FX , FY , Activations )        ;
}

bool N::Universe::LocalPressed(QWidget * w,QMouseEvent * e)
{
  if ( Performs . count ( ) <= 0 ) return false               ;
  /////////////////////////////////////////////////////////////
  Universe::mousePressEvent ( w , e )                         ;
  UUIDs U = Intersected ( Users [ MousePress ] )              ;
  bool  b = ( U . count ( ) > 0 )                             ;
  if ( ! b ) return false                                     ;
  /////////////////////////////////////////////////////////////
  ControlPoint press  = Users[MousePress]                     ;
  Camera     * C      = (Camera *)CurrentCamera               ;
  ControlPoint FX                                             ;
  ControlPoint FY                                             ;
  QPointF      p      = e -> pos ( )                          ;
  bool         meet   = true                                  ;
  if ( ! C -> EyePos ( Viewport , p     , FX ) ) meet = false ;
  if ( ! C -> At     (            press , FY ) ) meet = false ;
  if ( ! meet )                                               {
    return false                                              ;
  }                                                           ;
  /////////////////////////////////////////////////////////////
  ControlPoint AT                                             ;
  SUID         u                                              ;
  qint64       i = 0                                          ;
  if ( ! isMouseGrabbed ( u ) ) u = Nearest ( FX , FY , AT )  ;
  if ( u <= 0 )                                               {
    return ChildPressed ( FX , FY , Activations )             ;
  }                                                           ;
  /////////////////////////////////////////////////////////////
  SceneObject * sox = ChildOf<SceneObject> ( u )              ;
  if ( sox -> MouseVector ( FX , FY , Activations ) )         {
    if ( sox -> isFocus ( ) )                                 {
      SUID z = u                                              ;
      i = 0                                                   ;
      while ( ForChild ( u , i , U ) ) if ( u != z )          {
        SceneObject * sox = ChildOf<SceneObject> ( u )        ;
        sox -> setFocus ( false )                             ;
      }                                                       ;
    }                                                         ;
    return true                                               ;
  }                                                           ;
  /////////////////////////////////////////////////////////////
  return ChildPressed ( FX , FY , Activations )               ;
}

bool N::Universe::LocalReleased(QWidget * w,QMouseEvent * e)
{
  if ( Performs . count ( ) <= 0 ) return false               ;
  Universe::mouseReleaseEvent ( w , e )                       ;
  UUIDs U = Intersected ( Users [ MouseRelease ] )            ;
  bool  b = ( U . count ( ) > 0 )                             ;
  if ( ! b ) return false                                     ;
  /////////////////////////////////////////////////////////////
  ControlPoint press  = Users [ MouseRelease ]                ;
  Camera     * C      = (Camera *)CurrentCamera               ;
  ControlPoint FX                                             ;
  ControlPoint FY                                             ;
  QPointF      p      = e -> pos ( )                          ;
  if ( ! C -> EyePos ( Viewport , p     , FX ) ) return false ;
  if ( ! C -> At     (            press , FY ) ) return false ;
  /////////////////////////////////////////////////////////////
  ControlPoint AT                                             ;
  SUID         u                                              ;
  if ( ! isMouseGrabbed ( u ) ) u = Nearest ( FX , FY , AT )  ;
  if ( u <= 0 )                                               {
    return ChildReleased ( FX , FY , Activations )            ;
  }                                                           ;
  /////////////////////////////////////////////////////////////
  SceneObject * sox = ChildOf<SceneObject> ( u )              ;
  if ( sox -> MouseVector ( FX , FY , Activations ) )         {
    return true                                               ;
  }                                                           ;
  /////////////////////////////////////////////////////////////
  return ChildReleased ( FX , FY , Activations )              ;
}

bool N::Universe::LocalHover(QWidget * w,QMouseEvent * e)
{
  if ( Performs . count ( ) <= 0   ) return false ;
  Universe::mouseMoveEvent ( w , e )              ;
  LocalHovering            ( w , e )              ;
  if ( ! isActivated ( MouseLeft ) ) return false ;
  ControlPoint cursor = Users [ MouseMove  ]      ;
  ControlPoint press  = Users [ MousePress ]      ;
  Camera * C = (Camera *) CurrentCamera           ;
  if ( NULL == C ) return false                   ;
  ControlPoint FX                                 ;
  ControlPoint FY                                 ;
  if ( ! C -> At ( cursor , FX )   ) return false ;
  if ( ! C -> At ( press  , FY )   ) return false ;
  return Moving ( FY , FX )                       ;
}

bool N::Universe::LocalHovering(QWidget * w,QMouseEvent * e)
{
  if ( ! Value ( "Hover" ) . toBool ( )     ) return false    ;
  ControlPoint cr = Users [ MouseMove ]                       ;
  Camera     * C  = (Camera *) CurrentCamera                  ;
  ControlPoint FX                                             ;
  ControlPoint FY                                             ;
  QPointF      p  = e -> pos ( )                              ;
  UUIDs        U  = Performs                                  ;
  /////////////////////////////////////////////////////////////
  if ( ! C -> EyePos ( Viewport , p  , FX ) ) return false    ;
  if ( ! C -> At     (            cr , FY ) ) return false    ;
  /////////////////////////////////////////////////////////////
  SUID   u                                                    ;
  qint64 i = 0                                                ;
  if ( isMouseGrabbed ( u ) )                                 {
    SceneObject * sox = ChildOf<SceneObject> ( u )            ;
    if ( sox -> MouseHover ( FX , FY , Activations ) )        {
      return true                                             ;
    }                                                         ;
    ///////////////////////////////////////////////////////////
  }                                                           ;
  /////////////////////////////////////////////////////////////
  while ( ForChild ( u , i , U ) )                            {
    SceneObject * sox = ChildOf<SceneObject> ( u )            ;
    if ( sox -> hasFunction ( "Hover" ) )                     {
      sox -> MouseHover ( FX , FY , Activations )             ;
    }                                                         ;
  }                                                           ;
  /////////////////////////////////////////////////////////////
  return true                                                 ;
}

bool N::Universe::Moving(ControlPoint & from,ControlPoint & to)
{
  return false ;
}

bool N::Universe::LocalMoving(ControlPoint & From,ControlPoint & To)
{
  if ( Performs . count ( ) <= 0 ) return false              ;
  ////////////////////////////////////////////////////////////
  Camera * C = (Camera *) CurrentCamera                      ;
  if ( NULL == C ) return false                              ;
  ////////////////////////////////////////////////////////////
  ControlPoint FX1                                           ;
  ControlPoint FX2                                           ;
  ControlPoint FY1                                           ;
  ControlPoint FY2                                           ;
  QPointF      p1                                            ;
  QPointF      p2                                            ;
  ////////////////////////////////////////////////////////////
  p1 . setX ( From . x )                                     ;
  p1 . setY ( From . y )                                     ;
  p2 . setX ( To   . x )                                     ;
  p2 . setY ( To   . y )                                     ;
  ////////////////////////////////////////////////////////////
  if ( ! C -> EyePos ( Viewport , p1 , FX1 ) ) return false  ;
  if ( ! C -> EyePos ( Viewport , p2 , FX2 ) ) return false  ;
  if ( ! C -> At     ( From          , FY1 ) ) return false  ;
  if ( ! C -> At     ( To            , FY2 ) ) return false  ;
  ////////////////////////////////////////////////////////////
  UUIDs  U = Performs                                        ;
  SUID   u                                                   ;
  qint64 i = 0                                               ;
  while ( ForChild ( u , i , U ) )                           {
    SceneObject * sox = ChildOf<SceneObject> ( u )           ;
    sox -> MouseMove ( FX1 , FY1 , FX2 , FY2 , Activations ) ;
  }                                                          ;
  ////////////////////////////////////////////////////////////
  return false                                               ;
}

bool N::Universe::ChildPressed (
       ControlPoint & From     ,
       ControlPoint & To       ,
       BMAPs        & buttons  )
{
  UUIDs  U         = Performs                           ;
  SUID   u                                              ;
  qint64 i         = 0                                  ;
  bool   processed = false                              ;
  bool   focused   = false                              ;
  while ( ForChild ( u , i , U ) )                      {
    SceneObject * sox = ChildOf<SceneObject> ( u )      ;
    if ( ! processed )                                  {
      if ( sox -> MouseVector ( From , To , buttons ) ) {
        processed = true                                ;
        focused   = sox -> isFocus ( )                  ;
      }                                                 ;
    } else                                              {
      if ( focused ) sox -> setFocus ( false )          ;
    }                                                   ;
  }                                                     ;
  return processed                                      ;
}

bool N::Universe::ChildReleased (
       ControlPoint & From      ,
       ControlPoint & To        ,
       BMAPs        & buttons   )
{
  UUIDs  U = Performs                              ;
  SUID   u                                         ;
  qint64 i = 0                                     ;
  while ( ForChild ( u , i , U ) )                 {
    SceneObject * sox = ChildOf<SceneObject> ( u ) ;
    sox -> MouseVector ( From , To , buttons )     ;
  }                                                ;
  return true                                      ;
}

bool N::Universe::isMouseGrabbed(SUID & r)
{
  UUIDs  U = Performs                              ;
  SUID   u                                         ;
  qint64 i = 0                                     ;
  while ( ForChild ( u , i , U ) )                 {
    SceneObject * sox = ChildOf<SceneObject> ( u ) ;
    if ( sox -> isMouseGrabbed ( ) )               {
      r = u                                        ;
      return true                                  ;
    }                                              ;
  }                                                ;
  return false                                     ;
}

void N::Universe::Relocation(QRectF viewport)
{
  Viewport = viewport                                 ;
  nDropOut ( IsNull ( CurrentCamera ) )               ;
  QRectF W = viewport                                 ;
  QRectF V ( 0 , 0 , W . width ( ) , W . height ( ) ) ;
  CurrentCamera -> assignViewport ( V )               ;
}

void N::Universe::clear(void)
{
  LockAt                ( "Flush" ) ;
  CurrentCamera = NULL              ;
  Illuminates   . clear (         ) ;
  Performs      . clear (         ) ;
  oglenables    . clear (         ) ;
  Cameras       . clear (         ) ;
  Actors        . clear (         ) ;
  Lights        . clear (         ) ;
  UnlockAt              ( "Flush" ) ;
}

void N::Universe::FlushCamera(void)
{
  LockAt   ( "Cameras" )           ;
  if ( NotNull ( CurrentCamera ) ) {
    CurrentCamera -> setCamera ( ) ;
    CurrentCamera -> Transform ( ) ;
  }                                ;
  Transform                    ( ) ;
  UnlockAt ( "Cameras" )           ;
}

void N::Universe::PlaceLights(void)
{
  LockAt   ( "Lights" )                                                ;
  //////////////////////////////////////////////////////////////////////
  if ( Illuminates . count ( ) > 0 )                                   {
    UUIDs U   = Illuminates                                            ;
    SUID  u                                                            ;
    int   lid = 0                                                      ;
    foreach ( u , U           )  if ( Lights . contains ( u ) )        {
      if    ( lid < MaxLights )                                        {
        GLenum LID = (GLenum) ( lid + GL_LIGHT0 )                      ;
        Lights [ u ] -> lightId = LID                                  ;
        Lights [ u ] -> Enable ( )                                     ;
        Lights [ u ] -> Place  ( )                                     ;
      }                                                                ;
      lid++                                                            ;
    }                                                                  ;
    ////////////////////////////////////////////////////////////////////
    QtGL::Enable        ( GL_LIGHTING                                ) ;
    QtGL::ColorMaterial ( GL_FRONT_AND_BACK , GL_AMBIENT_AND_DIFFUSE ) ;
    QtGL::Enable        ( GL_COLOR_MATERIAL                          ) ;
    QtGL::LightModelf   ( GL_LIGHT_MODEL_LOCAL_VIEWER , GL_TRUE      ) ;
    QtGL::LightModelf   ( GL_LIGHT_MODEL_TWO_SIDE     , GL_FALSE     ) ;
  } else                                                               {
    QtGL::Disable       ( GL_LIGHTING                                ) ;
    QtGL::Disable       ( GL_COLOR_MATERIAL                          ) ;
  }                                                                    ;
  //////////////////////////////////////////////////////////////////////
  UnlockAt ( "Lights" )                                                ;
}

void N::Universe::RenderCameras(void)
{
  UUIDs U = Cameras . keys ( )       ;
  SUID  u                            ;
  if ( U . count ( ) <= 0 ) return   ;
  ////////////////////////////////////
  LockAt   ( "Cameras" )             ;
  ////////////////////////////////////
  foreach ( u , U )                  {
    Camera * camera = Cameras [ u ]  ;
    if   ( camera != CurrentCamera ) {
      if ( camera -> asObject      ) {
        camera -> Render ( )         ;
      }                              ;
    }                                ;
  }                                  ;
  ////////////////////////////////////
  UnlockAt ( "Cameras" )             ;
}

void N::Universe::RenderLights(void)
{
  UUIDs U = Lights  . keys ( )     ;
  SUID  u                          ;
  if ( U . count ( ) <= 0 ) return ;
  //////////////////////////////////
  LockAt   ( "Lights" )            ;
  //////////////////////////////////
  foreach ( u , U )                {
    Light * light = Lights [ u ]   ;
    if ( light -> asObject )       {
      light -> Render ( )          ;
    }                              ;
  }                                ;
  //////////////////////////////////
  UnlockAt ( "Lights" )            ;
}

void N::Universe::RenderActors(void)
{
  if ( Performs . count ( ) <= 0 ) return          ;
  //////////////////////////////////////////////////
  UUIDs  U = Performs                              ;
  SUID   u                                         ;
  qint64 i = 0                                     ;
  //////////////////////////////////////////////////
  LockAt   ( "Actors" )                            ;
  //////////////////////////////////////////////////
  while ( ForChild ( u , i , U ) )                 {
    SceneObject * nso = ChildOf<SceneObject> ( u ) ;
    if ( NotNull ( CurrentCamera ) )               {
      nso -> render ( *CurrentCamera , matrices )  ;
    } else                                         {
      nso -> render (                           )  ;
    }                                              ;
  }                                                ;
  //////////////////////////////////////////////////
  UnlockAt ( "Actors" )                            ;
}

void N::Universe::Flush(void)
{
  LockAt           ( "Flush" ) ;
  setProperties    (         ) ;
  QtGL::PushMatrix (         ) ;
  FlushCamera      (         ) ;
  PlaceLights      (         ) ;
  RenderLights     (         ) ;
  RenderCameras    (         ) ;
  RenderActors     (         ) ;
  QtGL::PopMatrix  (         ) ;
  UnlockAt         ( "Flush" ) ;
}

int N::Universe::RunnerRemoval(void * data,void * dispatcher,QVariants & values)
{
  if ( IsNull ( data       ) ) return -1                     ;
  if ( IsNull ( dispatcher ) ) return -1                     ;
  ////////////////////////////////////////////////////////////
  PathRunner * pr = (PathRunner *) data                      ;
  Universe   * uv = (Universe   *) dispatcher                ;
  ////////////////////////////////////////////////////////////
  if ( ! uv -> Runners . contains ( pr -> uuid ) ) return -1 ;
  ////////////////////////////////////////////////////////////
  uv -> Runners . take ( pr -> uuid )                        ;
  delete pr                                                  ;
  ////////////////////////////////////////////////////////////
  return 0                                                   ;
}

bool N::Universe::CommandRunners(int after)
{
  if ( Runners . count ( ) <= 0 ) return false  ;
  bool  c = false                               ;
  UUIDs U = Runners . keys ( )                  ;
  SUID  u                                       ;
  foreach ( u , U )                             {
    if ( Runners [ u ] -> step ( after ) >= 0 ) {
      if ( Runners [ u ] -> execute ( ) )       {
        c = true                                ;
      }                                         ;
    }                                           ;
  }                                             ;
  return c                                      ;
}

bool N::Universe::Animation(void)
{
  UUIDs U = Actors . keys ( )                           ;
  SUID  u                                               ;
  bool  animation = false                               ;
  ///////////////////////////////////////////////////////
  LockAt   ( "Animation" )                              ;
  ///////////////////////////////////////////////////////
  if ( CommandRunners ( 0 ) ) animation = true          ;
  foreach ( u , U )                                     {
    SceneObject * soj = ChildOf<SceneObject> ( u )      ;
    if ( NULL != soj )                                  {
      if ( soj -> Animation ( ) ) animation = true      ;
    }                                                   ;
  }                                                     ;
  if ( CommandRunners ( 1 ) ) animation = true          ;
  ///////////////////////////////////////////////////////
  UnlockAt ( "Animation" )                              ;
  ///////////////////////////////////////////////////////
  return animation                                      ;
}

void N::Universe::setActivate(int Id,bool enable)
{
  Activations [ Id ] = enable ;
}

bool N::Universe::isActivated(enum ActivationID Id)
{
  nKickOut ( IsNull(CurrentCamera) , false ) ;
  return Activations [ Id ]                  ;
}

bool N::Universe::Menu(QWidget * widget,QPointF pos,UUIDs & U)
{
  if ( isLocked ( "UsingActors" ) ) return false              ;
  Camera     * C      = (Camera *) CurrentCamera              ;
  if ( NULL == C                               ) return false ;
  ControlPoint press                                          ;
  ControlPoint FX                                             ;
  ControlPoint FY                                             ;
  /////////////////////////////////////////////////////////////
  press = pos                                                 ;
  if ( ! C -> EyePos ( Viewport , pos   , FX ) ) return false ;
  if ( ! C -> At     (            press , FY ) ) return false ;
  /////////////////////////////////////////////////////////////
  ControlPoint At                                             ;
  SUID         u = Nearest ( FX , FY , At )                   ;
  if ( u >= 0 )                                               {
    SceneObject * sox = ChildOf<SceneObject> ( u )            ;
    if ( sox -> Variables . contains ( "Menu" ) )             {
      bool isMenu = sox -> Value ( "Menu" ) . toBool ( )      ;
      if ( isMenu )                                           {
        if ( sox -> Menu ( widget , pos , FX , FY ) )         {
          return true                                         ;
        }                                                     ;
      }                                                       ;
    }                                                         ;
  }                                                           ;
  /////////////////////////////////////////////////////////////
  qint64 i = 0                                                ;
  while ( ForChild ( u , i , U ) )                            {
    SceneObject * sox = ChildOf<SceneObject> ( u )            ;
    if ( sox -> Variables . contains ( "Menu" ) )             {
      if ( sox -> Value ( "Menu" ) . toBool ( ) )             {
        if ( sox -> Menu ( widget , pos , FX , FY ) )         {
          return true                                         ;
        }                                                     ;
      }                                                       ;
    }                                                         ;
  }                                                           ;
  /////////////////////////////////////////////////////////////
  return false                                                ;
}

bool N::Universe::Menu(QWidget * widget,QPointF pos)
{
  return false ;
}

bool N::Universe::hasDrag(void)
{
  UUIDs  U = Performs                              ;
  SUID   u                                         ;
  qint64 i = 0                                     ;
  while ( ForChild ( u , i , U ) )                 {
    SceneObject * sox = ChildOf<SceneObject> ( u ) ;
    if ( NotNull ( sox ) )                         {
      if ( sox -> hasDrag ( ) ) return true        ;
    }                                              ;
  }                                                ;
  return false                                     ;
}

bool N::Universe::doDrag(void)
{
  UUIDs  U = Performs                              ;
  SUID   u                                         ;
  qint64 i = 0                                     ;
  while ( ForChild ( u , i , U ) )                 {
    SceneObject * sox = ChildOf<SceneObject> ( u ) ;
    if ( NotNull ( sox ) )                         {
      if ( sox -> hasDrag ( ) )                    {
        return sox -> doDrag ( )                   ;
      }                                            ;
    }                                              ;
  }                                                ;
  return false                                     ;
}

UUIDs N::Universe::Intersected(ControlPoint pos)
{
  UUIDs    U                                               ;
  Camera * C = (Camera *)CurrentCamera                     ;
  if ( IsNull ( C ) ) return U                             ;
  QPointF      cursor                                      ;
  ControlPoint FX                                          ;
  ControlPoint FY                                          ;
  cursor . setX ( pos . x )                                ;
  cursor . setX ( pos . y )                                ;
  if ( ! C -> EyePos ( Viewport , cursor , FX ) ) return U ;
  if ( ! C -> At     (            pos    , FY ) ) return U ;
  return Intersects  ( FX , FY                  )          ;
}

UUIDs N::Universe::Intersected(QPointF pos)
{
  ControlPoint p           ;
  p = pos                  ;
  return Intersected ( p ) ;
}

UUIDs N::Universe::Intersected(QPoint pos)
{
  QPointF p = pos          ;
  return Intersected ( p ) ;
}

UUIDs N::Universe::Intersects(ControlPoint p1,ControlPoint p2)
{
  UUIDs U                                          ;
  if ( Performs . count ( ) <= 0 ) return U        ;
  UUIDs        P = Performs                        ;
  ControlPoint P1                                  ;
  ControlPoint P2                                  ;
  SUID         u                                   ;
  qint64       i = 0                               ;
  P1     = p1                                      ;
  P2     = p2                                      ;
  P1 . t = 1                                       ;
  P2 . t = 1                                       ;
  while ( ForChild ( u , i , P ) )                 {
    SceneObject * sox = ChildOf<SceneObject> ( u ) ;
    if ( sox -> intersects ( P1 , P2 ) )           {
      U << u                                       ;
    }                                              ;
  }                                                ;
  //////////////////////////////////////////////////
  return U                                         ;
}

SUID N::Universe::Nearest(QPoint pos,ControlPoint & At)
{
  QPointF p = pos           ;
  return Nearest ( p , At ) ;
}

SUID N::Universe::Nearest(QPointF pos,ControlPoint & At)
{
  ControlPoint p            ;
  p = pos                   ;
  return Nearest ( p , At ) ;
}

SUID N::Universe::Nearest(ControlPoint pos,ControlPoint & At)
{
  Camera * C = (Camera *) CurrentCamera                    ;
  if ( IsNull ( C ) ) return 0                             ;
  QPointF      cursor                                      ;
  ControlPoint FX                                          ;
  ControlPoint FY                                          ;
  cursor . setX ( pos . x )                                ;
  cursor . setX ( pos . y )                                ;
  if ( ! C -> EyePos ( Viewport , cursor , FX ) ) return 0 ;
  if ( ! C -> At     (            pos    , FY ) ) return 0 ;
  return Nearest     ( FX , FY  , At            )          ;
}

SUID N::Universe::Nearest  (
       ControlPoint & From ,
       ControlPoint & To   ,
       ControlPoint & At   )
{
  if ( Performs . count ( ) <= 0 ) return 0        ;
  UUIDs             P = Performs                   ;
  ControlPoint      P1                             ;
  ControlPoint      P2                             ;
  ControlPoint      at                             ;
  SUID              u                              ;
  SUID              n = 0                          ;
  QMap<SUID,double> D                              ;
  double            z = 100000000000000000000000.0 ;
  double            d                              ;
  qint64            i = 0                          ;
  while ( ForChild ( u , i , P ) )                 {
    SceneObject * sox = ChildOf<SceneObject> ( u ) ;
    P1     = From                                  ;
    P2     = To                                    ;
    P1 . t = 1                                     ;
    P2 . t = 1                                     ;
    P1     = sox -> matrices [ 2 ] * P1            ;
    P2     = sox -> matrices [ 2 ] * P2            ;
    if ( sox -> Spot ( P1 , P2 , at ) )            {
      at -= P1                                     ;
      d   = at . length ( )                        ;
      D [ u ] = d                                  ;
      if ( d < z )                                 {
        n  = u                                     ;
        z  = d                                     ;
        At = at                                    ;
      }                                            ;
    }                                              ;
  }                                                ;
  return n                                         ;
}

void N::Universe::setCursor(const QCursor & cursor)
{
  if ( IsNull ( gui ) ) return ;
  gui -> setCursor ( cursor )  ;
}
