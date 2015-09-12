/*
 libpets2 - presentation and editing of time series

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

#ifndef _ptImagePlot_h
#define _ptImagePlot_h

#include "ptImage.h"

#include <GL/gl.h>

#include <glp/glpfile.h>

class ptImagePlot {
private:
  ptImage image;
public:
  ptImagePlot()
  {
  }
  ptImagePlot(const miutil::miString& fname)
  {
    image.setImage(fname, true);
  }
  ptImagePlot(const ptImage& im)
  {
    image = im;
  }
  ~ptImagePlot()
  {
  }

  void setimage(const miutil::miString& fname)
  {
    image.setImage(fname, true);
  }
  void setimage(const ptImage& im)
  {
    image = im;
  }
  void plot(const float& x, const float& y, const float& scale,
      const float pixw = 1, const float pixh = 1, GLPcontext* psoutput = 0,
      const bool blend = false)
  {
    if (image.Size()) {
      glPixelZoom(scale, scale);
      glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
      glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
      // 	glPixelStorei(GL_UNPACK_ROW_LENGTH,image->Width());
      glRasterPos2f(x, y);
      glDrawPixels((GLint) image.Width(), (GLint) image.Height(),
          (image.Alpha()) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image.Data());
      if (psoutput) {
        int w = image.Width();
        int h = image.Height();
        int size = image.Size();
        psoutput->AddImage(image.Data(), size, w, h, x / pixw, y / pixh, scale,
            scale, 0, 0, w - 1, h - 1, (image.Alpha() ? GL_RGBA : GL_RGB),
            GL_UNSIGNED_BYTE, blend);
      }
    }
  }
  int Width() const
  {
    return image.Width();
  }
  int Height() const
  {
    return image.Height();
  }
};

#endif
