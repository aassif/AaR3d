#ifndef __AA_R3D_IMAGE_RENDERER_3D__
#define __AA_R3D_IMAGE_RENDERER_3D__

#include "R3dImageRenderer.h"

namespace Aa
{
  namespace R3d
  {
    class AA_R3D_API ImageRenderer3d : public ImageRenderer
    {
      public:
        enum Slice
        {
          SLICE_NONE,
          // object-aligned slices.
          SLICE_OBJECT_X,
          SLICE_OBJECT_Y,
          SLICE_OBJECT_Z,
          // view-aligned slices.
          SLICE_VIEW_CUBE,
          SLICE_VIEW_TETRA,
          SLICE_VIEW_NAIVE
        };

      public:
        typedef GLbyte VertexIndex;
        typedef GLbyte EdgeIndex;
        static const EdgeIndex TABLE [256][8];
        static const float DEFAULT_BOX_COLOR [4];

      public:
        // Next slice mode.
        static Slice NextSlice (Slice);
        // Slice mode name.
        static const char * StrSlice (Slice);

      protected:
        GLuint  m_image_tex3d;
        GLfloat m_image_scales [3];
        GLfloat m_steps [2];
        GLuint  m_lut_tex1d [2];
        GLuint  m_lut_tex2d [2];
        dbox3   m_box;
        float   m_box_color [4];
        Slice   m_slice;
        bool    m_textured:1;
        bool    m_outlined:1;
        bool    m_boxed:1;

      protected:
        virtual void glSlice_ObjectAligned_X   (bool) const;
        virtual void glSlice_ObjectAligned_Y   (bool) const;
        virtual void glSlice_ObjectAligned_Z   (bool) const;
        virtual void glSlice_ViewAligned_Cube  (bool) const;
        virtual void glSlice_ViewAligned_Tetra (bool) const;
        virtual void glSlice_ViewAligned_Naive (bool) const;
        virtual void glSlice                   (bool) const;

      public:
        ImageRenderer3d (const Image * = NULL, const Lut * = NULL);
        virtual ~ImageRenderer3d ();
        virtual void setImg (const Image * = NULL);
        virtual void setLut (const Lut * = NULL);
        virtual void glDraw (bool = false);
        virtual void glPreDraw (bool = false) const = 0;
        virtual void glPostDraw (bool = false) const = 0;
        // Settings.
        enum Slice getSlice () const;
        void setSlice (enum Slice);
        // Step.
        double getStep (bool motion) const;
        void setStep (bool motion, double);
        // Outlined.
        bool isOutlined () const;
        void setOutlined (bool);
        // Textured.
        bool isTextured () const;
        void setTextured (bool);
        // Bounding box.
        bool isBoxed () const;
        void setBoxed (bool);
    };
  } // namespace R3d
} // namespace Aa

#endif // __AA_R3D_IMAGE_RENDERER_3D__

