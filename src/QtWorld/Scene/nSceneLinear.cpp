#include <qtworld.h>

N::SceneLinear:: SceneLinear(void)
               : SceneObject(    )
{
  name = "Linear" ;
}

N::SceneLinear:: SceneLinear( SceneDestructor * des )
               : SceneObject( des                   )
{
  name = "Linear" ;
}

N::SceneLinear::~SceneLinear(void)
{
}

void N::SceneLinear::setSize(int size)
{
  dataTypes [ S ] = size ;
}

void N::SceneLinear::setType(int axis,int type)
{
  dataTypes [ axis ] = type ;
}

void N::SceneLinear::allocate(int a)
{
  int ts = dataTypes[S]                       ;
  QByteArray B                                ;
  valueAddress[a] = B                         ;
  valueAddress[a] . resize(ts*sizeof(void *)) ;
  valueAddress[a] . fill(0)                   ;
  axis        [a] = true                      ;
}

void N::SceneLinear::assign(int a,int at,void * address)
{
  void ** d = (void **)valueAddress[a].data() ;
  d[at] = address                             ;
}

void N::SceneLinear::assign(int a,QByteArray & array)
{
  int ds     = Cpp::SizeOf((Cpp::ValueTypes)dataTypes[a]) ;
  int length = dataTypes[S]         ;
  ds *= length                      ;
  #define TX(U)                     \
    if (array.size()==ds)         { \
      U * idx = (U *)array.data() ; \
      for (int i=0;i<length;i++)  { \
        assign(a,i,&idx[i])       ; \
      }                           ; \
    }
  switch  ( dataTypes[a] )          {
    case Cpp::Char                  :
      TX(char)                      ;
    break                           ;
    case Cpp::Byte                  :
      TX(unsigned char)             ;
    break                           ;
    case Cpp::Short                 :
      TX(short)                     ;
    break                           ;
    case Cpp::UShort                :
      TX(unsigned short)            ;
    break                           ;
    case Cpp::Integer               :
      TX(int)                       ;
    break                           ;
    case Cpp::UInt                  :
      TX(unsigned int)              ;
    break                           ;
    case Cpp::LongLong              :
      TX(TUID)                      ;
    break                           ;
    case Cpp::ULongLong             :
      TX(SUID)                      ;
    break                           ;
    case Cpp::Float                 :
      TX(float)                     ;
    break                           ;
    case Cpp::Double                :
      TX(double)                    ;
    break                           ;
    default                         :
    break                           ;
  }                                 ;
  #undef  TX
}

void N::SceneLinear::assign(int a)
{
  assign(a,byteArrays[a]) ;
}

void N::SceneLinear::tail(int a,QByteArray & array)
{
  nDropOut ( array . size ( ) <= 0 )                                ;
  int             ds = byteArrays [ a ] . size ( )                  ;
  int             ss = array            . size ( )                  ;
  int             ts = ds - ss                                      ;
  unsigned char * d  = (unsigned char *)byteArrays [ a ] . data ( ) ;
  unsigned char * s  = (unsigned char *)array            . data ( ) ;
  memcpy( d    ,&d[ss],ts)                                          ;
  memcpy(&d[ts], s    ,ss)                                          ;
}

void N::SceneLinear::renderSegments(void)
{
  int           total = dataTypes[S]                       ;
  int           c [ 3 ]                                    ;
  double        p [ 3 ]                                    ;
  void *        a [ 3 ]                                    ;
  ControlPoint b ( 0 , 0 , 0 )                             ;
  ControlPoint v                                           ;
  if (points.contains(0)) b = points[0]                    ;
  p[0] = 0                                                 ;
  p[1] = 0                                                 ;
  p[2] = 0                                                 ;
  a[0] = NULL                                              ;
  a[1] = NULL                                              ;
  a[2] = NULL                                              ;
  for (int i=0;i<3;i++)                                    {
    c[i] = -1                                              ;
    if (axis.contains(i) && valueAddress.contains(i))      {
      if (axis[i])                                         {
        c[i] = dataTypes[i]                                ;
        a[i] = (void *)valueAddress[i].data()              ;
      }                                                    ;
    }                                                      ;
    if (c[i]<0)                                            {
      if (staticValues.contains(i))                        {
        p[i] = staticValues[i].toDouble()                  ;
      }                                                    ;
    }                                                      ;
  }                                                        ;
  if ( colors . contains ( 0 ) )                           {
    colors [ 0 ] . Color4d ( )                             ;
  }                                                        ;
  QtGL::LineWidth ( 0.5 )                                  ;
  QtGL::Strips    (     )                                  ;
  for (int i=0;i<total;i++)                                {
    for (int j=0;j<3;j++)                                  {
      #define TX(U) p[j] = (double)(*(U *)(((U **)a[j])[i]))
      switch  ( c[j] )                                     {
        case Cpp::Char                                     :
          TX(char)                                         ;
        break                                              ;
        case Cpp::Byte                                     :
          TX(unsigned char)                                ;
        break                                              ;
        case Cpp::Short                                    :
          TX(short)                                        ;
        break                                              ;
        case Cpp::UShort                                   :
          TX(unsigned short)                               ;
        break                                              ;
        case Cpp::Integer                                  :
          TX(int)                                          ;
        break                                              ;
        case Cpp::UInt                                     :
          TX(unsigned int)                                 ;
        break                                              ;
        case Cpp::LongLong                                 :
          TX(TUID)                                         ;
        break                                              ;
        case Cpp::ULongLong                                :
          TX(SUID)                                         ;
        break                                              ;
        case Cpp::Float                                    :
          TX(float)                                        ;
        break                                              ;
        case Cpp::Double                                   :
          TX(double)                                       ;
        break                                              ;
        default                                            :
        break                                              ;
      }                                                    ;
      #undef  TX
    }                                                      ;
    v.x = p[0]                                             ;
    v.y = p[1]                                             ;
    v.z = p[2]                                             ;
    v  += b                                                ;
    v   . Vertex3d ( )                                     ;
  }                                                        ;
  QtGL::End       (     )                                  ;
  QtGL::LineWidth ( 1.0 )                                  ;
}
