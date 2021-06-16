#include <qtworld.h>

N::SceneSegment:: SceneSegment ( int type )
                : type         (     type )
                , pointColor   ( false    )
                , lineWidth    ( -1       )
                , dissect      ( 100      )
{
}

N::SceneSegment::~SceneSegment(void)
{
}

void N::SceneSegment::clear(void)
{
  index . clear ( ) ;
}

int N::SceneSegment::points(void)
{
  return index . count ( ) ;
}

int N::SceneSegment::add(int i)
{
  index << i        ;
  return points ( ) ;
}

int N::SceneSegment::add(CUIDs & list)
{
  index << list     ;
  return points ( ) ;
}

void N::SceneSegment::set(int position,int i)
{
  index [ position ] = i ;
}

void N::SceneSegment::setColor(int position,int colorIndex)
{
  colors [ position ] = colorIndex ;
}

int N::SceneSegment::remove(int position)
{
  index . takeAt ( position ) ;
  return points  (          ) ;
}

int N::SceneSegment::at(int position)
{
  return index [ position ] ;
}

int N::SceneSegment::operator [] (int position)
{
  return index [ position ] ;
}
