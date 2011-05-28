#ifndef __AA_R3D_QLUT_EDITOR__
#define __AA_R3D_QLUT_EDITOR__

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
//#include <QMouseEvent>
//#include <QGraphicsSceneMouseEvent>

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
        QColor m_color;
        QLutKnob * m_prev;
        QLutKnob * m_next;
        int m_mode;

      protected:
        void setToolTip ();

      public:
        QLutKnob (QLutEditor *,
                  qreal x, qreal y,
                  const QColor & color,
                  QLutKnob * prev,
                  QLutKnob * next,
                  int mode = NORMAL);
        virtual ~QLutKnob ();

        void setColor (const QColor & c);
        QColor color () const;

        QLutKnob * prev () const {return m_prev;}
        QLutKnob * next () const {return m_next;}

        void validate ();

        virtual void mouseMoveEvent        (QGraphicsSceneMouseEvent *);
        virtual void mousePressEvent       (QGraphicsSceneMouseEvent *);
        virtual void mouseReleaseEvent     (QGraphicsSceneMouseEvent *);
        virtual void mouseDoubleClickEvent (QGraphicsSceneMouseEvent *);
        virtual void contextMenuEvent      (QGraphicsSceneContextMenuEvent *);
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
        static QColor Mix (const QColor   &, const QColor   &, qreal t);
        static QColor Mix (const QLutKnob *, const QLutKnob *, qreal x);

        //static QPointF CatmullRom (const QLutKnob *, const QLutKnob *, qreal t);

      protected:
        QGraphicsScene m_scene;
        QLutKnob m_first, m_last;
        QGraphicsRectItem m_background;
        QGraphicsPathItem m_path;
        QLutKnob * m_magic;
        QVector<QColor> m_table;
        bool m_enabled;

      protected:
        QLutKnob * project (QLutKnob *, QLutKnob *, const QPointF &);

      signals:
        void computed (const QVector<QColor> &);

      public:
        QLutEditor (qreal min = 0, qreal max = 256, QWidget * parent = NULL);
        virtual ~QLutEditor ();

        QVector<QColor> table () const {return m_table;}

        void add ();
        void remove (QLutKnob *);
        void compute ();

        ConstKnobPair find (qreal x) const;
        QColor color (qreal x) const;
        QVector<QColor> vector () const;

        void read (std::istream &);
        void write (std::ostream &) const;

        void import (const QColor [256]);

      public:
        virtual QSize sizeHint () const {return QSize (512, 512);}

      protected:
        virtual void resizeEvent (QResizeEvent *);
        virtual void mouseMoveEvent    (QMouseEvent *);
        virtual void mousePressEvent   (QMouseEvent *);
        virtual void mouseReleaseEvent (QMouseEvent *);
        //virtual void keyPressEvent (QKeyEvent *);
    };

  }
}

#endif // __AA_R3D_QLUT_EDITOR__

