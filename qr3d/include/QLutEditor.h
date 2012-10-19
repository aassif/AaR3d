#ifndef AA_R3D_QLUT_EDITOR__H
#define AA_R3D_QLUT_EDITOR__H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
//#include <QMouseEvent>
//#include <QGraphicsSceneMouseEvent>
#include <QDomDocument>

namespace Aa
{
  namespace R3d
  {

    class QLutKnob;
    class QLutEditor;

////////////////////////////////////////////////////////////////////////////////
// Aa::R3d::QLutKnob ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    class QLutKnob : public QGraphicsEllipseItem
    {
      public:
        static const int RADIUS = 3;
        enum {NORMAL = 0, VERTICAL = 1, MAGIC = 2};

      protected:
        QLutEditor * m_editor;
        QLutKnob * m_prev;
        QLutKnob * m_next;
        QColor m_color;
        int m_mode;

      protected:
        void setToolTip ();

      public:
        QLutKnob (QLutEditor *,
                  QLutKnob * prev,
                  QLutKnob * next,
                  const QPointF &,
                  const QColor &,
                  int mode = NORMAL);

        QLutKnob (QLutEditor *,
                  QLutKnob * prev,
                  QLutKnob * next,
                  const QDomElement &);

        virtual ~QLutKnob ();

        void setColor (const QColor &);
        QColor color () const;

        QLutKnob * prev () const {return m_prev;}
        QLutKnob * next () const {return m_next;}

        void validate ();

        QDomElement dom (QDomDocument &) const;
        void init (const QDomElement &);

        //virtual void mouseMoveEvent        (QGraphicsSceneMouseEvent *);
        virtual void mousePressEvent       (QGraphicsSceneMouseEvent *);
        //virtual void mouseReleaseEvent     (QGraphicsSceneMouseEvent *);
        virtual void mouseDoubleClickEvent (QGraphicsSceneMouseEvent *);
        virtual void contextMenuEvent      (QGraphicsSceneContextMenuEvent *);
        virtual QVariant itemChange (GraphicsItemChange, const QVariant &);
    };

////////////////////////////////////////////////////////////////////////////////
// Aa::R3d::QLutEditor /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    class QLutEditor : public QGraphicsView
    {
      Q_OBJECT

      public:
        typedef /***/ QLutKnob /***/Knob;
        typedef const QLutKnob ConstKnob;
        typedef QPair</***/Knob *, /***/Knob *> /***/KnobPair;
        typedef QPair<ConstKnob *, ConstKnob *> ConstKnobPair;

      public:
        static int    Mix (int,              int,              qreal t);
        static QColor Mix (const QColor   &, const QColor   &, qreal t);
        static QColor Mix (const QLutKnob *, const QLutKnob *, qreal x);

        //static QPointF CatmullRom (const QLutKnob *, const QLutKnob *, qreal t);

      protected:
        QGraphicsScene m_scene;
        QLutKnob m_first, m_last;
        QGraphicsRectItem m_frame;
        QGraphicsPathItem m_path;
        QLutKnob * m_magic;
        QVector<QColor> m_table;

      protected:
        QLutKnob * project (QLutKnob *, QLutKnob *, const QPointF &);

      signals:
        void computed (const QVector<QColor> &);

      public:
        QLutEditor (QWidget * parent = NULL);
        virtual ~QLutEditor ();

        QVector<QColor> table () const {return m_table;}

        void add ();
        void remove (QLutKnob *);
        void compute ();

        ConstKnobPair find (qreal x) const;
        QColor color (qreal x) const;

        QDomElement dom (QDomDocument &) const;
        void init (const QDomElement &);

        void import (const QColor [256]); // FIXME

      public:
        virtual QSize sizeHint () const {return QSize (512, 512);}

      protected:
        virtual void resizeEvent (QResizeEvent *);
        virtual void mouseMoveEvent (QMouseEvent *);
        //virtual void keyPressEvent (QKeyEvent *);
    };

  }
}

#endif // AA_R3D_QLUT_EDITOR__H

