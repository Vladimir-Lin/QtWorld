#include <qtworld.h>

#if   defined(Q_OS_ANDROID)
#elif defined(Q_OS_IOS)
#define GLdouble GLfloat
#else
#endif

N::SceneObject:: SceneObject ( void              )
               : SceneNode   ( Scene::Object     )
               , frontBack   ( GL_FRONT_AND_BACK )
{
  addType ( Scene::Abstract )               ;
  name = "Object"                        ;
  setVariable ( "AutoDeletion" , true  ) ;
  setVariable ( "Focus"        , false ) ;
  setVariable ( "Hold"         , false ) ;
  setVariable ( "Menu"         , false ) ;
  setVariable ( "FollowWindow" , false ) ;
  setVariable ( "ControlPanel" , false ) ;
  setVariable ( "Collision"    , true  ) ;
  setVariable ( "Mouse"        , false ) ;
  setVariable ( "Grab"         , false ) ;
  setVariable ( "Keyboard"     , false ) ;
  setVariable ( "AllowDrag"    , false ) ;
  setVariable ( "AcceptDrp"    , false ) ;
  setVariable ( "Runner"       , true  ) ;
  setVariable ( "Scale"        , 90    ) ;
  setVariable ( "Margin"       , 3     ) ;
  setVariable ( "Finalize"     , false ) ;
  setVariable ( "Inherit"      , false ) ;
  ////////////////////////////////////////
  setVariable ( "Top/Line"     , true  ) ;
  setVariable ( "Bottom/Line"  , true  ) ;
  setVariable ( "Left/Line"    , true  ) ;
  setVariable ( "Right/Line"   , true  ) ;
  setVariable ( "Top/Left"     , true  ) ;
  setVariable ( "Top/Right"    , true  ) ;
  setVariable ( "Bottom/Left"  , true  ) ;
  setVariable ( "Bottom/Right" , true  ) ;
}

N::SceneObject:: SceneObject ( SceneDestructor * des )
               : SceneNode   ( Scene::Object         )
               , frontBack   ( GL_FRONT_AND_BACK     )
{
  destructor = des                       ;
  JoinDestructor ( )                     ;
  name       = "Object"                  ;
  setVariable ( "AutoDeletion" , true  ) ;
  setVariable ( "Focus"        , false ) ;
  setVariable ( "Hold"         , false ) ;
  setVariable ( "Menu"         , false ) ;
  setVariable ( "FollowWindow" , false ) ;
  setVariable ( "ControlPanel" , false ) ;
  setVariable ( "Collision"    , true  ) ;
  setVariable ( "Mouse"        , false ) ;
  setVariable ( "Grab"         , false ) ;
  setVariable ( "Keyboard"     , false ) ;
  setVariable ( "AllowDrag"    , false ) ;
  setVariable ( "AcceptDrp"    , false ) ;
  setVariable ( "Runner"       , true  ) ;
  setVariable ( "Scale"        , 90    ) ;
  setVariable ( "Margin"       , 3     ) ;
  setVariable ( "Finalize"     , false ) ;
  setVariable ( "Inherit"      , true  ) ;
  ////////////////////////////////////////
  setVariable ( "Top/Line"     , true  ) ;
  setVariable ( "Bottom/Line"  , true  ) ;
  setVariable ( "Left/Line"    , true  ) ;
  setVariable ( "Right/Line"   , true  ) ;
  setVariable ( "Top/Left"     , true  ) ;
  setVariable ( "Top/Right"    , true  ) ;
  setVariable ( "Bottom/Left"  , true  ) ;
  setVariable ( "Bottom/Right" , true  ) ;
}

N::SceneObject::~SceneObject (void)
{
  AutoDeletion ( ) ;
}

bool N::SceneObject::ForUuid(SUID & u,qint64 & i,UUIDs & U)
{
  if ( Value ( "Finalize" ) . toBool ( ) ) return false ;
  if ( i >= U . count ( )                ) return false ;
  u = U [ i ]                                           ;
  i++                                                   ;
  return true                                           ;
}

bool N::SceneObject::ForChild(SUID & u,qint64 & i,UUIDs & U)
{
  if ( Value ( "Finalize" ) . toBool ( ) ) return false ;
  if ( i >= U . count ( )                ) return false ;
  SceneObject * sox = NULL                              ;
  while ( i < U . count ( ) )                           {
    u = U [ i ]                                         ;
    i++                                                 ;
    if ( children . contains ( u ) )                    {
      sox = children [ u ]                              ;
      if ( NotNull ( sox ) )                            {
        if ( Scene::Object == sox -> nodeType )         {
          return true                                   ;
        }                                               ;
      }                                                 ;
    }                                                   ;
  }                                                     ;
  return false                                          ;
}

void N::SceneObject::suicide(void)
{
  delete this ;
}

typedef struct        {
  N::SceneObject * me ;
} ClassStructure      ;

void N::SceneObject::ObjectPointer(QByteArray & pointer)
{
  pointer . resize ( sizeof(ClassStructure) )                 ;
  ClassStructure * cs = (ClassStructure *) pointer . data ( ) ;
  cs -> me = this                                             ;
}

void N::SceneObject::setCursor(const QCursor & cursor)
{
  if ( IsNull ( gui ) ) return ;
  gui -> setCursor ( cursor )  ;
}

bool N::SceneObject::FocusIn(void)
{
  return false ;
}

bool N::SceneObject::FocusOut(void)
{
  return false ;
}

QVariant N::SceneObject::inputMethodQuery(QWidget * w,Qt::InputMethodQuery query) const
{
  return QVariant ( ) ;
}

void N::SceneObject::inputMethodEvent(QWidget * w,QInputMethodEvent * e)
{
}

void N::SceneObject::keyPressEvent(QWidget * w,QKeyEvent * e)
{
}

void N::SceneObject::keyReleaseEvent(QWidget * w,QKeyEvent * e)
{
}

void N::SceneObject::enterEvent(QWidget * w,QEvent * e)
{
}

void N::SceneObject::leaveEvent(QWidget * w,QEvent * e)
{
}

N::SceneObject * N::SceneObject::EnabledObject (QString key)
{
  UUIDs  U = children . keys ( )                        ;
  SUID   u                                              ;
  qint64 i = 0                                          ;
  while ( ForChild ( u , i , U ) )                      {
    if ( children [ u ] -> Value ( key ) . toBool ( ) ) {
      return children [ u ]                             ;
    }                                                   ;
  }                                                     ;
  return NULL                                           ;
}

bool N::SceneObject::grabMouse(bool enabled)
{
  setVariable ( "Grab" , enabled )         ;
  if ( NotNull ( gui ) )                   {
    if ( enabled ) gui -> grabMouse    ( ) ;
              else gui -> releaseMouse ( ) ;
  }                                        ;
  return enabled                           ;
}

bool N::SceneObject::grabKeyboard(bool enabled)
{
  setVariable ( "Keyboard" , enabled )        ;
  if ( NotNull ( gui ) )                      {
    if ( enabled ) gui -> grabKeyboard    ( ) ;
              else gui -> releaseKeyboard ( ) ;
  }                                           ;
  return enabled                              ;
}

bool N::SceneObject::isMouseGrabbed(void)
{
  if ( Value ( "Grab" ) . toBool ( ) ) return true ;
  //////////////////////////////////////////////////
  UUIDs  U = children . keys ( )                   ;
  SUID   u                                         ;
  qint64 i = 0                                     ;
  while ( ForChild ( u , i , U ) )                 {
    if ( visibilities [ u ] )                      {
      SceneObject * sox = children [ u ]           ;
      if ( sox -> isMouseGrabbed ( ) ) return true ;
    }                                              ;
  }                                                ;
  return false                                     ;
}

bool N::SceneObject::isKeyboardGrabbed(void)
{
  if ( Value ( "Keyboard" ) . toBool ( ) ) return true ;
  //////////////////////////////////////////////////////
  UUIDs  U = children . keys ( )                       ;
  SUID   u                                             ;
  qint64 i = 0                                         ;
  while ( ForChild ( u , i , U ) )                     {
    SceneObject * sox = children [ u ]                 ;
    if ( sox -> isKeyboardGrabbed ( ) ) return true    ;
  }                                                    ;
  return false                                         ;
}

void N::SceneObject::AutoDeletion(void)
{
  if ( ! Variables . contains ( "AutoDeletion" )   ) return ;
  if ( ! Variables [ "AutoDeletion" ] . toBool ( ) ) return ;
  RemoveObjects ( 0 )                                       ;
}

void N::SceneObject::RemoveObjects(int what)
{
  QStringList S                                         ;
  QString     s                                         ;
  UUIDs       U                                         ;
  SUID        u                                         ;
  switch ( what )                                       {
    case   0                                            :
      for (int i = 101 ; i <= 113 ; i++ )               {
        RemoveObjects ( i )                             ;
      }                                                 ;
      for (int i = 201 ; i <= 205 ; i++ )               {
        RemoveObjects ( i )                             ;
      }                                                 ;
    break                                               ;
    case 101                                            :
      index        . clear ( )                          ;
    break                                               ;
    case 102                                            :
      points       . clear ( )                          ;
    break                                               ;
    case 103                                            :
      vectors      . clear ( )                          ;
    break                                               ;
    case 104                                            :
      colors       . clear ( )                          ;
    break                                               ;
    case 105                                            :
      texCoords    . clear ( )                          ;
    break                                               ;
    case 106                                            :
      segments     . clear ( )                          ;
    break                                               ;
    case 107                                            :
      faces        . clear ( )                          ;
    break                                               ;
    case 108                                            :
      tessllations . clear ( )                          ;
    break                                               ;
    case 109                                            :
      paraments    . clear ( )                          ;
    break                                               ;
    case 110                                            :
      pictures     . clear ( )                          ;
    break                                               ;
    case 111                                            :
      sequences    . clear ( )                          ;
    break                                               ;
    case 112                                            :
      behaviors    . clear ( )                          ;
    break                                               ;
    case 113                                            :
      materials    . clear ( )                          ;
    break                                               ;
    case 201                                            :
      U = images . keys ( )                             ;
      foreach ( u , U )                                 {
        if ( NULL != destructor )                       {
          QImage * is = images [ u ]                    ;
          if ( destructor -> images . contains ( is ) ) {
            (*destructor) -= is                         ;
            delete is                                   ;
          }                                             ;
        } else                                          {
          delete images [ u ]                           ;
        }                                               ;
      }                                                 ;
      images . clear ( )                                ;
    break                                               ;
    case 202                                            :
      U = textures . keys ( )                           ;
      foreach ( u , U )                                 {
        if ( NULL != destructor )                       {
          SceneTexture * st = textures [ u ]            ;
          if ( destructor -> nodes . contains ( st ) )  {
            (*destructor) -= st                         ;
            delete st                                   ;
          }                                             ;
        } else                                          {
          delete textures [ u ]                         ;
        }                                               ;
      }                                                 ;
      textures . clear ( )                              ;
    break                                               ;
    case 203                                            :
      U = SceneObject::children . keys ( )              ;
      foreach ( u , U )                                 {
        if ( NULL != destructor )                       {
          SceneObject * so                              ;
          so = SceneObject::children [ u ]              ;
          if ( destructor -> nodes . contains ( so ) )  {
            (*destructor) -= so                         ;
            delete so                                   ;
          }                                             ;
        } else                                          {
          delete SceneObject::children [ u ]            ;
        }                                               ;
      }                                                 ;
      SceneObject::children . clear ( )                 ;
    break                                               ;
    case 204                                            :
      U = runners . keys ( )                            ;
      foreach ( u , U )                                 {
        delete runners [ u ]                            ;
      }                                                 ;
      runners . clear ( )                               ;
    break                                               ;
    case 205                                            :
      S = userobjs . keys ( )                           ;
      foreach ( s , S ) if ( NULL != userobjs [ s ] )   {
        delete userobjs [ s ]                           ;
      }                                                 ;
      userobjs . clear ( )                              ;
    break                                               ;
  }                                                     ;
}

void N::SceneObject::clear(void)
{
  LockAt               ( "Operate" ) ;
  index        . clear (           ) ;
  points       . clear (           ) ;
  vectors      . clear (           ) ;
  colors       . clear (           ) ;
  texCoords    . clear (           ) ;
  segments     . clear (           ) ;
  faces        . clear (           ) ;
  tessllations . clear (           ) ;
  pictures     . clear (           ) ;
  sequences    . clear (           ) ;
  UnlockAt             ( "Operate" ) ;
}

void N::SceneObject::setDestructor(SceneDestructor * des)
{
  UUIDs U                           ;
  SUID  u                           ;
  ///////////////////////////////////
  destructor = des                  ;
  if ( NULL == des ) return         ;
  ///////////////////////////////////
  U          = images . keys ( )    ;
  foreach ( u , U )                 {
    (*destructor) += images   [ u ] ;
  }                                 ;
  ///////////////////////////////////
  U          = textures . keys ( )  ;
  foreach ( u , U )                 {
    (*destructor) += textures [ u ] ;
  }                                 ;
  ///////////////////////////////////
  U          = children . keys ( )  ;
  foreach ( u , U )                 {
    (*destructor) += children [ u ] ;
  }                                 ;
}

bool N::SceneObject::intersects(ControlPoint & p1,ControlPoint & p2)
{
  ControlPoint P1                                        ;
  ControlPoint P2                                        ;
  bool         collision                                 ;
  collision = SceneBox::intersects ( p1 , p2 , P1 , P2 ) ;
  if ( collision ) return true                           ;
  UUIDs  U = children . keys ( )                         ;
  SUID   u                                               ;
  qint64 i = 0                                           ;
  while ( ForChild ( u , i , U ) )                       {
    if ( children [ u ] -> intersects ( P1 , P2 ) )      {
      return true                                        ;
    }                                                    ;
  }                                                      ;
  return collision                                       ;
}

bool N::SceneObject::isFocus(void)
{
  return Value ( "Focus" ) . toBool ( ) ;
}

bool N::SceneObject::setFocus(bool focus)
{
  setVariable ( "Focus" , focus ) ;
  setVariable ( "Hover" , focus ) ;
  if ( focus )                    {
    FocusIn  ( )                  ;
  } else                          {
    FocusOut ( )                  ;
  }                               ;
  return focus                    ;
}

UUIDs N::SceneObject::list(QString type)
{
  UUIDs  U = children . keys ( )                               ;
  UUIDs  R                                                     ;
  SUID   u                                                     ;
  qint64 i = 0                                                 ;
  while ( ForChild ( u , i , U ) )                             {
    if ( type == children[u] -> Value("Type") . toString ( ) ) {
      R << u                                                   ;
    }                                                          ;
  }                                                            ;
  return R                                                     ;
}

bool N::SceneObject::xymSpot (
       ControlPoint & From   ,
       ControlPoint & To     ,
       ControlPoint & At     )
{
  ControlPoint V                          ;
  double       x                          ;
  double       y                          ;
  double       z                          ;
  double       n                          ;
  /////////////////////////////////////////
  V  = To                                 ;
  V -= From                               ;
  V  . normalize ( )                      ;
  /////////////////////////////////////////
  z  = V . z                              ;
  if ( z < 0         ) z = -z             ;
  if ( z < VerySmall ) return false       ;
  n  = maximum . z - From . z             ;
  n /= V    . z                           ;
  x  = V    . x                           ;
  x *= n                                  ;
  x += From . x                           ;
  y  = V    . y                           ;
  y *= n                                  ;
  y += From . y                           ;
  /////////////////////////////////////////
  At . x = x                              ;
  At . y = y                              ;
  At . z = maximum . z                    ;
  At . t = 1                              ;
  At . r = 0                              ;
  /////////////////////////////////////////
  if ( x < minimum . x ) return false     ;
  if ( x > maximum . x ) return false     ;
  if ( y < minimum . y ) return false     ;
  if ( y > maximum . y ) return false     ;
  /////////////////////////////////////////
  return true                             ;
}

QRectF N::SceneObject::BoxPlane(int plane)
{
  QRectF R                          ;
  switch ( plane )                  {
    case 0                          :
      R . setTop    ( minimum . y ) ;
      R . setBottom ( maximum . y ) ;
      R . setLeft   ( minimum . x ) ;
      R . setRight  ( maximum . x ) ;
    break                           ;
    case 1                          :
      R . setTop    ( minimum . z ) ;
      R . setBottom ( maximum . z ) ;
      R . setLeft   ( minimum . y ) ;
      R . setRight  ( maximum . y ) ;
    break                           ;
    case 2                          :
      R . setTop    ( minimum . x ) ;
      R . setBottom ( maximum . x ) ;
      R . setLeft   ( minimum . z ) ;
      R . setRight  ( maximum . z ) ;
    break                           ;
    default                         :
      R . setTop    ( maximum . y ) ;
      R . setBottom ( minimum . y ) ;
      R . setLeft   ( maximum . x ) ;
      R . setRight  ( minimum . x ) ;
    break                           ;
  }                                 ;
  return R                          ;
}

bool N::SceneObject::Within(QPointF & p)
{
  return false ;
}

bool N::SceneObject::CommandRunners(void)
{
  if ( runners . count ( ) <= 0 ) return false ;
  bool  c = false                              ;
  UUIDs U = runners . keys ( )                 ;
  SUID  u                                      ;
  foreach ( u , U )                            {
    if ( runners [ u ] -> step ( ) >= 0 )      {
      if ( runners [ u ] -> execute ( ) )      {
        c = true                               ;
      }                                        ;
    }                                          ;
  }                                            ;
  return true                                  ;
}

int N::SceneObject::addRunner(SUID u,PathRunner * runner)
{
  runners [ u ]  = runner    ;
  runner -> uuid = u         ;
  runner -> node = this      ;
  return runners . count ( ) ;
}

SUID N::SceneObject::appendRunner(SUID u,PathRunner * runner)
{
  while ( runners . contains ( u ) ) u++ ;
  runners [ u ]  = runner                ;
  runner -> uuid = u                     ;
  runner -> node = this                  ;
  return u                               ;
}

bool N::SceneObject::CommandRunners(int after)
{
  if ( ! Value ( "Runner" ) . toBool ( ) ) return false ;
  if ( runners . count ( ) <= 0          ) return false ;
  bool  c = false                                       ;
  UUIDs U = runners . keys ( )                          ;
  SUID  u                                               ;
  foreach ( u , U )                                     {
    if ( runners [ u ] -> step ( after ) >= 0 )         {
      if ( runners [ u ] -> execute ( ) )               {
        c = true                                        ;
      }                                                 ;
    }                                                   ;
  }                                                     ;
  return c                                              ;
}

bool N::SceneObject::Animation(void)
{
  UUIDs  U         = children . keys ( )           ;
  SUID   u                                         ;
  qint64 i         = 0                             ;
  bool   animation = false                         ;
  //////////////////////////////////////////////////
  LockAt   ( "Animation" )                         ;
  //////////////////////////////////////////////////
  if ( CommandRunners ( 0 ) ) animation = true     ;
  while ( ForChild ( u , i , U ) )                 {
    SceneObject * soj = ChildOf<SceneObject> ( u ) ;
    if ( soj -> Animation ( ) ) animation = true   ;
  }                                                ;
  if ( CommandRunners ( 1 ) ) animation = true     ;
  //////////////////////////////////////////////////
  UnlockAt ( "Animation" )                         ;
  //////////////////////////////////////////////////
  return animation                                 ;
}

int N::SceneObject::Last(void)
{
  int idx = 0                       ;
  for (int i=0;i<index.count();i++) {
    int v = index[i]                ;
    if (v>idx) idx = v              ;
  }                                 ;
  return idx + 1                    ;
}

int N::SceneObject::Last(int what)
{
  CUIDs Keys                       ;
  switch (what)                    {
    case Point                     :
      Keys = index                 ;
    break                          ;
    case Vector                    :
      Keys = vectors      . keys() ;
    break                          ;
    case Color                     :
      Keys = colors       . keys() ;
    break                          ;
    case Segment                   :
      Keys = segments     . keys() ;
    break                          ;
    case Face                      :
      Keys = faces        . keys() ;
    break                          ;
    case Tess                      :
      Keys = tessllations . keys() ;
    break                          ;
  }                                ;
  int idx = 0                      ;
  for (int i=0;i<Keys.count();i++) {
    int v = Keys[i]                ;
    if (v>idx) idx = v             ;
  }                                ;
  return idx + 1                   ;
}

void N::SceneObject::render(Camera & camera,QMap<int,Matrix> & m)
{
  if ( isLocked ( "Operate" ) ) return                   ;
  LockAt               ( "Operate" )                     ;
  UUIDs S = Sequential()                                 ;
  SUID  s                                                ;
  setProperties ( )                                      ;
  UUIDs M = materials.keys()                             ;
  if (M.count()>0)                                       {
    foreach (s,M)                                        {
      materials[s].Material((GLenum)frontBack,(GLenum)s) ;
    }                                                    ;
  }                                                      ;
  QtGL::PushMatrix     ( )                               ;
  Transform            ( )                               ;
  foreach (s,S)                                          {
    switch (s)                                           {
      case AssignColor                                   :
        assignColor    ( )                               ;
      break                                              ;
      case RenderPoints                                  :
        renderPoints   ( )                               ;
      break                                              ;
      case RenderColors                                  :
        renderColors   ( )                               ;
      break                                              ;
      case RenderSegments                                :
        renderSegments ( )                               ;
      break                                              ;
      case RenderCurves                                  :
        renderCurves   ( )                               ;
      break                                              ;
      case RenderFaces                                   :
        renderFaces    ( )                               ;
      break                                              ;
      case RenderSmooth                                  :
        renderSmooth   ( )                               ;
      break                                              ;
      case RenderSurfaces                                :
        renderSurfaces ( )                               ;
      break                                              ;
      case RenderGrids                                   :
        renderGrids    ( )                               ;
      break                                              ;
      case RenderTessllations                            :
        renderTessllations ( )                           ;
      break                                              ;
      case RenderBox                                     :
        renderBox      ( )                               ;
      break                                              ;
      case RenderTextures                                :
        renderTextures ( )                               ;
      break                                              ;
      case RenderCeramics                                :
        renderCeramics ( )                               ;
      break                                              ;
      case RenderPictures                                :
        renderPictures ( )                               ;
      break                                              ;
      case RenderVoxels                                  :
        renderVoxels   ( )                               ;
      break                                              ;
      case RenderChildren                                :
        renderChildren ( camera , m )                    ;
      break                                              ;
      case RenderUser                                    :
        renderUser     ( )                               ;
      break                                              ;
    }                                                    ;
  }                                                      ;
  QtGL::PopMatrix      ( )                               ;
  UnlockAt             ( "Operate" )                     ;
}

void N::SceneObject::render(void)
{
  if ( isLocked ( "Operate" ) ) return                   ;
  LockAt               ( "Operate" )                     ;
  UUIDs S = Sequential ( )                               ;
  SUID  s                                                ;
  setProperties        ( )                               ;
  UUIDs M = materials.keys()                             ;
  if (M.count()>0)                                       {
    foreach (s,M)                                        {
      materials[s].Material((GLenum)frontBack,(GLenum)s) ;
    }                                                    ;
  }                                                      ;
  QtGL::PushMatrix     ( )                               ;
  Transform            ( )                               ;
  foreach (s,S)                                          {
    switch (s)                                           {
      case AssignColor                                   :
        assignColor    ( )                               ;
      break                                              ;
      case RenderPoints                                  :
        renderPoints   ( )                               ;
      break                                              ;
      case RenderColors                                  :
        renderColors   ( )                               ;
      break                                              ;
      case RenderSegments                                :
        renderSegments ( )                               ;
      break                                              ;
      case RenderCurves                                  :
        renderCurves   ( )                               ;
      break                                              ;
      case RenderFaces                                   :
        renderFaces    ( )                               ;
      break                                              ;
      case RenderSmooth                                  :
        renderSmooth   ( )                               ;
      break                                              ;
      case RenderSurfaces                                :
        renderSurfaces ( )                               ;
      break                                              ;
      case RenderGrids                                   :
        renderGrids    ( )                               ;
      break                                              ;
      case RenderTessllations                            :
        renderTessllations ( )                           ;
      break                                              ;
      case RenderBox                                     :
        renderBox      ( )                               ;
      break                                              ;
      case RenderTextures                                :
        renderTextures ( )                               ;
      break                                              ;
      case RenderCeramics                                :
        renderCeramics ( )                               ;
      break                                              ;
      case RenderPictures                                :
        renderPictures ( )                               ;
      break                                              ;
      case RenderVoxels                                  :
        renderVoxels   ( )                               ;
      break                                              ;
      case RenderChildren                                :
        renderChildren ( )                               ;
      break                                              ;
      case RenderUser                                    :
        renderUser     ( )                               ;
      break                                              ;
    }                                                    ;
  }                                                      ;
  QtGL::PopMatrix      ( )                               ;
  UnlockAt             ( "Operate" )                     ;
}

void N::SceneObject::assignColor(void)
{
  if (colors.count()<=0) return                  ;
  CUIDs I = colors.keys()                        ;
  if (I.count()<=0) return                       ;
  int i = I[0]                                   ;
  if ( Variables . contains ( "DefaultColor" ) ) {
    i = Variables [ "DefaultColor" ] . toInt ( ) ;
  }                                              ;
  colors [ i ] . Color4d ( )                     ;
}

void N::SceneObject::renderPoints (void)
{
  QtGL::Points ( )                  ;
  for (int i=0;i<index.count();i++) {
    int v = index[i]                ;
    points[v].Vertex3d()            ;
  }                                 ;
  QtGL::End    ( )                  ;
}

void N::SceneObject::renderColors (void)
{
  QtGL::Points ( )                  ;
  for (int i=0;i<index.count();i++) {
    int v = index[i]                ;
    colors [ v ] . Color4d  ( )     ;
    points [ v ] . Vertex3d ( )     ;
  }                                 ;
  QtGL::End    ( )                  ;
}

void N::SceneObject::renderSegments(void)
{
  QList<int> K = segments.keys() ;
  int        k                   ;
  foreach (k,K) renderSegment(k) ;
}

void N::SceneObject::renderSegment(int idx)
{
  double lw = segments[idx].lineWidth      ;
  if (lw>0) QtGL::LineWidth ( lw )         ;
  switch (segments[idx].type)              {
    case SceneSegment::Lines               :
      QtGL::Lines  ( )                     ;
    break                                  ;
    case SceneSegment::Strip               :
      QtGL::Strips ( )                     ;
    break                                  ;
    case SceneSegment::Loop                :
      QtGL::Loop   ( )                     ;
    break                                  ;
  }                                        ;
  int  pts   = segments[idx].points()      ;
  bool color = segments[idx].pointColor    ;
  for (int i=0;i<pts;i++)                  {
    int id = segments[idx][i]              ;
    if (color                             &&
        segments[idx].colors.contains(id)) {
      int cid = segments[idx].colors[id]   ;
      if (colors.contains(cid))            {
        colors[cid] . Color4d()            ;
      }                                    ;
    }                                      ;
    points[id] . Vertex3d ( )              ;
  }                                        ;
  QtGL::End ( )                            ;
}

void N::SceneObject::renderCurves(void)
{
  QList<int> K = segments . keys (   ) ;
  int        k                         ;
  foreach ( k , K ) renderCurve  ( k ) ;
}

void N::SceneObject::renderCurve(int idx)
{
  int    pts   = segments [ idx ] . points ( )                ;
  bool   color = segments [ idx ] . pointColor                ;
  double lw    = segments [ idx ] . lineWidth                 ;
  int    ltype = segments [ idx ] . type                      ;
  nDropOut ( pts   <= 2                   )                   ;
  nDropOut ( ltype == SceneSegment::Lines )                   ;
  /////////////////////////////////////////////////////////////
  GLdouble * pv = NULL                                        ;
  GLdouble * cv = NULL                                        ;
  int        ct = 0                                           ;
  /////////////////////////////////////////////////////////////
  switch ( ltype )                                            {
    case SceneSegment::Strip                                  :
      ct   = pts                                              ;
      pv   = new GLdouble [ ct * 3 ]                          ;
      if ( color )                                            {
        cv = new GLdouble [ ct * 4 ]                          ;
      }                                                       ;
      for (int i = 0 ; i < pts ; i++ )                        {
        int id = segments [ idx ] [ i ]                       ;
        int ps = i * 3                                        ;
        int cs = i * 4                                        ;
        if ( color )                                          {
          bool found = false                                  ;
          if ( segments [ idx ] . colors . contains ( id ) )  {
            int cid = segments [ idx ] . colors [ id ]        ;
            if ( colors . contains ( cid ) )                  {
              cv [ cs     ] = colors [ cid ] . x              ;
              cv [ cs + 1 ] = colors [ cid ] . y              ;
              cv [ cs + 2 ] = colors [ cid ] . z              ;
              cv [ cs + 3 ] = colors [ cid ] . t              ;
              found = true                                    ;
            }                                                 ;
          }                                                   ;
          if ( ! found )                                      {
            cv [ cs     ] = 1.0                               ;
            cv [ cs + 1 ] = 1.0                               ;
            cv [ cs + 2 ] = 1.0                               ;
            cv [ cs + 3 ] = 1.0                               ;
          }                                                   ;
        }                                                     ;
        pv [ ps     ] = points [ id ] . x                     ;
        pv [ ps + 1 ] = points [ id ] . y                     ;
        pv [ ps + 2 ] = points [ id ] . z                     ;
      }                                                       ;
    break                                                     ;
    case SceneSegment::Loop                                   :
      ct   = pts + 1                                          ;
      pv   = new GLdouble [ ct * 3 ]                          ;
      if ( color )                                            {
        cv = new GLdouble [ ct * 4 ]                          ;
      }                                                       ;
      for (int i = 0 ; i <= pts ; i++ )                       {
        int id = segments [ idx ] [ i % pts ]                 ;
        int ps = i * 3                                        ;
        int cs = i * 4                                        ;
        if ( color )                                          {
          bool found = false                                  ;
          if ( segments [ idx ] . colors . contains ( id ) )  {
            int cid = segments [ idx ] . colors [ id ]        ;
            if ( colors . contains ( cid ) )                  {
              cv [ cs     ] = colors [ cid ] . x              ;
              cv [ cs + 1 ] = colors [ cid ] . y              ;
              cv [ cs + 2 ] = colors [ cid ] . z              ;
              cv [ cs + 3 ] = colors [ cid ] . t              ;
              found = true                                    ;
            }                                                 ;
          }                                                   ;
          if ( ! found )                                      {
            cv [ cs     ] = 1.0                               ;
            cv [ cs + 1 ] = 1.0                               ;
            cv [ cs + 2 ] = 1.0                               ;
            cv [ cs + 3 ] = 1.0                               ;
          }                                                   ;
        }                                                     ;
        pv [ ps     ] = points [ id ] . x                     ;
        pv [ ps + 1 ] = points [ id ] . y                     ;
        pv [ ps + 2 ] = points [ id ] . z                     ;
      }                                                       ;
    break                                                     ;
  }                                                           ;
  /////////////////////////////////////////////////////////////
  if ( ct <= 2 )                                              {
    nDeleteArray ( pv )                                       ;
    nDeleteArray ( cv )                                       ;
    return                                                    ;
  }                                                           ;
  if ( lw > 0 ) QtGL::LineWidth ( lw )                        ;
  #if   defined(Q_OS_ANDROID)
  #elif defined(Q_OS_IOS)
  #else
  QtGL::Enable   ( GL_MAP1_VERTEX_3                         ) ;
  QtGL::Map1d    ( GL_MAP1_VERTEX_3 , 0 , 1.0 , 3 , ct , pv ) ;
  if ( color )                                                {
    QtGL::Enable ( GL_MAP1_COLOR_4                          ) ;
    QtGL::Map1d  ( GL_MAP1_COLOR_4  , 0 , 1.0 , 4 , ct , cv ) ;
  }                                                           ;
  #endif
  /////////////////////////////////////////////////////////////
  switch ( ltype )                                            {
    case SceneSegment::Strip                                  :
      QtGL::Strips ( )                                        ;
    break                                                     ;
    case SceneSegment::Loop                                   :
      QtGL::Loop   ( )                                        ;
    break                                                     ;
  }                                                           ;
  /////////////////////////////////////////////////////////////
  int dissect = segments[idx].dissect                         ;
  if ( dissect < 4 ) dissect = 4                              ;
  EvalCurve ( dissect )                                       ;
  #if   defined(Q_OS_ANDROID)
  #elif defined(Q_OS_IOS)
  #else
  QtGL::End ( )                                               ;
  /////////////////////////////////////////////////////////////
  QtGL::Disable   ( GL_MAP1_VERTEX_3 )                        ;
  if ( color )                                                {
    QtGL::Disable ( GL_MAP1_COLOR_4  )                        ;
  }                                                           ;
  #endif
  /////////////////////////////////////////////////////////////
  nDeleteArray ( pv )                                         ;
  nDeleteArray ( cv )                                         ;
}

void N::SceneObject::EvalCurve(double u1,double u2,int dissect)
{
  double m = u2 - u1                    ;
  for (int i = 0 ; i <= dissect ; i++ ) {
    GLdouble f = i                      ;
    f /= dissect                        ;
    f *= m                              ;
    f += u1                             ;
    #if   defined(Q_OS_ANDROID)
    #elif defined(Q_OS_IOS)
    #else
    QtGL::EvalCoord1d ( f )             ;
    #endif
  }                                     ;
}

void N::SceneObject::EvalCurve(int dissect)
{
  for (int i = 0 ; i <= dissect ; i++ ) {
    GLdouble f = i                      ;
    f /= dissect                        ;
    #if   defined(Q_OS_ANDROID)
    #elif defined(Q_OS_IOS)
    #else
    QtGL::EvalCoord1d ( f )             ;
    #endif
  }                                     ;
}

void N::SceneObject::renderFaces(void)
{
  QList<int> K = faces.keys() ;
  int        k                ;
  foreach (k,K) renderFace(k) ;
}

void N::SceneObject::renderFace(int idx)
{
  switch (faces[idx].type)                {
    case SceneFace::Triangle              :
      QtGL::Triangles     ( )             ;
    break                                 ;
    case SceneFace::Quad                  :
      QtGL::Quadrilateral ( )             ;
    break                                 ;
    case SceneFace::Polygon               :
      QtGL::Polygon       ( )             ;
    break                                 ;
  }                                       ;
  int  pts   = faces[idx].points()        ;
  bool color = faces[idx].pointColor      ;
  for (int i=0;i<pts;i++)                 {
    int id = faces[idx][i]                ;
    if (color                            &&
        faces[idx].colors.contains(id))   {
      int cid = faces[idx].colors[id]     ;
      if (colors.contains(cid))           {
        colors[cid] . Color4d()           ;
      }                                   ;
    }                                     ;
    points[id] . Vertex3d ( )             ;
  }                                       ;
  QtGL::End ( )                           ;
}

void N::SceneObject::renderSmooth(void)
{
}

void N::SceneObject::renderSmooth(int index)
{
}

void N::SceneObject::renderSurfaces(void)
{
}

void N::SceneObject::renderSurface(int index)
{
}

void N::SceneObject::renderGrids(void)
{
}

void N::SceneObject::renderTessllations(void)
{
  if ( Variables . contains ( "TessColor" ) )     {
    int i = Variables [ "TessColor" ] . toInt ( ) ;
    colors[i].Color4d()                           ;
  }                                               ;
  /////////////////////////////////////////////////
  QList<int> K = tessllations . keys ()           ;
  int        k                                    ;
  foreach (k,K) renderTessllation ( k )           ;
}

typedef void (*GLUTessCallback)();

void N::SceneObject::renderTessllation(int ix)
{
  QList<int> K = tessllations [ ix ]                                      ;
  int        idx                                                          ;
  GLUtesselator * tess                                                    ;
  tess = ::gluNewTess ( )                                                 ;
  #ifdef Q_OS_WIN32
  ::gluTessCallback ( tess,GLU_TESS_BEGIN ,(void (__stdcall *)(void))glBegin     ) ;
  ::gluTessCallback ( tess,GLU_TESS_END   ,(void (__stdcall *)(void))glEnd       ) ;
  ::gluTessCallback ( tess,GLU_TESS_VERTEX,(void (__stdcall *)(void))glVertex3dv ) ;
  #else
  ::gluTessCallback ( tess,GLU_TESS_BEGIN ,(GLUTessCallback)glBegin     ) ;
  ::gluTessCallback ( tess,GLU_TESS_END   ,(GLUTessCallback)glEnd       ) ;
  ::gluTessCallback ( tess,GLU_TESS_VERTEX,(GLUTessCallback)glVertex3dv ) ;
  #endif
  ::gluTessBeginPolygon ( tess , NULL )                                   ;
  foreach ( idx , K )                                                     {
    ::gluTessBeginContour ( tess )                                        ;
    int  pts   = segments[idx].points()                                   ;
    if ( pts > 0 )                                                        {
      for (int i=pts-1;i>=0;i--)                                          {
        int id = segments[idx][i]                                         ;
        GLdouble * v = (GLdouble *) &( points [ id ] . x )                ;
        ::gluTessVertex ( tess , v , v )                                  ;
      }                                                                   ;
    }                                                                     ;
    gluTessEndContour ( tess )                                            ;
  }                                                                       ;
  ::gluTessEndPolygon ( tess )                                            ;
  ::gluDeleteTess     ( tess )                                            ;
}

void N::SceneObject::renderBox(void)
{
  ControlPoints B                                                            ;
  B [ 0 ] = ControlPoint ( minimum . x , minimum . y , minimum . z )         ;
  B [ 1 ] = ControlPoint ( maximum . x , minimum . y , minimum . z )         ;
  B [ 2 ] = ControlPoint ( maximum . x , maximum . y , minimum . z )         ;
  B [ 3 ] = ControlPoint ( minimum . x , maximum . y , minimum . z )         ;
  B [ 4 ] = ControlPoint ( minimum . x , minimum . y , maximum . z )         ;
  B [ 5 ] = ControlPoint ( maximum . x , minimum . y , maximum . z )         ;
  B [ 6 ] = ControlPoint ( maximum . x , maximum . y , maximum . z )         ;
  B [ 7 ] = ControlPoint ( minimum . x , maximum . y , maximum . z )         ;
  QtGL::Loop  ( )                                                            ;
  for (int i=0;i<4;i++) B [ i ] . Vertex3d ( )                               ;
  QtGL::End   ( )                                                            ;
  QtGL::Loop  ( )                                                            ;
  for (int i=4;i<8;i++) B [ i ] . Vertex3d ( )                               ;
  QtGL::End   ( )                                                            ;
  QtGL::Lines ( )                                                            ;
  B [ 0 ] . Vertex3d ( )                                                     ;
  B [ 4 ] . Vertex3d ( )                                                     ;
  B [ 1 ] . Vertex3d ( )                                                     ;
  B [ 5 ] . Vertex3d ( )                                                     ;
  B [ 2 ] . Vertex3d ( )                                                     ;
  B [ 6 ] . Vertex3d ( )                                                     ;
  B [ 3 ] . Vertex3d ( )                                                     ;
  B [ 7 ] . Vertex3d ( )                                                     ;
  QtGL::End   ( )                                                            ;
}

void N::SceneObject::renderTextures(void)
{
  QList<int> K = faces.keys()    ;
  int        k                   ;
  foreach (k,K) renderTexture(k) ;
}

void N::SceneObject::renderTexture(int index)
{
  bool texture                            ;
  texture = bindTexture(faces[index])>=0  ;
  switch (faces[index].type)              {
    case SceneFace::Triangle              :
      QtGL::Triangles     ( )             ;
    break                                 ;
    case SceneFace::Quad                  :
      QtGL::Quadrilateral ( )             ;
    break                                 ;
    case SceneFace::Polygon               :
      QtGL::Polygon       ( )             ;
    break                                 ;
  }                                       ;
  int  pts   = faces[index].points()      ;
  for (int i=0;i<pts;i++)                 {
    int id = faces[index][i]              ;
    if (texture)                          {
      int tid                             ;
      tid = faces[index].texCoords[id]    ;
      double tx = texCoords[tid].x        ;
      double ty = texCoords[tid].y        ;
      QtGL::TexCoord2d ( tx , ty )        ;
    }                                     ;
    points[id] . Vertex3d ( )             ;
  }                                       ;
  QtGL::End ( )                           ;
  if (texture)                            {
    releaseTexture(faces[index])          ;
  }                                       ;
}

void N::SceneObject::renderCeramics(void)
{
  QList<int> K = faces.keys()     ;
  int        k                    ;
  foreach (k,K) renderCeramics(k) ;
}

void N::SceneObject::renderCeramics(int index)
{
}

void N::SceneObject::renderPictures(void)
{
  UUIDs K = pictures.keys()      ;
  SUID  k                        ;
  foreach (k,K) if (pictures[k]) {
    if (textures.contains(k))    {
      textures[k]->Raster()      ;
    }                            ;
  }                              ;
}

void N::SceneObject::renderVoxels(void)
{
}

void N::SceneObject::renderVoxel(int index)
{
}

void N::SceneObject::renderChildren(Camera & camera,QMap<int,Matrix> & m)
{
  if ( hasFunction ( "Inherit" ) ) {
    inheritChildren ( camera , m ) ;
  } else                           {
    renderChildren  (            ) ;
  }                                ;
}

void N::SceneObject::inheritChildren(Camera & camera,QMap<int,Matrix> & m)
{
  UUIDs            U = children . keys ( )               ;
  SUID             u                                     ;
  qint64           i = 0                                 ;
  QMap<int,Matrix> M                                     ;
  ////////////////////////////////////////////////////////
  M [ 0 ] . set ( Cpp::Double , 4 , 4 )                  ;
  M [ 1 ] . set ( Cpp::Double , 4 , 4 )                  ;
  M [ 2 ] . set ( Cpp::Double , 4 , 4 )                  ;
  ////////////////////////////////////////////////////////
  Multiply ( M [ 0 ] , m        [ 0 ] , matrices [ 0 ] ) ;
  Multiply ( M [ 1 ] , m        [ 1 ] , matrices [ 1 ] ) ;
  Multiply ( M [ 2 ] , matrices [ 2 ] , m        [ 2 ] ) ;
  ////////////////////////////////////////////////////////
  while ( ForChild ( u , i , U ) )                       {
    if ( visibilities [ u ] )                            {
      children [ u ] -> render ( camera , M )            ;
    }                                                    ;
  }                                                      ;
}

void N::SceneObject::renderChildren(void)
{
  UUIDs  K = children . keys ( )   ;
  SUID   k                         ;
  qint64 i = 0                     ;
  while ( ForChild ( k , i , K ) ) {
    if ( visibilities [ k ] )      {
      children [ k ] -> render ( ) ;
    }                              ;
  }                                ;
}

void N::SceneObject::renderUser(void)
{
}

void N::SceneObject::PointSize(double pointSize)
{
  QtGL::PointSize ( pointSize ) ;
}

void N::SceneObject::LineWidth(double lineWidth)
{
  QtGL::LineWidth ( lineWidth ) ;
}

void N::SceneObject::LineWidth(QString key,double lineWidth)
{
  if ( Variables . contains ( key ) )            {
    lineWidth = Variables [ key ] . toDouble ( ) ;
  }                                              ;
  QtGL::LineWidth ( lineWidth )                  ;
}

void N::SceneObject::assumeColor(QString key)
{
  if ( ! Variables . contains ( key ) ) return ;
  int index = Value ( key ) . toInt ( )        ;
  if ( ! colors . contains ( index )  ) return ;
  colors [ index ] . Color4d ( )               ;
}

int N::SceneObject::addPoint(int idx,ControlPoint & point)
{
  index          << idx    ;
  points [ idx ]  = point  ;
  AddPoint         (point) ;
  return idx + 1           ;
}

int N::SceneObject::addVector(int idx,ControlPoint & vector)
{
  vectors [ idx ] = vector ;
  return idx + 1           ;
}

int N::SceneObject::addColor(int idx,ControlPoint & color)
{
  colors [ idx ] = color ;
  return idx + 1         ;
}

void N::SceneObject::setColor(QColor color)
{
  CUIDs        K = colors.keys() ;
  ControlPoint c                 ;
  int          k                 ;
  c = color                      ;
  foreach (k,K) colors[k] = c    ;
}

int N::SceneObject::addImage(SUID u,QImage * image)
{
  images [ u ] = image      ;
  return images . count ( ) ;
}

int N::SceneObject::addChild(SUID u,SceneObject * obj)
{
  children     [ u ] = obj    ;
  visibilities [ u ] = true   ;
  obj -> node    = u          ;
  return children . count ( ) ;
}

SUID N::SceneObject::appendChild(SUID u,SceneObject * obj)
{
  while ( children . contains ( u ) ) u++ ;
  children     [ u ] = obj                ;
  visibilities [ u ] = true               ;
  obj -> node        = u                  ;
  return u                                ;
}

int N::SceneObject::addTess(int ix,ControlPoints & Ps,QMap<int,CUIDs> & Cs)
{
  CUIDs IDs                                      ;
  int   id                                       ;
  int   pd                                       ;
  int   last = Last ( )                          ;
  int   idx  = last                              ;
  IDs = Ps . keys ( )                            ;
  foreach ( id , IDs )                           {
    Ps [ id ] . z = 16                           ;
    idx = addPoint ( idx , Ps [ id ] )           ;
  }                                              ;
  IDs = Cs . keys ( )                            ;
  foreach ( id , IDs )                           {
    CUIDs CDs = Cs [ id ]                        ;
    int   sl  = Last ( Segment )                 ;
    segments [ sl ] . type = SceneSegment::Loop  ;
    foreach ( pd , CDs )                         {
      segments [ sl ] . add ( pd + last )        ;
    }                                            ;
    tessllations [ ix ] << sl                    ;
  }                                              ;
  return Last ( Tess )                           ;
}

void N::SceneObject::appendSegments(int start,int count,SceneSegment & segment)
{
  for (int i=0;i<count;i++)          {
    segments [ start + i ] = segment ;
  }                                  ;
}

void N::SceneObject::appendFaces(int start,int count,SceneFace & face)
{
  for (int i=0;i<count;i++)    {
    faces [ start + i ] = face ;
  }                            ;
}

void N::SceneObject::addText (
       GlyphManager & glyph  ,
       ControlPoint   shift  ,
       QString        text   )
{
  int          L = text . length ( )                        ;
  ControlPoint Z ( shift . x , shift . y , shift . z )      ;
  ControlPoint W = Z                                        ;
  double       T = shift . r                                ;
  double       A = 0                                        ;
  double       B = 0                                        ;
  Z . y -= glyph . Height ( ) / 2                           ;
  W . y  = Z.y                                              ;
  for (int x = 0 ; x < L ; x++ )                            {
    int             unicode                                 ;
    int             last                                    ;
    ControlPoints   Ps                                      ;
    QMap<int,CUIDs> Cs                                      ;
    QRectF          R                                       ;
    Ps . clear ( )                                          ;
    Cs . clear ( )                                          ;
    unicode = text . at ( x ) . unicode ( )                 ;
    if ( unicode == QChar(' ').unicode() )                  {
      Z . x += glyph.Width() / 2                            ;
      A     += glyph.Width() / 2                            ;
    } else                                                  {
      last   = Last                 ( SceneObject::Tess )   ;
      R      = glyph . BoundingBox  ( unicode           )   ;
      B      = A                                            ;
      B     += R . width ( )                                ;
      B     += glyph . Width ( ) / 8                        ;
      if ( ( T > 0 ) && ( B > T ) )                         {
        Z . x  = W . x                                      ;
        Z . y -= glyph . Height ( )                         ;
        Z . y -= glyph . Height ( ) / 8                     ;
        A      = 0                                          ;
      }                                                     ;
      Z . x +=                      ( R . width ( ) / 2 )   ;
      Z . x += glyph . Width ( ) / 8                        ;
      A     +=                      ( R . width ( ) / 2 )   ;
      A     += glyph . Width ( ) / 8                        ;
      glyph  . points               ( unicode , Z  , Ps )   ;
      glyph  . contours             ( unicode , Cs      )   ;
      addTess                       ( last    , Ps , Cs )   ;
      Z . x +=                      ( R . width ( ) / 2 )   ;
      A     +=                      ( R . width ( ) / 2 )   ;
    }                                                       ;
    Z . x += glyph . Width ( ) / 8                          ;
    A     += glyph . Width ( ) / 8                          ;
  }                                                         ;
}

void N::SceneObject::addCube(double length)
{
  double v[8][3]                                          ;
  double e     = length / 2                               ;
  int    index = Last()                                   ;
  int    last  = index                                    ;
  int    si                                               ;
  GLint faces[6][4] =                                     {
    { 0 , 1 , 2 , 3 }                                     ,
    { 3 , 2 , 6 , 7 }                                     ,
    { 7 , 6 , 5 , 4 }                                     ,
    { 4 , 5 , 1 , 0 }                                     ,
    { 5 , 6 , 2 , 1 }                                     ,
    { 7 , 4 , 0 , 3 }                                   } ;
  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -e              ;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] =  e              ;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -e              ;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] =  e              ;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -e              ;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] =  e              ;
  for (int i=0;i<8;i++)                                   {
    ControlPoint p(v[i][0],v[i][1],v[i][2])               ;
    index = addPoint ( index , p )                        ;
  }                                                       ;
  /////////////////////////////////////////////////////////
  si = Last ( Segment )                                   ;
  SceneSegment F                                          ;
  appendSegments ( si , 6 , F )                           ;
  segments [ si     ] . type = SceneSegment::Loop         ;
  segments [ si     ] . add ( last + 3 )                  ;
  segments [ si     ] . add ( last + 0 )                  ;
  segments [ si     ] . add ( last + 4 )                  ;
  segments [ si     ] . add ( last + 7 )                  ;
  segments [ si + 1 ] . type = SceneSegment::Loop         ;
  segments [ si + 1 ] . add ( last + 5 )                  ;
  segments [ si + 1 ] . add ( last + 6 )                  ;
  segments [ si + 1 ] . add ( last + 2 )                  ;
  segments [ si + 1 ] . add ( last + 1 )                  ;
  segments [ si + 2 ] . type = SceneSegment::Lines        ;
  segments [ si + 2 ] . add ( last + 3 )                  ;
  segments [ si + 2 ] . add ( last + 2 )                  ;
  segments [ si + 3 ] . type = SceneSegment::Lines        ;
  segments [ si + 3 ] . add ( last + 0 )                  ;
  segments [ si + 3 ] . add ( last + 1 )                  ;
  segments [ si + 4 ] . type = SceneSegment::Lines        ;
  segments [ si + 4 ] . add ( last + 4 )                  ;
  segments [ si + 4 ] . add ( last + 5 )                  ;
  segments [ si + 5 ] . type = SceneSegment::Lines        ;
  segments [ si + 5 ] . add ( last + 7 )                  ;
  segments [ si + 5 ] . add ( last + 6 )                  ;
  /////////////////////////////////////////////////////////
  si = Last ( Face )                                      ;
  SceneFace P                                             ;
  this->faces [ si     ] = P                              ;
  this->faces [ si + 1 ] = P                              ;
  this->faces [ si + 2 ] = P                              ;
  this->faces [ si + 3 ] = P                              ;
  this->faces [ si + 4 ] = P                              ;
  this->faces [ si + 5 ] = P                              ;
  for (int i=0;i<6;i++)                                   {
    this->faces [ si + i] . type = SceneFace::Quad        ;
    for (int j=0;j<4;j++)                                 {
      this->faces [ si + i ] . add ( faces[i][j] + last ) ;
    }                                                     ;
  }                                                       ;
}

void N::SceneObject::addCube(double length,ControlPoint & shift)
{
  double v[8][3]                                          ;
  double e     = length / 2                               ;
  int    index = Last()                                   ;
  int    last  = index                                    ;
  int    si                                               ;
  GLint faces[6][4] =                                     {
    { 0 , 1 , 2 , 3 }                                     ,
    { 3 , 2 , 6 , 7 }                                     ,
    { 7 , 6 , 5 , 4 }                                     ,
    { 4 , 5 , 1 , 0 }                                     ,
    { 5 , 6 , 2 , 1 }                                     ,
    { 7 , 4 , 0 , 3 }                                   } ;
  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -e              ;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] =  e              ;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -e              ;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] =  e              ;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -e              ;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] =  e              ;
  for (int i=0;i<8;i++)                                   {
    ControlPoint p(v[i][0],v[i][1],v[i][2])               ;
    p    += shift                                         ;
    index = addPoint ( index , p )                        ;
  }                                                       ;
  /////////////////////////////////////////////////////////
  si = Last ( Segment )                                   ;
  SceneSegment F                                          ;
  appendSegments ( si , 6 , F )                           ;
  segments [ si     ] . type = SceneSegment::Loop         ;
  segments [ si     ] . add ( last + 3 )                  ;
  segments [ si     ] . add ( last + 0 )                  ;
  segments [ si     ] . add ( last + 4 )                  ;
  segments [ si     ] . add ( last + 7 )                  ;
  segments [ si + 1 ] . type = SceneSegment::Loop         ;
  segments [ si + 1 ] . add ( last + 5 )                  ;
  segments [ si + 1 ] . add ( last + 6 )                  ;
  segments [ si + 1 ] . add ( last + 2 )                  ;
  segments [ si + 1 ] . add ( last + 1 )                  ;
  segments [ si + 2 ] . type = SceneSegment::Lines        ;
  segments [ si + 2 ] . add ( last + 3 )                  ;
  segments [ si + 2 ] . add ( last + 2 )                  ;
  segments [ si + 3 ] . type = SceneSegment::Lines        ;
  segments [ si + 3 ] . add ( last + 0 )                  ;
  segments [ si + 3 ] . add ( last + 1 )                  ;
  segments [ si + 4 ] . type = SceneSegment::Lines        ;
  segments [ si + 4 ] . add ( last + 4 )                  ;
  segments [ si + 4 ] . add ( last + 5 )                  ;
  segments [ si + 5 ] . type = SceneSegment::Lines        ;
  segments [ si + 5 ] . add ( last + 7 )                  ;
  segments [ si + 5 ] . add ( last + 6 )                  ;
  /////////////////////////////////////////////////////////
  si = Last ( Face )                                      ;
  SceneFace P                                             ;
  this -> faces [ si     ] = P                            ;
  this -> faces [ si + 1 ] = P                            ;
  this -> faces [ si + 2 ] = P                            ;
  this -> faces [ si + 3 ] = P                            ;
  this -> faces [ si + 4 ] = P                            ;
  this -> faces [ si + 5 ] = P                            ;
  for (int i=0;i<6;i++)                                   {
    this->faces [ si + i] . type = SceneFace::Quad        ;
    for (int j=0;j<4;j++)                                 {
      this->faces [ si + i ] . add ( faces[i][j] + last ) ;
    }                                                     ;
  }                                                       ;
}

void N::SceneObject::addColorCube(double length,ControlPoint & shift)
{
  double v[8][3]                                           ;
  int    c[8][3]                                           ;
  double e     = length / 2                                ;
  int    index = Last()                                    ;
  int    last  = index                                     ;
  int    si                                                ;
  CUIDs  I                                                 ;
  GLint faces[6][4] =                                      {
    { 0 , 1 , 2 , 3 }                                      ,
    { 3 , 2 , 6 , 7 }                                      ,
    { 7 , 6 , 5 , 4 }                                      ,
    { 4 , 5 , 1 , 0 }                                      ,
    { 5 , 6 , 2 , 1 }                                      ,
    { 7 , 4 , 0 , 3 }                                    } ;
  //////////////////////////////////////////////////////////
  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -e               ;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] =  e               ;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -e               ;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] =  e               ;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -e               ;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] =  e               ;
  //////////////////////////////////////////////////////////
  c[0][0] = c[1][0] = c[2][0] = c[3][0] =    0             ;
  c[4][0] = c[5][0] = c[6][0] = c[7][0] =  255             ;
  c[0][1] = c[1][1] = c[4][1] = c[5][1] =    0             ;
  c[2][1] = c[3][1] = c[6][1] = c[7][1] =  255             ;
  c[0][2] = c[3][2] = c[4][2] = c[7][2] =    0             ;
  c[1][2] = c[2][2] = c[5][2] = c[6][2] =  255             ;
  //////////////////////////////////////////////////////////
  for (int i=0;i<8;i++)                                    {
    QColor       X(c[i][0],c[i][1],c[i][2])                ;
    ControlPoint p(v[i][0],v[i][1],v[i][2])                ;
    ControlPoint C                                         ;
    C      = X                                             ;
    p     += shift                                         ;
    addPoint ( index , p )                                 ;
    addColor ( index , C )                                 ;
    I     << index                                         ;
    index ++                                               ;
  }                                                        ;
  //////////////////////////////////////////////////////////
  si = Last ( Segment )                                    ;
  SceneSegment F                                           ;
  F.pointColor = true                                      ;
  appendSegments ( si , 6 , F )                            ;
  segments [ si     ] . type = SceneSegment::Loop          ;
  segments [ si     ] . add      ( last + 3        )       ;
  segments [ si     ] . add      ( last + 0        )       ;
  segments [ si     ] . add      ( last + 4        )       ;
  segments [ si     ] . add      ( last + 7        )       ;
  segments [ si     ] . setColor ( last + 3 , I[3] )       ;
  segments [ si     ] . setColor ( last + 0 , I[0] )       ;
  segments [ si     ] . setColor ( last + 4 , I[4] )       ;
  segments [ si     ] . setColor ( last + 7 , I[7] )       ;
  segments [ si + 1 ] . type = SceneSegment::Loop          ;
  segments [ si + 1 ] . add      ( last + 5        )       ;
  segments [ si + 1 ] . add      ( last + 6        )       ;
  segments [ si + 1 ] . add      ( last + 2        )       ;
  segments [ si + 1 ] . add      ( last + 1        )       ;
  segments [ si + 1 ] . setColor ( last + 5 , I[5] )       ;
  segments [ si + 1 ] . setColor ( last + 6 , I[6] )       ;
  segments [ si + 1 ] . setColor ( last + 2 , I[2] )       ;
  segments [ si + 1 ] . setColor ( last + 1 , I[1] )       ;
  segments [ si + 2 ] . type = SceneSegment::Lines         ;
  segments [ si + 2 ] . add      ( last + 3        )       ;
  segments [ si + 2 ] . add      ( last + 2        )       ;
  segments [ si + 2 ] . setColor ( last + 3 , I[3] )       ;
  segments [ si + 2 ] . setColor ( last + 2 , I[2] )       ;
  segments [ si + 3 ] . type = SceneSegment::Lines         ;
  segments [ si + 3 ] . add      ( last + 0        )       ;
  segments [ si + 3 ] . add      ( last + 1        )       ;
  segments [ si + 3 ] . setColor ( last + 0 , I[0] )       ;
  segments [ si + 3 ] . setColor ( last + 1 , I[1] )       ;
  segments [ si + 4 ] . type = SceneSegment::Lines         ;
  segments [ si + 4 ] . add      ( last + 4        )       ;
  segments [ si + 4 ] . add      ( last + 5        )       ;
  segments [ si + 4 ] . setColor ( last + 4 , I[4] )       ;
  segments [ si + 4 ] . setColor ( last + 5 , I[5] )       ;
  segments [ si + 5 ] . type = SceneSegment::Lines         ;
  segments [ si + 5 ] . add      ( last + 7        )       ;
  segments [ si + 5 ] . add      ( last + 6        )       ;
  segments [ si + 5 ] . setColor ( last + 7 , I[7] )       ;
  segments [ si + 5 ] . setColor ( last + 6 , I[6] )       ;
  //////////////////////////////////////////////////////////
  si = Last ( Face )                                       ;
  SceneFace P                                              ;
  this->faces [ si     ] = P                               ;
  this->faces [ si + 1 ] = P                               ;
  this->faces [ si + 2 ] = P                               ;
  this->faces [ si + 3 ] = P                               ;
  this->faces [ si + 4 ] = P                               ;
  this->faces [ si + 5 ] = P                               ;
  for (int i=0;i<6;i++)                                    {
    this->faces [ si + i] . type = SceneFace::Quad         ;
    this->faces [ si + i] . pointColor = true              ;
    for (int j=0;j<4;j++)                                  {
      int bi = faces[i][j]                                 ;
      int ci = I[bi]                                       ;
      this->faces [ si + i ] . add      ( bi + last      ) ;
      this->faces [ si + i ] . setColor ( bi + last , ci ) ;
    }                                                      ;
  }                                                        ;
}

void N::SceneObject::addPlane(Plane & plane)
{
  int index = Last()                              ;
  int last  = index                               ;
  int w     = plane.W + 1                         ;
  int h     = plane.H + 1                         ;
  int si                                          ;
  ControlPoint O = plane.O                        ;
  for (int y=0;y<h;y++)                           {
    for (int x=0;x<w;x++)                         {
      ControlPoint o = O                          ;
      ControlPoint X = plane.X                    ;
      ControlPoint Y = plane.Y                    ;
      X *= x                                      ;
      Y *= y                                      ;
      o += X                                      ;
      o += Y                                      ;
      index = addPoint ( index , o )              ;
    }                                             ;
  }                                               ;
  for (int y=0;y<h;y++)                           {
    int           s = y * w                       ;
    SceneSegment F                                ;
    si = Last ( Segment )                         ;
    s += last                                     ;
    segments [ si ] = F                           ;
    segments [ si ] . type = SceneSegment::Strip  ;
    for (int x=0;x<w;x++)                         {
      segments [ si ] . add ( s + x )             ;
    }                                             ;
  }                                               ;
  for (int x=0;x<w;x++)                           {
    SceneSegment F                                ;
    si = Last ( Segment )                         ;
    segments [ si ] = F                           ;
    segments [ si ] . type = SceneSegment::Strip  ;
    for (int y=0;y<h;y++)                         {
      int s = (y * w) + x + last                  ;
      segments [ si ] . add ( s )                 ;
    }                                             ;
  }                                               ;
  for (int y=0;y<plane.H;y++)                     {
    for (int x=0;x<plane.W;x++)                   {
      SceneFace F                                 ;
      si = Last ( Face )                          ;
      faces [ si ] = F                            ;
      int v1 = (y * w) + last + x                 ;
      int v2 = v1 + 1                             ;
      int v4 = v1 + w                             ;
      int v3 = v4 + 1                             ;
      faces [ si ] . type = SceneFace::Quad       ;
      faces [ si ] . add ( v1 )                   ;
      faces [ si ] . add ( v2 )                   ;
      faces [ si ] . add ( v3 )                   ;
      faces [ si ] . add ( v4 )                   ;
    }                                             ;
  }                                               ;
}

void N::SceneObject::addPlane(SUID textureId,Plane & plane)
{
  int idx  = Last()                               ;
  int last = idx                                  ;
  int w    = plane.W + 1                          ;
  int h    = plane.H + 1                          ;
  int si                                          ;
  ControlPoint O = plane.O                        ;
  for (int y=0;y<h;y++)                           {
    for (int x=0;x<w;x++)                         {
      ControlPoint o = O                          ;
      ControlPoint X = plane.X                    ;
      ControlPoint Y = plane.Y                    ;
      ControlPoint T                              ;
      double       dx                             ;
      double       dy                             ;
      X *= x                                      ;
      Y *= y                                      ;
      o += X                                      ;
      o += Y                                      ;
      dx = x                                      ;
      dy = y                                      ;
      dx/= plane.W                                ;
      dy/= plane.H                                ;
      T  . x = dx                                 ;
      T  . y = dy                                 ;
      T  . z = 0                                  ;
      texCoords [ idx ] = T                       ;
      idx = addPoint ( idx , o )                  ;
    }                                             ;
  }                                               ;
  for (int y=0;y<h;y++)                           {
    int           s = y * w                       ;
    SceneSegment F                                ;
    si = Last ( Segment )                         ;
    s += last                                     ;
    segments [ si ] = F                           ;
    segments [ si ] . type = SceneSegment::Strip  ;
    for (int x=0;x<w;x++)                         {
      segments [ si ] . add ( s + x )             ;
    }                                             ;
  }                                               ;
  for (int x=0;x<w;x++)                           {
    SceneSegment F                                ;
    si = Last ( Segment )                         ;
    segments [ si ] = F                           ;
    segments [ si ] . type = SceneSegment::Strip  ;
    for (int y=0;y<h;y++)                         {
      int s = (y * w) + x + last                  ;
      segments [ si ] . add ( s )                 ;
    }                                             ;
  }                                               ;
  for (int y=0;y<plane.H;y++)                     {
    for (int x=0;x<plane.W;x++)                   {
      SceneFace F                                 ;
      si = Last ( Face )                          ;
      faces [ si ] = F                            ;
      faces [ si ] . textureId = textureId        ;
      int v1 = (y * w) + last + x                 ;
      int v2 = v1 + 1                             ;
      int v4 = v1 + w                             ;
      int v3 = v4 + 1                             ;
      faces [ si ] . type = SceneFace::Quad       ;
      faces [ si ] . add ( v1 )                   ;
      faces [ si ] . add ( v2 )                   ;
      faces [ si ] . add ( v3 )                   ;
      faces [ si ] . add ( v4 )                   ;
      faces [ si ] . texCoords[v1] = v1           ;
      faces [ si ] . texCoords[v2] = v2           ;
      faces [ si ] . texCoords[v3] = v3           ;
      faces [ si ] . texCoords[v4] = v4           ;
    }                                             ;
  }                                               ;
}

void N::SceneObject::addColors(QByteArray & rgbColors)
{
  unsigned char * d                     ;
  int t = rgbColors.size()              ;
  int p = 0                             ;
  t /= 4                                ;
  d = (unsigned char *)rgbColors.data() ;
  for (int i=0;i<t;i++,p+=4)            {
    int b = d [ p   ]                   ;
    int g = d [ p+1 ]                   ;
    int r = d [ p+2 ]                   ;
    QColor C(r,g,b)                     ;
    ControlPoint P(r,g,b)               ;
    ControlPoint X                      ;
    X = C                               ;
    addPoint ( i , P )                  ;
    addColor ( i , X )                  ;
  }                                     ;
}

void N::SceneObject::addRgba(QByteArray & rgbColors)
{
  unsigned char * d                     ;
  int t = rgbColors.size()              ;
  int p = 0                             ;
  t /= 4                                ;
  d = (unsigned char *)rgbColors.data() ;
  for (int i=0;i<t;i++,p+=4)            {
    int b = d [ p   ]                   ;
    int g = d [ p+1 ]                   ;
    int r = d [ p+2 ]                   ;
    int a = d [ p+3 ]                   ;
    QColor C(r,g,b,a)                   ;
    ControlPoint P(r,g,b)               ;
    ControlPoint X                      ;
    X = C                               ;
    addPoint ( i , P )                  ;
    addColor ( i , X )                  ;
  }                                     ;
}

UUIDs N::SceneObject::Sequential(void)
{
  UUIDs  ID = sequences . keys ( ) ;
  UUIDs  SQ                        ;
  SUID   UD                        ;
  foreach ( UD , ID )              {
    SQ << sequences[UD]            ;
  }                                ;
  return SQ                        ;
}

void N::SceneObject::addSequence(SUID order,SUID renderId)
{
  sequences[order] = renderId ;
}

int N::SceneObject::bindTexture(SUID uuid)
{
  if (!textures.contains(uuid)) return -1 ;
  SceneTexture * st = NULL                ;
  st = textures[uuid]                     ;
  st -> begin ( )                         ;
  return st->bind()                       ;
}

int N::SceneObject::bindTexture(SceneFace& face)
{
  return bindTexture(face.textureId) ;
}

int N::SceneObject::releaseTexture(SUID uuid)
{
  if (!textures.contains(uuid)) return -1 ;
  SceneTexture * st = NULL                ;
  st = textures [ uuid ]                  ;
  st -> release ( )                       ;
  st -> end     ( )                       ;
  return 0                                ;
}

int N::SceneObject::releaseTexture(SceneFace & face)
{
  return releaseTexture(face.textureId) ;
}

bool N::SceneObject::allowDrag(void)
{
  return Value ( "AllowDrag" ) . toBool ( ) ;
}

bool N::SceneObject::canDrop(void)
{
  return Value ( "AcceptDrop" ) . toBool ( ) ;
}

bool N::SceneObject::removeDrop(void)
{
  if ( ! canDrop ( ) ) return false ;
  return sceneRemoveDrop ( )        ;
}

bool N::SceneObject::dragEnter (
       ControlPoint    & From  ,
       ControlPoint    & To    ,
       QDragEnterEvent * e     )
{
  if ( ! canDrop ( ) ) return false       ;
  return sceneDragEnter ( From , To , e ) ;
}

bool N::SceneObject:: dragMove (
       ControlPoint   & From   ,
       ControlPoint   & To     ,
       QDragMoveEvent * e      )
{
  if ( ! canDrop ( ) ) return false      ;
  return sceneDragMove ( From , To , e ) ;
}

bool N::SceneObject::drop  (
       ControlPoint & From ,
       ControlPoint & To   ,
       QDropEvent   * e    )
{
  if ( ! canDrop ( ) ) return false  ;
  return sceneDrop ( From , To , e ) ;
}

bool N::SceneObject::MouseClicked (
       ControlPoint & From        ,
       ControlPoint & To          ,
       BMAPs        & buttons     )
{
  return false ;
}

bool N::SceneObject::MouseVector (
       ControlPoint & From       ,
       ControlPoint & To         ,
       BMAPs        & buttons    )
{
  return false ;
}

bool N::SceneObject::MouseHover (
       ControlPoint & From      ,
       ControlPoint & To        ,
       BMAPs        & buttons   )
{
  return false ;
}

bool N::SceneObject::MouseMove (
       ControlPoint & From1    ,
       ControlPoint & To1      ,
       ControlPoint & From2    ,
       ControlPoint & To2      ,
       BMAPs        & buttons  )
{
  return false ;
}

bool N::SceneObject::CursorClicked (
       ControlPoint & From         ,
       ControlPoint & To           ,
       BMAPs        & buttons      )
{
  if ( ! Value ( "Mouse" ) . toBool ( )  ) return false             ;
  if ( ! matrices . contains ( 2 )       ) return false             ;
  if ( buttons [ Universe::MouseMiddle ] ) return false             ;
  if ( buttons [ Universe::MouseRight  ] ) return false             ;
  ///////////////////////////////////////////////////////////////////
  ControlPoint At                                                   ;
  bool         iGrab   = hasFunction    ( "Grab" )                  ;
  bool         grab    = isMouseGrabbed (        )                  ;
  bool         collide = Collide ( From , To , At )                 ;
  bool         overlap                                              ;
  ///////////////////////////////////////////////////////////////////
  if ( ( ! iGrab ) && ( ! grab ) && ( ! collide ) ) return false    ;
  ///////////////////////////////////////////////////////////////////
  if ( iGrab )                                                      {
    LocalClicked ( From , To , At , buttons , false , false )       ;
    LocalClicked ( From , To , At , buttons , true  , false )       ;
    return true                                                     ;
  } else                                                            {
    if ( grab ) return GrabClicked ( From , To , buttons )          ;
  }                                                                 ;
  ///////////////////////////////////////////////////////////////////
  LocalClicked ( From , To , At , buttons , false , false )         ;
  overlap = ChildrenClicked ( From , To , buttons )                 ;
  return LocalClicked ( From , To , At , buttons , true , overlap ) ;
}

bool N::SceneObject::CursorVector (
       ControlPoint & From        ,
       ControlPoint & To          ,
       BMAPs        & buttons     )
{
  if ( ! Value ( "Mouse" ) . toBool ( )  ) return false               ;
  if ( ! matrices . contains ( 2 )       ) return false               ;
  if ( buttons [ Universe::MouseMiddle ] ) return false               ;
  if ( buttons [ Universe::MouseRight  ] ) return false               ;
  /////////////////////////////////////////////////////////////////////
  ControlPoint At                                                     ;
  bool         iGrab   = hasFunction    ( "Grab" )                    ;
  bool         grab    = isMouseGrabbed (        )                    ;
  bool         collide = Collide ( From , To , At )                   ;
  bool         overlap                                                ;
  if ( ( ! iGrab ) && ( ! grab ) && ( ! collide ) ) return false      ;
  /////////////////////////////////////////////////////////////////////
  if ( ( ! iGrab ) && grab )                                          {
    if ( collide )                                                    {
      LocalVector ( From , To , At , buttons , false , true )         ;
    }                                                                 ;
    grab = GrabVector ( From , To , buttons )                         ;
    if ( collide )                                                    {
      LocalVector ( From , To , At , buttons , true  , true )         ;
    }                                                                 ;
    return grab                                                       ;
  }                                                                   ;
  /////////////////////////////////////////////////////////////////////
  overlap = ChildrenVector ( From , To , buttons )                    ;
  if ( ( ! overlap ) || ( ! buttons [ Universe::MouseLeft ] ) )       {
    LocalVector        ( From , To , At , buttons , false , false   ) ;
    return LocalVector ( From , To , At , buttons , true  , overlap ) ;
  }                                                                   ;
  return true                                                         ;
}

bool N::SceneObject::CursorHover (
       ControlPoint & From       ,
       ControlPoint & To         ,
       BMAPs        & buttons    )
{
  if ( ! Value ( "Mouse" ) . toBool ( )  ) return false          ;
  if ( ! isFocus             (  )        ) return false          ;
  if ( ! matrices . contains ( 2 )       ) return false          ;
  ////////////////////////////////////////////////////////////////
  ControlPoint At                                                ;
  bool         iGrab   = hasFunction    ( "Grab" )               ;
  bool         grab    = isMouseGrabbed (        )               ;
  bool         collide = Collide ( From , To , At )              ;
  bool         overlap                                           ;
  if ( ( ! iGrab ) && ( ! grab ) && ( ! collide ) ) return false ;
  ////////////////////////////////////////////////////////////////
  if ( iGrab )                                                   {
    LocalHover ( From , To , At , buttons , false , false )      ;
    GrabHover  ( From , To , buttons                      )      ;
    LocalHover ( From , To , At , buttons , true  , false )      ;
    return true                                                  ;
  } else                                                         {
    if ( collide )                                               {
      LocalHover ( From , To , At , buttons , false , true )     ;
    }                                                            ;
    grab = GrabHover ( From , To , buttons )                     ;
    if ( collide )                                               {
      LocalHover ( From , To , At , buttons , true  , true )     ;
    }                                                            ;
    return grab                                                  ;
  }                                                              ;
  ////////////////////////////////////////////////////////////////
  LocalHover ( From , To , At , buttons , false , false )        ;
  overlap = ChildrenHover ( From , To , buttons )                ;
  LocalHover ( From , To , At , buttons , true , overlap )       ;
  ////////////////////////////////////////////////////////////////
  return true                                                    ;
}

bool N::SceneObject::ChildrenClicked (
       ControlPoint & From           ,
       ControlPoint & To             ,
       BMAPs        & buttons        )
{
  ControlPoint p1                                      ;
  ControlPoint p2                                      ;
  UUIDs        U = children . keys ( )                 ;
  SUID         u                                       ;
  qint64       i = 0                                   ;
  bool         overlapped = false                      ;
  //////////////////////////////////////////////////////
  p1 = matrices [ 2 ] * From                           ;
  p2 = matrices [ 2 ] * To                             ;
  //////////////////////////////////////////////////////
  while ( ForChild ( u , i , U ) )                     {
    if ( visibilities [ u ] )                          {
      SceneObject * sox = children [ u ]               ;
      if ( sox -> MouseClicked ( p1 , p2 , buttons ) ) {
        overlapped = true                              ;
      }                                                ;
    }                                                  ;
  }                                                    ;
  //////////////////////////////////////////////////////
  return overlapped                                    ;
}

bool N::SceneObject::ChildrenVector (
       ControlPoint & From          ,
       ControlPoint & To            ,
       BMAPs        & buttons       )
{
  ControlPoint p1                                     ;
  ControlPoint p2                                     ;
  UUIDs        U          = children . keys ( )       ;
  SUID         u                                      ;
  qint64       i          = 0                         ;
  bool         overlapped = false                     ;
  /////////////////////////////////////////////////////
  p1 = matrices [ 2 ] * From                          ;
  p2 = matrices [ 2 ] * To                            ;
  /////////////////////////////////////////////////////
  while ( ForChild ( u , i , U ) )                    {
    if ( visibilities [ u ] )                         {
      SceneObject * sox = children [ u ]              ;
      if ( sox -> MouseVector ( p1 , p2 , buttons ) ) {
        overlapped = true                             ;
      }                                               ;
    }                                                 ;
  }                                                   ;
  /////////////////////////////////////////////////////
  return overlapped                                   ;
}

bool N::SceneObject::ChildrenHover (
       ControlPoint & From         ,
       ControlPoint & To           ,
       BMAPs        & buttons      )
{
  ControlPoint p1                                    ;
  ControlPoint p2                                    ;
  UUIDs        U          = children . keys ( )      ;
  SUID         u                                     ;
  qint64       i          = 0                        ;
  bool         overlapped = false                    ;
  ////////////////////////////////////////////////////
  p1 = matrices [ 2 ] * From                         ;
  p2 = matrices [ 2 ] * To                           ;
  ////////////////////////////////////////////////////
  while ( ForChild ( u , i , U ) )                   {
    if ( visibilities [ u ] )                        {
      SceneObject * sox = children [ u ]             ;
      if ( sox -> MouseHover ( p1 , p2 , buttons ) ) {
        overlapped = true                            ;
      }                                              ;
    }                                                ;
  }                                                  ;
  ////////////////////////////////////////////////////
  return overlapped                                  ;
}

bool N::SceneObject::GrabClicked (
       ControlPoint & From       ,
       ControlPoint & To         ,
       BMAPs        & buttons    )
{
  ControlPoint p1                                        ;
  ControlPoint p2                                        ;
  UUIDs        U = children . keys ( )                   ;
  SUID         u                                         ;
  qint64       i = 0                                     ;
  ////////////////////////////////////////////////////////
  p1 = matrices [ 2 ] * From                             ;
  p2 = matrices [ 2 ] * To                               ;
  ////////////////////////////////////////////////////////
  while ( ForChild ( u , i , U ) )                       {
    if ( visibilities [ u ] )                            {
      SceneObject * sox = children [ u ]                 ;
      if ( sox -> isMouseGrabbed ( ) )                   {
        if ( sox -> MouseClicked ( p1 , p2 , buttons ) ) {
          return true                                    ;
        }                                                ;
      }                                                  ;
    }                                                    ;
  }                                                      ;
  ////////////////////////////////////////////////////////
  return false                                           ;
}

bool N::SceneObject::GrabVector (
       ControlPoint & From      ,
       ControlPoint & To        ,
       BMAPs        & buttons   )
{
  ControlPoint p1                                       ;
  ControlPoint p2                                       ;
  UUIDs        U = children . keys ( )                  ;
  SUID         u                                        ;
  qint64       i = 0                                    ;
  ///////////////////////////////////////////////////////
  p1 = matrices [ 2 ] * From                            ;
  p2 = matrices [ 2 ] * To                              ;
  ///////////////////////////////////////////////////////
  while ( ForChild ( u , i , U ) )                      {
    if ( visibilities [ u ] )                           {
      SceneObject * sox = children [ u ]                ;
      if ( sox -> isMouseGrabbed ( ) )                  {
        if ( sox -> MouseVector ( p1 , p2 , buttons ) ) {
          return true                                   ;
        }                                               ;
      }                                                 ;
    }                                                   ;
  }                                                     ;
  ///////////////////////////////////////////////////////
  return false                                          ;
}

bool N::SceneObject::GrabHover (
       ControlPoint & From     ,
       ControlPoint & To       ,
       BMAPs        & buttons  )
{
  ControlPoint p1                                      ;
  ControlPoint p2                                      ;
  UUIDs        U = children . keys ( )                 ;
  SUID         u                                       ;
  qint64       i = 0                                   ;
  //////////////////////////////////////////////////////
  p1 = matrices [ 2 ] * From                           ;
  p2 = matrices [ 2 ] * To                             ;
  //////////////////////////////////////////////////////
  while ( ForChild ( u , i , U ) )                     {
    if ( visibilities [ u ] )                          {
      SceneObject * sox = children [ u ]               ;
      if   ( sox -> isMouseGrabbed ( )               ) {
        if ( sox -> MouseHover ( p1 , p2 , buttons ) ) {
          return true                                  ;
        }                                              ;
      }                                                ;
    }                                                  ;
  }                                                    ;
  //////////////////////////////////////////////////////
  return false                                         ;
}

bool N::SceneObject::LocalClicked (
       ControlPoint & From        ,
       ControlPoint & To          ,
       ControlPoint & At          ,
       BMAPs        & buttons     ,
       bool           after       ,
       bool           overlap     )
{
  if ( ! after )                             {
    if ( buttons [ Universe::MouseLeft   ] ) {
      setFocus ( true )                      ;
      setVariable  ( "Hit" , true    )       ;
    } else                                   {
      if ( Value ( "Hit" ) . toBool ( ) )    {
        setVariable      ( "Hit" , false   ) ;
      }                                      ;
    }                                        ;
    return true                              ;
  }                                          ;
  ////////////////////////////////////////////
  if ( ! overlap )                           {
    return true                              ;
  }                                          ;
  ////////////////////////////////////////////
  return overlap                             ;
}

bool N::SceneObject::LocalVector (
       ControlPoint & From       ,
       ControlPoint & To         ,
       ControlPoint & At         ,
       BMAPs        & buttons    ,
       bool           after      ,
       bool           overlap    )
{
  if ( ! after )                             {
    if ( buttons [ Universe::MouseLeft   ] ) {
      setFocus ( true )                      ;
      setVariable  ( "Hit" , true    )       ;
    } else                                   {
      if ( Value ( "Hit" ) . toBool ( ) )    {
        setVariable      ( "Hit" , false   ) ;
      }                                      ;
    }                                        ;
    return true                              ;
  }                                          ;
  ////////////////////////////////////////////
  if ( ! overlap )                           {
    return true                              ;
  }                                          ;
  ////////////////////////////////////////////
  return overlap                             ;
}

bool N::SceneObject::LocalHover (
       ControlPoint & From      ,
       ControlPoint & To        ,
       ControlPoint & At        ,
       BMAPs        & buttons   ,
       bool           after     ,
       bool           overlap   )
{
  if ( ! after ) {
    return true  ;
  }              ;
  return true    ;
}

bool N::SceneObject::sceneDragEnter (
       ControlPoint    & From       ,
       ControlPoint    & To         ,
       QDragEnterEvent * event      )
{
  return false ;
}

bool N::SceneObject::sceneDragMove (
       ControlPoint    & From      ,
       ControlPoint    & To        ,
       QDragMoveEvent  * event     )
{
  return false ;
}

bool N::SceneObject::sceneDrop (
       ControlPoint    & From  ,
       ControlPoint    & To    ,
       QDropEvent      * event )
{
  return false ;
}

bool N::SceneObject::sceneRemoveDrop(void)
{
  UUIDs  U       = children . keys ( ) ;
  SUID   u                             ;
  qint64 i       = 0                   ;
  bool   dropped = false               ;
  while ( ForChild ( u , i , U ) )     {
    SceneObject * sox = children [ u ] ;
    if ( sox -> removeDrop ( ) )       {
      dropped = true                   ;
    }                                  ;
  }                                    ;
  return dropped                       ;
}

// 0 - Outside
// 1 - Inside
// 2 - left
// 3 - right
// 4 - top
// 5 - bottom
// 6 - top    / left
// 7 - top    / right
// 8 - bottom / left
// 9 - bottom / right

int N::SceneObject::RectRegion(ControlPoint & At)
{
  int    m = Value ( "Margin" ) . toInt ( ) ;
  double dx                                 ;
  double dy                                 ;
  ///////////////////////////////////////////
  if ( At . x < minimum . x ) return 0      ;
  if ( At . x > maximum . x ) return 0      ;
  if ( At . y < minimum . y ) return 0      ;
  if ( At . y > maximum . y ) return 0      ;
  ///////////////////////////////////////////
  bool left   = false                       ;
  bool right  = false                       ;
  bool top    = false                       ;
  bool bottom = false                       ;
  ///////////////////////////////////////////
  dx = At . x - minimum . x                 ;
  dy = At . y - minimum . y                 ;
  if ( dx < 0 ) dx     = -dx                ;
  if ( dx < m ) left   = true               ;
  if ( dy < 0 ) dy     = -dy                ;
  if ( dy < m ) bottom = true               ;
  ///////////////////////////////////////////
  dx = maximum . x - At . x                 ;
  dy = maximum . y - At . y                 ;
  if ( dx < 0 ) dx     = -dx                ;
  if ( dx < m ) right  = true               ;
  if ( dy < 0 ) dy     = -dy                ;
  if ( dy < m ) top    = true               ;
  ///////////////////////////////////////////
  if ( left  )                              {
    if ( top    ) return 6                  ;
    if ( bottom ) return 8                  ;
    return 2                                ;
  }                                         ;
  ///////////////////////////////////////////
  if ( right )                              {
    if ( top    ) return 7                  ;
    if ( bottom ) return 9                  ;
    return 3                                ;
  }                                         ;
  ///////////////////////////////////////////
  if   ( top    ) return 4                  ;
  if   ( bottom ) return 5                  ;
  ///////////////////////////////////////////
  return 1                                  ;
}

// 0 - Outside
// 1 - Inside

int N::SceneObject::InsideRegion(ControlPoint & At)
{
  if ( At . x < minimum . x ) return 0 ;
  if ( At . x > maximum . x ) return 0 ;
  if ( At . y < minimum . y ) return 0 ;
  if ( At . y > maximum . y ) return 0 ;
  //////////////////////////////////////
  return 1                             ;
}

int N::SceneObject::AtRegion(ControlPoint & At)
{
  return -1 ;
}

QString N::SceneObject::ToolTip(void)
{
  return "" ;
}

void N::SceneObject::AssignFocus(void)
{
  setFocus ( true ) ;
}

void N::SceneObject::BoxChanged(bool hover)
{
}

void N::SceneObject::FinalizeSelection(void)
{
}

void N::SceneObject::AssignCursor(int region)
{
  switch ( region )                                           {
    case 0                                                    : // 0 - Outside
      setCursor   ( Qt::ArrowCursor     )                     ;
    break                                                     ;
    case 1                                                    : // 1 - Inside
      if ( Value ( "Selectable" ) . toBool ( ) )              {
        setCursor ( Qt::CrossCursor     )                     ;
        return                                                ;
      }                                                       ;
      if ( Value ( "Movable" ) . toBool ( ) )                 {
        setCursor ( Qt::OpenHandCursor  )                     ;
        ToolTip   (                     )                     ;
      }                                                       ;
    break                                                     ;
    case 2                                                    : // 2 - left
      if ( Value ( "Selectable" ) . toBool ( ) )              {
        setCursor ( Qt::CrossCursor     )                     ;
        return                                                ;
      }                                                       ;
      if ( Value ( "Left/Line" ) . toBool ( ) )               {
        if ( Value ( "Resizable" ) . toBool ( ) )             {
          setCursor ( Qt::SizeHorCursor   )                   ;
          ToolTip   (                     )                   ;
        }                                                     ;
      }                                                       ;
    break                                                     ;
    case 3                                                    : // 3 - right
      if ( Value ( "Selectable" ) . toBool ( ) )              {
        setCursor ( Qt::CrossCursor     )                     ;
        return                                                ;
      }                                                       ;
      if ( Value ( "Right/Line" ) . toBool ( ) )              {
        if ( Value ( "Resizable" ) . toBool ( ) )             {
          setCursor ( Qt::SizeHorCursor   )                   ;
          ToolTip   (                     )                   ;
        }                                                     ;
      }                                                       ;
    break                                                     ;
    case 4                                                    : // 4 - top
      if ( Value ( "Selectable" ) . toBool ( ) )              {
        setCursor ( Qt::CrossCursor     )                     ;
        return                                                ;
      }                                                       ;
      if ( Value ( "Top/Line" ) . toBool ( ) )                {
        if ( Value ( "Resizable" ) . toBool ( ) )             {
          setCursor ( Qt::SizeVerCursor   )                   ;
          ToolTip   (                     )                   ;
        }                                                     ;
      }                                                       ;
    break                                                     ;
    case 5                                                    : // 5 - bottom
      if ( Value ( "Selectable" ) . toBool ( ) )              {
        setCursor ( Qt::CrossCursor     )                     ;
        return                                                ;
      }                                                       ;
      if ( Value ( "Bottom/Line" ) . toBool ( ) )             {
        if ( Value ( "Resizable" ) . toBool ( ) )             {
          setCursor ( Qt::SizeVerCursor   )                   ;
          ToolTip   (                     )                   ;
        }                                                     ;
      }                                                       ;
    break                                                     ;
    case 6                                                    : // 6 - top    / left
      if ( Value ( "Selectable" ) . toBool ( ) )              {
        setCursor ( Qt::CrossCursor     )                     ;
        return                                                ;
      }                                                       ;
      if ( Value ( "Top/Left" ) . toBool ( ) )                {
        if ( Value ( "Resizable" ) . toBool ( ) )             {
          setCursor ( Qt::SizeFDiagCursor )                   ;
          ToolTip   (                     )                   ;
        }                                                     ;
      }                                                       ;
    break                                                     ;
    case 7                                                    : // 7 - top    / right
      if ( Value ( "Selectable" ) . toBool ( ) )              {
        setCursor ( Qt::CrossCursor     )                     ;
        return                                                ;
      }                                                       ;
      if ( Value ( "Top/Right" ) . toBool ( ) )               {
        if ( Value ( "Resizable" ) . toBool ( ) )             {
          setCursor ( Qt::SizeBDiagCursor )                   ;
          ToolTip   (                     )                   ;
        }                                                     ;
      }                                                       ;
    break                                                     ;
    case 8                                                    : // 8 - bottom / left
      if ( Value ( "Selectable" ) . toBool ( ) )              {
        setCursor ( Qt::CrossCursor     )                     ;
        return                                                ;
      }                                                       ;
      if ( Value ( "Bottom/Left" ) . toBool ( ) )             {
        if ( Value ( "Resizable" ) . toBool ( ) )             {
          setCursor ( Qt::SizeBDiagCursor )                   ;
          ToolTip   (                     )                   ;
        }                                                     ;
      }                                                       ;
    break                                                     ;
    case 9                                                    : // 9 - bottom / right
      if ( Value ( "Selectable" ) . toBool ( ) )              {
        setCursor ( Qt::CrossCursor     )                     ;
        return                                                ;
      }                                                       ;
      if ( Value ( "Bottom/Right" ) . toBool ( ) )            {
        if ( Value ( "Resizable" ) . toBool ( ) )             {
          setCursor ( Qt::SizeFDiagCursor )                   ;
         ToolTip   (                     )                    ;
        }                                                     ;
      }                                                       ;
    break                                                     ;
  }                                                           ;
}

void N::SceneObject::MouseProcessor(bool hover)
{
  int          region = Value ( "HitRegion" ) . toInt ( )                    ;
  ControlPoint minp                                                          ;
  ControlPoint maxp                                                          ;
  QPointF      P1                                                            ;
  QPointF      P2                                                            ;
  QPointF      dP                                                            ;
  ////////////////////////////////////////////////////////////////////////////
  if ( hover )                                                               {
    switch ( region )                                                        {
      case 0                                                                 : // 0 - Outside
      break                                                                  ;
      case 1                                                                 : // 1 - Inside
        if ( Value ( "Movable" ) . toBool ( ) )                              {
          P1        = Value    ( "P1"         ) . toPointF ( )               ;
          P2        = Value    ( "P2"         ) . toPointF ( )               ;
          dP        = P2 - P1                                                ;
          minp      = GetPoint ( "OldMinimum" )                              ;
          maxp      = GetPoint ( "OldMaximum" )                              ;
          minp . x += dP .x ( )                                              ;
          maxp . x += dP .x ( )                                              ;
          minp . y += dP .y ( )                                              ;
          maxp . y += dP .y ( )                                              ;
          LockAt     ( "Operate" )                                           ;
          minimum   = minp                                                   ;
          maximum   = maxp                                                   ;
          Finish     (           )                                           ;
          Dual       (           )                                           ;
          UnlockAt   ( "Operate" )                                           ;
          BoxChanged ( hover     )                                           ;
        }                                                                    ;
      break                                                                  ;
      case 2                                                                 : // 2 - left
        if ( Value ( "Left/Line" ) . toBool ( ) )                            {
          if ( Value ( "Resizable" ) . toBool ( ) )                          {
            P1        = Value    ( "P1"         ) . toPointF ( )             ;
            P2        = Value    ( "P2"         ) . toPointF ( )             ;
            dP        = P2 - P1                                              ;
            minp      = GetPoint ( "OldMinimum" )                            ;
            minp . x += dP . x ( )                                           ;
            LockAt     ( "Operate" )                                         ;
            minimum   = minp                                                 ;
            Finish     (           )                                         ;
            Dual       (           )                                         ;
            UnlockAt   ( "Operate" )                                         ;
            BoxChanged ( hover     )                                         ;
          }                                                                  ;
        }                                                                    ;
      break                                                                  ;
      case 3                                                                 : // 3 - right
        if ( Value ( "Right/Line" ) . toBool ( ) )                           {
          if ( Value ( "Resizable" ) . toBool ( ) )                          {
            P1        = Value    ( "P1"         ) . toPointF ( )             ;
            P2        = Value    ( "P2"         ) . toPointF ( )             ;
            dP        = P2 - P1                                              ;
            maxp      = GetPoint ( "OldMaximum" )                            ;
            maxp . x += dP . x ( )                                           ;
            LockAt     ( "Operate" )                                         ;
            maximum   = maxp                                                 ;
            Finish     (           )                                         ;
            Dual       (           )                                         ;
            UnlockAt   ( "Operate" )                                         ;
            BoxChanged ( hover     )                                         ;
          }                                                                  ;
        }                                                                    ;
      break                                                                  ;
      case 4                                                                 : // 4 - top
        if ( Value ( "Top/Line" ) . toBool ( ) )                             {
          if ( Value ( "Resizable" ) . toBool ( ) )                          {
            P1        = Value    ( "P1"         ) . toPointF ( )             ;
            P2        = Value    ( "P2"         ) . toPointF ( )             ;
            dP        = P2 - P1                                              ;
            maxp      = GetPoint ( "OldMaximum" )                            ;
            maxp . y += dP . y ( )                                           ;
            LockAt     ( "Operate" )                                         ;
            maximum   = maxp                                                 ;
            Finish     (           )                                         ;
            Dual       (           )                                         ;
            UnlockAt   ( "Operate" )                                         ;
            BoxChanged ( hover     )                                         ;
          }                                                                  ;
        }                                                                    ;
      break                                                                  ;
      case 5                                                                 : // 5 - bottom
        if ( Value ( "Bottom/Line" ) . toBool ( ) )                          {
          if ( Value ( "Resizable" ) . toBool ( ) )                          {
            P1        = Value    ( "P1"         ) . toPointF ( )             ;
            P2        = Value    ( "P2"         ) . toPointF ( )             ;
            dP        = P2 - P1                                              ;
            minp      = GetPoint ( "OldMinimum" )                            ;
            minp . y += dP . y ( )                                           ;
            LockAt     ( "Operate" )                                         ;
            minimum   = minp                                                 ;
            Finish     (           )                                         ;
            Dual       (           )                                         ;
            UnlockAt   ( "Operate" )                                         ;
            BoxChanged ( hover     )                                         ;
          }                                                                  ;
        }                                                                    ;
      break                                                                  ;
      case 6                                                                 : // 6 - top    / left
        if ( Value ( "Top/Left" ) . toBool ( ) )                             {
          if ( Value ( "Resizable" ) . toBool ( ) )                          {
            P1        = Value    ( "P1"         ) . toPointF ( )             ;
            P2        = Value    ( "P2"         ) . toPointF ( )             ;
            dP        = P2 - P1                                              ;
            minp      = GetPoint ( "OldMinimum" )                            ;
            maxp      = GetPoint ( "OldMaximum" )                            ;
            minp . x += dP .x ( )                                            ;
            maxp . y += dP .y ( )                                            ;
            LockAt     ( "Operate" )                                         ;
            minimum   = minp                                                 ;
            maximum   = maxp                                                 ;
            Finish     (           )                                         ;
            Dual       (           )                                         ;
            UnlockAt   ( "Operate" )                                         ;
            BoxChanged ( hover     )                                         ;
          }                                                                  ;
        }                                                                    ;
      break                                                                  ;
      case 7                                                                 : // 7 - top    / right
        if ( Value ( "Top/Right" ) . toBool ( ) )                            {
          if ( Value ( "Resizable" ) . toBool ( ) )                          {
            P1        = Value    ( "P1"         ) . toPointF ( )             ;
            P2        = Value    ( "P2"         ) . toPointF ( )             ;
            dP        = P2 - P1                                              ;
            maxp      = GetPoint ( "OldMaximum" )                            ;
            maxp . x += dP .x ( )                                            ;
            maxp . y += dP .y ( )                                            ;
            LockAt     ( "Operate" )                                         ;
            maximum   = maxp                                                 ;
            Finish     (           )                                         ;
            Dual       (           )                                         ;
            UnlockAt   ( "Operate" )                                         ;
            BoxChanged ( hover     )                                         ;
          }                                                                  ;
        }                                                                    ;
      break                                                                  ;
      case 8                                                                 : // 8 - bottom / left
        if ( Value ( "Bottom/Left" ) . toBool ( ) )                          {
          if ( Value ( "Resizable" ) . toBool ( ) )                          {
            P1        = Value    ( "P1"         ) . toPointF ( )             ;
            P2        = Value    ( "P2"         ) . toPointF ( )             ;
            dP        = P2 - P1                                              ;
            minp      = GetPoint ( "OldMinimum" )                            ;
            minp . x += dP .x ( )                                            ;
            minp . y += dP .y ( )                                            ;
            LockAt     ( "Operate" )                                         ;
            minimum   = minp                                                 ;
            Finish     (           )                                         ;
            Dual       (           )                                         ;
            UnlockAt   ( "Operate" )                                         ;
            BoxChanged ( hover     )                                         ;
          }                                                                  ;
        }                                                                    ;
      break                                                                  ;
      case 9                                                                 : // 9 - bottom / right
        if ( Value ( "Bottom/Right" ) . toBool ( ) )                         {
          if ( Value ( "Resizable" ) . toBool ( ) )                          {
            P1        = Value    ( "P1"         ) . toPointF ( )             ;
            P2        = Value    ( "P2"         ) . toPointF ( )             ;
            dP        = P2 - P1                                              ;
            minp      = GetPoint ( "OldMinimum" )                            ;
            maxp      = GetPoint ( "OldMaximum" )                            ;
            maxp . x += dP .x ( )                                            ;
            minp . y += dP .y ( )                                            ;
            LockAt     ( "Operate" )                                         ;
            minimum   = minp                                                 ;
            maximum   = maxp                                                 ;
            Finish     (           )                                         ;
            Dual       (           )                                         ;
            UnlockAt   ( "Operate" )                                         ;
            BoxChanged ( hover     )                                         ;
          }                                                                  ;
        }                                                                    ;
      break                                                                  ;
    }                                                                        ;
    return                                                                   ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  switch ( region )                                                          {
    case 0                                                                   : // 0 - Outside
    break                                                                    ;
    case 1                                                                   : // 1 - Inside
      if ( Value ( "Movable" ) . toBool ( )  )                               {
        setCursor   ( Qt::ClosedHandCursor   )                               ;
        AssignPoint ( "OldMinimum" , minimum )                               ;
        AssignPoint ( "OldMaximum" , maximum )                               ;
      }                                                                      ;
    break                                                                    ;
    case 2                                                                   : // 2 - left
      if ( Value ( "Left/Line" ) . toBool ( ) )                              {
        if ( Value ( "Resizable" ) . toBool ( )  )                           {
          AssignPoint ( "OldMinimum" , minimum )                             ;
          AssignPoint ( "OldMaximum" , maximum )                             ;
        }                                                                    ;
      }                                                                      ;
    break                                                                    ;
    case 3                                                                   : // 3 - right
      if ( Value ( "Right/Line" ) . toBool ( ) )                             {
        if ( Value ( "Resizable" ) . toBool ( )  )                           {
          AssignPoint ( "OldMinimum" , minimum )                             ;
          AssignPoint ( "OldMaximum" , maximum )                             ;
        }                                                                    ;
      }                                                                      ;
    break                                                                    ;
    case 4                                                                   : // 4 - top
      if ( Value ( "Top/Line" ) . toBool ( ) )                               {
        if ( Value ( "Resizable" ) . toBool ( )  )                           {
          AssignPoint ( "OldMinimum" , minimum )                             ;
          AssignPoint ( "OldMaximum" , maximum )                             ;
        }                                                                    ;
      }                                                                      ;
    break                                                                    ;
    case 5                                                                   : // 5 - bottom
      if ( Value ( "Bottom/Line" ) . toBool ( ) )                            {
        if ( Value ( "Resizable" ) . toBool ( )  )                           {
          AssignPoint ( "OldMinimum" , minimum )                             ;
          AssignPoint ( "OldMaximum" , maximum )                             ;
        }                                                                    ;
      }                                                                      ;
    break                                                                    ;
    case 6                                                                   : // 6 - top    / left
      if ( Value ( "Top/Left" ) . toBool ( ) )                               {
        if ( Value ( "Resizable" ) . toBool ( )  )                           {
          AssignPoint ( "OldMinimum" , minimum )                             ;
          AssignPoint ( "OldMaximum" , maximum )                             ;
        }                                                                    ;
      }                                                                      ;
    break                                                                    ;
    case 7                                                                   : // 7 - top    / right
      if ( Value ( "Top/Right" ) . toBool ( ) )                              {
        if ( Value ( "Resizable" ) . toBool ( )  )                           {
          AssignPoint ( "OldMinimum" , minimum )                             ;
          AssignPoint ( "OldMaximum" , maximum )                             ;
        }                                                                    ;
      }                                                                      ;
    break                                                                    ;
    case 8                                                                   : // 8 - bottom / left
      if ( Value ( "Bottom/Left" ) . toBool ( ) )                            {
        if ( Value ( "Resizable" ) . toBool ( )  )                           {
          AssignPoint ( "OldMinimum" , minimum )                             ;
          AssignPoint ( "OldMaximum" , maximum )                             ;
        }                                                                    ;
      }                                                                      ;
    break                                                                    ;
    case 9                                                                   : // 9 - bottom / right
      if ( Value ( "Bottom/Right" ) . toBool ( ) )                           {
        if ( Value ( "Resizable" ) . toBool ( )  )                           {
          AssignPoint ( "OldMinimum" , minimum )                             ;
          AssignPoint ( "OldMaximum" , maximum )                             ;
        }                                                                    ;
      }                                                                      ;
    break                                                                    ;
  }                                                                          ;
}

bool N::SceneObject::ScalingRegion (
       ControlPoint & From         ,
       ControlPoint & To           ,
       ControlPoint & At           ,
       BMAPs        & buttons      ,
       bool           hover        )
{
  QPointF AT ( At . x , At . y )                                             ;
  if ( hover )                                                               {
    if ( Value ( "Hit" ) . toBool ( ) )                                      {
      setVariable        ( "P2"        , AT     )                            ;
      MouseProcessor     ( true                 )                            ;
      ToolTip            (                      )                            ;
      return true                                                            ;
    }                                                                        ;
    return false                                                             ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  if ( buttons [ Universe::MouseLeft   ] )                                   {
    AssignFocus ( )                                                          ;
    if ( Value ( "Hit" ) . toBool ( ) )                                      {
      setVariable        ( "P2"        , AT     )                            ;
      ToolTip            (                      )                            ;
      return true                                                            ;
    } else                                                                   {
      int region = AtRegion ( At )                                           ;
      setVariable        ( "HitRegion" , region )                            ;
      AssignCursor       ( region               )                            ;
      switch ( region )                                                      {
        case 0                                                               :
          setVariable    ( "Hit"       , false  )                            ;
        break                                                                ;
        default                                                              :
          setVariable    ( "P1"        , AT     )                            ;
          setVariable    ( "P2"        , AT     )                            ;
          setVariable    ( "Hit"       , true   )                            ;
          setVariable    ( "Grab"      , true   )                            ;
          MouseProcessor ( false                )                            ;
          ToolTip        (                      )                            ;
        break                                                                ;
      }                                                                      ;
    }                                                                        ;
  } else                                                                     {
    if ( Value ( "Hit" ) . toBool ( ) )                                      {
      setCursor          ( Qt::ArrowCursor      )                            ;
      setVariable        ( "Hit"       , false  )                            ;
      setVariable        ( "Grab"      , false  )                            ;
      return true                                                            ;
    }                                                                        ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  return true                                                                ;
}

bool N::SceneObject::SelectRegion (
       ControlPoint & From        ,
       ControlPoint & To          ,
       ControlPoint & At          ,
       BMAPs        & buttons     ,
       bool           hover       )
{
  QPointF AT ( At . x , At . y )                                             ;
  if ( hover )                                                               {
    if ( Value ( "Hit" ) . toBool ( ) )                                      {
      setVariable         ( "P2"        , AT     )                           ;
      ToolTip             (                      )                           ;
      return true                                                            ;
    }                                                                        ;
    return false                                                             ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  if ( buttons [ Universe::MouseLeft   ] )                                   {
    AssignFocus ( )                                                          ;
    if ( Value ( "Hit" ) . toBool ( ) )                                      {
      setVariable         ( "P2"        , AT     )                           ;
      ToolTip             (                      )                           ;
      return true                                                            ;
    } else                                                                   {
      int region = AtRegion ( At )                                           ;
      setVariable         ( "HitRegion" , region )                           ;
      AssignCursor        ( region               )                           ;
      switch ( region )                                                      {
        case 0                                                               :
          setVariable     ( "Hit"       , false  )                           ;
        break                                                                ;
        default                                                              :
          setVariable     ( "P1"        , AT     )                           ;
          setVariable     ( "P2"        , AT     )                           ;
          setVariable     ( "Hit"       , true   )                           ;
          setVariable     ( "Grab"      , true   )                           ;
          ToolTip         (                      )                           ;
        break                                                                ;
      }                                                                      ;
    }                                                                        ;
  } else                                                                     {
    if ( Value ( "Hit" ) . toBool ( ) )                                      {
      setCursor           ( Qt::ArrowCursor      )                           ;
      setVariable         ( "Hit"       , false  )                           ;
      setVariable         ( "Grab"      , false  )                           ;
      FinalizeSelection   (                      )                           ;
      return true                                                            ;
    }                                                                        ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  return true                                                                ;
}

bool N::SceneObject::MouseEditing (
       ControlPoint & From        ,
       ControlPoint & To          ,
       ControlPoint & At          ,
       BMAPs        & buttons     ,
       bool           hover       )
{
  if ( Value ( "Selectable" ) . toBool ( ) )                  {
    return SelectRegion  ( From , To , At , buttons , hover ) ;
  }                                                           ;
  return   ScalingRegion ( From , To , At , buttons , hover ) ;
}

void N::SceneObject::renderSelectRect(void)
{
  if ( ! Value ( "Selectable" ) . toBool ( ) ) return ;
  if ( ! Value ( "Hit"        ) . toBool ( ) ) return ;
  int region = Value ( "HitRegion" ) . toInt ( )      ;
  if ( region <= 0 ) return                           ;
  /////////////////////////////////////////////////////
  QPointF P1 = Value ( "P1" ) . toPointF ( )          ;
  QPointF P2 = Value ( "P2" ) . toPointF ( )          ;
  double  z  = maximum . z + 0.01                     ;
  double  x1                                          ;
  double  x2                                          ;
  double  y1                                          ;
  double  y2                                          ;
  /////////////////////////////////////////////////////
  if ( P1 . x ( ) > P2 . x ( ) )                      {
    x1 = P2 . x ( )                                   ;
    x2 = P1 . x ( )                                   ;
  } else                                              {
    x1 = P1 . x ( )                                   ;
    x2 = P2 . x ( )                                   ;
  }                                                   ;
  if ( P1 . y ( ) > P2 . y ( ) )                      {
    y1 = P2 . y ( )                                   ;
    y2 = P1 . y ( )                                   ;
  } else                                              {
    y1 = P1 . y ( )                                   ;
    y2 = P2 . y ( )                                   ;
  }                                                   ;
  /////////////////////////////////////////////////////
  QtGL::PushMatrix    (                             ) ;
  assumeColor         ( "SelectionColor"            ) ;
  QtGL::Quadrilateral (                             ) ;
  QtGL::Vertex3d      ( x1 , y1 , z                 ) ;
  QtGL::Vertex3d      ( x2 , y1 , z                 ) ;
  QtGL::Vertex3d      ( x2 , y2 , z                 ) ;
  QtGL::Vertex3d      ( x1 , y2 , z                 ) ;
  QtGL::End           (                             ) ;
  QtGL::PopMatrix     (                             ) ;
}

QRectF N::SceneObject::SelectionArea(void)
{
  QRectF  R                                  ;
  QPointF P1 = Value ( "P1" ) . toPointF ( ) ;
  QPointF P2 = Value ( "P2" ) . toPointF ( ) ;
  ////////////////////////////////////////////
  if ( P1 . x ( ) > P2 . x ( ) )             {
    R . setLeft   ( P2 . x ( ) )             ;
    R . setRight  ( P1 . x ( ) )             ;
  } else                                     {
    R . setLeft   ( P1 . x ( ) )             ;
    R . setRight  ( P2 . x ( ) )             ;
  }                                          ;
  if ( P1 . y ( ) > P2 . y ( ) )             {
    R . setTop    ( P2 . y ( ) )             ;
    R . setBottom ( P1 . y ( ) )             ;
  } else                                     {
    R . setTop    ( P1 . y ( ) )             ;
    R . setBottom ( P2 . y ( ) )             ;
  }                                          ;
  ////////////////////////////////////////////
  return R                                   ;
}

QImage * N::SceneObject::CurrentImage(QString key)
{
  if ( ! Variables . contains ( key ) ) return NULL ;
  SUID  u = Value ( key ) . toULongLong ( )         ;
  ///////////////////////////////////////////////////
  if ( ! textures . contains ( u ) ) return NULL    ;
  SceneTexture * t = textures [ u ]                 ;
  if ( IsNull ( t )  ) return NULL                  ;
  ///////////////////////////////////////////////////
  QImage * image = t -> original                    ;
  if ( IsNull(image) ) return NULL                  ;
  ///////////////////////////////////////////////////
  return image                                      ;
}

bool N::SceneObject::Relocation(QRectF region)
{
  return false ;
}

bool N::SceneObject::CentralRelocation(QRectF vw)
{
  if ( ! hasFunction ( "FollowWindow" ) ) return false ;
  double ww = vw . width  ( )                          ;
  double hh = vw . height ( )                          ;
  maximum    . x =     ww                              ;
  maximum    . y =     hh                              ;
  DOFs [ 0 ] . x = - ( ww / 2 )                        ;
  DOFs [ 0 ] . y = - ( hh / 2 )                        ;
  Finish             (        )                        ;
  Dual               (        )                        ;
  BoxChanged         ( true   )                        ;
  return true                                          ;
}

bool N::SceneObject::Spot  (
       ControlPoint & From ,
       ControlPoint & To   ,
       ControlPoint & At   )
{
  return false ;
}

bool N::SceneObject::Collide (
       ControlPoint & From   ,
       ControlPoint & To     ,
       ControlPoint & At     )
{
  if ( ! matrices . contains ( 2 ) ) return false ;
  /////////////////////////////////////////////////
  ControlPoint P1                                 ;
  ControlPoint P2                                 ;
  P1 = matrices [ 2 ] * From                      ;
  P2 = matrices [ 2 ] * To                        ;
  return Spot ( P1 , P2 , At )                    ;
}

SUID N::SceneObject::Nearest (
       ControlPoint & From   ,
       ControlPoint & To     ,
       ControlPoint & At     )
{
  if ( children . count ( ) <= 0 ) return 0        ;
  UUIDs             P = children . keys ( )        ;
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
    SceneObject * sox = children [ u ]             ;
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

N::ControlPoint N::SceneObject::GetPoint(QString key)
{
  ControlPoint p                      ;
  /////////////////////////////////////
  if ( Variables . contains ( key ) ) {
    int i = Value ( key ) . toInt ( ) ;
    p = points [ i ]                  ;
  }                                   ;
  /////////////////////////////////////
  return p                            ;
}

int N::SceneObject::AssignPoint(QString key,ControlPoint & p)
{
  int i                               ;
  /////////////////////////////////////
  if ( Variables . contains ( key ) ) {
    i = Value ( key ) . toInt ( )     ;
    index        << i                 ;
    points [ i ]  = p                 ;
  } else                              {
    i = Last ( SceneObject::Point )   ;
    index        << i                 ;
    points [ i ]  = p                 ;
    setVariable ( key , i )           ;
  }                                   ;
  /////////////////////////////////////
  return i                            ;
}

bool N::SceneObject::hasDrag(void)
{
  return false ;
}

bool N::SceneObject::doDrag(void)
{
  return false ;
}

bool N::SceneObject::Menu    (
       QWidget      * widget ,
       QPointF        pos    ,
       ControlPoint & From   ,
       ControlPoint & To     )
{
  return false ;
}

bool N::SceneObject::ChildMenu (
       QWidget      * widget   ,
       QPointF        pos      ,
       ControlPoint & From     ,
       ControlPoint & To       )
{
  ControlPoint At                               ;
  ControlPoint p1                               ;
  ControlPoint p2                               ;
  SUID         u                                ;
  ///////////////////////////////////////////////
  p1  = matrices [ 2 ] * From                   ;
  p2  = matrices [ 2 ] * To                     ;
  u = Nearest ( p1 , p2 , At )                  ;
  if ( u <= 0 ) return false                    ;
  ///////////////////////////////////////////////
  SceneObject * sox                             ;
  sox = ChildOf<SceneObject>(u)                 ;
  if ( IsNull(sox) ) return false               ;
  return sox -> Menu ( widget , pos , p1 , p2 ) ;
}

void N::SceneObject::FeatureCube(ControlPoint & minp,ControlPoint & maxp)
{
  if             ( Value ( "Back"  ) . toBool ( ) ) { // Back
    assumeColor  ( "BackColor"                    ) ;
    CubeVertex   ( 100 , minp , maxp              ) ;
  }                                                 ;
  ///////////////////////////////////////////////////
  if             ( Value ( "Front" ) . toBool ( ) ) { // Front
    assumeColor  ( "FrontColor"                   ) ;
    CubeVertex   ( 101 , minp , maxp              ) ;
  }                                                 ;
  ///////////////////////////////////////////////////
  if             ( Value ( "Side"  ) . toBool ( ) ) { // Side
    assumeColor  ( "SideColor"                    ) ;
    for (int i = 102 ; i <= 105 ; i++ )             {
      CubeVertex (   i , minp , maxp              ) ;
    }                                               ;
  }                                                 ;
}

N::ControlPoint N::SceneObject::Mix(ControlPoint & P1,ControlPoint & P2,double factor)
{
  ControlPoint ps                ;
  double       mf = 1.0 - factor ;
  ////////////////////////////////
  ps . x  = P1 . x * factor      ;
  ps . y  = P1 . y * factor      ;
  ps . z  = P1 . z * factor      ;
  ps . t  = P1 . t * factor      ;
  ps . x += P2 . x * mf          ;
  ps . y += P2 . y * mf          ;
  ps . z += P2 . z * mf          ;
  ps . t += P2 . t * mf          ;
  ////////////////////////////////
  return ps                      ;
}

void N::SceneObject::GradientBox(ControlPoints & P)
{
  int          scale = Value ( "Scale" ) . toInt ( )                         ;
  ControlPoint P1    = P [ 0 ]                                               ;
  ControlPoint P2    = P [ 1 ]                                               ;
  ControlPoint C1    = P [ 2 ]                                               ;
  ControlPoint C2    = P [ 3 ]                                               ;
  ControlPoint c1                                                            ;
  ControlPoint c2                                                            ;
  double       s     = scale                                                 ;
  double       L                                                             ;
  ////////////////////////////////////////////////////////////////////////////
  QtGL::Disable ( GL_LINE_SMOOTH    )                                        ;
  QtGL::Disable ( GL_POLYGON_SMOOTH )                                        ;
  ////////////////////////////////////////////////////////////////////////////
  L = P2 . y - P1 . y                                                        ;
  for (int i = 0 ; i < scale ; i++ )                                         {
    int    n1 = i                                                            ;
    int    n2 = n1 + 1                                                       ;
    double s1 = ( (double) n1 ) / s                                          ;
    double s2 = ( (double) n2 ) / s                                          ;
    double a1 = ( s1 * 180.0  )                                              ;
    double a2 = ( s2 * 180.0  )                                              ;
    double f1 = Math::fastCosine ( a1 )                                      ;
    double f2 = Math::fastCosine ( a2 )                                      ;
    if ( f1 < 0 ) f1 = -f1                                                   ;
    if ( f2 < 0 ) f2 = -f2                                                   ;
    c1  = Mix ( C1 , C2 , f1 )                                               ;
    c2  = Mix ( C1 , C2 , f2 )                                               ;
    s1 *= L                                                                  ;
    s2 *= L                                                                  ;
    s1 += P1 . y                                                             ;
    s2 += P1 . y                                                             ;
    QtGL::Quadrilateral (                      )                             ;
    c1  . Color4d       (                      )                             ;
    QtGL::Vertex3d      ( P1 . x , s1 , P2 . z )                             ;
    c1  . Color4d       (                      )                             ;
    QtGL::Vertex3d      ( P2 . x , s1 , P2 . z )                             ;
    c2  . Color4d       (                      )                             ;
    QtGL::Vertex3d      ( P2 . x , s2 , P2 . z )                             ;
    c2  . Color4d       (                      )                             ;
    QtGL::Vertex3d      ( P1 . x , s2 , P2 . z )                             ;
    QtGL::End           (                      )                             ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  L = P2 . y - P1 . y                                                        ;
  for (int i = 0 ; i < scale ; i++ )                                         {
    int    n1 = i                                                            ;
    int    n2 = n1 + 1                                                       ;
    double s1 = ( (double) n1 ) / s                                          ;
    double s2 = ( (double) n2 ) / s                                          ;
    double a1 = ( s1 * 180.0  )                                              ;
    double a2 = ( s2 * 180.0  )                                              ;
    double f1 = Math::fastCosine ( a1 )                                      ;
    double f2 = Math::fastCosine ( a2 )                                      ;
    if ( f1 < 0 ) f1 = -f1                                                   ;
    if ( f2 < 0 ) f2 = -f2                                                   ;
    c1  = Mix ( C1 , C2 , f1 )                                               ;
    c2  = Mix ( C1 , C2 , f2 )                                               ;
    s1 *= L                                                                  ;
    s2 *= L                                                                  ;
    s1 += P1 . y                                                             ;
    s2 += P1 . y                                                             ;
    QtGL::Quadrilateral (                      )                             ;
    c1  . Color4d       (                      )                             ;
    QtGL::Vertex3d      ( P1 . x , s1 , P1 . z )                             ;
    c1  . Color4d       (                      )                             ;
    QtGL::Vertex3d      ( P2 . x , s1 , P1 . z )                             ;
    c2  . Color4d       (                      )                             ;
    QtGL::Vertex3d      ( P2 . x , s2 , P1 . z )                             ;
    c2  . Color4d       (                      )                             ;
    QtGL::Vertex3d      ( P1 . x , s2 , P1 . z )                             ;
    QtGL::End           (                      )                             ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  L = P2 . z - P1 . z                                                        ;
  for (int i = 0 ; i < scale ; i++ )                                         {
    int    n1 = i                                                            ;
    int    n2 = n1 + 1                                                       ;
    double s1 = ( (double) n1 ) / s                                          ;
    double s2 = ( (double) n2 ) / s                                          ;
    double a1 = ( s1 * 180.0  )                                              ;
    double a2 = ( s2 * 180.0  )                                              ;
    double f1 = Math::fastCosine ( a1 )                                      ;
    double f2 = Math::fastCosine ( a2 )                                      ;
    if ( f1 < 0 ) f1 = -f1                                                   ;
    if ( f2 < 0 ) f2 = -f2                                                   ;
    c1  = Mix ( C1 , C2 , f1 )                                               ;
    c2  = Mix ( C1 , C2 , f2 )                                               ;
    s1 *= L                                                                  ;
    s2 *= L                                                                  ;
    s1 += P1 . z                                                             ;
    s2 += P1 . z                                                             ;
    QtGL::Quadrilateral (                      )                             ;
    c2  . Color4d       (                      )                             ;
    QtGL::Vertex3d      ( P1 . x , P1 . y , s2 )                             ;
    c1  . Color4d       (                      )                             ;
    QtGL::Vertex3d      ( P1 . x , P1 . y , s1 )                             ;
    c1  . Color4d       (                      )                             ;
    QtGL::Vertex3d      ( P2 . x , P1 . y , s1 )                             ;
    c2  . Color4d       (                      )                             ;
    QtGL::Vertex3d      ( P2 . x , P1 . y , s2 )                             ;
    QtGL::End           (                      )                             ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  L = P2 . z - P1 . z                                                        ;
  for (int i = 0 ; i < scale ; i++ )                                         {
    int    n1 = i                                                            ;
    int    n2 = n1 + 1                                                       ;
    double s1 = ( (double) n1 ) / s                                          ;
    double s2 = ( (double) n2 ) / s                                          ;
    double a1 = ( s1 * 180.0  )                                              ;
    double a2 = ( s2 * 180.0  )                                              ;
    double f1 = Math::fastCosine ( a1 )                                      ;
    double f2 = Math::fastCosine ( a2 )                                      ;
    if ( f1 < 0 ) f1 = -f1                                                   ;
    if ( f2 < 0 ) f2 = -f2                                                   ;
    c1  = Mix ( C1 , C2 , f1 )                                               ;
    c2  = Mix ( C1 , C2 , f2 )                                               ;
    s1 *= L                                                                  ;
    s2 *= L                                                                  ;
    s1 += P1 . z                                                             ;
    s2 += P1 . z                                                             ;
    QtGL::Quadrilateral (                      )                             ;
    c2  . Color4d       (                      )                             ;
    QtGL::Vertex3d      ( P1 . x , P2 . y , s2 )                             ;
    c2  . Color4d       (                      )                             ;
    QtGL::Vertex3d      ( P2 . x , P2 . y , s2 )                             ;
    c1  . Color4d       (                      )                             ;
    QtGL::Vertex3d      ( P2 . x , P2 . y , s1 )                             ;
    c1  . Color4d       (                      )                             ;
    QtGL::Vertex3d      ( P1 . x , P2 . y , s1 )                             ;
    QtGL::End           (                      )                             ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  L = P2 . y - P1 . y                                                        ;
  for (int i = 0 ; i < scale ; i++ )                                         {
    int    n1 = i                                                            ;
    int    n2 = n1 + 1                                                       ;
    double s1 = ( (double) n1 ) / s                                          ;
    double s2 = ( (double) n2 ) / s                                          ;
    double a1 = ( s1 * 180.0  )                                              ;
    double a2 = ( s2 * 180.0  )                                              ;
    double f1 = Math::fastCosine ( a1 )                                      ;
    double f2 = Math::fastCosine ( a2 )                                      ;
    if ( f1 < 0 ) f1 = -f1                                                   ;
    if ( f2 < 0 ) f2 = -f2                                                   ;
    c1  = Mix ( C1 , C2 , f1 )                                               ;
    c2  = Mix ( C1 , C2 , f2 )                                               ;
    s1 *= L                                                                  ;
    s2 *= L                                                                  ;
    s1 += P1 . y                                                             ;
    s2 += P1 . y                                                             ;
    QtGL::Quadrilateral (                      )                             ;
    c1  . Color4d       (                      )                             ;
    QtGL::Vertex3d      ( P1 . x , s1 , P1 . z )                             ;
    c1  . Color4d       (                      )                             ;
    QtGL::Vertex3d      ( P1 . x , s1 , P2 . z )                             ;
    c2  . Color4d       (                      )                             ;
    QtGL::Vertex3d      ( P1 . x , s2 , P2 . z )                             ;
    c2  . Color4d       (                      )                             ;
    QtGL::Vertex3d      ( P1 . x , s2 , P1 . z )                             ;
    QtGL::End           (                      )                             ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  L = P2 . y - P1 . y                                                        ;
  for (int i = 0 ; i < scale ; i++ )                                         {
    int    n1 = i                                                            ;
    int    n2 = n1 + 1                                                       ;
    double s1 = ( (double) n1 ) / s                                          ;
    double s2 = ( (double) n2 ) / s                                          ;
    double a1 = ( s1 * 180.0  )                                              ;
    double a2 = ( s2 * 180.0  )                                              ;
    double f1 = Math::fastCosine ( a1 )                                      ;
    double f2 = Math::fastCosine ( a2 )                                      ;
    if ( f1 < 0 ) f1 = -f1                                                   ;
    if ( f2 < 0 ) f2 = -f2                                                   ;
    c1  = Mix ( C1 , C2 , f1 )                                               ;
    c2  = Mix ( C1 , C2 , f2 )                                               ;
    s1 *= L                                                                  ;
    s2 *= L                                                                  ;
    s1 += P1 . y                                                             ;
    s2 += P1 . y                                                             ;
    QtGL::Quadrilateral (                      )                             ;
    c1  . Color4d       (                      )                             ;
    QtGL::Vertex3d      ( P2 . x , s1 , P2 . z )                             ;
    c1  . Color4d       (                      )                             ;
    QtGL::Vertex3d      ( P2 . x , s1 , P1 . z )                             ;
    c2  . Color4d       (                      )                             ;
    QtGL::Vertex3d      ( P2 . x , s2 , P1 . z )                             ;
    c2  . Color4d       (                      )                             ;
    QtGL::Vertex3d      ( P2 . x , s2 , P2 . z )                             ;
    QtGL::End           (                      )                             ;
  }                                                                          ;
}
