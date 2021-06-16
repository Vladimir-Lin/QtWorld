#include <qtworld.h>

N::SceneFace:: SceneFace  (int type)
             : type       (    type)
             , pointColor (false   )
{
}

N::SceneFace::~SceneFace (void)
{
}

void N::SceneFace::clear(void)
{
  index . clear ( ) ;
}

int N::SceneFace::points(void)
{
  return index . count ( ) ;
}

int N::SceneFace::add(int i)
{
  index << i      ;
  return points() ;
}

int N::SceneFace::add(CUIDs & list)
{
  index << list   ;
  return points() ;
}

void N::SceneFace::set(int position,int i)
{
  index [position] = i ;
}

void N::SceneFace::setColor(int position,int colorIndex)
{
  colors [position] = colorIndex ;
}

int N::SceneFace::remove(int position)
{
  index.takeAt(position) ;
  return points() ;
}

int N::SceneFace::at(int position)
{
  return index[position] ;
}

int N::SceneFace::operator [] (int position)
{
  return index[position] ;
}
