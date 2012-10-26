#ifndef AA_R3D_TRANSFER_FUNCTION__H
#define AA_R3D_TRANSFER_FUNCTION__H

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

    class QTransferKnob;
    class QTransferFunction;

////////////////////////////////////////////////////////////////////////////////
// Aa::R3d::QTransferKnob ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    class QTransferKnob : public QGraphicsEllipseItem
    {
      public:
        static const int RADIUS = 3;
        enum {NORMAL = 0, VERTICAL = 1, MAGIC = 2};

      protected:
        QTransferFunction * m_editor;
        QTransferKnob * m_prev;
        QTransferKnob * m_next;
        QColor m_color;
        int m_mode;

      protected:
        void setToolTip ();

      public:
        QTransferKnob (QTransferFunction *,
                       QTransferKnob * prev,
                       QTransferKnob * next,
                       const QPointF &,
                       const QColor &,
                       int mode = NORMAL);

        QTransferKnob (QTransferFunction *,
                       QTransferKnob * prev,
                       QTransferKnob * next,
                       const QDomElement &);

        virtual ~QTransferKnob ();

        void setColor (const QColor &);
        QColor color () const;

        QTransferKnob * prev () const {return m_prev;}
        QTransferKnob * next () const {return m_next;}

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
// Aa::R3d::QTransferFunction //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    class QTransferFunction : public QGraphicsView
    {
      Q_OBJECT

      public:
        typedef /***/ QTransferKnob /***/Knob;
        typedef const QTransferKnob ConstKnob;
        typedef QPair</***/Knob *, /***/Knob *> /***/KnobPair;
        typedef QPair<ConstKnob *, ConstKnob *> ConstKnobPair;

      public:
        static int    Mix (int,                   int,                   qreal t);
        static QColor Mix (const QColor        &, const QColor        &, qreal t);
        static QColor Mix (const QTransferKnob *, const QTransferKnob *, qreal x);

        //static QPointF CatmullRom (const QTransferKnob *, const QTransferKnob *, qreal t);

      protected:
        QGraphicsScene m_scene;
        QTransferKnob m_first, m_last;
        QGraphicsRectItem m_frame;
        QGraphicsPathItem m_path;
        QTransferKnob * m_magic;
        QVector<QColor> m_table;

      protected:
        QTransferKnob * project (QTransferKnob *, QTransferKnob *, const QPointF &);

      signals:
        void computed (const QVector<QColor> &);

      public:
        QTransferFunction (QWidget * parent = NULL);
        virtual ~QTransferFunction ();

        QVector<QColor> table () const {return m_table;}

        void add ();
        void remove (QTransferKnob *);
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

#endif // AA_R3D_TRANSFER_FUNCTION__H

