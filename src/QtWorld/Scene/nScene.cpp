#include <Cognitive>
#include "QtOgre.hpp"

#define QOGRE ((QtOgre *)OgreData)

nScene:: nScene    (QObject * parent,nPlan * plan)
      :  QObject   (          parent             )
      ,  Plan      (                         plan)
      ,  Ready     (false                        )
      ,  Running   (false                        )
      ,  GL        (NULL                         )
      ,  OgreData  (new QtOgre()                 )
{
}

nScene::~nScene(void)
{
}

nScene & nScene::operator = (nScene & scene)
{
  Ready    = scene.Ready                 ;
  Running  = scene.Running               ;
  Plan     = scene.Plan                  ;
  GL       = scene.GL                    ;
  (*QOGRE) = *((QtOgre *)scene.OgreData) ;
//  setParent  ( scene . parent () )       ;
  return (*this)                         ;
}

bool nScene::Initialize(int width,int height)
{
  if (IsNull(Plan) ) return false                                         ;
  if (!Plan->Scene ) return false                                         ;
  if (IsNull(QOGRE)) return false                                         ;
  QString App         = Plan->Application                                 ;
  QString Plugins     = Plan->RenderPlugins                               ;
  QString Renderer    = Plan->Renderer                                    ;
  QString Conf        = Plan->Path(QString("User/%1.cfg").arg(App))       ;
  QString Log         = Plan->Path(QString("Temp/%1.log").arg(App))       ;
  Ogre::String Plugin = Plugins . toStdString ( )                         ;
  Ogre::String OsConf = Conf    . toStdString ( )                         ;
  Ogre::String OsLog  = Log     . toStdString ( )                         ;
  /////////////////////////////////////////////////////////////////////////
  QOGRE->Root = new Ogre::Root ( Plugin , OsConf , OsLog )                ;
  Ogre::RenderSystemList renderers = QOGRE->Root->getAvailableRenderers() ;
  if (renderers.empty())                                                  {
    delete QOGRE->Root                                                    ;
    QOGRE->Root = NULL                                                    ;
    return false                                                          ;
  }                                                                       ;
  /////////////////////////////////////////////////////////////////////////
  Ogre::RenderSystem * renderSystem = NULL                                ;
  Ogre::RenderSystem * rs           = NULL                                ;
  for (Ogre::RenderSystemList::iterator it=renderers.begin()              ;
       IsNull(renderSystem) && it!=renderers.end();it++                 ) {
    QString name                                                          ;
    rs = (*it)                                                            ;
    name = QString::fromStdString(rs->getName())                          ;
    if (name==Renderer) renderSystem = rs                                 ;
  }                                                                       ;
  if (IsNull(renderSystem)) return false                                  ;
  QOGRE -> Root -> setRenderSystem ( renderSystem )                       ;
  /////////////////////////////////////////////////////////////////////////
  QString dimensions = QString("%1x%2").arg(width).arg(height)            ;
  renderSystem -> setConfigOption                                         (
    "Video Mode",dimensions.toStdString()                               ) ;
  QOGRE->Root->getRenderSystem()->setConfigOption ( "Full Screen","No"  ) ;
  /////////////////////////////////////////////////////////////////////////
  QOGRE->Root->initialise (false)                                         ;
  return true                                                             ;
}

WId nScene::createWinId(unsigned long wid,int width,int height)
{
  Ogre::String            winHandle                        ;
  Ogre::NameValuePairList params                           ;
  winHandle += Ogre::StringConverter::toString(wid)        ;
  params["parentWindowHandle"] = winHandle                 ;
  params["border"            ] = "none"                    ;
  QOGRE->Window = QOGRE->Root->createRenderWindow          (
             WinWidgetString().toStdString()               ,
             width                                         ,
             height                                        ,
             false                                         ,
             &params                                     ) ;
  QOGRE->Window->setActive(true)                           ;
  WId ogreWinId = 0x0                                      ;
  QOGRE->Window->getCustomAttribute("WINDOW",&ogreWinId)   ;
  return ogreWinId                                         ;
}

QString nScene::WinWidgetString(void) const
{
  return "QOgreWidgetRenderWindow";
}

QColor nScene::background(void) const
{
  return QColor(204,204,255) ;
}

QColor nScene::ambient(void) const
{
  return QColor(255,255,255) ;
}

void * nScene::defaultCamera(QString CameraName)
{
  Ogre::Camera * camera                                ;
  Ogre::String   name = CameraName . toStdString (   ) ;
  camera  = QOGRE->Scene->createCamera (name         ) ;
  camera -> setPosition         (Ogre::Vector3(5,5,5)) ;
  camera -> lookAt              (Ogre::Vector3(0,0,0)) ;
  camera -> setNearClipDistance (1.0                 ) ;
  return (void *)camera                                ;
}

void nScene::OpenScene(void)
{
  if (Ready) return                                                            ;
  Ogre::SceneType scene_manager_type = Ogre::ST_EXTERIOR_CLOSE                 ;
  QOGRE->Scene     = QOGRE->Root -> createSceneManager  ( scene_manager_type ) ;
  QOGRE->Scene            -> setAmbientLight    ( QOGRE->Color(ambient   ()) ) ;
  QOGRE->Camera    = (Ogre::Camera *) defaultCamera (                        ) ;
  QOGRE->Viewport  = QOGRE -> Window -> addViewport ( QOGRE->Camera          ) ;
  QOGRE->Viewport -> setBackgroundColour        ( QOGRE->Color(background()) ) ;
  Ready            = true                                                      ;
  InitializeObjects ( )                                                        ;
}

bool nScene::focusInEvent(QFocusEvent * event)
{
  return false ;
}

bool nScene::focusOutEvent(QFocusEvent * event)
{
  return false ;
}

bool nScene::enterEvent(QEvent * event)
{
  return false ;
}

bool nScene::leaveEvent(QEvent * event)
{
  return false ;
}

bool nScene::mousePressEvent(QMouseEvent * event)
{
  return false ;
}

bool nScene::mouseMoveEvent(QMouseEvent * event)
{
  return false ;
}

bool nScene::mouseDoubleClickEvent(QMouseEvent * event)
{
  return false ;
}

bool nScene::mouseReleaseEvent(QMouseEvent * event)
{
  return false ;
}

bool nScene::wheelEvent(QWheelEvent * event)
{
  return false ;
}

bool nScene::tabletEvent(QTabletEvent * event)
{
  return false ;
}

bool nScene::contextMenuEvent(QContextMenuEvent * event)
{
  return false ;
}

bool nScene::keyPressEvent(QKeyEvent * event)
{
  return false ;
}

bool nScene::keyReleaseEvent(QKeyEvent * event)
{
  return false ;
}

bool nScene::Resize(int width,int height)
{
  if (!Ready        ) return false           ;
  if (IsNull(QOGRE )) return false           ;
  QOGRE -> MovedOrResized ( width , height ) ;
  return true                                ;
}

bool nScene::Paint(void)
{
  if (!Ready        ) return false ;
  if (IsNull(QOGRE )) return false ;
  QOGRE -> Render ( )              ;
  return true                      ;
}

bool nScene::Frame(void)
{
  return false ;
}

void nScene::InitializeObjects(void)
{
}
