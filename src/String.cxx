/*
 * String.cxx
 * Daniel Nelson - 11/10/0
 *
 * Copyright (C) 2000  Daniel Nelson
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Daniel Nelson - aluminumangel.org
 * 174 W. 18th Ave.
 * Columbus, OH  43210
 *
 * Holds string display utilities.
 */

#include <cctype>
#include <cstring>
#include <GL/glut.h>

#ifndef _WIN32
#else
#  include <glext.h>
#endif

using namespace std;

#include "Game.h"
#include "Displayer.h"
#include "String.h"
#include "TextureLoader.h"

const char *String::letter_texture_files[DC_FONT_NUMBER][DC_LETTER_NUMBER]
 = { { GC_DATA_DIRECTORY "font0_0.tga",
       GC_DATA_DIRECTORY "font0_1.tga",
       GC_DATA_DIRECTORY "font0_2.tga",
       GC_DATA_DIRECTORY "font0_3.tga",
       GC_DATA_DIRECTORY "font0_4.tga",
       GC_DATA_DIRECTORY "font0_5.tga",
       GC_DATA_DIRECTORY "font0_6.tga",
       GC_DATA_DIRECTORY "font0_7.tga",
       GC_DATA_DIRECTORY "font0_8.tga",
       GC_DATA_DIRECTORY "font0_9.tga",
       GC_DATA_DIRECTORY "font0_ca.tga",
       GC_DATA_DIRECTORY "font0_cb.tga",
       GC_DATA_DIRECTORY "font0_cc.tga",
       GC_DATA_DIRECTORY "font0_cd.tga",
       GC_DATA_DIRECTORY "font0_ce.tga",
       GC_DATA_DIRECTORY "font0_cf.tga",
       GC_DATA_DIRECTORY "font0_cg.tga",
       GC_DATA_DIRECTORY "font0_ch.tga",
       GC_DATA_DIRECTORY "font0_ci.tga",
       GC_DATA_DIRECTORY "font0_cj.tga",
       GC_DATA_DIRECTORY "font0_ck.tga",
       GC_DATA_DIRECTORY "font0_cl.tga",
       GC_DATA_DIRECTORY "font0_cm.tga",
       GC_DATA_DIRECTORY "font0_cn.tga",
       GC_DATA_DIRECTORY "font0_co.tga",
       GC_DATA_DIRECTORY "font0_cp.tga",
       GC_DATA_DIRECTORY "font0_cq.tga",
       GC_DATA_DIRECTORY "font0_cr.tga",
       GC_DATA_DIRECTORY "font0_cs.tga",
       GC_DATA_DIRECTORY "font0_ct.tga",
       GC_DATA_DIRECTORY "font0_cu.tga",
       GC_DATA_DIRECTORY "font0_cv.tga",
       GC_DATA_DIRECTORY "font0_cw.tga",
       GC_DATA_DIRECTORY "font0_cx.tga",
       GC_DATA_DIRECTORY "font0_cy.tga",
       GC_DATA_DIRECTORY "font0_cz.tga",
       GC_DATA_DIRECTORY "font0_a.tga",
       GC_DATA_DIRECTORY "font0_b.tga",
       GC_DATA_DIRECTORY "font0_c.tga",
       GC_DATA_DIRECTORY "font0_d.tga",
       GC_DATA_DIRECTORY "font0_e.tga",
       GC_DATA_DIRECTORY "font0_f.tga",
       GC_DATA_DIRECTORY "font0_g.tga",
       GC_DATA_DIRECTORY "font0_h.tga",
       GC_DATA_DIRECTORY "font0_i.tga",
       GC_DATA_DIRECTORY "font0_j.tga",
       GC_DATA_DIRECTORY "font0_k.tga",
       GC_DATA_DIRECTORY "font0_l.tga",
       GC_DATA_DIRECTORY "font0_m.tga",
       GC_DATA_DIRECTORY "font0_n.tga",
       GC_DATA_DIRECTORY "font0_o.tga",
       GC_DATA_DIRECTORY "font0_p.tga",
       GC_DATA_DIRECTORY "font0_q.tga",
       GC_DATA_DIRECTORY "font0_r.tga",
       GC_DATA_DIRECTORY "font0_s.tga",
       GC_DATA_DIRECTORY "font0_t.tga",
       GC_DATA_DIRECTORY "font0_u.tga",
       GC_DATA_DIRECTORY "font0_v.tga",
       GC_DATA_DIRECTORY "font0_w.tga",
       GC_DATA_DIRECTORY "font0_x.tga",
       GC_DATA_DIRECTORY "font0_y.tga",
       GC_DATA_DIRECTORY "font0_z.tga",
       GC_DATA_DIRECTORY "font0_mn.tga",
       GC_DATA_DIRECTORY "font0_cln.tga",
       GC_DATA_DIRECTORY "font0_pe.tga",
       GC_DATA_DIRECTORY "font0_cma.tga",
       GC_DATA_DIRECTORY "font0_ep.tga",
       GC_DATA_DIRECTORY "font0_at.tga",
       GC_DATA_DIRECTORY "font0_td.tga",
       GC_DATA_DIRECTORY "font0_pd.tga",
       GC_DATA_DIRECTORY "font0_ds.tga",
       GC_DATA_DIRECTORY "font0_ps.tga",
       GC_DATA_DIRECTORY "font0_and.tga",
       GC_DATA_DIRECTORY "font0_pl.tga",
       GC_DATA_DIRECTORY "font0_pr.tga",
       GC_DATA_DIRECTORY "font0_sl.tga",
       GC_DATA_DIRECTORY "font0_lt.tga",
       GC_DATA_DIRECTORY "font0_gt.tga",
       GC_DATA_DIRECTORY "font0_qm.tga",
       GC_DATA_DIRECTORY "font0_eq.tga",
       GC_DATA_DIRECTORY "font0_pu.tga",
       GC_DATA_DIRECTORY "font0_br.tga",
       GC_DATA_DIRECTORY "font0_za.tga",
       GC_DATA_DIRECTORY "font0_zb.tga",
       GC_DATA_DIRECTORY "font0_zc.tga",
       GC_DATA_DIRECTORY "font0_zd.tga" } };

const char String::letter_mapping[DC_LETTER_NUMBER]
 = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',

     'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
     'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
     'U', 'V', 'W', 'X', 'Y', 'Z',
     
     'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
     'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
     'u', 'v', 'w', 'x', 'y', 'z',
     
     '-', ':', '.', ',', '!', '@', '~', '#', '$', '%',
     '&', '(', ')', '/', '<', '>', '?', '=', '+', '|',
     ';', '[', '*', ']' };

const int String::letter_widths[DC_FONT_NUMBER][DC_LETTER_NUMBER]
 = { { 20, 12, 17, 14, 16, 15, 16, 19, 17, 16,

       28, 20, 17, 20, 18, 20, 20, 19, 11, 17,
       19, 17, 30, 20, 20, 21, 22, 22, 16, 21,
       20, 20, 30, 25, 22, 19,

       19, 14, 13, 14, 13, 14, 15, 14,  8, 12,
       14, 12, 22, 15, 14, 14, 16, 16, 12, 15,
       14, 15, 22, 18, 17, 13,

       20, 12, 11, 10, 11, 21, 23, 23, 18, 19,
       20, 12, 13, 18, 16, 16, 17, 20, 23, 16,
       22, 22, 16, 22 } };
       

const float String::colors[DC_FONT_COLOR_NUMBER][3]
 = { { 0.2f, 0.2f, 1.0f },
     { 1.0f, 0.1f, 0.1f },
     { 0.1f, 1.0f, 0.1f },
     { 0.8f, 0.8f, 0.1f },
     { 0.9f, 0.0f, 0.9f },
     { 0.1f, 0.8f, 0.8f },
     { 0.9f, 0.3f, 0.1f },
     { 1.0f, 0.5f, 0.5f },
     { 0.4f, 0.0f, 1.0f },
     { 0.1f, 0.1f, 0.1f } };

GLubyte *String::letter_textures[DC_FONT_NUMBER][DC_LETTER_NUMBER];

int String::mapCharToCode ( char c )
{
  int code = -1;
  for (int n = DC_LETTER_NUMBER; n--; )
    if (c == letter_mapping[n]) {
      code = n;
      break;
    }
  return code;
}

int String::stringWidth ( const char *string, int max_width )
/*
 * Determines the pixel width of a string.
 */
{
  int length = 0;

  int font = 0;
  for (unsigned int n = 0; n < strlen(string) && length < max_width; n++) {

    if (string[n] == '~') {
      switch (string[++n]) {
      case '<':
        if (length >= DC_BACK_SPACE_WIDTH) length -= DC_BACK_SPACE_WIDTH;
        break;
      case '>':
        length += DC_BACK_SPACE_WIDTH;
        break;
      default:
        int c = string[n] - 'a';
        if (c >= 0 && c < DC_FONT_NUMBER)
          font = c;
        break;
      }

      if (string[n] != '~') continue;

    } else if (string[n] == ' ') {
      length += DC_SPACE_WIDTH;
      continue;
    }

    int code = mapCharToCode(string[n]);    
    if (code != -1) {
      if (length + letter_widths[font][code] > max_width) break;
      length += letter_widths[font][code];
    }
  }

  return length;
}

void String::readyLetterTextures (   )
{
  for (int n = DC_FONT_NUMBER; n--; )
    for (int m = DC_LETTER_NUMBER; m--; )
      letter_textures[n][m]
       = TextureLoader::loadAlphaTGA(letter_texture_files[n][m],
       DC_LETTER_TEX_LENGTH, DC_LETTER_TEX_LENGTH);
}

void String::freeLetterTextures (   )
{
  for (int n = DC_FONT_NUMBER; n--; )
    for (int m = DC_LETTER_NUMBER; m--; )
      if (letter_textures[n][m] != null) {
        delete [] letter_textures[n][m];
        letter_textures[n][m] = null;
      }
      
}

void String::fillStringTexture ( const char *string, GLubyte *texture,
 int width, bool use_alpha, int texture_width )
/*
 * Fills the memory space pointed to by texture with a string texture.  The
 * texture height must equal DC_LETTER_TEX_LENGTH.  readyLetterTextures() must
 * be called first, and freeLetterTextures() must be called after this
 * function's final use.
 */
{
  if (texture_width == 0)
    texture_width = width;

  // initialize texture
  if (!use_alpha)
    for (int t = DC_LETTER_TEX_LENGTH; t--; )
      for (int s = width; s--; ) {
        texture[(t * texture_width + s) * 4 + 0] = 0;
        texture[(t * texture_width + s) * 4 + 1] = 0;
        texture[(t * texture_width + s) * 4 + 2] = 0;
        texture[(t * texture_width + s) * 4 + 3] = 255;
      }

  int cursor = 0;
  int font = 0;
  int base_color = DC_DEFAULT_FONT_COLOR;
  for (unsigned int n = 0; n < strlen(string) && cursor < width; n++) {

    // special cases
    if (string[n] == '~') {
      switch (string[++n]) {
      case '<':
        if (cursor > DC_BACK_SPACE_WIDTH)
          cursor -= DC_BACK_SPACE_WIDTH;
        else
          cursor = 0;
        break;
      case '>':
        cursor += DC_BACK_SPACE_WIDTH;
        break;
      default:
        // color change
        if (isdigit(string[n])) {
          int c = string[n] - '0';
          if (c < DC_FONT_COLOR_NUMBER)
            base_color = c;

        // font change
        } else {
          int c = string[n] - 'a';
          if (c >= 0 && c < DC_FONT_NUMBER)
            font = c;
        }
        break;
      }

      if (string[n] != '~') continue;

    } else if (string[n] == ' ') {
      cursor += DC_SPACE_WIDTH;
      continue;
    }

    int code = mapCharToCode(string[n]);
    if (code == -1) continue;

    if (cursor + letter_widths[font][code] > width) break;

    // copy letter
    if (!use_alpha)
      for (int t = DC_LETTER_TEX_LENGTH; t--; )
        for (int s = DC_LETTER_TEX_LENGTH; s--; ) {
          if (cursor + s >= width) continue;

          float alpha = letter_textures[font][code][t * DC_LETTER_TEX_LENGTH
           + s] * (1.0f / 255.0f);
          for (int c = 3; c--; ) {
            float color = colors[base_color][c] + (1.0f - colors[base_color][c])
             * (DC_LETTER_TEX_LENGTH - t + (s >= letter_widths[font][code]
             ? DC_LETTER_TEX_LENGTH : s + (DC_LETTER_TEX_LENGTH
             - letter_widths[font][code])))
             * (0.5f / (float) DC_LETTER_TEX_LENGTH);
            float result = (255.0f * color * alpha)
             + texture[(t * texture_width + cursor + s) * 4 + c];
            if (result >= 255.0f)
              texture[(t * texture_width + cursor + s) * 4 + c] = (GLubyte) 255;
            else
              texture[(t * texture_width + cursor + s) * 4 + c]
               = (GLubyte) result;
          }
        }

    else
      for (int t = DC_LETTER_TEX_LENGTH; t--; )
        for (int s = DC_LETTER_TEX_LENGTH; s--; ) {
          if (cursor + s >= width)
            continue;
          if (letter_textures[font][code][t * DC_LETTER_TEX_LENGTH + s] == 0)
            continue;

          int old_alpha = texture[(t * texture_width + cursor + s) * 4 + 3];
          int new_alpha = letter_textures[font][code][t * DC_LETTER_TEX_LENGTH
           + s];
          if (new_alpha + old_alpha >= 255)
            texture[(t * texture_width + cursor + s) * 4 + 3] = 255;
          else
            texture[(t * texture_width + cursor + s) * 4 + 3]
             = (GLubyte) (new_alpha + old_alpha);

          for (int c = 3; c--; ) {
            float color = colors[base_color][c] + (1.0f - colors[base_color][c])
             * (DC_LETTER_TEX_LENGTH - t + (s >= letter_widths[font][code]
             ? DC_LETTER_TEX_LENGTH : s + (DC_LETTER_TEX_LENGTH
             - letter_widths[font][code])))
             * (0.5f / (float) DC_LETTER_TEX_LENGTH);
            float result = (255.0f * color) + (old_alpha * (1.0f / 255.0f))
             * texture[(t * texture_width + cursor + s) * 4 + c];
            if (result >= 255.0f)
              texture[(t * texture_width + cursor + s) * 4 + c] = 255;
            else
              texture[(t * texture_width + cursor + s) * 4 + c]
               = (GLubyte) result;
          }
        }

    cursor += letter_widths[font][code];
  }
}
