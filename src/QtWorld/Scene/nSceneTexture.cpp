#include <qtworld.h>

N::SceneTexture:: SceneTexture ( void           )
                : SceneNode    ( Scene::Texture )
                , original     ( NULL           )
                , textureID    ( -1             )
                , textureOp    ( GL_TEXTURE_2D  )
                , LOD          ( false          )
                , minimumLevel ( 1024           )
                , width        ( 0              )
                , height       ( 0              )
{
  #if   defined(Q_OS_ANDROID)
  #elif defined(Q_OS_IOS)
  #else
  paraments    [ GL_TEXTURE_WRAP_S     ] = GL_CLAMP                  ;
  paraments    [ GL_TEXTURE_WRAP_T     ] = GL_CLAMP                  ;
  #endif
  paraments    [ GL_TEXTURE_MAG_FILTER ] = GL_NEAREST                ;
  paraments    [ GL_TEXTURE_MIN_FILTER ] = GL_NEAREST                ;
  environments [ GL_TEXTURE_ENV_MODE   ] = GL_REPLACE                ;
  values       [ TexInternal           ] = GL_RGBA                   ;
  values       [ TexFormat             ] = GL_RGBA                   ;
  values       [ TexType               ] = GL_UNSIGNED_BYTE          ;
  values       [ RasterFormat          ] = GL_BGRA_EXT               ;
  values       [ RasterType            ] = GL_UNSIGNED_BYTE          ;
  values       [ PixelStore            ] = 1                         ;
  rasterizers  [ RasterZoom            ] = ControlPoint( 1,-1, 0, 1) ;
}

N::SceneTexture:: SceneTexture ( SceneDestructor * des )
                : SceneNode    ( Scene::Texture        )
                , original     ( NULL                  )
                , textureID    ( -1                    )
                , textureOp    ( GL_TEXTURE_2D         )
                , LOD          ( false                 )
                , minimumLevel ( 1024                  )
                , width        ( 0                     )
                , height       ( 0                     )
{
  destructor = des                                                        ;
  #if   defined(Q_OS_ANDROID)
  #elif defined(Q_OS_IOS)
  #else
  paraments    [ GL_TEXTURE_WRAP_S     ] = GL_CLAMP                       ;
  paraments    [ GL_TEXTURE_WRAP_T     ] = GL_CLAMP                       ;
  #endif
  paraments    [ GL_TEXTURE_MAG_FILTER ] = GL_NEAREST                     ;
  paraments    [ GL_TEXTURE_MIN_FILTER ] = GL_NEAREST                     ;
  environments [ GL_TEXTURE_ENV_MODE   ] = GL_REPLACE                     ;
  values       [ TexInternal           ] = GL_RGBA                        ;
  values       [ TexFormat             ] = GL_RGBA                        ;
  values       [ TexType               ] = GL_UNSIGNED_BYTE               ;
  values       [ RasterFormat          ] = GL_BGRA_EXT                    ;
  values       [ RasterType            ] = GL_UNSIGNED_BYTE               ;
  values       [ PixelStore            ] = 1                              ;
  rasterizers  [ RasterZoom            ] = ControlPoint( 1 , -1 , 0 , 1 ) ;
  JoinDestructor ( )                                                      ;
}

N::SceneTexture::~SceneTexture (void)
{
  if   ( NULL != original   )           {
    if ( NULL != destructor )           {
      destructor -> Remove ( original ) ;
    }                                   ;
  }                                     ;
}

void N::SceneTexture::clear(void)
{
  mipmaps . clear ( ) ;
  sizes   . clear ( ) ;
}

void N::SceneTexture::Raster(void)
{
  if ( NULL == original    ) return             ;
  QSize s = original -> size ( )                ;
  if ( s . width  ( ) <= 0 ) return             ;
  if ( s . height ( ) <= 0 ) return             ;
  #if   defined(Q_OS_ANDROID)
  #elif defined(Q_OS_IOS)
  #else
  QtGL::PixelStorei                             (
      GL_UNPACK_ALIGNMENT                       ,
      values [ PixelStore ]                   ) ;
  if ( rasterizers . contains ( RasterPos ) )   {
    QtGL::RasterPos4d                           (
        rasterizers [ RasterPos  ] . x          ,
        rasterizers [ RasterPos  ] . y          ,
        rasterizers [ RasterPos  ] . z          ,
        rasterizers [ RasterPos  ] . r        ) ;
    bindError ( )                               ;
  }                                             ;
  if ( rasterizers . contains ( RasterZoom ) )  {
    QtGL::PixelZoom                             (
        rasterizers [ RasterZoom ] . x          ,
        rasterizers [ RasterZoom ] . y        ) ;
    bindError ( )                               ;
  }                                             ;
  QtGL::DrawPixels                              (
      s                                         ,
      (GLenum) values [ RasterFormat ]          ,
      (GLenum) values [ RasterType   ]          ,
      (void *) original -> bits ( )           ) ;
  bindError ( )                                 ;
  QtGL::PixelStorei ( GL_UNPACK_ALIGNMENT , 4 ) ;
  #endif
}

void N::SceneTexture::setRaster(QImage * image)
{
  nDropOut ( IsNull ( image ) )                         ;
  int bpl  = image -> bytesPerLine ( )                  ;
  original = image                                      ;
  width    = original -> width  ( )                     ;
  height   = original -> height ( )                     ;
  switch ( original -> format ( ) )                     {
    case QImage::Format_Invalid                         :
      original = NULL                                   ;
    break                                               ;
    case QImage::Format_Mono                            :
      original = NULL                                   ;
    break                                               ;
    case QImage::Format_MonoLSB                         :
      original = NULL                                   ;
    break                                               ;
    case QImage::Format_Indexed8                        :
      original = NULL                                   ;
    break                                               ;
    case QImage::Format_RGB32                           :
      values [ RasterFormat ] = GL_BGRA_EXT             ;
      values [ RasterType   ] = GL_UNSIGNED_BYTE        ;
      values [ PixelStore   ] = 1                       ;
    break                                               ;
    case QImage::Format_ARGB32                          :
      values [ RasterFormat ] = GL_BGRA_EXT             ;
      values [ RasterType   ] = GL_UNSIGNED_BYTE        ;
      values [ PixelStore   ] = 1                       ;
    break                                               ;
    case QImage::Format_ARGB32_Premultiplied            :
      values [ RasterFormat ] = GL_BGRA_EXT             ;
      values [ RasterType   ] = GL_UNSIGNED_BYTE        ;
      values [ PixelStore   ] = 1                       ;
    break                                               ;
    case QImage::Format_RGB16                           :
      original = NULL                                   ;
    break                                               ;
    case QImage::Format_ARGB8565_Premultiplied          :
      original = NULL                                   ;
    break                                               ;
    case QImage::Format_RGB666                          :
      original = NULL                                   ;
    break                                               ;
    case QImage::Format_ARGB6666_Premultiplied          :
      original = NULL                                   ;
    break                                               ;
    case QImage::Format_RGB555                          :
      original = NULL                                   ;
    break                                               ;
    case QImage::Format_ARGB8555_Premultiplied          :
      original = NULL                                   ;
    break                                               ;
    case QImage::Format_RGB888                          :
      #if   defined(Q_OS_ANDROID)
      #elif defined(Q_OS_IOS)
      #else
      values [ RasterFormat ] = GL_BGR_EXT              ;
      #endif
      values [ RasterType   ] = GL_UNSIGNED_BYTE        ;
      values [ PixelStore   ] = 4                       ;
      if ( 0 != ( bpl % 4 ) ) values [ PixelStore ] = 1 ;
    break                                               ;
    case QImage::Format_RGB444                          :
      original = NULL                                   ;
    break                                               ;
    case QImage::Format_ARGB4444_Premultiplied          :
      original = NULL                                   ;
    break                                               ;
  }                                                     ;
}

bool N::SceneTexture::allow(QSize & size)
{
  nKickOut ( size . width  ( ) < minimumLevel , false ) ;
  nKickOut ( size . height ( ) < minimumLevel , false ) ;
  return true                                           ;
}

QSize N::SceneTexture::MipSize(QSize & size)
{
  int w = SceneBox::MipSize ( size . width  ( ) ) ;
  int h = SceneBox::MipSize ( size . height ( ) ) ;
  return QSize              ( w , h             ) ;
}

int N::SceneTexture::setLod(int lod)
{
  minimumLevel = SceneBox::MipSize ( lod ) ;
  return minimumLevel                      ;
}

double N::SceneTexture::setParament(int parament,double value)
{
  paraments [ parament ] = value ;
  return value                   ;
}

int N::SceneTexture::takeParament(int parament)
{
  paraments . remove       ( parament ) ;
  return paraments . count (          ) ;
}

N::SceneTexture & N::SceneTexture::operator = (QImage * image)
{
  create ( image , false , false ) ;
  return ME                        ;
}

bool N::SceneTexture::create(QImage * image,bool keep,bool Lod)
{
  textureID = -1                                ;
  LOD       = Lod                               ;
  width     = 0                                 ;
  height    = 0                                 ;
  mipmaps   . clear ( )                         ;
  sizes     . clear ( )                         ;
  nKickOut ( IsNull ( image )         , false ) ;
  nKickOut ( image -> width  ( ) <= 0 , false ) ;
  nKickOut ( image -> height ( ) <= 0 , false ) ;
  if ( keep )                                   {
    original = image                            ;
    if ( NULL != destructor )                   {
      destructor -> Append  ( original )        ;
    }                                           ;
  }                                             ;
  width   = image -> width  (          )        ;
  height  = image -> height (          )        ;
  updateMipmaps             ( image    )        ;
  return true                                   ;
}

void N::SceneTexture::updateMipmaps(QImage * image)
{
  QSize IS = image -> size ( )      ;
  QSize MS                          ;
  int   level = 0                   ;
  MS = MipSize ( IS             )   ;
  setLevel     ( 0 , MS , image )   ;
  nDropOut     ( ! LOD          )   ;
  ///////////////////////////////////
  IS /= 2                           ;
  MS  = MipSize ( IS )              ;
  while ( allow ( MS ) )            {
    level ++                        ;
    setLevel ( level , MS , image ) ;
    IS /= 2                         ;
    MS  = MipSize ( IS )            ;
  }                                 ;
}

bool N::SceneTexture::setLevel(int level,QSize mip,QImage * image)
{
  QSize      s = image -> size ( )                     ;
  QByteArray m                                         ;
  int        t = 4                                     ;
  t *= mip . width  (   )                              ;
  t *= mip . height (   )                              ;
  m .  resize       ( t )                              ;
  if ( s == mip )                                      {
    QImage I = QGLWidget::convertToGLFormat ( *image ) ;
    memcpy(m.data(),I.bits(),t)                        ;
  } else                                               {
    QImage Z = image -> scaled                         (
                 mip . width  ( )                      ,
                 mip . height ( )                      ,
                 Qt::IgnoreAspectRatio                 ,
                 Qt::SmoothTransformation            ) ;
    QImage I = QGLWidget::convertToGLFormat ( Z      ) ;
    ::memcpy ( m . data ( ) , I . bits ( ) , t       ) ;
  }                                                    ;
  mipmaps [ level ] = m                                ;
  sizes   [ level ] = mip                              ;
  return true                                          ;
}

TUID N::SceneTexture::bind(void)
{
  nKickOut ( sizes   . count ( ) != mipmaps . count ( ) , -1 )             ;
  nKickOut ( mipmaps . count ( ) <= 0                   , -1 )             ;
  //////////////////////////////////////////////////////////////////////////
  GLenum op  = (GLenum) textureOp                                          ;
  GLuint TID = (GLuint) textureID                                          ;
  //////////////////////////////////////////////////////////////////////////
  if ( textureID >= 0 )                                                    {
    if ( QtGL::BindTexture ( op , TID ) ) return textureID                 ;
  }                                                                        ;
  //////////////////////////////////////////////////////////////////////////
  textureID = -1                                                           ;
  TID       = 0                                                            ;
  if ( ! QtGL::PixelStorei   ( GL_UNPACK_ALIGNMENT , 1       ) ) return -1 ;
  if ( ! QtGL::GenTexture    (      TID                      ) ) return -1 ;
  if ( ! QtGL::BindTexture   ( op , TID                      ) ) return -1 ;
  if ( ! QtGL::TexParameterm ( op             , paraments    ) ) return -1 ;
  if ( ! QtGL::TexEnvm       ( GL_TEXTURE_ENV , environments ) ) return -1 ;
  //////////////////////////////////////////////////////////////////////////
  int level = 0                                                            ;
  if ( ! bindLevel ( level ) )                                             {
    QtGL::DeleteTexture ( TID )                                            ;
    return -1                                                              ;
  }                                                                        ;
  //////////////////////////////////////////////////////////////////////////
  if ( ! LOD )                                                             {
    textureID = (TUID) TID                                                 ;
    return textureID                                                       ;
  }                                                                        ;
  //////////////////////////////////////////////////////////////////////////
  level++                                                                  ;
  while ( mipmaps . contains ( level ) )                                   {
    if  ( ! bindLevel        ( level ) )                                   {
      QtGL::DeleteTexture    ( TID   )                                     ;
      return -1                                                            ;
    }                                                                      ;
    level++                                                                ;
  }                                                                        ;
  //////////////////////////////////////////////////////////////////////////
  textureID = (TUID) TID                                                   ;
  return textureID                                                         ;
}

bool N::SceneTexture::bindLevel(int level)
{
  QSize    s  = sizes [ level ]                         ;
  GLvoid * d  = (GLvoid *) mipmaps [ level ] . data ( ) ;
  QtGL::TexImage2D ( textureOp                          ,
                     level                              ,
                     values [ TexInternal ]             ,
                     s                                  ,
                     values [ TexFormat   ]             ,
                     values [ TexType     ]             ,
                     d                                ) ;
  return ( ! bindError ( ) )                            ;
}

bool N::SceneTexture::release(void)
{
  nKickOut ( textureID < 0 , false ) ;
  QtGL::DeleteTexture ( textureID  ) ;
  textureID = -1                     ;
  return ( ! bindError ( ) )         ;
}

bool N::SceneTexture::bindError(void)
{
  GLenum err = (GLenum) QtGL::GetError ( ) ;
  return ( err != GL_NO_ERROR )            ;
}

void N::SceneTexture::begin(void)
{
  QtGL::PixelStorei ( GL_UNPACK_ALIGNMENT , 4 ) ;
  QtGL::Enable      ( textureOp               ) ;
}

void N::SceneTexture::end(void)
{
  QtGL::Disable     ( textureOp               ) ;
  QtGL::PixelStorei ( GL_UNPACK_ALIGNMENT , 4 ) ;
}

void N::SceneTexture::createImage(QSize s,bool keep,bool Lod)
{
  QImage * O = new QImage ( s , QImage::Format_ARGB32 ) ;
  O -> fill               ( QColor ( 0 , 0 , 0 , 0 )  ) ;
  create                  ( O , keep , Lod            ) ;
}

void N::SceneTexture::setImageColor(QImage & image,QColor color)
{
  unsigned int * bits  = (unsigned int *) image . bits ( )      ;
  int            total = image . width ( ) * image . height ( ) ;
  QRgb           rgba  = color . rgba ( )                       ;
  unsigned int   v     = (unsigned int) rgba                    ;
  if ( total <= 0 ) return                                      ;
  for (int i = 0 ; i < total ; i++ ) bits [ i ] = v             ;
}

void N::SceneTexture::setImageColor(QColor color)
{
  nDropOut      ( IsNull(original)  ) ;
  setImageColor ( *original , color ) ;
  updateMipmaps ( original          ) ;
}

void N::SceneTexture::drawText  (
       QImage  & image     ,
       QColor    textColor ,
       QFont     font      ,
       QRectF  & rectangle ,
       int       flags     ,
       QString & text      )
{
  QPainter p                                     ;
  QRectF   B = rectangle                         ;
  p . begin    ( &image                        ) ;
  p . setPen   ( QPen   (textColor)            ) ;
  p . setBrush ( QBrush (textColor)            ) ;
  p . setFont  ( font                          ) ;
  p . drawText ( rectangle , flags , text , &B ) ;
  p . end      (                               ) ;
}

void N::SceneTexture::drawText  (
       QColor    textColor ,
       QFont     font      ,
       QRectF  & rectangle ,
       int       flags     ,
       QString & text      )
{
  nDropOut      ( IsNull(original)                                        ) ;
  drawText      ( *original , textColor , font , rectangle , flags , text ) ;
  updateMipmaps (  original                                               ) ;
}

void N::SceneTexture::setAlpha(int alpha,int level)
{
  QList<int>    Levels                                  ;
  int           L                                       ;
  unsigned char v = (unsigned char) alpha               ;
  if ( level < 0 ) Levels  = mipmaps . keys ( )         ;
              else Levels << level                      ;
  foreach ( L , Levels )                                {
    QSize           s = sizes [ L ]                     ;
    int             t = s . width ( ) * s . height ( )  ;
    int             a = 3                               ;
    unsigned char * d                                   ;
    d = (unsigned char *) mipmaps [ L ] . data ( )      ;
    for (int i = 0 ; i < t ; i++ , a += 4 ) d [ a ] = v ;
  }                                                     ;
}
