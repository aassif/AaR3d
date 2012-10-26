#include <fstream>
#include <cmath>
//#include <GL/glew.h>
//#include <AaText>
//#include <AaGL>
#include "R3dImageRenderer2d.h"

using namespace std;
using namespace Aa::Math;

#ifdef DEBUG
#define ASSERT(x) assert (x)
#else
#define ASSERT(x)
#endif

namespace Aa
{
  namespace R3d
  {
    /*
    ** ImageRenderer2d.
    */

    unsigned char * padImage2dxy (unsigned short sx, unsigned short sy,
                                  const unsigned char * src,
                                  unsigned short dx, unsigned short dy)
    {
#if 0
      cout << "padImage2dxy ("
           << "sx = " << sx << ", sy = " << sy << ", "
           << "src = " << (void *) src << ", "
           << "dx = " << dx << ", dy = " << dy << ")\n";
#endif
      // Aliases.
      unsigned long dxdy = dx * dy;
      // Memory allocation.
      unsigned char * dst = new unsigned char [dxdy];
      // Iterators.
      const unsigned char * s = src;
      unsigned char * d = dst;
#if 0
      // Fill with zeros.
      fill (dst, dst + dxdy, 0);
      for (unsigned short y = sy; y--; d += dx) copy (s, s += sx, d);
#else
      // Copy border.
      unsigned short vx = dx - sx;
      for (unsigned short y = sy; y--;)
      {
        d = copy (s, s + sx, d); s += sx;
        unsigned char c = d [-1]; // crash if sx < 1
        fill (d, d + vx, c); d += vx;
      }
      unsigned short vy = dy - sy;
      for (unsigned short y = vy; y--;)
        d = copy (d - dx, d, d); // crash if sy < 1
#endif
      return dst;
    }

    unsigned char * padImage2d_y (unsigned short sx, unsigned short sy,
                                  const unsigned char * src,
                                  unsigned short dy)
    {
#if 0
      cout << "padImage2dy_ ("
           << "sx = " << sx << ", sy = " << sy << ", "
           << "src = " << (void *) src << ", "
           << "dy = " << dy << ")\n";
#endif
      // Aliases.
      unsigned long sxdy = sx * dy;
      // Memory allocation.
      unsigned char * dst = new unsigned char [sxdy];
#if 0
      // Fill with zeros.
      fill (dst, dst + sxdy, 0);
      copy (src, src + sx * sy, dst);
#else
      // Copy border.
      unsigned char * d = dst;
      unsigned short vy = dy - sy;
      d = copy (src, src + sx * sy, dst);
      for (unsigned short y = vy; y--;)
        d = copy (d - sx, d, d); // crash if sy < 1
#endif
      return dst;
    }

    unsigned char * padImage2d__ (unsigned short sx, unsigned short sy,
                                  const unsigned char * src)
    {
#if 0
      cout << "padImage2d__ ("
           << "sx = " << sx << ", sy = " << sy << ", "
           << "src = " << (void *) src << ")\n";
#endif
      unsigned long sxsy = sx * sy;
      unsigned char * dst = new unsigned char [sxsy];
      copy (src, src + sxsy, dst);
      return dst;
    }

    unsigned char * padImage2d (unsigned short sx, unsigned short sy,
                                const unsigned char * src,
                                unsigned short dx, unsigned short dy)
    {
      if (dx != sx) return padImage2dxy (sx, sy, src, dx, dy);
      if (dy != sy) return padImage2d_y (sx, sy, src,     dy);
      /*---------*/ return padImage2d__ (sx, sy, src        );
    }

    ImageRenderer2d::ImageRenderer2d (const Image * img, const Lut * lut) :
      ImageRenderer (),
      m_dz (0),
      m_img_textures (NULL),
      m_lut ()
    {
      this->setImg (img);
      this->setLut (lut);
    }

    ImageRenderer2d::~ImageRenderer2d ()
    {
      this->setImg (NULL);
      this->setLut (NULL);
    }

    void ImageRenderer2d::setImg (const Image * image)
    {
      if (m_img_textures != NULL)
      {
        m_dz = 0;
        glDeleteTextures (m_dz, m_img_textures);
        delete[] m_img_textures; m_img_textures = NULL;
      }

      if (image != NULL)
      {
        const unsigned int d1x = image->dx ();
        const unsigned int d1y = image->dy ();
        const unsigned int d1z = image->dz ();

        unsigned int d2x = (1 << (unsigned int) ceil (log2 (d1x)));
        unsigned int d2y = (1 << (unsigned int) ceil (log2 (d1y)));

        m_dz = d1z;
        m_img_textures = new GLuint [d1z];
        glGenTextures (d1z, m_img_textures);
        m_img_scales [0] = (double) d1x / d2x;
        m_img_scales [1] = (double) d1y / d2y;
        m_box = image->box ();

        glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
        const unsigned char * b = (const unsigned char *) image->begin ();
        for (unsigned int z = 0; z < d1z; ++z, b += d1x * d1y)
        {
          glBindTexture (GL_TEXTURE_2D, m_img_textures [z]);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
          const unsigned char * b2 = padImage2d (d1x, d1y, b, d2x, d2y);
          glTexImage2D (GL_TEXTURE_2D, 0,
                        GL_COLOR_INDEX8_EXT, d1x, d1y, 0,
                        GL_COLOR_INDEX, GL_UNSIGNED_BYTE, b2);
          //cout << "glTexImage2D : " << gluErrorString (glGetError ()) << endl;
          delete[] b2;
        }
      }
    }

    void ImageRenderer2d::setLut (const Lut * lut)
    {
      if (lut == NULL) return;
      for (AaUInt k = 0; k < 256; ++k)
      {
#ifdef MODE_MIP
        *(l2++) = l1 [0];
        *(l2++) = l1 [1];
        *(l2++) = l1 [2];
        *(l2++) = 255;
#else
        const Lut::Entry & e = (*lut) [k];
        float alpha = e [3] / 255.0;
        m_lut [4 * k + 0] = (AaUInt8) rint (alpha * e [0]);
        m_lut [4 * k + 1] = (AaUInt8) rint (alpha * e [1]);
        m_lut [4 * k + 2] = (AaUInt8) rint (alpha * e [2]);
        m_lut [4 * k + 3] = (AaUInt8) rint (alpha * 255);
#endif
      }
    }

    void ImageRenderer2d::glDraw (bool /* motion */)
    {
      if (m_dz == 0) return;

      // Texture2d ON.
      glEnable (GL_TEXTURE_2D);

      // glDraw should *always* be called with matrixMode == GL_MODELVIEW.
      //GLint matrixMode; glGetIntegerv (GL_MATRIX_MODE, &matrixMode);

      // We use the texture matrix to eliminate the 2**k padding.
      glMatrixMode (GL_TEXTURE);
      glPushMatrix ();
      glLoadIdentity ();
      glBindTexture (GL_TEXTURE_2D, m_img_textures [0]);
      glScaled (m_img_scales [0], m_img_scales [1], 1.0);

      // Then we switch back to the previous matrix mode.
      glMatrixMode (GL_MODELVIEW); // matrixMode

      // We use this transfromation so that normalized coordinates
      // map to real world coordinates of the bounding box ...
      glPushMatrix ();
      GL::Translate (m_box.pos ());
      GL::Scale     (m_box.dim ());

      // Lighting OFF.
      glDisable (GL_LIGHTING);
      // Blending ON.
      glEnable (GL_BLEND);
      glDepthMask (GL_FALSE); //FIXME
      glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
#ifdef MODE_MIP
      // MIP blending equation.
      glBlendEquationEXT (GL_MIN);
      //glBlendEquation (GL_MAX);
#else
      // Alpha test ON.
      glEnable (GL_ALPHA_TEST);
      glAlphaFunc (GL_GREATER, 0.0);
      glBlendEquationEXT (GL_FUNC_ADD);
      glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
#endif

      // Transfer function.
      glEnable (GL_SHARED_TEXTURE_PALETTE_EXT);
      glColorTableEXT (GL_SHARED_TEXTURE_PALETTE_EXT, GL_RGBA, 256, GL_RGBA, GL_UNSIGNED_BYTE, m_lut);

#ifdef MODE_MIP
      // Bounding box vertices.
      static const double bbv [] =
      {
        0.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        1.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 0.0, 1.0,
        0.0, 1.0, 1.0,
        1.0, 1.0, 1.0
      };

      // Bounding box faces.
      static const unsigned char bbf [] =
      {
        0, 1, 3, 2, // Far
        0, 2, 6, 4, // Left
        4, 6, 7, 5, // Near
        1, 5, 7, 3, // Right
        0, 4, 5, 1, // Down
        2, 3, 7, 6  // Up
      };

      // Bounding box background (BBB).
      glColor4f (1.0, 1.0, 1.0, 1.0); // FIXME: min
      //glColor4f (0.0, 0.0, 0.0, 1.0); // FIXME: max
      glEnableClientState (GL_VERTEX_ARRAY);
      glVertexPointer (3, GL_DOUBLE, 0, bbv);
      glDrawElements (GL_QUADS, 24, GL_UNSIGNED_BYTE, bbf);
      glDisableClientState (GL_VERTEX_ARRAY);
#endif

      double inter [][3] = {
        {0.0, 0.0, NAN},
        {1.0, 0.0, NAN},
        {0.0, 1.0, NAN},
        {1.0, 1.0, NAN}};

      /* Contour : uncomment the next two lines */
      /* Contour */ //glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
      /* Contour */ //glColor4f (0.0, 0.0, 0.0, 1.0);

      for (unsigned short z = 0; z < m_dz; ++z)
        {
          double k = (0.5 + z) / m_dz;
          inter [0][2] = k; inter [1][2] = k;
          inter [2][2] = k; inter [3][2] = k;
          // Texture binding.
          glBindTexture (GL_TEXTURE_2D, m_img_textures [z]);
          // glQuad.
          glBegin (GL_QUADS);
          glTexCoord2dv (inter [0]); glVertex3dv (inter [0]);
          glTexCoord2dv (inter [1]); glVertex3dv (inter [1]);
          glTexCoord2dv (inter [3]); glVertex3dv (inter [3]);
          glTexCoord2dv (inter [2]); glVertex3dv (inter [2]);
          glEnd ();
        }
      for (unsigned short z = m_dz; z--;)
        {
          double k = (0.5 + z) / m_dz;
          inter [0][2] = k; inter [1][2] = k;
          inter [2][2] = k; inter [3][2] = k;
          // Texture binding.
          glBindTexture (GL_TEXTURE_2D, m_img_textures [z]);
          // The transfer function is up to date.
          // glQuad.
          glBegin (GL_QUADS);
          glTexCoord2dv (inter [2]); glVertex3dv (inter [2]);
          glTexCoord2dv (inter [3]); glVertex3dv (inter [3]);
          glTexCoord2dv (inter [1]); glVertex3dv (inter [1]);
          glTexCoord2dv (inter [0]); glVertex3dv (inter [0]);
          glEnd ();
        }

      glDisable (GL_SHARED_TEXTURE_PALETTE_EXT);
      glMatrixMode (GL_TEXTURE);
      glPopMatrix ();
      glMatrixMode (GL_MODELVIEW);
      glPopMatrix ();
      glDisable (GL_TEXTURE_2D);
      glDisable (GL_BLEND);
      //glDisable (GL_ALPHA_TEST);
      glDepthMask (GL_TRUE); //FIXME
#ifdef MODE_MIP
      glBlendEquation (GL_FUNC_ADD);
#else
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif
    }
  } // namespace R3d
} // namespace Aa

