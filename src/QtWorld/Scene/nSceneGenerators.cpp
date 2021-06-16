#include <qtworld.h>

///////////////////////////////////////////////////////////////////////////////

N::SceneObject * N::Scene::Axis(double L1,double L2,double R1,double R2)
{
  ControlPoint     BC                                ;
  SceneObject    * ZA = new SceneObject ( )          ;
  SceneCone      * H1 = new SceneCone   ( )          ;
  SceneCone      * A1 = new SceneCone   ( )          ;
  SceneCone      * H2 = new SceneCone   ( )          ;
  SceneCone      * A2 = new SceneCone   ( )          ;
  SceneCone      * H3 = new SceneCone   ( )          ;
  SceneCone      * A3 = new SceneCone   ( )          ;
  ////////////////////////////////////////////////////
  ControlPoint     P1                                ;
  ControlPoint     P2                                ;
  ControlPoint     P3                                ;
  BC = QColor(255,  0,  0)                           ;
  A1->paraments[0] = R2                              ;
  A1->paraments[1] = 0.00001                         ;
  P1 = ControlPoint( L1,  0,  0)                     ;
  P2 = ControlPoint( L2,  0,  0)                     ;
  P3 = ControlPoint(  0,  1,  0)                     ;
  A1->addPoint    ( 0,P1                          )  ;
  A1->addPoint    ( 1,P2                          )  ;
  A1->addPoint    ( 2,P3                          )  ;
  A1->addColor    ( 0,BC                          )  ;
  H1->paraments[0] = R1                              ;
  H1->paraments[1] = R1                              ;
  P1 = ControlPoint(  0,  0,  0)                     ;
  P2 = ControlPoint( L1,  0,  0)                     ;
  P3 = ControlPoint(  0,  1,  0)                     ;
  H1->addPoint    ( 0,P1                          )  ;
  H1->addPoint    ( 1,P2                          )  ;
  H1->addPoint    ( 2,P3                          )  ;
  H1->addColor    ( 0,BC                          )  ;
  ////////////////////////////////////////////////////
  BC = QColor(  0,255,  0)                           ;
  A2->paraments[0] = R2                              ;
  A2->paraments[1] = 0.00001                         ;
  P1 = ControlPoint(  0, L1,  0)                     ;
  P2 = ControlPoint(  0, L2,  0)                     ;
  P3 = ControlPoint(  0,  0,  1)                     ;
  A2->addPoint    ( 0,P1                          )  ;
  A2->addPoint    ( 1,P2                          )  ;
  A2->addPoint    ( 2,P3                          )  ;
  A2->addColor    ( 0,BC                          )  ;
  H2->paraments[0] = R1                              ;
  H2->paraments[1] = R1                              ;
  P1 = ControlPoint(  0,  0,  0)                     ;
  P2 = ControlPoint(  0, L1,  0)                     ;
  P3 = ControlPoint(  0,  0,  1)                     ;
  H2->addPoint    ( 0,P1                          )  ;
  H2->addPoint    ( 1,P2                          )  ;
  H2->addPoint    ( 2,P3                          )  ;
  H2->addColor    ( 0,BC                          )  ;
  ////////////////////////////////////////////////////
  BC = QColor(  0,  0,255)                           ;
  A3->paraments[0] = R2                              ;
  A3->paraments[1] = 0.00001                         ;
  P1 = ControlPoint(  0,  0, L1)                     ;
  P2 = ControlPoint(  0,  0, L2)                     ;
  P3 = ControlPoint(  1,  0,  0)                     ;
  A3->addPoint    ( 0,P1                          )  ;
  A3->addPoint    ( 1,P2                          )  ;
  A3->addPoint    ( 2,P3                          )  ;
  A3->addColor    ( 0,BC                          )  ;
  H3->paraments[0] = R1                              ;
  H3->paraments[1] = R1                              ;
  P1 = ControlPoint(  0,  0,  0)                     ;
  P2 = ControlPoint(  0,  0, L1)                     ;
  P3 = ControlPoint(  1,  0,  0)                     ;
  H3->addPoint    ( 0,P1                          )  ;
  H3->addPoint    ( 1,P2                          )  ;
  H3->addPoint    ( 2,P3                          )  ;
  H3->addColor    ( 0,BC                          )  ;
  ////////////////////////////////////////////////////
  A1->Align       (                               )  ;
  H1->Align       (                               )  ;
  A2->Align       (                               )  ;
  H2->Align       (                               )  ;
  A3->Align       (                               )  ;
  H3->Align       (                               )  ;
  ////////////////////////////////////////////////////
  H1->Finish      (                               )  ;
  A1->Finish      (                               )  ;
  H2->Finish      (                               )  ;
  A2->Finish      (                               )  ;
  H3->Finish      (                               )  ;
  A3->Finish      (                               )  ;
  ////////////////////////////////////////////////////
  H1->Dual        (                               )  ;
  A1->Dual        (                               )  ;
  H2->Dual        (                               )  ;
  A2->Dual        (                               )  ;
  H3->Dual        (                               )  ;
  A3->Dual        (                               )  ;
  ////////////////////////////////////////////////////
  H1->addSequence ( 0,SceneObject::RenderSurfaces )  ;
  A1->addSequence ( 0,SceneObject::RenderSurfaces )  ;
  H2->addSequence ( 0,SceneObject::RenderSurfaces )  ;
  A2->addSequence ( 0,SceneObject::RenderSurfaces )  ;
  H3->addSequence ( 0,SceneObject::RenderSurfaces )  ;
  A3->addSequence ( 0,SceneObject::RenderSurfaces )  ;
  ////////////////////////////////////////////////////
  ZA->children[0] = H1                               ;
  ZA->children[1] = A1                               ;
  ZA->children[2] = H2                               ;
  ZA->children[3] = A2                               ;
  ZA->children[4] = H3                               ;
  ZA->children[5] = A3                               ;
  ZA->addSequence ( 0,SceneObject::RenderChildren )  ;
  ////////////////////////////////////////////////////
  return            ZA                               ;
}

///////////////////////////////////////////////////////////////////////////////

N::SceneObject * N::Scene::Circle(N::Circle C,QColor color)
{
  SceneObject * R = new SceneObject ( )      ;
  SceneSegment  F                            ;
  SceneFace     P                            ;
  ControlPoint  E                            ;
  double         da = 360                    ;
  da          /= C.N                         ;
  E            = color                       ;
  F.pointColor = true                        ;
  P.pointColor = true                        ;
  F.type       = SceneSegment::Loop          ;
  P.type       = SceneFace::Polygon          ;
  R->segments [ 1 ] = F                      ;
  R->faces    [ 1 ] = P                      ;
  for (int i=0;i<C.N;i++)                    {
    ControlPoint V                           ;
    double        A = da                     ;
    int           v = i + 1                  ;
    A *= i                                   ;
    C  . Angle                     ( A , V ) ;
    R -> addPoint                  ( v , V ) ;
    R -> addColor                  ( v , E ) ;
    R -> segments [ 1 ] . add      ( v     ) ;
    R -> segments [ 1 ] . setColor ( v , v ) ;
    R -> faces    [ 1 ] . add      ( v     ) ;
    R -> faces    [ 1 ] . setColor ( v , v ) ;
  }                                          ;
  return         R                           ;
}

//////////////////////////////////////////////////////////////////////////////

N::SceneObject * N::Scene::Arc(N::Circle & Outer,N::Circle & Inner,QColor Color,double FOA,double angle)
{
  SceneObject *  R = new SceneObject ( )     ;
  SceneSegment   F                           ;
  SceneFace      P                           ;
  ControlPoint   E                           ;
  double         B  = ( -FOA / 2 ) + angle   ;
  double         da = 360                    ;
  int            T  = 0                      ;
  int            T2 = 0                      ;
  da          /= Outer.N                     ;
  T            = (int)( FOA / da )           ;
  T2           = T * 2                       ;
  E            = Color                       ;
  F.pointColor = true                        ;
  P.pointColor = true                        ;
  F.type       = SceneSegment::Loop          ;
  P.type       = SceneFace::Polygon          ;
  R->segments [ 1 ] = F                      ;
  R->faces    [ 1 ] = P                      ;
  for (int i=0;i<T2;i++)                     {
    ControlPoint V                           ;
    int           v = i + 1                  ;
    R -> addPoint                  ( v , V ) ;
    R -> addColor                  ( v , E ) ;
    R -> segments [ 1 ] . add      ( v     ) ;
    R -> segments [ 1 ] . setColor ( v , v ) ;
    R -> faces    [ 1 ] . add      ( v     ) ;
    R -> faces    [ 1 ] . setColor ( v , v ) ;
  }                                          ;
  for (int i=0;i<T;i++)                      {
    ControlPoint V1                          ;
    ControlPoint V2                          ;
    int          v1 = i  + 1                 ;
    int          v2 = T2 - i                 ;
    double       A  = ( i * da ) + B         ;
    Outer . Angle ( A , V1 )                 ;
    Inner . Angle ( A , V2 )                 ;
    R -> points [ v1 ] = V1                  ;
    R -> points [ v2 ] = V2                  ;
  }                                          ;
  return R                                   ;
}

//////////////////////////////////////////////////////////////////////////////

bool N::Scene::Arc(SceneObject * R,N::Circle & Outer,N::Circle & Inner,double FOA,double angle)
{
  double B  = ( -FOA / 2 ) + angle ;
  double da = 360                  ;
  int    T  = 0                    ;
  int    T2 = 0                    ;
  da       /= Outer.N              ;
  T         = (int)( FOA / da )    ;
  T2        = T * 2                ;
  for (int i=0;i<T;i++)            {
    ControlPoint V1                ;
    ControlPoint V2                ;
    int           v1 = i  + 1      ;
    int           v2 = T2 - i      ;
    double        A  = i           ;
    A *= da                        ;
    A += B                         ;
    if (A<  0.0) A += 360.0        ;
    if (A>360.0) A -= 360.0        ;
    Outer . Angle ( A , V1 )       ;
    Inner . Angle ( A , V2 )       ;
    R -> points [ v1 ] = V1        ;
    R -> points [ v2 ] = V2        ;
  }                                ;
  return true                      ;
}
