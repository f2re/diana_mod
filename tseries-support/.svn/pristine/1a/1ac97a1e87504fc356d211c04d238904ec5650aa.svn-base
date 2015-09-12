/*
  libpuDatatypes - Diverse datatypes: Regions, coordinates and alike

  $Id$

  Copyright (C) 2006 met.no

  Contact information:
  Norwegian Meteorological Institute
  Box 43 Blindern
  0313 OSLO
  NORWAY
  email: diana@met.no

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "miRegions.h"

#include <cmath>
#include <iostream>
#include <list>

using namespace std;

const bool debugmode=false;


miRegions::miBoundaryBox::miBoundaryBox(const miCoordinates& ll, const miCoordinates& ur)
{
  ll_lon = ll.iLon();
  ll_lat = ll.iLat();
  ur_lon = ur.iLon();
  ur_lat = ur.iLat();
}


miRegions::BoundaryComp
miRegions::miBoundaryBox::compare( miBoundaryBox& lhs)
{
  if(ll_lat >  lhs.ur_lat)  return PASSANT;
  if(ll_lon >  lhs.ur_lon)  return PASSANT;
  if(ur_lat <  lhs.ll_lat)  return PASSANT;
  if(ur_lon <  lhs.ll_lon)  return PASSANT;

  if(ll_lat == lhs.ur_lat ) return HTANGENT;
  if(ur_lat == lhs.ll_lat ) return HTANGENT;

  if(ll_lon == lhs.ur_lon ) return VTANGENT;
  if(ur_lon == lhs.ll_lon ) return VTANGENT;

  return INTERSECTION;
}


bool miRegions::miBoundaryBox::isInside(miCoordinates& mc, int t)
{
  int mc_lat = mc.iLat();
  int mc_lon = mc.iLon();


  if( ll_lat+t >  mc_lat)  return false;
  if( ll_lon+t >  mc_lon)  return false;
  if( ur_lat-t <  mc_lat)  return false;
  if( ur_lon-t <  mc_lon)  return false;

  return true;
}



// --------------------------------------------
miRegions::miBoundaryBox  miRegions::getBoundary() const
{
  return miBoundaryBox(lower_left_corner(),upper_right_corner());
}

miRegions::miBoundaryBox miRegions::getFuzzyBoundary(float deg) const
{
  miCoordinates addc(deg,deg);

  miCoordinates ll = lower_left_corner()-addc;
  miCoordinates ur = upper_right_corner()+addc;

  return miBoundaryBox(ll,ur);
}




void miRegions::clear()
{
  corner.clear();
  border.clear();
  cornerset.clear();
  name_ = "";
  idn   = 0;
  area_ = 0;
  triangles_.clear();
}


void miRegions::addCorner(int lon,int lat)
{
  miCoordinates tmp(lon,lat);
  addCorner(tmp);
}


void miRegions::addCorner(miCoordinates mc)
{
  if(cornerset.count(mc))
    return;
  cornerset.insert(mc);
  corner.push_back(mc);
  setBorders();
}


void miRegions::setCorners(const vector<miCoordinates> &c )
{
  cornerset.clear();
  corner.clear();

  for(int i=0;i<c.size();i++) {
    if(cornerset.count(c[i]))
      continue;
    cornerset.insert(c[i]);
    corner.push_back(c[i]);
  }
  setBorders();
}



void miRegions::setBorders()
{

  if ( corner.size() < 3 )
    return;

  border.clear();

  miLine tmp;

  vector<miCoordinates> corn = corner;

  if(corn[0] != corn[corn.size() -1 ] )
    corn.push_back(corner[0]);

  for( int i = 0; i < (corn.size()-1); i++) {
    tmp.set(corn[i],corn[i+1]);
    border.push_back(tmp);
  }
  area_=0;
  triangles_.clear();


  int urx = -10000000;
  int ury = -10000000;
  int llx = 10000000;
  int lly = 10000000;
  float cx  = 0;
  float cy  = 0;
  float sum = 0;

  for ( int i = 0; i < corner.size(); i++){
    llx = ( corner[i].iLon() < llx ? corner[i].iLon() : llx );
    lly = ( corner[i].iLat() < lly ? corner[i].iLat() : lly );

    urx = ( corner[i].iLon() > urx ? corner[i].iLon() : urx );
    ury = ( corner[i].iLat() > ury ? corner[i].iLat() : ury );

    cx+= corner[i].dLon();
    cy+= corner[i].dLat();
    sum++;
  }

  cx/=sum;
  cy/=sum;

  upper_right  = miCoordinates(urx,ury);
  lower_left   = miCoordinates(llx,lly);
  center_      = miCoordinates(cx,cy);

}


ostream& operator<<(ostream& out, const miRegions& rhs)
{

  vector<miCoordinates> crn = rhs.corner;

  out<<"REGION:\t" << rhs.name_ << "\n";
  for(int i = 0; i < crn.size(); i++ )
    out << crn[i] << endl;

 return out;
}


bool miRegions::isInside( const miCoordinates & seek_pt ) const
{
  miLine target;
  target.set(orig,seek_pt);
  int count = no_of_crosses(target);

  return count%2;
}


int miRegions::no_of_crosses(const miLine& target) const
{
  int count = 0;
  for (int i = 0; i < border.size(); i++ )
    if ( border[i].cross( target ) )
      count++;
  return count;
}

vector<miRegions> miRegions::triangles()
{
  if(!triangles_.empty())
    return triangles_;

  turnCounterClockwise();

  vector<miCoordinates> tcorners=corner;
  vector<miCoordinates> t(3);

  if(tcorners.size() < 3 )
    return triangles_;

  miRegions tmp;
  int curr=0,next,prev;

  while(!tcorners.empty()) {

    if(tcorners.size() == 3) {
      tmp.setCorners(tcorners);
      triangles_.push_back(tmp);
      break;
    }

    if(++curr >= tcorners.size()) {
      curr=0;
      prev=tcorners.size()-1;
      next=1;
    } else {
      prev=curr-1;
      next=curr+1;
    }

    if(prev<0)
      prev=tcorners.size()-1;

    if( next >= tcorners.size() )
      next=0;

    miCoordinates a = tcorners[curr] - tcorners[prev];
    miCoordinates b = tcorners[next] - tcorners[curr];

    if( a.cross(b) > 0  ) {

      t[0]=tcorners[prev];
      t[1]=tcorners[curr];
      t[2]=tcorners[next];

      tmp.setCorners(t);

      bool inside=false;

      for(int j=0;j<tcorners.size();j++)
	if(j!=prev && j!=curr && j!= next)
	  if(tmp.isInside(tcorners[j])) {
	    inside=true;
	    break;
	  }

      if(!inside) {
	triangles_.push_back(tmp);
	tcorners.erase(tcorners.begin()+curr);
      }
    }
  }
  return triangles_;
}


miRegions miRegions::subregion(float c,std::string sector, bool& inside,int rnd) const
{
  miCoordinates urc = upper_right_corner();
  miCoordinates llc = lower_left_corner();

  miCoordinates beg,end,tmp;

  inside = false;

  if(sector=="N" || sector=="S") {

    if(c >= urc.dLat() || c <= llc.dLat() )
      return *this;

    beg  = miCoordinates(llc.dLon(),c);
    end  = miCoordinates(urc.dLon(),c);

  } else if ( sector=="E" || sector=="W" ) {

    if(c >= urc.dLon() || c <= llc.dLon() )
      return *this;

    beg  = miCoordinates(c,llc.dLat());
    end  = miCoordinates(c,urc.dLat());

  } else
    return *this;

  miLine clipline(beg,end);

  if(no_of_crosses(clipline) != 2) /// itchy polygon possibly several ...
    return *this;

  inside = true;

  vector<miCoordinates>   sub;

  for (int i = 0; i < border.size(); i++ ) {
    if( isPartOfSubregion(clipline, border[i].begin(),sector))
      sub.push_back(border[i].begin());

    if(border[i].crossingPoint(clipline,tmp)) {
      if(rnd) {
	vector<miCoordinates> rgrid=tmp.roundedGrid(rnd);
	for(int i=0; i<rgrid.size();i++) {
	  if(isInside(rgrid[i])) {
	    tmp=rgrid[i];
	    break;
	  }
	}
      }
      sub.push_back(tmp);
    }

    if( isPartOfSubregion(clipline, border[i].end(),sector))
      sub.push_back(border[i].end());
  }


  miRegions subreg;
  subreg.setPriority( priority_ );
  subreg.setName(     name_     );
  subreg.setOrigin(   orig      );
  subreg.setCorners(  sub       );

  return subreg;
}

bool miRegions::isPartOfSubregion(const miLine& clipline,
				  const miCoordinates& p,
				  const std::string& sector) const
{
  if(sector=="N")
    return (p.dLat() >= clipline.begin().dLat());
  if(sector=="S")
    return (p.dLat() <= clipline.begin().dLat());
  if(sector=="E")
    return (p.dLon() >= clipline.begin().dLon());
  if(sector=="W")
    return (p.dLon() <= clipline.begin().dLon());
  return false;
}


/// in a convex polygon the cross product does
/// not change direction
bool miRegions::isConvex()
{
  if(corner.size() <  3) return false;
  if(corner.size() == 3) return true;

   miCoordinates a = corner[1] - corner[0];
   miCoordinates b = corner[2] - corner[1];

   double c1 = a.cross(b);

   for ( int i = 2; i < corner.size()-1; i++ ) {
     a=b;
     b = corner[i+1] - corner[i];
     double c2 = a.cross(b);

     if( (c1*c2) < 0 ) return false;
   }
   return true;
}



/// checking the cross-product positive=counterclockwise

bool miRegions::isCounterClockwise()
{
  if(corner.size() < 3)
    return true;

  double crossproduct=0;

  if( isConvex() ) {
    for(int i=1;i<corner.size()-2;i++) {
      miCoordinates a = corner[i] - corner[i-1];
      miCoordinates b = corner[i+1] - corner[i];
      crossproduct= a.cross(b);
      if(crossproduct!=0)
	break;
    }

    return (crossproduct > 0);
  }
  double A=0;
  int    i=0;

  // for concav polygons, the sum of the polygon areas has
  // to be positive to be counter clockwise

  for (; i<corner.size()-1; i++ )
    A +=  (corner[i].dLon() * corner[i+1].dLat() )
      -   (corner[i].dLat() * corner[i+1].dLon() );

  i=corner.size()-1;

  // if the polygon is not closed - close it !
  if(corner[i] != corner[0] )
    A +=  (corner[i].dLon() * corner[0].dLat() )
      -   (corner[i].dLat() * corner[0].dLon() );


  A=A/2;
  return (A > 0 );
}


void miRegions::turnCounterClockwise()
{
  if(isCounterClockwise())
    return;

  if(debugmode) cerr << "region is Clockwise, turning"
		     << corner.size() << " points" << endl;

  vector<miCoordinates> corn;
  for(int i=corner.size()-1;i>=0;i--)
    corn.push_back( corner[i]  );

  corner=corn;
  setBorders();

  if(!isCounterClockwise())
    if(debugmode) cerr << "ALERT the region is clockwise AGAIN" << endl;
}


bool miRegions::isCloseOrInside(const miCoordinates& a) const
{
//   if(isInside(a))
//     return true;

  for(int i=0;i<corner.size();i++)
    if(a==corner[i])
      return true;
  return false;
}


/// JOIN -----------------------------------------------------------

bool miRegions::join(miRegions lhs,miRegions rhs,int tolerance)
{
  if(debugmode) cerr << " JOIN -------------------------- " << endl;

  miBoundaryBox lboundary = lhs.getBoundary();
  miBoundaryBox rboundary = rhs.getBoundary();


  BoundaryComp  bc= lboundary.compare(rboundary);

  // check if lhs and rhs are joinable at all ...

  if(bc == PASSANT) {
    if(debugmode) cerr << "false / passant" << endl;
    return false;
  }
  rhs.turnCounterClockwise();
  lhs.turnCounterClockwise();

  vector<miCoordinates> firstc  = lhs.getCorners();
  vector<miCoordinates> secondc = rhs.getCorners();

  // define new regions to eventually swap them

  miRegions firstR  = lhs;
  miRegions secondR = rhs;

  int f;

  bool foundcorner=false;

  /// search for a corner in firstR which is out of
  /// secondR's boundary..

  for(f=0;f<firstc.size();f++)
    if(!rboundary.isInside(firstc[f])) {
      foundcorner=true;
      break;
    }

  /// firstR is surounded by secondR - swap firstR and secondR
  /// and try again

  if(!foundcorner) {
    if(debugmode) cerr << "Have to Swap ..." << endl;
    swap(firstc,secondc);
    firstR  = rhs;
    secondR = lhs;
    for(f=0;f<firstc.size();f++)
    if(!lboundary.isInside(firstc[f])) {
      foundcorner=true;
      break;
    }
  }

  miBoundaryBox fFuzzy   = firstR.getFuzzyBoundary(0.2);
  miBoundaryBox sFuzzy   = secondR.getFuzzyBoundary(0.2);



  /* firstR and secondR have identical boundary boxes.
     the are possibly identical, but are too uncertain to
     join */

  if(!foundcorner) {
    if(debugmode) cerr << "false / identical boxes" << endl;
    return false;
  }

  bool foundcross1=false;
  bool foundcross2=false;

  list<miCoordinates> res;

  int s1,s2;
  int f1,f2;


  // start at the corner in firstc which is definitly outside
  // secondc. try to find the point where firstc and secondc crosses
  // firsttime (counterclockwise)

  miCoordinates firstcross1,firstcross2;

  for(f1=f;f1<firstc.size();f1++) {
    res.push_back(firstc[f1]);
    if(sFuzzy.isInside(firstc[f1]))
      for(s1=0;s1<secondc.size();s1++) {
	if( firstc[f1].isCloserThan(secondc[s1],tolerance)) {
	  foundcross1=true;
	  firstcross1=secondc[s1];
	  firstcross2=firstc[f1];
	  if(debugmode) cerr << "Firstcross at : " << f1
			     << " from " << firstc.size() << endl
			     << s1 <<  " from " << secondc.size() << endl;

	  break;
	}
      }
    if(foundcross1)
      break;
  }

  if(!foundcross1) {
    if(debugmode) cerr << "false / no cross1" << endl;
    return false;
  }
  // after the cross - walk along secondc until the next cross ...
  // check first that there is enought space between first cross and second cross...


  for(f2=f-1;f2>=0;f2--) {
    res.push_front(firstc[f2]);

    if(sFuzzy.isInside(firstc[f2])) {
      for(s2=0;s2<secondc.size();s2++) {
	if( firstc[f2].isCloserThan(secondc[s2],tolerance)) {

	  if(!firstcross1.isCloserThan(secondc[s2],tolerance)  &&
	     !firstcross2.isCloserThan(firstc[f2],tolerance)   ){

	    if(debugmode) cerr << "secondcross at : "       << f2
			       << " from " << firstc.size() << endl
			       << s2 <<  " from " << secondc.size() << endl;

	    foundcross2=true;
	    break;
	  }

	}
      }
    }
    if(foundcross2)
      break;
  }


  if(!foundcross2) {
    for(f2=firstc.size()-1;f2>=0;f2--) {
      res.push_front(firstc[f2]);

      if(sFuzzy.isInside(firstc[f2]))
	for(s2=0;s2<secondc.size();s2++) {



	  if( firstc[f2].isCloserThan(secondc[s2],tolerance)) {

 	    if(!firstcross1.isCloserThan(secondc[s2],tolerance)  &&
	       !firstcross2.isCloserThan(firstc[f2],tolerance)   ){

	      if(debugmode) cerr << "secondcross at : "       << f2
				 << " from " << firstc.size() << endl
				 << s2 <<  " from " << secondc.size() << endl;

	      foundcross2=true;
	      break;
	    }
	  }
 	}
      if(foundcross2)
	break;
    }
  }

  if(!foundcross2) {
    if(debugmode) cerr << "false / !cross2" << endl;
    return false;
  }






  bool complete=false;

  for(int s=s1;s<secondc.size();s++ ) {
    res.push_back(secondc[s]);
    if(s==s2) {
      complete=true;
      break;
    }
  }

  if(!complete)
    for(int s=0;s<=s2;s++ )
      res.push_back(secondc[s]);

  vector<miCoordinates> oldcorners    = corner;
  set<miCoordinates>    oldcornerset  = cornerset;
  int                   oldarea       = area_;
  vector<miRegions>     oldtriangles  = triangles_;

  // when  you join 2 regions with at least 3 corners
  // and the regions are not complete identical -
  // the result must have at least 4 corners ...

  if(res.size() <  4 ) {
    if(debugmode) cerr << "false / size < 4" << endl;
    return false;
  }

  list<miCoordinates>::iterator itr=res.begin();

  triangles_.clear();
  corner.clear();
  setOrigin(lhs.origo());

  cornerset.clear();

  for(;itr!=res.end();itr++) {
    if(cornerset.count(*itr))
      continue;
    cornerset.insert(*itr);
    corner.push_back(*itr);
  }

  if(corner.size() < 4) {
    if(debugmode) cerr << "false / size < 4 after" << endl;
    cornerset = oldcornerset;
    corner    = oldcorners;
    return false;
  }

  setBorders();

  turnCounterClockwise();

  if(debugmode) cerr << "Successful join: " << size() << " corners" << endl;

  return true;
}


int miRegions::area()
{
  if(!isRegion() ) {
    area_=0;
    return area_;
  }

  if(isTriangle()) {
    float S,s,a,b,c;
    a=corner[0].distance(corner[1]);
    b=corner[1].distance(corner[2]);
    c=corner[2].distance(corner[0]);
    s=(a+b+c)/2;
    S=s*(s-a)*(s-b)*(s-c);

    area_=int(sqrt(S));
    if(area_ < 0 )
      area_=0;

    return area_;
  }

  if(!area_) {
    vector<miRegions> t = triangles();
    for(int i=0;i<t.size();i++)
      area_+=t[i].area();
  }

  return area_;
}




bool miRegions::isInside( const miRegions& lhs, int threshold) const
{
  miBoundaryBox lboundary = lhs.getBoundary();
  miBoundaryBox rboundary = getBoundary();


  // compare the boundaries ... if false -> they are not even close
  // together

  BoundaryComp  bc= lboundary.compare(rboundary);

  if(bc == PASSANT)
    return false;

  // check if all corners of lhs are inside of this

  vector<miCoordinates> c=lhs.getCorners();

  bool isCompleteInside=true;

  for(int i=0; i<c.size();i++ )
    if(!isInside(c[i])) {
      isCompleteInside=false;
      break;
    }

  if(isCompleteInside)
    return true;

  // compare internal boundary Grid ...

  c=lhs.getBoundaryGrid(15);

  float total= c.size();

  float match=0;

  for(int i=0;i<c.size(); i++ )
    if(isInside(c[i]))
      match+=1.0;

  // at least tolerance% of the boundary grid of the inside area
  // should be inside the big area ..

  if(debugmode) cerr <<"ISINSIDE checking if \"" << lhs.regName() << "\" is inside of \""
		     << regName() << "\" : " << match <<":"<< total
		     <<":"<<  match / total * 100 << "%";


  if ( ( (match / total) * 100 ) < threshold ){
    if ( debugmode ) cerr << "... NOT OK" << endl;
    return false;
  }

  if ( debugmode ) cerr << "... OK" << endl;
  return true;
}


bool miRegions::cornerCompare( vector<miCoordinates> c) const
{
  set<miCoordinates> s;

  if(c.size() != corner.size() )
    return false;

  for(int i=0;i<c.size();i++)
    s.insert(c[i]);

  if(s.size() != corner.size() )
    return false;

  for(int i=0;i<corner.size();i++)
    if(!s.count(corner[i]))
      return false;

  return true;
}



bool miRegions::isIdentical( miRegions& lhs,int threshold) const
{

  if(!isRegion() || !lhs.isRegion())
     return false;

  // Are the 2 regions complete identical (check the corners)
  // unlikely but possible - in that case the areas are really identical


  if(cornerCompare( lhs.getCorners() )) {
    if(debugmode) cerr << "cornercompare=true" << endl;
    return true;
  }

  // if not - we try a 3 step test...
  // 1) are the boundary boxes nearly identical
  // or even better ... identical ? (distance in km ...)

  if( upper_right.distance(lhs.upper_right_corner()) > 50 ) return false;
  if( lower_left.distance(lhs.lower_left_corner())   > 50 ) return false;


  // 2) put a grid over the boundary box (8x8)
  //    check how many of the grid points in
  //    region A are in region B

  int incr=8;
  vector<miCoordinates> gridPoints = getBoundaryGrid(incr);

  float total=gridPoints.size();

  // we need some grid points to make this tests ... to garantuee
  // enough points we increase the gridpoints until we find at least
  // 20 points inside the area ...

  if(total<20)
    while(total < 20 ) {
      incr+=3;
      gridPoints = getBoundaryGrid(incr);
      total=gridPoints.size();
    }

  float match=0;
  for(int i=0;i<gridPoints.size(); i++ )
    if(lhs.isInside(gridPoints[i]))
      match++;



  if( (match / total * 100 ) < threshold )
    return false;

  if (debugmode) cerr << "first test gridpoints" << match << "/"
		       << total << " :  " << match / total*100  << endl;

  // try vice versa

  gridPoints = lhs.getBoundaryGrid(incr);
  total      = gridPoints.size();
  if(total<20)
    while(total < 20 ) {
      incr+=3;
      gridPoints = lhs.getBoundaryGrid(incr);
      total      = gridPoints.size();
    }

  match=0;
  for(int i=0;i<gridPoints.size(); i++ )
    if(isInside(gridPoints[i]))
      match++;

  if( (match / total * 100 ) < threshold )
    return false;

  if(debugmode) cerr << "second test gridpoints" << match << "/"
		     << total << " :  " << match / total*100  << endl;


  return true;
}



vector<miCoordinates> miRegions::getBoundaryGrid(int noOfGrids, bool inside) const
{
  miCoordinates incr= ( upper_right - lower_left ) / float(noOfGrids);

  vector<miCoordinates> boundaryGrid;

  float latAdd=incr.dLat();
  float lonAdd=incr.dLon();

  for( int i=1;i<noOfGrids;i++ )
    for(int j=1;j<noOfGrids;j++) {
      miCoordinates gridPoint = lower_left + miCoordinates(lonAdd*float(i),latAdd*float(j));

      if(inside)
	if(!isInside(gridPoint))
	  continue;

      boundaryGrid.push_back(gridPoint);
    }

  return boundaryGrid;
}
