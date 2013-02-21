//#include <fstream>
#include <cmath>
#include <vector>
//#include <AaPrimitives>
#include "R3dSlicer.h"

using namespace std;

#ifdef DEBUG
#define ASSERT(x) assert (x)
#else
#define ASSERT(x)
#endif

namespace Aa
{
  namespace R3d
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::R3d::Slicer /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    Slicer::Slicer ()
    {
    }

    Slicer::~Slicer ()
    {
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::R3d::SlicerObjectX //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    SlicerObjectX::SlicerObjectX (GLfloat dx) :
      Slicer (),
      m_dx (dx)
    {
    }

    SlicerObjectX::~SlicerObjectX ()
    {
    }

    void SlicerObjectX::glDraw (const mat4 &, GLfloat step)
    {
      GLfloat s = step / m_dx;
      for (GLfloat x1 = 0.5f * fmod (1.0f, s), x2 = 1.0f - x1; x2 >= 0.0f; x1 += s, x2 -= s)
      {
        glBegin (GL_QUADS);

        glTexCoord3f (x1, 0.0f, 0.0f); glVertex3f (x1, 0.0f, 0.0f);
        glTexCoord3f (x1, 1.0f, 0.0f); glVertex3f (x1, 1.0f, 0.0f);
        glTexCoord3f (x1, 1.0f, 1.0f); glVertex3f (x1, 1.0f, 1.0f);
        glTexCoord3f (x1, 0.0f, 1.0f); glVertex3f (x1, 0.0f, 1.0f);

        glTexCoord3f (x2, 0.0f, 0.0f); glVertex3f (x2, 0.0f, 0.0f);
        glTexCoord3f (x2, 0.0f, 1.0f); glVertex3f (x2, 0.0f, 1.0f);
        glTexCoord3f (x2, 1.0f, 1.0f); glVertex3f (x2, 1.0f, 1.0f);
        glTexCoord3f (x2, 1.0f, 0.0f); glVertex3f (x2, 1.0f, 0.0f);

        glEnd ();
      }
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::R3d::SlicerObjectY //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    SlicerObjectY::SlicerObjectY (GLfloat dy) :
      Slicer (),
      m_dy (dy)
    {
    }

    SlicerObjectY::~SlicerObjectY ()
    {
    }

    void SlicerObjectY::glDraw (const mat4 &, GLfloat step)
    {
      GLfloat s = step / m_dy;
      for (GLfloat y1 = 0.5f * fmod (1.0f, s), y2 = 1.0f - y1; y2 >= 0.0f; y1 += s, y2 -= s)
      {
        glBegin (GL_QUADS);

        glTexCoord3f (0.0f, y2, 0.0f); glVertex3f (0.0f, y2, 0.0f);
        glTexCoord3f (1.0f, y2, 0.0f); glVertex3f (1.0f, y2, 0.0f);
        glTexCoord3f (1.0f, y2, 1.0f); glVertex3f (1.0f, y2, 1.0f);
        glTexCoord3f (0.0f, y2, 1.0f); glVertex3f (0.0f, y2, 1.0f);

        glTexCoord3f (0.0f, y1, 0.0f); glVertex3f (0.0f, y1, 0.0f);
        glTexCoord3f (0.0f, y1, 1.0f); glVertex3f (0.0f, y1, 1.0f);
        glTexCoord3f (1.0f, y1, 1.0f); glVertex3f (1.0f, y1, 1.0f);
        glTexCoord3f (1.0f, y1, 0.0f); glVertex3f (1.0f, y1, 0.0f);

        glEnd ();
      }
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::R3d::SlicerObjectZ //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    SlicerObjectZ::SlicerObjectZ (GLfloat dz) :
      Slicer (),
      m_dz (dz)
    {
    }

    SlicerObjectZ::~SlicerObjectZ ()
    {
    }

    void SlicerObjectZ::glDraw (const mat4 &, GLfloat step)
    {
      GLfloat s = step / m_dz;
      for (GLfloat z1 = 0.5f * fmod (1.0f, s), z2 = 1.0f - z1; z2 >= 0.0f; z1 += s, z2 -= s)
      {
        glBegin (GL_QUADS);

        glTexCoord3f (0.0f, 0.0f, z1); glVertex3f (0.0f, 0.0f, z1);
        glTexCoord3f (1.0f, 0.0f, z1); glVertex3f (1.0f, 0.0f, z1);
        glTexCoord3f (1.0f, 1.0f, z1); glVertex3f (1.0f, 1.0f, z1);
        glTexCoord3f (0.0f, 1.0f, z1); glVertex3f (0.0f, 1.0f, z1);

        glTexCoord3f (0.0f, 0.0f, z2); glVertex3f (0.0f, 0.0f, z2);
        glTexCoord3f (0.0f, 1.0f, z2); glVertex3f (0.0f, 1.0f, z2);
        glTexCoord3f (1.0f, 1.0f, z2); glVertex3f (1.0f, 1.0f, z2);
        glTexCoord3f (1.0f, 0.0f, z2); glVertex3f (1.0f, 0.0f, z2);

        glEnd ();
      }
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::R3d::SlicerViewNaive ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    SlicerViewNaive::SlicerViewNaive () :
      Slicer ()
    {
    }

    SlicerViewNaive::~SlicerViewNaive ()
    {
    }

    void SlicerViewNaive::glDraw (const mat4 & modelview, GLfloat step)
    {
      // Cube vertices.
      static const vec4 V [8] =
      {
        vec (0.0f, 0.0f, 0.0f, 1.0f),
        vec (1.0f, 0.0f, 0.0f, 1.0f),
        vec (0.0f, 1.0f, 0.0f, 1.0f),
        vec (1.0f, 1.0f, 0.0f, 1.0f),
        vec (0.0f, 0.0f, 1.0f, 1.0f),
        vec (1.0f, 0.0f, 1.0f, 1.0f),
        vec (0.0f, 1.0f, 1.0f, 1.0f),
        vec (1.0f, 1.0f, 1.0f, 1.0f)
      };

      // Projected cube vertices.
      vec3 projVertices [8];
      for (int i = 0; i < 8; ++i)
        projVertices [i] = modelview * V[i];

      // Maximum and minimum depths.
      GLfloat zMin = projVertices [0][2], zMax = zMin;
      for (int i = 1; i < 8; ++i)
        {
          GLfloat z = projVertices [i][2];
          if (z < zMin) zMin = z;
          if (z > zMax) zMax = z;
        }

      // Bounding box edges.
      static const VertexIndex EDGE_VERTICES [12][2] = {
        {0, 1}, {4, 5}, {2, 3}, {6, 7}, // along X
        {0, 2}, {4, 6}, {1, 3}, {5, 7}, // along Y
        {0, 4}, {2, 6}, {1, 5}, {3, 7}  // along Z
      };

      // Bounding box depth per edge.
      vec3 projEdgeVertices [12][2];
      for (int i = 0; i < 12; ++i)
      {
        projEdgeVertices [i][0] = projVertices [EDGE_VERTICES [i][0]];
        projEdgeVertices [i][1] = projVertices [EDGE_VERTICES [i][1]];
      }

      GLfloat edgeIntersections [12][3] = {
        {NAN, 0.0, 0.0},  //  0
        {NAN, 0.0, 1.0},  //  1
        {NAN, 1.0, 0.0},  //  2
        {NAN, 1.0, 1.0},  //  3
        {0.0, NAN, 0.0},  //  4
        {0.0, NAN, 1.0},  //  5
        {1.0, NAN, 0.0},  //  6
        {1.0, NAN, 1.0},  //  7
        {0.0, 0.0, NAN},  //  8
        {0.0, 1.0, NAN},  //  9
        {1.0, 0.0, NAN},  // 10
        {1.0, 1.0, NAN}}; // 11

      vec3 projEdgeIntersections [12];

      // Back-to-Front.
      for (GLfloat z = zMin + 0.5 * fmod (zMax - zMin, step); z <= zMax; z += step)
        {
          vector<EdgeIndex> realIntersections;
          for (int i = 0; i < 12; ++i)
          {
            GLfloat dz = projEdgeVertices [i][0][2] - projEdgeVertices [i][1][2];
            if (dz != 0.0)
            {
              GLfloat k = (projEdgeVertices [i][0][2] - z) / dz;
              if (k >= 0.0 && k <= 1.0)
              {
                edgeIntersections [i][i >> 2] = k;
                projEdgeIntersections [i] = projEdgeVertices [i][0] + (projEdgeVertices [i][1] - projEdgeVertices [i][0]) * k;
                realIntersections.push_back (i);
              }
            }
          }

          // B.M.E. Moret & H.D. Shapiro "P to NP" pp. 453
          int n = realIntersections.size ();

          vec3 center;
          for (int j = 0; j < n; ++j)
            center += projEdgeIntersections [j];
          center /= n;

          for (int j = 0; j < n; ++j){
            double theta = -10;
            int next = j;
            for (int k = j; k < n; ++k){
              vec3 d = projEdgeIntersections [realIntersections [k]] - center;
              if ((d[0] == 0) && (d[1] == 0)) {
                next = k;
                cout << "What the--" << endl;
                break;
              }
              double tt = d[1] / (fabs (d[0]) + fabs (d[1]));
              if (d[0] < 0.0) tt = 2.0 - tt;
              else if (d[1] < 0.0) tt = 4.0 + tt;
              if (theta <= tt) {
                next = k;
                theta = tt;
              }
            }
            int tmp = realIntersections [j];
            realIntersections [j] = realIntersections [next];
            realIntersections [next] = tmp;
          }

          glBegin (GL_POLYGON);
          for (int i = n; i--;)
          {
            const GLfloat * e = edgeIntersections [realIntersections [i]];
            glTexCoord3fv (e);
            glVertex3fv   (e);
          }
          glEnd ();
        }
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::R3d::SlicerViewTetra ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    void SlicerViewTetra::Edge (const vec3 & v0, const vec3 & v1, GLfloat z0, GLfloat z1, GLfloat z)
    {
      GLfloat k = (z - z0) / (z1 - z0);
      vec3 v = v0 + k * (v1 - v0);
      GL::TexCoord (v);
      GL::Vertex   (v);
    }

    void SlicerViewTetra::Tetra (const vec3 V [8], const GLfloat Z [8], GLfloat z, int i0, int i1, int i2, int i3)
    {
      int flags = 0x00;
      if (Z [i0] < z) flags |= 1;
      if (Z [i1] < z) flags |= 2;
      if (Z [i2] < z) flags |= 4;
      if (Z [i3] < z) flags |= 8;

      glBegin (GL_TRIANGLE_FAN);
      switch (flags)
      {
        case 0x00:
          break;

        case 0x01: // ok !
          SlicerViewTetra::Edge (V[i0], V[i3], Z[i0], Z[i3], z);
          SlicerViewTetra::Edge (V[i0], V[i2], Z[i0], Z[i2], z);
          SlicerViewTetra::Edge (V[i0], V[i1], Z[i0], Z[i1], z);
          break;

        case 0x02:
          SlicerViewTetra::Edge (V[i1], V[i0], Z[i1], Z[i0], z);
          SlicerViewTetra::Edge (V[i1], V[i2], Z[i1], Z[i2], z);
          SlicerViewTetra::Edge (V[i1], V[i3], Z[i1], Z[i3], z);
          break;

        case 0x03: // ok !
          SlicerViewTetra::Edge (V[i1], V[i2], Z[i1], Z[i2], z);
          SlicerViewTetra::Edge (V[i1], V[i3], Z[i1], Z[i3], z);
          SlicerViewTetra::Edge (V[i0], V[i3], Z[i0], Z[i3], z);
          SlicerViewTetra::Edge (V[i0], V[i2], Z[i0], Z[i2], z);
          break;

        case 0x04:
          SlicerViewTetra::Edge (V[i2], V[i3], Z[i2], Z[i3], z);
          SlicerViewTetra::Edge (V[i2], V[i1], Z[i2], Z[i1], z);
          SlicerViewTetra::Edge (V[i2], V[i0], Z[i2], Z[i0], z);
          break;

        case 0x05: // ok ?
          SlicerViewTetra::Edge (V[i0], V[i3], Z[i0], Z[i3], z);
          SlicerViewTetra::Edge (V[i2], V[i3], Z[i2], Z[i3], z);
          SlicerViewTetra::Edge (V[i1], V[i2], Z[i1], Z[i2], z);
          SlicerViewTetra::Edge (V[i0], V[i1], Z[i0], Z[i1], z);
          break;

        case 0x06:
          SlicerViewTetra::Edge (V[i0], V[i2], Z[i0], Z[i2], z);
          SlicerViewTetra::Edge (V[i2], V[i3], Z[i2], Z[i3], z);
          SlicerViewTetra::Edge (V[i1], V[i3], Z[i1], Z[i3], z);
          SlicerViewTetra::Edge (V[i0], V[i1], Z[i0], Z[i1], z);
          break;

        case 0x07: // ok !
          SlicerViewTetra::Edge (V[i3], V[i0], Z[i3], Z[i0], z);
          SlicerViewTetra::Edge (V[i3], V[i2], Z[i3], Z[i2], z);
          SlicerViewTetra::Edge (V[i3], V[i1], Z[i3], Z[i1], z);
          break;

        case 0x08: // ok !
          SlicerViewTetra::Edge (V[i3], V[i1], Z[i3], Z[i1], z);
          SlicerViewTetra::Edge (V[i3], V[i2], Z[i3], Z[i2], z);
          SlicerViewTetra::Edge (V[i3], V[i0], Z[i3], Z[i0], z);
          break;

        case 0x09:
          SlicerViewTetra::Edge (V[i0], V[i1], Z[i0], Z[i1], z);
          SlicerViewTetra::Edge (V[i1], V[i3], Z[i1], Z[i3], z);
          SlicerViewTetra::Edge (V[i2], V[i3], Z[i2], Z[i3], z);
          SlicerViewTetra::Edge (V[i0], V[i2], Z[i0], Z[i2], z);
          break;

        case 0x0A: // ok ?
          SlicerViewTetra::Edge (V[i0], V[i1], Z[i0], Z[i1], z);
          SlicerViewTetra::Edge (V[i1], V[i2], Z[i1], Z[i2], z);
          SlicerViewTetra::Edge (V[i2], V[i3], Z[i2], Z[i3], z);
          SlicerViewTetra::Edge (V[i0], V[i3], Z[i0], Z[i3], z);
          break;

        case 0x0B: // ok !
          SlicerViewTetra::Edge (V[i2], V[i0], Z[i2], Z[i0], z);
          SlicerViewTetra::Edge (V[i2], V[i1], Z[i2], Z[i1], z);
          SlicerViewTetra::Edge (V[i2], V[i3], Z[i2], Z[i3], z);
          break;

        case 0x0C: // ok !
          SlicerViewTetra::Edge (V[i0], V[i2], Z[i0], Z[i2], z);
          SlicerViewTetra::Edge (V[i0], V[i3], Z[i0], Z[i3], z);
          SlicerViewTetra::Edge (V[i1], V[i3], Z[i1], Z[i3], z);
          SlicerViewTetra::Edge (V[i1], V[i2], Z[i1], Z[i2], z);
          break;

        case 0x0D: // ok !
          SlicerViewTetra::Edge (V[i1], V[i3], Z[i1], Z[i3], z);
          SlicerViewTetra::Edge (V[i1], V[i2], Z[i1], Z[i2], z);
          SlicerViewTetra::Edge (V[i1], V[i0], Z[i1], Z[i0], z);
          break;

        case 0x0E:
          SlicerViewTetra::Edge (V[i0], V[i1], Z[i0], Z[i1], z);
          SlicerViewTetra::Edge (V[i0], V[i2], Z[i0], Z[i2], z);
          SlicerViewTetra::Edge (V[i0], V[i3], Z[i0], Z[i3], z);
          break;

        case 0x0F:
          break;
      }
      glEnd ();
    }

    void SlicerViewTetra::glDraw (const mat4 & m, GLfloat step)
    {
      // Cube vertices.
      static const vec4 V [8] =
      {
        vec (0.0f, 0.0f, 0.0f, 1.0f),
        vec (1.0f, 0.0f, 0.0f, 1.0f),
        vec (0.0f, 1.0f, 0.0f, 1.0f),
        vec (1.0f, 1.0f, 0.0f, 1.0f),
        vec (0.0f, 0.0f, 1.0f, 1.0f),
        vec (1.0f, 0.0f, 1.0f, 1.0f),
        vec (0.0f, 1.0f, 1.0f, 1.0f),
        vec (1.0f, 1.0f, 1.0f, 1.0f)
      };

      vec4 m2 = vec (m[0][2], m[1][2], m[2][2], m[3][2]);

      // Projected cube vertices.
      GLfloat Z [8];
      for (int i = 0; i < 8; ++i)
        Z [i] = DotProd (m2, V [i]);

      // Maximum and minimum depths.
      GLfloat zMin = Z [0], zMax = zMin;
      for (int i = 1; i < 8; ++i)
        {
          GLfloat z = Z [i];
          if (z < zMin) zMin = z;
          if (z > zMax) zMax = z;
        }

      // Back-to-Front.
      for (GLfloat z = zMin + 0.5 * fmod (zMax - zMin, step); z <= zMax; z += step)
      {
        SlicerViewTetra::Tetra (V, Z, z, 0, 1, 5, 7);
        SlicerViewTetra::Tetra (V, Z, z, 0, 5, 4, 7);
        SlicerViewTetra::Tetra (V, Z, z, 0, 4, 6, 7);
        SlicerViewTetra::Tetra (V, Z, z, 0, 6, 2, 7);
        SlicerViewTetra::Tetra (V, Z, z, 0, 2, 3, 7);
        SlicerViewTetra::Tetra (V, Z, z, 0, 3, 1, 7);
      }
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::R3d::SlicerViewCube /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    //         X
    //         |
    //         |
    //         +---- Y
    //        /
    //       Z

    // based on the mc table designed
    // by Paul Bourke and Cory Gene Bloyd.

    const SlicerViewCube::EdgeIndex SlicerViewCube::TABLE [256][8] =
    {
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 4,  8,  0, -1, -1, -1, -1, -1},
      { 0, 10,  6, -1, -1, -1, -1, -1},
      { 4,  8,  6, 10, -1, -1, -1, -1},
      { 4,  2,  9, -1, -1, -1, -1, -1},
      { 2,  9,  0,  8, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 2,  9,  6,  8, 10, -1, -1, -1},
      { 2,  6, 11, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 0, 10,  2, 11, -1, -1, -1, -1},
      { 4,  8,  2, 10, 11, -1, -1, -1},
      { 9,  4, 11,  6, -1, -1, -1, -1},
      { 6, 11,  0,  9,  8, -1, -1, -1},
      { 0, 10,  4, 11,  9, -1, -1, -1},
      { 9,  8, 11, 10, -1, -1, -1, -1},
      { 5,  1,  8, -1, -1, -1, -1, -1},
      { 5,  1,  4,  0, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {10,  6,  1,  4,  5, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 5,  1,  9,  0,  2, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 6,  2, 10,  9,  1,  5, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 2,  4, 11,  5, 10,  1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 5,  1,  9,  0, 11,  6, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {11,  9, 10,  5,  1, -1, -1, -1},
      { 1,  7, 10, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 1,  7,  0,  6, -1, -1, -1, -1},
      { 1,  7,  8,  6,  4, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 2,  9,  6,  8,  7,  1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {11,  2,  7,  0,  1, -1, -1, -1},
      {11,  2,  7,  4,  1,  8, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 7, 11,  1,  9,  0,  4, -1, -1},
      { 9,  8, 11,  1,  7, -1, -1, -1},
      { 7, 10,  5,  8, -1, -1, -1, -1},
      { 0,  4, 10,  5,  7, -1, -1, -1},
      { 8,  5,  0,  7,  6, -1, -1, -1},
      { 4,  5,  6,  7, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {10,  0,  7,  2,  5,  9, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 7,  6,  5,  2,  9, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 8,  5,  0,  7,  2, 11, -1, -1},
      { 5,  7,  4, 11,  2, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 7, 11,  5,  9, -1, -1, -1, -1},
      { 9,  3,  5, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 4,  2,  5,  3, -1, -1, -1, -1},
      { 8,  0,  5,  2,  3, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 5,  8,  3, 10,  2,  6, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 3,  5, 11,  4,  6, -1, -1, -1},
      { 5,  8,  3,  0, 11,  6, -1, -1},
      { 0, 10,  4, 11,  5,  3, -1, -1},
      {10, 11,  8,  3,  5, -1, -1, -1},
      { 9,  3,  8,  1, -1, -1, -1, -1},
      { 9,  3,  4,  1,  0, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {10,  6,  1,  4,  3,  9, -1, -1},
      { 4,  2,  8,  3,  1, -1, -1, -1},
      { 2,  3,  0,  1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 3,  1,  2, 10,  6, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {11,  3,  6,  1,  4,  8, -1, -1},
      { 1,  0,  3,  6, 11, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 1, 10,  3, 11, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 7, 10,  3,  8,  9, -1, -1, -1},
      { 0,  4, 10,  9,  7,  3, -1, -1},
      { 3,  7,  9,  6,  8,  0, -1, -1},
      { 6,  4,  7,  9,  3, -1, -1, -1},
      { 4,  2,  8,  3, 10,  7, -1, -1},
      { 2,  3,  0,  7, 10, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 6,  2,  7,  3, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {11,  3,  7, -1, -1, -1, -1, -1},
      {11,  7,  3, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 2,  6,  3,  7, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 7,  3, 10,  2,  0, -1, -1, -1},
      { 7,  3, 10,  2,  8,  4, -1, -1},
      { 9,  4,  3,  6,  7, -1, -1, -1},
      { 0,  6,  8,  7,  9,  3, -1, -1},
      { 3,  9,  7,  4, 10,  0, -1, -1},
      { 8, 10,  9,  7,  3, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {10,  1, 11,  3, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 6,  0, 11,  1,  3, -1, -1, -1},
      { 8,  1,  4,  3,  6, 11, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {10,  1,  6,  3,  2, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 2,  0,  3,  1, -1, -1, -1, -1},
      { 3,  2,  1,  4,  8, -1, -1, -1},
      { 9,  4,  3,  6,  1, 10, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 1,  3,  0,  9,  4, -1, -1, -1},
      { 3,  9,  1,  8, -1, -1, -1, -1},
      { 3, 11,  5, 10,  8, -1, -1, -1},
      { 3, 11,  5, 10,  4,  0, -1, -1},
      { 6,  0, 11,  8,  3,  5, -1, -1},
      { 4,  5,  6,  3, 11, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 6, 10,  2,  8,  3,  5, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 2,  0,  3,  8,  5, -1, -1, -1},
      { 2,  4,  3,  5, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 9,  5,  3, -1, -1, -1, -1, -1},
      {11,  7,  9,  5, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {11,  7,  2,  5,  4, -1, -1, -1},
      {11,  7,  2,  5,  0,  8, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 2,  6,  9,  7,  5, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 9,  2,  5,  0,  7, 10, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 4,  6,  5,  7, -1, -1, -1, -1},
      { 7,  5,  6,  8,  0, -1, -1, -1},
      { 5,  4,  7,  0, 10, -1, -1, -1},
      {10,  7,  8,  5, -1, -1, -1, -1},
      { 1,  8,  7,  9, 11, -1, -1, -1},
      { 4,  9,  0, 11,  1,  7, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 8,  4,  1,  2,  7, 11, -1, -1},
      { 0,  2,  1, 11,  7, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 1,  8,  7,  9,  6,  2, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 6,  7,  4,  1,  8, -1, -1, -1},
      { 7,  1,  6,  0, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 1, 10,  7, -1, -1, -1, -1, -1},
      { 5,  9,  1, 11, 10, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 6,  0, 11,  1,  9,  5, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 1,  5, 10,  4, 11,  2, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 5,  9,  1,  2, 10,  6, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 0,  1,  2,  5,  9, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 4,  6,  5, 10,  1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 1,  5,  0,  4, -1, -1, -1, -1},
      { 5,  8,  1, -1, -1, -1, -1, -1},
      { 9, 11,  8, 10, -1, -1, -1, -1},
      {11, 10,  9,  0,  4, -1, -1, -1},
      { 9, 11,  8,  6,  0, -1, -1, -1},
      { 4,  9,  6, 11, -1, -1, -1, -1},
      {10,  8, 11,  4,  2, -1, -1, -1},
      {10,  0, 11,  2, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 2, 11,  6, -1, -1, -1, -1, -1},
      { 8,  9, 10,  2,  6, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1},
      { 9,  2,  8,  0, -1, -1, -1, -1},
      { 4,  9,  2, -1, -1, -1, -1, -1},
      { 8,  4, 10,  6, -1, -1, -1, -1},
      { 0,  6, 10, -1, -1, -1, -1, -1},
      { 4,  0,  8, -1, -1, -1, -1, -1},
      {-1, -1, -1, -1, -1, -1, -1, -1}
    };

    SlicerViewCube::SlicerViewCube () :
      Slicer ()
    {
    }

    SlicerViewCube::~SlicerViewCube ()
    {
    }

    void SlicerViewCube::glDraw (const mat4 & m, GLfloat step)
    {
      // Cube vertices.
      static const vec4 V [8] =
      {
        vec (0.0f, 0.0f, 0.0f, 1.0f),
        vec (1.0f, 0.0f, 0.0f, 1.0f),
        vec (0.0f, 1.0f, 0.0f, 1.0f),
        vec (1.0f, 1.0f, 0.0f, 1.0f),
        vec (0.0f, 0.0f, 1.0f, 1.0f),
        vec (1.0f, 0.0f, 1.0f, 1.0f),
        vec (0.0f, 1.0f, 1.0f, 1.0f),
        vec (1.0f, 1.0f, 1.0f, 1.0f)
      };

      vec4 m2 = vec (m[0][2], m[1][2], m[2][2], m[3][2]);

      // Bounding box depth per vertex.
      GLfloat vZ [8]; // cube depths.
      for (int i = 0; i < 8; ++i)
        vZ [i] = DotProd (m2, V[i]);

      // Maximum and minimum depths.
      GLfloat zMin = vZ [0], zMax = zMin;
      for (int i = 1; i < 8; ++i)
        {
          GLfloat z = vZ [i];
          if (z < zMin) zMin = z;
          if (z > zMax) zMax = z;
        }

      // Bounding box edges.
      static const VertexIndex EDGE_VERTICES [12][2] = {
        {0, 1}, {4, 5}, {2, 3}, {6, 7}, // along X
        {0, 2}, {4, 6}, {1, 3}, {5, 7}, // along Y
        {0, 4}, {2, 6}, {1, 5}, {3, 7}  // along Z
      };

      // Bounding box depth per edge.
      double eZ [12][2];
      for (int i = 0; i < 12; ++i)
      {
        eZ [i][0] = vZ [EDGE_VERTICES [i][0]];
        eZ [i][1] = vZ [EDGE_VERTICES [i][1]];
      }

      double edgeIntersections [12][3] = {
        {NAN, 0.0, 0.0}, //  0
        {NAN, 0.0, 1.0}, //  1
        {NAN, 1.0, 0.0}, //  2
        {NAN, 1.0, 1.0}, //  3
        {0.0, NAN, 0.0}, //  4
        {0.0, NAN, 1.0}, //  5
        {1.0, NAN, 0.0}, //  6
        {1.0, NAN, 1.0}, //  7
        {0.0, 0.0, NAN}, //  8
        {0.0, 1.0, NAN}, //  9
        {1.0, 0.0, NAN}, // 10
        {1.0, 1.0, NAN}  // 11
      };

      // Back-to-Front.
      for (GLfloat z = zMin + 0.5 * fmod (zMax - zMin, step); z <= zMax; z += step)
        {
          // This bitfield tells us which vertices of the bounding box
          // have already been passed by the "clipping" plane.
          int tableIndex = 0x00;
          for (int k = 8; k--;) if (vZ [k] < z) tableIndex |= (1 << k);

          const EdgeIndex * t = TABLE [tableIndex];
          if (t[0] != -1)
            {
              glBegin (GL_TRIANGLE_STRIP);

              // At least 3 vertices.
              EdgeIndex t0 = t[0];
              GLdouble * e0 = edgeIntersections [t0];
              e0 [t0 >> 2] = (eZ [t0][0] - z) / (eZ [t0][0] - eZ [t0][1]);
              glTexCoord3dv (e0);
              glVertex3dv   (e0);

              EdgeIndex t1 = t[1];
              GLdouble * e1 = edgeIntersections [t1];
              e1 [t1 >> 2] = (eZ [t1][0] - z) / (eZ [t1][0] - eZ [t1][1]);
              glTexCoord3dv (e1);
              glVertex3dv   (e1);

              EdgeIndex t2 = t[2];
              GLdouble * e2 = edgeIntersections [t2];
              e2 [t2 >> 2] = (eZ [t2][0] - z) / (eZ [t2][0] - eZ [t2][1]);
              glTexCoord3dv (e2);
              glVertex3dv   (e2);

              // And maybe more...
              for (int k = 3; t[k] != -1;)
                {
                  EdgeIndex tk = t[k++];
                  double * ek = edgeIntersections [tk];
                  ek [tk >> 2] = (eZ [tk][0] - z) / (eZ [tk][0] - eZ [tk][1]);
                  glTexCoord3dv (ek);
                  glVertex3dv   (ek);
                }

              glEnd ();
            }
        }
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::R3d::SlicerViewCubeGLSLv1 ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    SlicerViewCubeGLSLv1::SlicerViewCubeGLSLv1 (Aa::GL::Program * program) :
      SlicerViewCube (),
      m_program (program)
    {
      m_program->attach (GL_VERTEX_SHADER, GL::Program::String ("/Aa/R3d/McSlicing.vertex"));
    }

    SlicerViewCubeGLSLv1::~SlicerViewCubeGLSLv1 ()
    {
    }

    void SlicerViewCubeGLSLv1::glDraw (const mat4 & m, GLfloat dz)
    {
      // Cube vertices.
      static const vec4 V [8] =
      {
        vec (0.0f, 0.0f, 0.0f, 1.0f),
        vec (1.0f, 0.0f, 0.0f, 1.0f),
        vec (0.0f, 1.0f, 0.0f, 1.0f),
        vec (1.0f, 1.0f, 0.0f, 1.0f),
        vec (0.0f, 0.0f, 1.0f, 1.0f),
        vec (1.0f, 0.0f, 1.0f, 1.0f),
        vec (0.0f, 1.0f, 1.0f, 1.0f),
        vec (1.0f, 1.0f, 1.0f, 1.0f)
      };

      vec4 m2 = vec (m[0][2], m[1][2], m[2][2], m[3][2]);

      // Bounding box depth per vertex.
      GLfloat vertex_depths [8]; // cube depths.
      for (int i = 0; i < 8; ++i)
        vertex_depths [i] = DotProd (m2, V[i]);

      // Maximum and minimum depths.
      GLfloat zMin = vertex_depths [0], zMax = zMin;
      for (int i = 1; i < 8; ++i)
        {
          GLfloat z = vertex_depths [i];
          if (z < zMin) zMin = z;
          if (z > zMax) zMax = z;
        }

      // Bounding box edges.
      static const VertexIndex EDGES [12][2] = {
        {0, 1}, {4, 5}, {2, 3}, {6, 7}, // along X
        {0, 2}, {4, 6}, {1, 3}, {5, 7}, // along Y
        {0, 4}, {2, 6}, {1, 5}, {3, 7}  // along Z
      };

      // Bounding box depth per edge.
      vec2 edge_depths [12];
      for (int i = 0; i < 12; ++i)
      {
        edge_depths [i][0] = vertex_depths [EDGES [i][0]];
        edge_depths [i][1] = vertex_depths [EDGES [i][1]];
      }

      m_program->set<GLfloat, 2> ("aa_r3d_slicer_edge_depths", 12, edge_depths);

      GLint aa_r3d_slicer_z = m_program->location ("aa_r3d_slicer_z");
      if (aa_r3d_slicer_z == -1) {cerr << "aa_r3d_slicer_z" << endl; throw 1;}

      // Back-to-Front.
      for (GLfloat z = zMin + 0.5 * fmod (zMax - zMin, dz); z <= zMax; z += dz)
        {
          glUniform1fARB (aa_r3d_slicer_z, z);

          // This bitfield tells us which vertices of the bounding box
          // have already been passed by the "clipping" plane.
          int tableIndex = 0;
          for (int k = 8; k--;)
            if (vertex_depths [k] < z)
              tableIndex |= (1 << k);

          const EdgeIndex * t = TABLE [tableIndex];
          if (t[0] != -1)
            {
              glBegin (GL_TRIANGLE_STRIP);

              // At least 3 vertices.
              glVertexAttrib1s (0, t[0]);
              glVertexAttrib1s (0, t[1]);
              glVertexAttrib1s (0, t[2]);
              // And maybe more...
              for (int k = 3; t[k] != -1;) glVertexAttrib1s (0, t[k++]);

              glEnd ();
            }
        }
    }
////////////////////////////////////////////////////////////////////////////////
// Aa::R3d::SlicerViewCubeGLSLv2 ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    SlicerViewCubeGLSLv2::SlicerViewCubeGLSLv2 (Aa::GL::Program * program) :
      SlicerViewCube (),
      m_program (program),
      m_table_tex2d (0)
    {
      m_program->attach (GL_VERTEX_SHADER,   GL::Program::String ("/Aa/PassThru.vertex"));
      m_program->attach (GL_GEOMETRY_SHADER, GL::Program::String ("/Aa/R3d/McSlicing.geometry"));

#if 0
      GLuint id = m_program->id ();
      glProgramParameteriARB (id, GL_GEOMETRY_INPUT_TYPE_ARB,   GL_POINTS);
      glProgramParameteriARB (id, GL_GEOMETRY_OUTPUT_TYPE_ARB,  GL_TRIANGLE_STRIP);
      glProgramParameteriARB (id, GL_GEOMETRY_VERTICES_OUT_ARB, 6);
#endif

      glGenTextures (1, &m_table_tex2d);
      glBindTexture (GL_TEXTURE_2D, m_table_tex2d);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
      glTexImage2D (GL_TEXTURE_2D, 0, GL_R8I, 8, 256, 0, GL_RED_INTEGER, GL_BYTE, TABLE);
    }

    SlicerViewCubeGLSLv2::~SlicerViewCubeGLSLv2 ()
    {
    }

    void SlicerViewCubeGLSLv2::glDraw (const mat4 & m, GLfloat dz)
    {
      // Cube vertices.
      static const vec4 V [8] =
      {
        vec (0.0f, 0.0f, 0.0f, 1.0f),
        vec (1.0f, 0.0f, 0.0f, 1.0f),
        vec (0.0f, 1.0f, 0.0f, 1.0f),
        vec (1.0f, 1.0f, 0.0f, 1.0f),
        vec (0.0f, 0.0f, 1.0f, 1.0f),
        vec (1.0f, 0.0f, 1.0f, 1.0f),
        vec (0.0f, 1.0f, 1.0f, 1.0f),
        vec (1.0f, 1.0f, 1.0f, 1.0f)
      };

      vec4 m2 = vec (m[0][2], m[1][2], m[2][2], m[3][2]);

      // Bounding box depth per vertex.
      GLfloat vertex_depths [8]; // cube depths.
      for (int i = 0; i < 8; ++i)
        vertex_depths [i] = DotProd (m2, V[i]);

      // Maximum and minimum depths.
      GLfloat zMin = vertex_depths [0], zMax = zMin;
      for (int i = 1; i < 8; ++i)
      {
        GLfloat z = vertex_depths [i];
        if (z < zMin) zMin = z;
        if (z > zMax) zMax = z;
      }

      // Bounding box edges.
      static const VertexIndex EDGES [12][2] =
      {
        {0, 1}, {4, 5}, {2, 3}, {6, 7}, // along X
        {0, 2}, {4, 6}, {1, 3}, {5, 7}, // along Y
        {0, 4}, {2, 6}, {1, 5}, {3, 7}  // along Z
      };

      // Bounding box depth per edge.
      vec2 edge_depths [12];
      for (int i = 0; i < 12; ++i)
      {
        edge_depths [i][0] = vertex_depths [EDGES [i][0]];
        edge_depths [i][1] = vertex_depths [EDGES [i][1]];
      }

      glActiveTexture (GL_TEXTURE3);
      glBindTexture (GL_TEXTURE_2D, m_table_tex2d);

      glActiveTexture (GL_TEXTURE0);

      m_program->set<GLint>      ("aa_r3d_slicer_table",         3);
      m_program->set<GLfloat>    ("aa_r3d_slicer_vertex_depths", 8, vertex_depths);
      m_program->set<GLfloat, 2> ("aa_r3d_slicer_edge_depths",  12, edge_depths);

      // Back-to-Front.
      glBegin (GL_POINTS);
      GLfloat z0 = zMin + 0.5 * fmod (zMax - zMin, dz);
      for (GLfloat z = z0; z <= zMax; z += dz)
        glVertexAttrib1f (0, z);
      glEnd ();
    }

#if 0
    void ImageRenderer3d::glSlice (bool motion)
    {
      glEnable (GL_TEXTURE_GEN_S);
      glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
      static const GLfloat S [] = {1.0f, 0.0f, 0.0f, 0.0f};
      glTexGenfv (GL_S, GL_OBJECT_PLANE, S);

      glEnable (GL_TEXTURE_GEN_T);
      glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
      static const GLfloat T [] = {0.0f, 1.0f, 0.0f, 0.0f};
      glTexGenfv (GL_T, GL_OBJECT_PLANE, T);

      glEnable (GL_TEXTURE_GEN_R);
      glTexGeni (GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
      static const GLfloat R [] = {0.0f, 0.0f, 1.0f, 0.0f};
      glTexGenfv (GL_R, GL_OBJECT_PLANE, R);

      switch (m_slice)
      {
        case SLICER_NONE       : break;
        case SLICER_OBJECT_X   : glSlice_ObjectAligned_X   (motion); break;
        case SLICER_OBJECT_Y   : glSlice_ObjectAligned_Y   (motion); break;
        case SLICER_OBJECT_Z   : glSlice_ObjectAligned_Z   (motion); break;
        case SLICER_VIEW_CUBE  : glSlice_ViewAligned_Cube  (motion); break;
        case SLICER_VIEW_TETRA : glSlice_ViewAligned_Tetra (motion); break;
        case SLICER_VIEW_NAIVE : glSlice_ViewAligned_Naive (motion); break;
      }

      glDisable (GL_TEXTURE_GEN_S);
      glDisable (GL_TEXTURE_GEN_T);
      glDisable (GL_TEXTURE_GEN_R);
    }

    enum SlicerManager::SlicerType SlicerManager::Next (enum SlicerManager::SlicerType t)
    {
      switch (t)
      {
        case SLICER_NONE           : return SLICER_OBJECT_X;
        case SLICER_OBJECT_X       : return SLICER_OBJECT_Y;
        case SLICER_OBJECT_Y       : return SLICER_OBJECT_Z;
        case SLICER_OBJECT_Z       : return SLICER_VIEW_NAIVE;
        case SLICER_VIEW_NAIVE     : return SLICER_VIEW_TETRA;
        case SLICER_VIEW_TETRA     : return SLICER_VIEW_CUBE;
        case SLICER_VIEW_CUBE      : return SLICER_VIEW_CUBE_GLSL;
        case SLICER_VIEW_CUBE_GLSL : return SLICER_NONE;
        default                    : return SLICER_NONE;
      }
    }

    std::string SlicerManager::Name (enum SlicerManager::SlicerType t)
    {
      switch (t)
      {
        case SLICER_NONE           : return "SLICER_NONE";
        case SLICER_OBJECT_X       : return "SLICER_OBJECT_X";
        case SLICER_OBJECT_Y       : return "SLICER_OBJECT_Y";
        case SLICER_OBJECT_Z       : return "SLICER_OBJECT_Z";
        case SLICER_VIEW_NAIVE     : return "SLICER_VIEW_NAIVE";
        case SLICER_VIEW_TETRA     : return "SLICER_VIEW_TETRA";
        case SLICER_VIEW_CUBE      : return "SLICER_VIEW_CUBE";
        case SLICER_VIEW_CUBE_GLSL : return "SLICER_VIEW_CUBE_GLSL";
        default                    : return "?";
      }
    }
#endif

  } // namespace R3d
} // namespace Aa

