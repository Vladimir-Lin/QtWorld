/****************************************************************************
 *                                                                          *
 * Copyright (C) 2001~2016 Neutrino International Inc.                      *
 *                                                                          *
 * Author : Brian Lin <lin.foxman@gmail.com>, Skype: wolfram_lin            *
 *                                                                          *
 ****************************************************************************/

#ifndef QT_WORLD_H
#define QT_WORLD_H

#include <QtCore>
#include <QtGui>
#include <QtNetwork>
#include <QtSql>
#include <QtScript>
#include <QtPositioning>
#include <QtOpenGL>
#include <QtCUDA>
#include <QtOpenCV>
#include <Essentials>
#include <QtCalendar>
#include <QtGMP>
#include <QtGSL>
#include <QtAlgebra>
#include <QtDiscrete>
#include <QtFFT>
#include <Mathematics>
#include <QtFuzzy>
#include <QtFLP>
#include <QtGeometry>
#include <QtGadgets>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#    if defined(QT_BUILD_QTWORLD_LIB)
#      define Q_WORLD_EXPORT Q_DECL_EXPORT
#    else
#      define Q_WORLD_EXPORT Q_DECL_IMPORT
#    endif
#else
#    define Q_WORLD_EXPORT
#endif

namespace N
{

class Q_WORLD_EXPORT SceneBox        ;
class Q_WORLD_EXPORT SceneNode       ;
class Q_WORLD_EXPORT SceneDestructor ;
class Q_WORLD_EXPORT Camera          ;
class Q_WORLD_EXPORT Light           ;
class Q_WORLD_EXPORT SceneTexture    ;
class Q_WORLD_EXPORT SceneSegment    ;
class Q_WORLD_EXPORT SceneFace       ;
class Q_WORLD_EXPORT SceneObject     ;
class Q_WORLD_EXPORT SceneCone       ;
class Q_WORLD_EXPORT SceneLinear     ;
class Q_WORLD_EXPORT Bookcase        ;
class Q_WORLD_EXPORT Universe        ;
class Q_WORLD_EXPORT FlatUniverse    ;
class Q_WORLD_EXPORT SpinnerUniverse ;
class Q_WORLD_EXPORT RasterUniverse  ;
class Q_WORLD_EXPORT PathRunner      ;
class Q_WORLD_EXPORT LinearRunner    ;
class Q_WORLD_EXPORT RotateRunner    ;
class Q_WORLD_EXPORT ScaleRunner     ;
class Q_WORLD_EXPORT ShockRunner     ;

typedef QList < Light       > Lights       ;
typedef QList < SceneObject > SceneObjects ;

namespace Scene
{

Q_WORLD_EXPORT SceneObject * Axis   (double L1,double L2,double R1,double R2) ;
Q_WORLD_EXPORT SceneObject * Circle (N::Circle C,QColor color) ;
Q_WORLD_EXPORT SceneObject * Arc    (N::Circle & Outer,N::Circle & Inner,QColor Color,double FOA,double angle = 0) ;
Q_WORLD_EXPORT bool          Arc    (SceneObject * arc,N::Circle & Outer,N::Circle & Inner,double FOA,double angle) ;

}

/*****************************************************************************
 *                                                                           *
 *                               Scene Objects                               *
 *                                                                           *
 *****************************************************************************/

class Q_WORLD_EXPORT SceneBox
{
  public:

    SUID                            muid             ;
    Matrix                          matrix           ;
    ControlPoint                    minimum          ;
    ControlPoint                    maximum          ;
    ControlPoint                    length           ;
    ControlPoints                   DOFs             ;
    QMap<int,Matrix>                matrices         ;
    WMAPs                           Variables        ;
    CUIDs                           ClassTypes       ;
    QMap<QString,DynamicAssignment> DynamicFunctions ;

    explicit     SceneBox      (void) ;
    virtual     ~SceneBox      (void) ;

    QVariant &   setVariable   (QString key,QVariant value) ;
    QVariant &   Value         (QString key) ;

    virtual void setUniverse   (SUID uuid) ;
    virtual void setParentUuid (SUID uuid) ;

    virtual SUID UniverseUuid  (void) ;
    virtual SUID ParentUuid    (void) ;

    virtual int  addType       (int objectType) ;
    virtual bool containsType  (int objectType) ;

    void         Prepare       (void) ;
    void         AddPoint      (ControlPoint & point) ;
    virtual bool BoundingBox   (ControlPoint & minp,ControlPoint & maxp) ;
    virtual bool contains      (ControlPoints & box,ControlPoint  & point) ;
    virtual bool contains      (ControlPoints & box,ControlPoints & points) ;
    virtual bool isInside      (ControlPoints & box,ControlPoints & boundingbox) ;
    void         Finish        (void) ;

    void         GetMatrix     (GLenum pname,Matrix & matrix) ; // glGetDoublev
    void         GetMatrix     (GLenum pname = GL_PROJECTION_MATRIX) ;

    void         SetMatrix     (Matrix & matrix) ;
    void         MultMatrix    (Matrix & matrix) ;
    void         SetMatrix     (void) ;
    void         MultMatrix    (void) ;
    void         Transform     (void) ;

    bool         Multiply      (Matrix & result,Matrix & A,Matrix & B) ;

    void         setFrustum    (void) ;
    void         setOrtho      (void) ;

    int          addDof        (int Id,ControlPoint & dof) ;
    int          removeDof     (int Id) ;

    bool         Transform     (ControlPoint & Source,ControlPoint & Target) ;

    virtual void Dual          (void) ;
    virtual bool intersects    (ControlPoint & p1,ControlPoint & p2) ;
    virtual bool intersects    (ControlPoint & p1,ControlPoint & p2  ,
                                ControlPoint & P1,ControlPoint & P2) ;

    virtual void TextureRect   (QRectF & area,double z) ;
    virtual void TextureRect   (QRectF & area,double z,QSizeF s) ;
    virtual void CubeVertex    (int            side   ,
                                ControlPoint & minp   ,
                                ControlPoint & maxp ) ;

    virtual void setRect       (QRectF & bar) ;
    virtual int  MipSize       (int size) ;

    virtual int  CallDynamic   (QString     func     ,
                                void      * data     ,
                                void      * dispatch ,
                                QVariants & values ) ;

  protected:

    virtual bool  ForUuid      (SUID & u,qint64 & i,UUIDs & U) ;

  private:

};

class Q_WORLD_EXPORT SceneNode : public Node
                               , public SceneBox
{
  public:

    enum                    {
      LimitLights      =  1 ,
      LimitTextureSize =  2 ,
      LimitEvalOrder   =  3
    }                       ;

    BMAPs             properties ;
    BMAPs             oglenables ;
    IMAPs             limits     ;
    SceneDestructor * destructor ;
    QWidget         * gui        ;
    Mutexz            NodeMutex  ;

    explicit     SceneNode      (void) ;
    explicit     SceneNode      (int Type) ;
    virtual     ~SceneNode      (void) ;

    virtual void suicide        (void) ;
    virtual void ObjectPointer  (QByteArray & pointer) ;

    virtual void setProperties  (void) ;
    virtual void setProperty    (GLenum pname,bool enable) ;

    virtual void setFunction    (int id,bool enable) ;
    virtual bool isFunction     (int id) ;
    virtual bool hasFunction    (QString key) ;

    void         setCache       (int size) ;
    void         setCacheLayout (int type) ;
    void         setCacheState  (TUID state) ;
    void *       cache          (void) ;
    int          cacheLayout    (void) ;
    TUID         cacheState     (void) ;

    bool         JoinDestructor (SceneNode * node) ;
    bool         JoinDestructor (QImage    * image) ;
    bool         JoinDestructor (void) ;

    void         LockAt         (int     index) ;
    void         UnlockAt       (int     index) ;
    bool         isLocked       (int     index) ;
    void         LockAt         (qint64  index) ;
    void         UnlockAt       (qint64  index) ;
    bool         isLocked       (qint64  index) ;
    void         LockAt         (QString index) ;
    void         UnlockAt       (QString index) ;
    bool         isLocked       (QString index) ;

  protected:

    QByteArray Cache       ;
    int        CacheLayout ;
    TUID       CacheState  ;

  private:

    QMap<qint64 ,bool> isMutexLocked ;
    QMap<QString,bool> isSutexLocked ;

};

class Q_WORLD_EXPORT SceneDestructor
{
  public:

    QList<QImage    *> images   ;
    QList<QImage    *> original ;
    QList<SceneNode *> nodes    ;

    explicit SceneDestructor (void) ;
    virtual ~SceneDestructor (void) ;

    void operator += (QImage      * image ) ;
    void operator += (SceneNode   * node  ) ;
    void operator -= (QImage      * image ) ;
    void operator -= (SceneNode   * node  ) ;
    void addChildren (SceneObject * object) ;
    void addTree     (SceneObject * object) ;
    bool Destroy     (SceneNode   * node  ) ;
    void Append      (QImage      * image ) ;
    void Remove      (QImage      * image ) ;

    void cleanup     (void) ;

  protected:

  private:

};

class Q_WORLD_EXPORT PathRunner
{
  public:

    SceneNode       * node           ;
    bool              Deletion       ;
    SUID              uuid           ;
    int               DOF            ;
    DynamicAssignment RemoveFunction ;
    void            * Dispatcher     ;
    QDateTime         Start          ;
    QDateTime         Final          ;
    ControlPoints     Points         ;

    explicit     PathRunner (void) ;
    virtual     ~PathRunner (void) ;

    virtual int  step       (int after = -1) ;
    virtual bool execute    (void) ;

  protected:

    virtual void AutoDeletion (void) ;

  private:

};

class Q_WORLD_EXPORT Camera : public SceneNode
{
  public:

    enum ViewStyles      {
      Reality      = 0   ,
      Perfect      = 1   ,
      Perspective  = 2   ,
      Orthographic = 3   ,
      Frustum      = 4   ,
      Fit          = 5   ,
      Parking      = 6   ,
      Floating     = 7   ,
      NodeMatrix   = 8 } ;

    enum AlignTypes        {
      AlignCenter      = 0 ,
      AlignTopLeft     = 1 ,
      AlignTopRight    = 2 ,
      AlignLeftBottom  = 3 ,
      AlignRightBottom = 4
    }                      ;

    enum ReferenceCorners {
      ReferenceCorner = 0 ,
      ReferenceSize   = 1
    };

    int           mode       ; // 0 - Perspective , 1 - Orthographic , 2 - Frustum
    QRectF        viewport   ;
    double        FOV        ;
    ControlPoint  eye        ;
    ControlPoint  center     ;
    ControlPoint  up         ;
    ControlPoints bounding   ;
    ControlPoints references ;
    Screen        screen     ;
    BMAPs         operating  ;
    bool          asObject   ;

    explicit     Camera         (void) ;
    explicit     Camera         (int mode) ;
    explicit     Camera         (SceneDestructor * destructor) ;
    virtual     ~Camera         (void) ;

    double       Factor         (void) ; // Viewport.width() / Viewport.height()
    double       Factor         (QRectF & area) ; // area.width() / area.height()
    Camera &     operator =     (QRectF & viewport) ;
    void         setViewport    (void) ;
    void         setWindowPort  (void) ;
    void         setFactorPort  (void) ;
    void         setNearFar     (double Near,double Far) ;

    void         setOrthogonal  (void) ;
    void         setPerspective (void) ;
    void         setFloating    (void) ;
    void         Perfection     (void) ;
    void         Partial        (void) ;
    void         setCamera      (void) ;
    void         setPerspective (double factor) ;

    void         Render         (void) ;
    void         setBounding    (void) ;
    void         FitWindow      (void) ;
    void         Dock           (void) ;

    virtual void assignViewport (QRectF & viewport) ;
    virtual void setViewport    (QRectF & area) ;
    virtual bool atPort         (QRectF & area,QPoint pos,QPointF & portPos) ;

    virtual bool EyePos             (QRectF & window,QPointF cursor,ControlPoint & pos) ;
    virtual bool EyePosPerspective  (QRectF & window,QPointF cursor,ControlPoint & pos) ;
    virtual bool EyePosOrthographic (QRectF & window,QPointF cursor,ControlPoint & pos) ;
    virtual bool EyePosFrustum      (QRectF & window,QPointF cursor,ControlPoint & pos) ;
    virtual bool EyePosFit          (QRectF & window,QPointF cursor,ControlPoint & pos) ;
    virtual bool EyePosParking      (QRectF & window,QPointF cursor,ControlPoint & pos) ;
    virtual bool EyePosFloating     (QRectF & window,QPointF cursor,ControlPoint & pos) ;

    virtual bool At             (ControlPoint & cursor,ControlPoint & plane) ;
    virtual bool AtPerspective  (ControlPoint & cursor,ControlPoint & plane) ;
    virtual bool AtOrthographic (ControlPoint & cursor,ControlPoint & plane) ;
    virtual bool AtFrustum      (ControlPoint & cursor,ControlPoint & plane) ;
    virtual bool AtFit          (ControlPoint & cursor,ControlPoint & plane) ;
    virtual bool AtParking      (ControlPoint & cursor,ControlPoint & plane) ;
    virtual bool AtFloating     (ControlPoint & cursor,ControlPoint & plane) ;

    virtual bool ParkingArea    (QRectF & area) ;

    virtual bool PerspectPos    (QRectF & area,ControlPoint & cursor,ControlPoint & plane) ;
    virtual bool OrthoPos       (QRectF & area,ControlPoint & cursor,ControlPoint & plane) ;

    double FocusPoint           (double pixels,double length,double distance) ; // ( pixels * distance ) / ( 2 * length )
    double Distance             (double pixels,double length,double fp) ; // length * fp / pixels
    double ObjectLength         (double pixels,double distance,double fp) ; // pixels * distance / fp
    double ObjectPixels         (double length,double distance,double fp) ; // length * fp / distance
    // arctan(Measure/(2*Focus)) * 360 / M_PI
    // Measure = Max(Width,Height) in pixels
    // Focus   = CCD focus point in pixels
    double CcdFov               (double Measure,double Focus) ;

  protected:

  private:

};

class Q_WORLD_EXPORT Light : public SceneNode
{
  public:

    int              lightId    ;
    BMAPs            properties ;
    ControlPoints    vectors    ;
    QMap<int,double> values     ;
    bool             asObject   ;

    explicit Light    (void) ;
    explicit Light    (SceneDestructor * destructor) ;
    virtual ~Light    (void) ;

    void setVector    (GLenum pname,ControlPoint & Point) ;
    void setValue     (GLenum pname,double value) ;

    void Enable       (void) ; // glEnable(LightId) ;
    void Disable      (void) ; // glDisable(LightId) ;
    void Place        (void) ; // Place light into scene
    void Render       (void) ;

    void setAmbient   (QColor & color) ; // Ambient color
    void setDiffuse   (QColor & color) ; // Diffuse color
    void setSpecular  (QColor & color) ; // Specular color

    void setPosition  (ControlPoint & Position ) ; // position
    void setDirection (ControlPoint & Direction) ; // Spot direction

    void setExponent  (double Exponent ) ; // Spot exponent
    void setCutoff    (double Cutoff   ) ; // Spot cutoff
    void setConstant  (double Constant ) ; // Constant attenuation
    void setLinear    (double Linear   ) ; // Linear attenuation
    void setQuadratic (double Quadratic) ; // Quadratic attenuation

  protected:

  private:

};

class Q_WORLD_EXPORT SceneTexture : public SceneNode
{
  public:

    enum TextureValues {
      TexInternal  = 0 ,
      TexFormat    = 1 ,
      TexType      = 2 ,
      RasterPos    = 3 ,
      RasterZoom   = 4 ,
      RasterFormat = 5 ,
      RasterType   = 6 ,
      PixelStore   = 7
    }                  ;

    QImage *             original     ;
    TUID                 textureID    ;
    TUID                 textureOp    ;
    bool                 LOD          ; // Level of details
    int                  minimumLevel ;
    int                  width        ;
    int                  height       ;
    QMap<int,double    > paraments    ;
    QMap<int,double    > environments ;
    QMap<int,QByteArray> mipmaps      ;
    QMap<int,QSize     > sizes        ;
    QMap<int,int       > values       ;
    ControlPoints        rasterizers  ;
    // glPixelStorei(GL_UNPACK_ALIGNMENT,Values[PixelStore]) ;
    // Rasterizers[RasterPos ] = glRasterPos4d(x,y,z,r) if exists
    // where normally z =  0 , w =  1
    // Rasterizers[RasterZoom] = glPixelZoom(x,y) if exists
    // where normally x = 1 , y = -1, default (1,-1,0,1) z,w is now useless
    // Values[RasterFormat] , GL_BGRA_EXT for QImage RGBA format
    //                        GL_BGR_EXT  for QImage RGB  format
    // default GL_BGRA_EXT
    // Values[RasterType] default is GL_UNSIGNED_BYTE

    explicit        SceneTexture (void) ;
    explicit        SceneTexture (SceneDestructor * destructor) ;
    virtual        ~SceneTexture (void) ;

    virtual void    clear        (void) ;

    // Rasterization
    virtual void    Raster       (void) ;
    void            setRaster    (QImage * image);

    int             setLod       (int lod) ;
    double          setParament  (int parament,double value) ;
    int             takeParament (int parament) ;

    SceneTexture &  operator =   (QImage * image) ;
    bool            create       (QImage * image,bool keep = false,bool Lod = false) ;

    virtual TUID    bind         (void) ;
    virtual bool    release      (void) ;

    virtual void    begin        (void) ;
    virtual void    end          (void) ;

    virtual void createImage     (QSize size,bool keep = true,bool Lod = true) ;
    virtual void setImageColor   (QImage  & image,QColor color) ;
    virtual void setImageColor   (QColor color) ;
    virtual void drawText        (QImage  & image     ,
                                  QColor    textColor ,
                                  QFont     font      ,
                                  QRectF  & rectangle ,
                                  int       flags     ,
                                  QString & text    ) ;
    virtual void drawText        (QColor    textColor ,
                                  QFont     font      ,
                                  QRectF  & rectangle ,
                                  int       flags     ,
                                  QString & text    ) ;
    virtual void setAlpha        (int alpha,int level = -1) ;
    void         updateMipmaps   (QImage * image) ;

  protected:

    bool  setLevel               (int level,QSize mip,QImage * image) ;
    bool  bindLevel              (int level) ;
    bool  allow                  (QSize & size) ;
    bool  bindError              (void) ;
    QSize MipSize                (QSize & size) ;

  private:

};

class Q_WORLD_EXPORT SceneSegment
{
  public:

    enum          {
      Lines = 0   ,
      Strip = 1   ,
      Loop  = 2 } ;

    int    type       ;
    bool   pointColor ;
    double lineWidth  ;
    int    dissect    ;
    CUIDs  index      ;
    IMAPs  colors     ;

    explicit     SceneSegment (int type = Lines) ;
    virtual     ~SceneSegment (void) ;

    virtual void clear        (void) ;
    virtual int  points       (void) ;

    virtual int  add          (int index) ;
    virtual int  add          (CUIDs & list) ;
    virtual void set          (int position,int index) ;
    virtual void setColor     (int position,int colorIndex) ;

    virtual int  remove       (int position) ;
    virtual int  at           (int position) ;

    virtual int  operator []  (int position) ;

  protected:

  private:

};

class Q_WORLD_EXPORT SceneFace
{
  public:

    enum             {
      Triangle = 1   ,
      Quad     = 2   ,
      Polygon  = 3 } ;

    int              type            ;
    bool             pointColor      ;
    SUID             textureId       ;
    CUIDs            index           ;
    BMAPs            properties      ;
    Plane            plane           ;
    IMAPs            colors          ;
    IMAPs            texCoords       ;
    IMAPs            paraments       ;
    IMAPs            dissectU        ;
    IMAPs            dissectV        ;
    CMAPs            smoothTextures  ;
    QMap<int,Matrix> smooth          ;
    QMap<int,Matrix> smoothColors    ;
    QMap<int,Matrix> smoothTexCoords ;

    explicit     SceneFace   (int type = Triangle) ;
    virtual     ~SceneFace   (void) ;

    virtual void clear       (void) ;
    virtual int  points      (void) ;

    virtual int  add         (int index) ;
    virtual int  add         (CUIDs & list) ;
    virtual void set         (int position,int index) ;
    virtual void setColor    (int position,int colorIndex) ;

    virtual int  remove      (int position) ;
    virtual int  at          (int position) ;

    virtual int  operator [] (int position) ;

  protected:

  private:

};

class Q_WORLD_EXPORT SceneObject : public SceneNode
{
  public:

    enum Items      {
      Point   = 0   ,
      Vector  = 1   ,
      Color   = 2   ,
      Segment = 3   ,
      Face    = 4   ,
      Tess    = 5 } ;

    enum RenderFunctions      {
      AssignColor        =  0 ,
      RenderPoints       =  1 ,
      RenderColors       =  2 ,
      RenderSegments     =  3 ,
      RenderCurves       =  4 ,
      RenderFaces        =  5 ,
      RenderSmooth       =  6 ,
      RenderSurfaces     =  7 ,
      RenderGrids        =  8 ,
      RenderTessllations =  9 ,
      RenderBox          = 10 ,
      RenderTextures     = 11 ,
      RenderCeramics     = 12 ,
      RenderPictures     = 13 ,
      RenderVoxels       = 14 ,
      RenderChildren     = 15 ,
      RenderUser         = 16
    }                         ;

    CUIDs                        index        ;
    ControlPoints                points       ;
    ControlPoints                vectors      ;
    ControlPoints                colors       ;
    ControlPoints                texCoords    ;
    QMap<int    ,SceneSegment  > segments     ;
    QMap<int    ,SceneFace     > faces        ;
    QMap<int    ,CUIDs         > tessllations ;
    QMap<SUID   ,QImage       *> images       ;
    QMap<SUID   ,SceneTexture *> textures     ;
    QMap<SUID   ,ControlPoint  > materials    ;
    QMap<SUID   ,SceneObject  *> children     ;
    QMap<QString,void         *> userobjs     ;
    QMap<SUID   ,PathRunner   *> runners      ;
    QMap<int    ,double        > paraments    ;
    XMAPs                        pictures     ;
    SMAPs                        sequences    ;
    BMAPs                        behaviors    ;
    BMAPs                        visibilities ;
    SUID                         frontBack    ;

    explicit SceneObject            (void) ;
    explicit SceneObject            (SceneDestructor * destructor) ;
    virtual ~SceneObject            (void) ;

    virtual void suicide            (void) ;
    virtual void ObjectPointer      (QByteArray & pointer) ;
    virtual void clear              (void) ;
    void         setDestructor      (SceneDestructor * destructor) ;

    virtual void render             (Camera & camera,QMap<int,Matrix> & matrices) ;
    virtual void render             (void) ;
    virtual void assignColor        (void) ;
    virtual void renderPoints       (void) ;
    virtual void renderColors       (void) ;
    virtual void renderSegments     (void) ;
    virtual void renderCurves       (void) ;
    virtual void renderFaces        (void) ;
    virtual void renderSmooth       (void) ;
    virtual void renderSurfaces     (void) ;
    virtual void renderGrids        (void) ;
    virtual void renderTessllations (void) ;
    virtual void renderBox          (void) ;
    virtual void renderTextures     (void) ;
    virtual void renderCeramics     (void) ;
    virtual void renderPictures     (void) ;
    virtual void renderVoxels       (void) ;
    virtual void renderChildren     (void) ;
    virtual void renderChildren     (Camera & camera,QMap<int,Matrix> & matrices) ;
    virtual void renderUser         (void) ;
    virtual void renderSegment      (int index) ;
    virtual void renderCurve        (int index) ;
    virtual void renderFace         (int index) ;
    virtual void renderSmooth       (int index) ;
    virtual void renderSurface      (int index) ;
    virtual void renderTessllation  (int index) ;
    virtual void renderTexture      (int index) ;
    virtual void renderCeramics     (int index) ;
    virtual void renderVoxel        (int index) ;

    int          Last               (void) ;
    int          Last               (int what) ;
    int          bindTexture        (SUID uuid) ;
    int          bindTexture        (SceneFace & face) ;
    int          releaseTexture     (SUID uuid) ;
    int          releaseTexture     (SceneFace & face) ;
    virtual void PointSize          (double pointSize) ;
    virtual void LineWidth          (double lineWidth) ;
    virtual void LineWidth          (QString key,double lineWidth) ;
    virtual void assumeColor        (QString key) ;
    virtual bool isFocus            (void) ;
    virtual bool setFocus           (bool focus) ;
    virtual UUIDs  list             (QString type) ;
    virtual QRectF BoxPlane         (int plane) ;
    virtual bool Within             (QPointF & p) ;

    virtual int  addPoint           (int index,ControlPoint & point ) ;
    virtual int  addVector          (int index,ControlPoint & vector) ;
    virtual int  addColor           (int index,ControlPoint & color ) ;
    virtual int  addTess            (int index,ControlPoints & points,QMap<int,CUIDs> & shapes) ;
    virtual int  addImage           (SUID uuid,QImage * image) ;
    virtual int  addChild           (SUID uuid,SceneObject * obj) ;
    virtual SUID appendChild        (SUID startUuid,SceneObject * obj) ;
    virtual int  addRunner          (SUID uuid,PathRunner * runner) ;
    virtual SUID appendRunner       (SUID startUuid,PathRunner * runner) ;

    virtual bool CommandRunners     (int after = 0) ;

    virtual void setColor           (QColor color) ;

    virtual void addText            (GlyphManager & glyph  ,
                                     ControlPoint   shift  ,
                                     QString        text ) ;

    virtual void addCube            (double length) ;
    virtual void addCube            (double length,ControlPoint & shift) ;
    virtual void addColorCube       (double length,ControlPoint & shift) ;
    virtual void addPlane           (Plane & plane) ;
    virtual void addPlane           (SUID textureId,Plane & plane) ;
    virtual void addColors          (QByteArray & rgbColors) ;
    virtual void addRgba            (QByteArray & rgbColors) ;

    virtual void addSequence        (SUID order,SUID renderId) ;

    virtual ControlPoint GetPoint   (QString key) ;
    virtual int  AssignPoint        (QString key,ControlPoint & point) ;

    virtual bool intersects         (ControlPoint & p1   ,
                                     ControlPoint & p2 ) ;

    virtual bool CommandRunners     (void) ;
    virtual bool Animation          (void) ;

    virtual bool allowDrag          (void) ;
    virtual bool canDrop            (void) ;
    virtual bool removeDrop         (void) ;
    virtual bool dragEnter          (ControlPoint    & From    ,
                                     ControlPoint    & To      ,
                                     QDragEnterEvent * event ) ;
    virtual bool dragMove           (ControlPoint    & From    ,
                                     ControlPoint    & To      ,
                                     QDragMoveEvent  * event ) ;
    virtual bool drop               (ControlPoint    & From    ,
                                     ControlPoint    & To      ,
                                     QDropEvent      * event ) ;

    virtual bool MouseClicked       (ControlPoint & From      ,
                                     ControlPoint & To        ,
                                     BMAPs        & buttons ) ;
    virtual bool MouseVector        (ControlPoint & From      ,
                                     ControlPoint & To        ,
                                     BMAPs        & buttons ) ;
    virtual bool MouseHover         (ControlPoint & From      ,
                                     ControlPoint & To        ,
                                     BMAPs        & buttons ) ;
    virtual bool MouseMove          (ControlPoint & From1     ,
                                     ControlPoint & To1       ,
                                     ControlPoint & From2     ,
                                     ControlPoint & To2       ,
                                     BMAPs        & buttons ) ;
    virtual bool Relocation         (QRectF viewport) ;
    virtual bool xymSpot            (ControlPoint & From ,
                                     ControlPoint & To   ,
                                     ControlPoint & At ) ;
    virtual bool Spot               (ControlPoint & From ,
                                     ControlPoint & To   ,
                                     ControlPoint & At ) ;
    virtual int  AtRegion           (ControlPoint & At ) ;
    virtual bool Collide            (ControlPoint & From ,
                                     ControlPoint & To   ,
                                     ControlPoint & At ) ;
    virtual SUID Nearest            (ControlPoint & From ,
                                     ControlPoint & To   ,
                                     ControlPoint & At ) ;
    virtual bool Menu               (QWidget      * widget ,
                                     QPointF        pos    ,
                                     ControlPoint & From   ,
                                     ControlPoint & To   ) ;
    virtual bool ChildMenu          (QWidget      * widget ,
                                     QPointF        pos    ,
                                     ControlPoint & From   ,
                                     ControlPoint & To   ) ;

    UUIDs        Sequential         (void) ;

    void         EvalCurve          (double u1,double u2,int dissect) ;
    void         EvalCurve          (int dissect) ;

    void         appendSegments     (int start,int count,SceneSegment & segment) ;
    void         appendFaces        (int start,int count,SceneFace    & face   ) ;

    virtual ControlPoint Mix        (ControlPoint & P1,ControlPoint & P2,double factor) ;
    virtual void GradientBox        (ControlPoints & P) ;
    virtual void FeatureCube        (ControlPoint & minp,ControlPoint & maxp) ;

    virtual void RemoveObjects      (int what) ;
    virtual void AutoDeletion       (void) ;

    virtual bool FocusIn            (void) ;
    virtual bool FocusOut           (void) ;

    virtual void setCursor          (const QCursor & cursor) ;
    virtual QString ToolTip         (void) ;

    template<typename T> T * ChildOf (SUID u)
    {
      if ( ! SceneObject::children . contains ( u ) ) return NULL ;
      return (T *) SceneObject::children [ u ]                    ;
    }

    virtual SceneObject * EnabledObject (QString key) ;

    virtual QVariant inputMethodQuery   (nDeclWidget,Qt::InputMethodQuery q) const ;
    virtual void inputMethodEvent       (nDeclWidget,QInputMethodEvent * e) ;
    virtual void keyPressEvent          (nDeclWidget,QKeyEvent         * e) ;
    virtual void keyReleaseEvent        (nDeclWidget,QKeyEvent         * e) ;
    virtual void enterEvent             (nDeclWidget,QEvent            * e) ;
    virtual void leaveEvent             (nDeclWidget,QEvent            * e) ;

    virtual bool grabMouse          (bool enabled) ;
    virtual bool grabKeyboard       (bool enabled) ;
    virtual bool isMouseGrabbed     (void) ;
    virtual bool isKeyboardGrabbed  (void) ;

    virtual bool  hasDrag           (void) ;
    virtual bool  doDrag            (void) ;

  protected:

    virtual bool ForUuid            (SUID & u,qint64 & i,UUIDs & U) ;
    virtual bool ForChild           (SUID & u,qint64 & i,UUIDs & U) ;

    // actual children rendering with camera and matrices
    void         inheritChildren    (Camera & camera,QMap<int,Matrix> & matrices) ;

    bool         CursorClicked      (ControlPoint & From      ,
                                     ControlPoint & To        ,
                                     BMAPs        & buttons ) ;
    bool         CursorVector       (ControlPoint & From      ,
                                     ControlPoint & To        ,
                                     BMAPs        & buttons ) ;
    bool         CursorHover        (ControlPoint & From      ,
                                     ControlPoint & To        ,
                                     BMAPs        & buttons ) ;
    bool         ChildrenClicked    (ControlPoint & From      ,
                                     ControlPoint & To        ,
                                     BMAPs        & buttons ) ;
    bool         ChildrenVector     (ControlPoint & From      ,
                                     ControlPoint & To        ,
                                     BMAPs        & buttons ) ;
    bool         ChildrenHover      (ControlPoint & From      ,
                                     ControlPoint & To        ,
                                     BMAPs        & buttons ) ;
    bool         GrabClicked        (ControlPoint & From      ,
                                     ControlPoint & To        ,
                                     BMAPs        & buttons ) ;
    bool         GrabVector         (ControlPoint & From      ,
                                     ControlPoint & To        ,
                                     BMAPs        & buttons ) ;
    bool         GrabHover          (ControlPoint & From      ,
                                     ControlPoint & To        ,
                                     BMAPs        & buttons ) ;
    virtual bool LocalClicked       (ControlPoint & From      ,
                                     ControlPoint & To        ,
                                     ControlPoint & At        ,
                                     BMAPs        & buttons   ,
                                     bool           after     ,
                                     bool           overlap ) ;
    virtual bool LocalVector        (ControlPoint & From      ,
                                     ControlPoint & To        ,
                                     ControlPoint & At        ,
                                     BMAPs        & buttons   ,
                                     bool           after     ,
                                     bool           overlap ) ;
    virtual bool LocalHover         (ControlPoint & From      ,
                                     ControlPoint & To        ,
                                     ControlPoint & At        ,
                                     BMAPs        & buttons   ,
                                     bool           after     ,
                                     bool           overlap ) ;

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
    int          RectRegion         (ControlPoint & At) ;
    // 0 - Outside
    // 1 - Inside
    int          InsideRegion       (ControlPoint & At) ;
    virtual bool MouseEditing       (ControlPoint & From      ,
                                     ControlPoint & To        ,
                                     ControlPoint & At        ,
                                     BMAPs        & buttons   ,
                                     bool           hover   ) ;
    virtual bool SelectRegion       (ControlPoint & From      ,
                                     ControlPoint & To        ,
                                     ControlPoint & At        ,
                                     BMAPs        & buttons   ,
                                     bool           hover   ) ;
    virtual bool ScalingRegion      (ControlPoint & From      ,
                                     ControlPoint & To        ,
                                     ControlPoint & At        ,
                                     BMAPs        & buttons   ,
                                     bool           hover   ) ;
    virtual void MouseProcessor     (bool hover) ;
    virtual void AssignCursor       (int region) ;
    virtual void BoxChanged         (bool hover) ;
    virtual void FinalizeSelection  (void) ;
    virtual void AssignFocus        (void) ;
    virtual bool CentralRelocation  (QRectF viewport) ;

    virtual QImage * CurrentImage   (QString key) ;

    virtual void renderSelectRect   (void) ;
    virtual QRectF SelectionArea    (void) ;

    virtual bool sceneDragEnter     (ControlPoint    & From    ,
                                     ControlPoint    & To      ,
                                     QDragEnterEvent * event ) ;
    virtual bool sceneDragMove      (ControlPoint    & From    ,
                                     ControlPoint    & To      ,
                                     QDragMoveEvent  * event ) ;
    virtual bool sceneDrop          (ControlPoint    & From    ,
                                     ControlPoint    & To      ,
                                     QDropEvent      * event ) ;
    virtual bool sceneRemoveDrop    (void) ;

  private:

};

class Q_WORLD_EXPORT SceneCone : public SceneObject
                               , public Cylinder
{
  public:

    explicit     SceneCone      (void) ;
    explicit     SceneCone      (SceneDestructor * destructor) ;
    virtual     ~SceneCone      (void) ;

    virtual void renderSmooth   (void) ;
    virtual void renderSurfaces (void) ;

    void         Align          (void) ;

  protected:

  private:

};

class Q_WORLD_EXPORT SceneLinear : public SceneObject
{
  public:

    enum AxisValues {
      X = 0         ,
      Y = 1         ,
      Z = 2         ,
      S = 3           // Size
    }               ;

    BMAPs                axis         ;
    IMAPs                dataTypes    ;
    QMap<int,QVariant  > staticValues ;
    QMap<int,QByteArray> byteArrays   ;
    QMap<int,QByteArray> valueAddress ;

    explicit     SceneLinear    (void) ;
    explicit     SceneLinear    (SceneDestructor * destructor) ;
    virtual     ~SceneLinear    (void) ;

    void         setType        (int axis,int type) ;
    void         setSize        (int size) ;
    void         allocate       (int axis) ;
    void         assign         (int axis,int at,void * address) ;
    void         assign         (int axis,QByteArray & array) ;
    void         assign         (int axis) ;
    void         tail           (int axis,QByteArray & array) ;

    virtual void renderSegments (void) ;

  protected:

  private:

};

class Q_WORLD_EXPORT Bookcase : public SceneObject
{
  public:

    enum               {
      NoSide     = 0   ,
      FrontSide  = 1   ,
      BackSide   = 2   ,
      LeftSide   = 3   ,
      RightSide  = 4   ,
      TopSide    = 5   ,
      BottomSide = 6 } ;

    explicit           Bookcase    (void) ;
    explicit           Bookcase    (SceneDestructor * destructor) ;
    virtual           ~Bookcase    (void) ;

    virtual void       addBox      (double length,double depth) ;
    virtual void       attache     (int side,SceneTexture * texture) ;
    virtual void       prepareText (void) ;
    virtual void       setText     (QFont   font                       ,
                                    QString text                       ,
                                    int     flags = Qt::AlignVCenter   |
                                                    Qt::AlignHCenter   |
                                                    Qt::TextWordWrap ) ;

  protected:

    virtual void       Configure   (void) ;

  private:

};

class Q_WORLD_EXPORT Universe : public SceneNode
                              , public OcTree
{
  public:

    enum ActivationID                 {
         MouseDClick       =     -4   ,
         MouseRelease      =     -3   ,
         MouseMove         =     -2   ,
         MousePress        =     -1   ,
         MouseLeft         =     11   ,
         MouseMiddle       =     12   ,
         MouseRight        =     13   ,
         FollowWindowSize  = 700001   ,
         WindowMovable     = 700002   ,
         HasUniverseMenu   = 700003   ,
         ControlPanel      = 700004   ,
         FixedPosition     = 700005   ,
         AudioAmplitude    = 700006   ,
         RasterSizing      = 700007   ,
         RasterZooming     = 700008   ,
         EndOfActivationID = 700009 } ;

    Physics::Unit             Unit          ;
    int                       MaxLights     ;
    QRectF                    Viewport      ;
    BMAPs                     Activations   ;
    UUIDs                     Illuminates   ;
    UUIDs                     Performs      ;
    Camera                 *  CurrentCamera ;
    QMap<SUID,Camera       *> Cameras       ;
    QMap<SUID,SceneNode    *> Actors        ;
    QMap<SUID,SceneTexture *> Textures      ;
    QMap<SUID,Light        *> Lights        ;
    QMap<SUID,PathRunner   *> Runners       ;
    QMap<QString,void      *> userobjs      ;
    ControlPoints             Users         ;
    QMap<QString,UUIDs>       MUIDs         ;
    NAMEs                     Names         ;

    explicit      Universe              (void) ;
    virtual      ~Universe              (void) ;

    virtual void  suicide               (void) ;
    virtual void  ObjectPointer         (QByteArray & pointer) ;
    virtual void  clear                 (void) ;
    virtual void  Flush                 (void) ;

    static  int   RunnerRemoval         (void * data,void * dispatcher,QVariants & values) ;
    virtual bool  CommandRunners        (int after = 0) ;
    virtual bool  Animation             (void) ;

    virtual int   addCamera             (SUID uuid,Camera * camera) ;
    virtual int   removeCamera          (SUID uuid) ;
    virtual void  setCurrentCamera      (SUID uuid) ;

    virtual int   addLight              (SUID uuid,Light * light) ;
    virtual int   removeLight           (SUID uuid) ;
    virtual void  illuminate            (SUID uuid,bool enable) ;

    virtual int   addActor              (SUID uuid,SceneNode * actor) ;
    virtual int   removeActor           (SUID uuid) ;
    virtual void  Perform               (SUID uuid,bool show) ;

    virtual void  Relocation            (QRectF viewport) ;

    virtual bool  allowDrag             (void) ;
    virtual bool  canDrop               (void) ;
    virtual bool  removeDrop            (void) ;
    virtual bool  dragEnter             (QDragEnterEvent * event) ;
    virtual bool  dragMove              (QDragMoveEvent  * event) ;
    virtual bool  drop                  (QDropEvent      * event) ;

    virtual bool  mouseDoubleClickEvent (nDeclWidget,QMouseEvent       * e) ;
    virtual bool  mouseMoveEvent        (nDeclWidget,QMouseEvent       * e) ;
    virtual bool  mousePressEvent       (nDeclWidget,QMouseEvent       * e) ;
    virtual bool  mouseReleaseEvent     (nDeclWidget,QMouseEvent       * e) ;
    virtual bool  wheelEvent            (nDeclWidget,QWheelEvent       * e) ;
    virtual void  inputMethodEvent      (nDeclWidget,QInputMethodEvent * e) ;
    virtual void  keyPressEvent         (nDeclWidget,QKeyEvent         * e) ;
    virtual void  keyReleaseEvent       (nDeclWidget,QKeyEvent         * e) ;
    virtual void  enterEvent            (nDeclWidget,QEvent            * e) ;
    virtual void  leaveEvent            (nDeclWidget,QEvent            * e) ;

    virtual QVariant inputMethodQuery   (nDeclWidget,Qt::InputMethodQuery query) const ;

    virtual SceneNode   * EnabledObject   (QString key) ;
    virtual SceneObject * CaptureMouse    (void) ;
    virtual SceneObject * CaptureKeyboard (void) ;

    void          setButtons            (Qt::MouseButtons buttons) ;

    virtual bool  Menu                  (QWidget * widget,QPointF pos,UUIDs & U) ;
    virtual bool  Menu                  (QWidget * widget,QPointF pos) ;

    virtual void  setActivate           (int Id,bool enable) ;
    virtual bool  isActivated           (enum ActivationID Id) ;

    virtual UUIDs Intersects            (ControlPoint p1,ControlPoint p2) ;
    virtual UUIDs Intersected           (ControlPoint pos) ;
    virtual UUIDs Intersected           (QPointF pos) ;
    virtual UUIDs Intersected           (QPoint  pos) ;

    virtual SUID  Nearest               (ControlPoint & From ,
                                         ControlPoint & To   ,
                                         ControlPoint & At ) ;
    virtual SUID  Nearest               (ControlPoint   pos  ,
                                         ControlPoint & At ) ;
    virtual SUID  Nearest               (QPointF        pos  ,
                                         ControlPoint & At ) ;
    virtual SUID  Nearest               (QPoint         pos  ,
                                         ControlPoint & At ) ;

    virtual void  RemoveObjects         (int what) ;
    virtual void  AutoDeletion          (void) ;

    template<typename T> T * ChildOf (SUID u)
    {
      if ( ! Universe::Actors . contains ( u ) ) return NULL ;
      return (T *) Universe::Actors [ u ]                    ;
    }

    virtual bool isMouseGrabbed         (SUID & uuid) ;

    virtual bool hasDrag                (void) ;
    virtual bool doDrag                 (void) ;

    virtual void setCursor              (const QCursor & cursor) ;

  protected:

    virtual bool ForUuid                (SUID & u,qint64 & i,UUIDs & U) ;
    virtual bool ForChild               (SUID & u,qint64 & i,UUIDs & U) ;

    virtual void FlushCamera            (void) ;
    virtual void PlaceLights            (void) ;
    virtual void RenderCameras          (void) ;
    virtual void RenderLights           (void) ;
    virtual void RenderActors           (void) ;

    virtual bool worldRemoveDrop        (void) ;
    virtual bool worldDragEnter         (QDragEnterEvent * event) ;
    virtual bool worldDragMove          (QDragMoveEvent  * event) ;
    virtual bool worldDrop              (QDropEvent      * event) ;

    virtual bool Moving                 (ControlPoint & from,ControlPoint & to) ;
    virtual bool LocalMoving            (ControlPoint & from,ControlPoint & to) ;
    virtual bool LocalDoubleClick       (QWidget * widget,QMouseEvent * event) ;
    virtual bool LocalPressed           (QWidget * widget,QMouseEvent * event) ;
    virtual bool LocalReleased          (QWidget * widget,QMouseEvent * event) ;
    virtual bool LocalHover             (QWidget * widget,QMouseEvent * event) ;
    virtual bool LocalHovering          (QWidget * widget,QMouseEvent * event) ;

    virtual bool ChildPressed           (ControlPoint & From      ,
                                         ControlPoint & To        ,
                                         BMAPs        & buttons ) ;
    virtual bool ChildReleased          (ControlPoint & From      ,
                                         ControlPoint & To        ,
                                         BMAPs        & buttons ) ;

  private:

} ;

class Q_WORLD_EXPORT FlatUniverse : public Universe
{
  public:

    explicit FlatUniverse (void) ;
    virtual ~FlatUniverse (void) ;

    virtual void setAlign  (int align) ;
    virtual void setArea   (QRect Port,QRectF Region) ;

    virtual bool mouseDoubleClickEvent (QWidget * widget,QMouseEvent * event) ;
    virtual bool mouseMoveEvent        (QWidget * widget,QMouseEvent * event) ;
    virtual bool mousePressEvent       (QWidget * widget,QMouseEvent * event) ;
    virtual bool mouseReleaseEvent     (QWidget * widget,QMouseEvent * event) ;

    virtual bool Menu                  (QWidget * widget,QPointF pos) ;

  protected:

    virtual void Configure (void) ;
    virtual bool Moving    (ControlPoint & from,ControlPoint & to) ;

  private:

};

class Q_WORLD_EXPORT SpinnerUniverse : public FlatUniverse
{
  public:

    QMap<int,Circle> Circles      ;
    QMap<int,QColor> Colors       ;
    double           ShiftAngle   ;
    double           FOA          ;
    double *         AnglePointer ;

    explicit SpinnerUniverse       (void) ;
    virtual ~SpinnerUniverse       (void) ;

    virtual void setDefault        (void) ;
    virtual bool Place             (void) ;
    virtual void showAngle         (double angle) ;

    virtual bool mouseMoveEvent    (QWidget * widget,QMouseEvent * event) ;
    virtual bool mousePressEvent   (QWidget * widget,QMouseEvent * event) ;
    virtual bool mouseReleaseEvent (QWidget * widget,QMouseEvent * event) ;

  protected:

    QPolygonF        HandleArea  ;
    QMap<int,double> AngleValues ;

    virtual void Configure (void) ;

  private:

};

class Q_WORLD_EXPORT RasterUniverse : public Universe
{
  public:

    enum RasterOp             {
      RasterScaling   = 10001 ,
      RasterAnimation = 10002 ,
      RasterObjectId  = 10003
    }                         ;

    explicit RasterUniverse   (void) ;
    explicit RasterUniverse   (SceneDestructor * destructor) ;
    virtual ~RasterUniverse   (void) ;

    virtual bool Animation    (void) ;
    virtual void setDimension (QSize size,int bufferSize,QImage::Format fmt) ;

    void     MoveRasterizer   (int NewActorId) ;

    int      Frames           (void) ;
    QImage * imageAt          (int index) ;
    bool   * imageDisplay     (int index) ;

    virtual void Relocation   (QRectF viewport) ;

  protected:

    virtual void Configure    (void) ;

  private:

};

class Q_WORLD_EXPORT LinearRunner : public PathRunner
{
  public:

    ControlPoint P1 ;
    ControlPoint P2 ;

    explicit     LinearRunner (void) ;
    virtual     ~LinearRunner (void) ;

    virtual int  step         (int after = -1) ;
    virtual bool execute      (void) ;

  protected:

  private:

} ;

class Q_WORLD_EXPORT RotateRunner : public PathRunner
{
  public:

    ControlPoint R   ;
    double       rps ; // seconds for a round ( angle 360 )

    explicit     RotateRunner (void) ;
    virtual     ~RotateRunner (void) ;

    virtual int  step         (int after = -1) ;
    virtual bool execute      (void) ;

  protected:

  private:

} ;

class Q_WORLD_EXPORT ScaleRunner : public PathRunner
{
  public:

    ControlPoint S1 ;
    ControlPoint S2 ;

    explicit     ScaleRunner  (void) ;
    virtual     ~ScaleRunner  (void) ;

    virtual int  step         (int after = -1) ;
    virtual bool execute      (void) ;

  protected:

  private:

} ;

class Q_WORLD_EXPORT ShockRunner : public PathRunner
{
  public:

    ControlPoint R          ;
    double       rps        ; // seconds for a round ( angle 360 )
    double       ShockAngle ; // vibrate angle

    explicit     ShockRunner  (void) ;
    virtual     ~ShockRunner  (void) ;

    virtual int  step         (int after = -1) ;
    virtual bool execute      (void) ;

  protected:

  private:

} ;

}

Q_DECLARE_METATYPE(N::SceneBox)
Q_DECLARE_METATYPE(N::SceneNode)
Q_DECLARE_METATYPE(N::SceneDestructor)
Q_DECLARE_METATYPE(N::Camera)
Q_DECLARE_METATYPE(N::Light)
Q_DECLARE_METATYPE(N::SceneTexture)
Q_DECLARE_METATYPE(N::SceneSegment)
Q_DECLARE_METATYPE(N::SceneFace)
Q_DECLARE_METATYPE(N::SceneObject)
Q_DECLARE_METATYPE(N::SceneCone)
Q_DECLARE_METATYPE(N::SceneLinear)
Q_DECLARE_METATYPE(N::Universe)
Q_DECLARE_METATYPE(N::FlatUniverse)
Q_DECLARE_METATYPE(N::SpinnerUniverse)
Q_DECLARE_METATYPE(N::RasterUniverse)

Q_DECLARE_INTERFACE ( N::PathRunner  , "com.neutrino.world.runner" )

QT_END_NAMESPACE

#endif
