/***************************************************************************
 copyright            : (C) 2006 by Benoit Jacob
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "textrenderer.h"

CharRenderer::CharRenderer()
{
  m_texture = 0;
  m_displayList = 0;
}

CharRenderer::~CharRenderer()
{
  if (m_texture)
    glDeleteTextures(1, &m_texture);
  if (m_displayList)
    glDeleteLists(m_displayList, 1);
}

bool CharRenderer::initialize(QChar c, const QFont &font)
{
  if (m_displayList)
    return true;

  QFontMetrics fontMetrics(font);
  m_width = fontMetrics.width(c);
  m_height = fontMetrics.height();
  //m_height = fontMetrics.height() * 0.75;
  //m_height = fontMetrics.ascent();
  if (m_width == 0 || m_height == 0)
    return false;
  QImage image(m_width, m_height, QImage::Format_RGB32);

  QPainter painter;
  painter.begin(&image);
  painter.setFont(font);
  painter.setRenderHint(QPainter::TextAntialiasing);
  painter.setBackground(Qt::black);
  painter.eraseRect(image.rect());
  painter.setPen(Qt::blue);
  //painter.drawText(0, 0, m_width, m_height, Qt::AlignBottom, c);
  painter.drawText(0, 0, m_width, m_height, Qt::AlignVCenter, c);
  painter.end();

  GLubyte *bitmap = new GLubyte[m_width * m_height];
  if (bitmap == 0)
    return false;

  for (int j = m_height - 1, n = 0; j >= 0; j--)
    for (int i = 0; i < m_width; i++, n++) {
      bitmap[n] = qBlue(image.pixel(i, j));
    }

  glGenTextures(1, &m_texture);
  if (m_texture == 0)
    return false;

  glBindTexture(GL_TEXTURE_2D, m_texture);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, m_width, m_height, 0, GL_ALPHA,
      GL_UNSIGNED_BYTE, bitmap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  delete[] bitmap;

  m_displayList = glGenLists(1);
  if (m_displayList == 0)
    return false;

  glNewList(m_displayList, GL_COMPILE );
  glBindTexture(GL_TEXTURE_2D, m_texture);
  glBegin(GL_QUADS);
  glTexCoord2f(0, 0);
  glVertex2f(0, 0);
  glTexCoord2f(1, 0);
  glVertex2f(m_width, 0);
  glTexCoord2f(1, 1);
  glVertex2f(m_width, m_height);
  glTexCoord2f(0, 1);
  glVertex2f(0, m_height);
  glEnd();
  glTranslatef(m_width, 0, 0);
  glEndList();
  return true;
}

TextRenderer::TextRenderer()
:
  descent(0.0), vpwidth(1), vpheight(1), glwidth(1.0), glheight(1.0),
  xscale(1.0), yscale(1.0)
{
  m_isBetweenBeginAndEnd = false;
  vpwidth=vpheight=1;
}

TextRenderer::~TextRenderer()
{
  QHash<QChar, CharRenderer *>::iterator i = m_charTable.begin();
  while (i != m_charTable.end()) {
    delete i.value();
    i = m_charTable.erase(i);
  }
}

void TextRenderer::setup(const QFont &font)
{
  m_font = font;

  // extract the fonts descent
  QFontMetrics fontMetrics(m_font);
  descent = fontMetrics.descent();
}

void TextRenderer::do_begin()
{
  m_wasEnabled_LIGHTING = glIsEnabled(GL_LIGHTING);
  m_wasEnabled_FOG = glIsEnabled(GL_FOG);
  m_wasEnabled_TEXTURE_2D = glIsEnabled(GL_TEXTURE_2D);
  m_wasEnabled_BLEND = glIsEnabled(GL_BLEND);
  m_wasEnabled_DEPTH_TEST = glIsEnabled(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glDisable(GL_FOG);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

/*
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, vpwidth, 0, vpheight, -1, 1);
*/
  glMatrixMode(GL_MODELVIEW);
}

void TextRenderer::begin()
{
  if (m_isBetweenBeginAndEnd)
    return;
  m_isBetweenBeginAndEnd = true;
  do_begin();
}

void TextRenderer::do_end()
{
  if (!m_wasEnabled_TEXTURE_2D)
    glDisable(GL_TEXTURE_2D);
  if (!m_wasEnabled_BLEND)
    glDisable(GL_BLEND);
  if (m_wasEnabled_DEPTH_TEST)
    glEnable(GL_DEPTH_TEST);
  if (m_wasEnabled_LIGHTING)
    glEnable(GL_LIGHTING);
  if (m_wasEnabled_FOG)
    glEnable(GL_FOG);

/*
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
*/
  glMatrixMode(GL_MODELVIEW);
}

void TextRenderer::end()
{
  if (m_isBetweenBeginAndEnd)
    do_end();
  m_isBetweenBeginAndEnd = false;
}

void TextRenderer::print(float x, float y, float a, const QString &string)
{
  if (string.isEmpty())
    return;

  if (!m_isBetweenBeginAndEnd)
    do_begin();

  glPushMatrix();
/*  glLoadIdentity();*/
  // test lowering the text by the font's descent
  glTranslatef(x, y - descent * yscale, 0);
  glRotatef(a, 0, 0, 1);
  glScalef(xscale,yscale,1.0);

  for (int i = 0; i < string.size(); i++) {
    if (m_charTable.contains(string[i]))
      m_charTable.value(string[i])->draw();
    else {
      CharRenderer *c = new CharRenderer;
      if (c->initialize(string[i], m_font)) {
        m_charTable.insert(string[i], c);
        c->draw();
      } else
        delete c;
    }
  }
  glPopMatrix();

  if (!m_isBetweenBeginAndEnd)
    do_end();
}

void TextRenderer::set_geometry(int w, int h, float glw, float glh)
{
  vpwidth=w;
  vpheight=h;
  glwidth=glw;
  glheight=glh;

  xscale = glwidth  / float(vpwidth);
  yscale = glheight / float(vpheight);
}

