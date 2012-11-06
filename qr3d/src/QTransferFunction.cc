#include <cmath>
#include <iostream>
#include <QColorDialog>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
//#include <AaXml>
#include "QTransferFunction.h"

//#include <QMessageBox> // debug

using namespace std;

namespace Aa
{
  namespace R3d
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::R3d::QTransferKnob //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    QTransferKnob::QTransferKnob (QTransferFunction * editor,
                                  QTransferKnob * prev,
                                  QTransferKnob * next,
                                  const QPointF & pos,
                                  const QColor & color,
                                  int mode) :
      QGraphicsEllipseItem (-RADIUS, -RADIUS, 2*RADIUS + 1, 2*RADIUS + 1),
      m_editor (editor),
      m_prev (prev),
      m_next (next),
      m_color (),
      m_mode (mode)
    {
      if (m_mode != MAGIC)
      {
        if (m_prev != NULL) m_prev->m_next = this;
        if (m_next != NULL) m_next->m_prev = this;
      }

      setPos (pos);
      setColor (color);
      setToolTip ();

      setFlag (QGraphicsItem::ItemIsMovable);
      setFlag (QGraphicsItem::ItemSendsGeometryChanges);
    }

    QTransferKnob::QTransferKnob (QTransferFunction * editor,
                                  QTransferKnob * prev,
                                  QTransferKnob * next,
                                  const QDomElement & element) :
      QGraphicsEllipseItem (-RADIUS, -RADIUS, 2*RADIUS + 1, 2*RADIUS + 1),
      m_editor (editor),
      m_prev (prev),
      m_next (next),
      m_color (),
      m_mode (NORMAL)
    {
      if (m_mode != MAGIC)
      {
        if (m_prev != NULL) m_prev->m_next = this;
        if (m_next != NULL) m_next->m_prev = this;
      }

      this->init (element);
      setToolTip ();

      setFlag (QGraphicsItem::ItemIsMovable);
      setFlag (QGraphicsItem::ItemSendsGeometryChanges);
    }

    QTransferKnob::~QTransferKnob ()
    {
      if (m_mode != MAGIC)
      {
        if (m_prev != NULL) m_prev->m_next = m_next;
        if (m_next != NULL) m_next->m_prev = m_prev;
      }
    }

    void QTransferKnob::setToolTip ()
    {
      static const QString FORMAT = QString ("(%1, %2)");
      QGraphicsItem::setToolTip (FORMAT.arg (this->x ()).arg (this->y ()));
    }

    void QTransferKnob::setColor (const QColor & c)
    {
      int a = (m_mode == MAGIC ? c.alpha () : 255);
      m_color = QColor (c.red (), c.green (), c.blue (), a);

      setBrush (m_color);
      setPen (QPen (QColor (0, 0, 0, a), 1.5));
      setZValue (m_mode == MAGIC ? 2 : 3);
    }

    QColor QTransferKnob::color () const
    {
      return QColor (m_color.red (),
                     m_color.green (),
                     m_color.blue (),
                     qBound (0.0, this->y (), 255.0));
    }

    void QTransferKnob::validate ()
    {
      if (m_mode == MAGIC)
      {
        m_mode = NORMAL;
        setColor (m_color);
        if (m_prev != NULL) m_prev->m_next = this;
        if (m_next != NULL) m_next->m_prev = this;
      }
    }

    QDomElement QTransferKnob::dom (QDomDocument & doc) const
    {
      QDomElement e = doc.createElement ("knob");

      e.setAttribute ("x", this->x ());
      e.setAttribute ("y", this->y ());

      e.setAttribute ("color", m_color.name ());

      return e;
    }

    void QTransferKnob::init (const QDomElement & e)
    {
      double x = e.attribute ("x").toDouble ();
      double y = e.attribute ("y").toDouble ();
      this->setPos (x, y);

      this->setColor (QColor (e.attribute ("color")));
    }

    void QTransferKnob::mousePressEvent (QGraphicsSceneMouseEvent * e)
    {
      QGraphicsItem::mousePressEvent (e);
      if (m_mode == MAGIC) m_editor->add ();
      //e->accept ();
    }

    void QTransferKnob::mouseDoubleClickEvent (QGraphicsSceneMouseEvent *)
    {
      QColor c = QColorDialog::getColor (m_color);
      if (c.isValid ())
      {
        setColor (c);
        m_editor->compute ();
      }
      //e->accept ();
    }

    void QTransferKnob::contextMenuEvent (QGraphicsSceneContextMenuEvent * e)
    {
      QMenu menu;
      QAction * remove = menu.addAction ("Remove");
      remove->setEnabled (m_mode != VERTICAL);
      QAction * action = menu.exec (e->screenPos ());
      if (action == remove) m_editor->remove (this);
    }

    QVariant QTransferKnob::itemChange (GraphicsItemChange change, const QVariant & value)
    {
      switch (change)
      {
        case ItemPositionChange:
        {
          QPointF p0 = this->pos ();
          QPointF p1 = value.toPointF ();

          double y = qBound (0.0, p1.y (), 256.0);

          if (m_mode == VERTICAL)
            return QPointF (p0.x (), y);

          //double min = (m_prev != NULL ?   0.0 : m_prev->x ());
          //double max = (m_next != NULL ? 256.0 : m_next->x ());
          double x = qBound (m_prev->x (), p1.x (), m_next->x ());

          return QPointF (x, y);
        }

        case ItemPositionHasChanged:
          this->setToolTip ();
          m_editor->compute ();
          return QVariant ();

        default:
          return QGraphicsItem::itemChange (change, value);
      }
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::R3d::QTransferFunction //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


    QTransferFunction::QTransferFunction (QWidget * parent) :
      QGraphicsView (parent),
      m_scene (),
      m_first (this, NULL,     NULL, QPointF (0,   0),   Qt::black, QTransferKnob::VERTICAL),
      m_last  (this, &m_first, NULL, QPointF (256, 256), Qt::white, QTransferKnob::VERTICAL),
      m_frame (0, 0, 256, 256),
      m_path (),
      m_magic (NULL),
      m_table (256)
    {
      // Checker.
      QImage checker (16, 16, QImage::Format_ARGB32);
      QPainter p (&checker);
      p.fillRect (0, 0, 16, 16, Qt::lightGray);
      p.fillRect (0, 8,  8,  8, Qt::darkGray);
      p.fillRect (8, 0,  8,  8, Qt::darkGray);
      m_scene.setBackgroundBrush (checker);

      // Frame.
      m_frame.setZValue (0);
      m_frame.setPen (QPen (Qt::black, 1.5));

      // Path.
      m_path.setZValue (1);
      m_path.setPen (QPen (Qt::black, 1.5));

      m_scene.addItem (&m_frame);
      m_scene.addItem (&m_path);
      m_scene.addItem (&m_first);
      m_scene.addItem (&m_last);

      scale (1, -1);
      setScene (&m_scene);
      setSceneRect (0, 0, 256, 256);
      setRenderHints (QPainter::Antialiasing);
      setCacheMode (QGraphicsView::CacheBackground);
      setMouseTracking (true);
      setWindowTitle ("QTransferFunction");

      compute ();
    }

    QTransferFunction::~QTransferFunction ()
    {
#if 0
      for (Knob * k = m_first.next (); k != &m_last; k = k->next ())
      {
        m_scene.removeItem (k);
        delete k;
      }

      if (m_magic != NULL)
      {
        m_scene.removeItem (m_magic);
        delete m_magic;
        m_magic = NULL;
      }

      m_scene.removeItem (&m_background);
      m_scene.removeItem (&m_path);
      m_scene.removeItem (&m_first);
      m_scene.removeItem (&m_last);
#endif

      setScene (NULL);
    }

    QTransferFunction::ConstKnobPair QTransferFunction::find (qreal x) const
    {
      if (x < m_first.x ()) return ConstKnobPair (NULL, &m_first);
      if (x > m_last .x ()) return ConstKnobPair (&m_last,  NULL);

      for (ConstKnob * k = &m_first; k != &m_last; k = k->next ())
      {
        ConstKnob * k0 = k;          qreal x0 = k0->x ();
        ConstKnob * k1 = k->next (); qreal x1 = k1->x ();
        if (x == x0           ) return ConstKnobPair (k0, k0);
        if (x >  x0 && x <  x1) return ConstKnobPair (k0, k1);
        if (           x == x1) return ConstKnobPair (k1, k1);
      }

      return ConstKnobPair (NULL, NULL);
    }

    void QTransferFunction::add ()
    {
      m_magic->validate ();
      m_magic = NULL;
      compute ();
    }

    void QTransferFunction::remove (QTransferKnob * knob)
    {
      delete knob;
      compute ();
    }

    QTransferKnob * QTransferFunction::project (QTransferKnob * k0,
                                                QTransferKnob * k1,
                                                const QPointF & p)
    {
      static const qreal r = QTransferKnob::RADIUS;
      static const qreal R = QTransferKnob::RADIUS * 8;

      QLineF segment (k0->pos (), k1->pos ());
      QLineF n = segment.normalVector ().unitVector ();
      QLineF line = QLineF (QPointF (p.x () - R * n.dx (), p.y () - R * n.dy ()),
                            QPointF (p.x () + R * n.dx (), p.y () + R * n.dy ()));

      QPointF proj;
      QLineF::IntersectType type = segment.intersect (line, &proj);
      if (type != QLineF::BoundedIntersection) return NULL;

      QColor c = QTransferFunction::Mix (k0, k1, proj.x ());
      qreal a0 = qBound (0.0, (QLineF (p, k0->pos ()).length () - 2 * r) / r, 1.0);
      qreal a1 = qBound (0.0, (QLineF (p, k1->pos ()).length () - 2 * r) / r, 1.0);
      qreal a  = qBound (0.0, (a0 * a1) - (QLineF (p, proj).length () - r) / R, 1.0);
      return new QTransferKnob (this,
                                k0, k1,
                                proj,
                                QColor (c.red (), c.green (), c.blue (), 255.0 * a),
                                QTransferKnob::MAGIC);
    }

    int QTransferFunction::Mix (int c0, int c1, qreal t)
    {
      return qBound (0, qRound ((1 - t) * c0 + t * c1), 255);
    }

    QColor QTransferFunction::Mix (const QColor & c0, const QColor & c1, qreal t)
    {
      return QColor (QTransferFunction::Mix (c0.red   (), c1.red   (), t),
                     QTransferFunction::Mix (c0.green (), c1.green (), t),
                     QTransferFunction::Mix (c0.blue  (), c1.blue  (), t),
                     QTransferFunction::Mix (c0.alpha (), c1.alpha (), t));
    }

    QColor QTransferFunction::Mix (const QTransferKnob * k0, const QTransferKnob * k1, qreal x)
    {
      if (k0 == NULL) return (k1 != NULL) ? k1->color () : Qt::transparent;
      if (k1 == NULL) return (k0 != NULL) ? k0->color () : Qt::transparent;

      qreal x0 = k0->x ();
      qreal x1 = k1->x ();
      qreal t = (x0 != x1) ? (x - x0) / (x1 - x0) : 0.5;
      return QTransferFunction::Mix (k0->color (), k1->color (), t);
    }

#if 0
    QPointF QTransferFunction::CatmullRom (const QTransferKnob * k1, const QTransferKnob * k2, qreal t)
    {
      QPointF p1 = k1->pos ();
      QPointF p2 = k2->pos ();
      qreal dx = p2.x () - p1.x ();
      QPointF p0 = (k1->prev () != NULL) ? k1->prev ()->pos () : QPointF (p1.x () - dx, p2.y ());
      QPointF p3 = (k2->next () != NULL) ? k2->next ()->pos () : QPointF (p2.x () + dx, p1.y ());
      return 0.5 * ((2 * p1)
                  + (-p0 + p2) * t
                  + (2*p0 - 5*p1 + 4*p2 - p3) * t*t
                  + (-p0 + 3*p1- 3*p2 + p3) * t*t*t);
    }
#endif

    QColor QTransferFunction::color (qreal x) const
    {
      ConstKnobPair f = this->find (x);
      return QTransferFunction::Mix (f.first, f.second, x);
    }

    void QTransferFunction::compute ()
    {
      QPainterPath path (m_first.pos ());
      for (Knob * k0 = &m_first; k0 != &m_last; k0 = k0->next ())
      {
        Knob * k1 = k0->next ();
        path.lineTo (k1->pos ());
      }
      m_path.setPath (path);

      QImage image (256, 1, QImage::Format_ARGB32);
      Knob * k = &m_first;
      for (int i = 0; i < 256; ++i)
      {
        qreal x = i + 0.5;
        while (x > k->next ()->x ()) k = k->next ();
        QColor c = QTransferFunction::Mix (k, k->next (), x);
        m_table [i] = c;
        image.setPixel (i, 0, c.rgba ());
      }
      m_frame.setBrush (QBrush (image));

      emit computed (m_table);
    }

    QDomElement QTransferFunction::dom (QDomDocument & doc) const
    {
      QDomElement e = doc.createElement ("lut");

      for (const Knob * k = &m_first; k != NULL; k = k->next ())
        e.appendChild (k->dom (doc));

      return e;
    }

    void QTransferFunction::init (const QDomElement & element)
    {
      while (m_first.next () != &m_last)
        delete m_first.next ();

      QDomElement first = element.firstChildElement ("knob");
      m_first.init (first);

      QDomElement last = element.lastChildElement ("knob");
      m_last.init (last);

      Knob * knob = &m_first;
      for (QDomElement e = first.nextSiblingElement ("knob");
           e != last;
           e = e.nextSiblingElement ("knob"))
      {
        knob = new QTransferKnob (this, knob, &m_last, e);
        m_scene.addItem (knob);
      }

      compute ();
    }

    void QTransferFunction::import (const QColor table [256])
    {
      int d1r [256]; d1r [0] = 0;
      int d1g [256]; d1g [0] = 0;
      int d1b [256]; d1b [0] = 0;
      int d1a [256]; d1a [0] = 0;

      m_table [0] = table [0];
      for (int k = 1; k < 256; ++k)
      {
        m_table [k] = table [k];
        d1r [k] = m_table [k].red   () - m_table [k-1].red   ();
        d1g [k] = m_table [k].green () - m_table [k-1].green ();
        d1b [k] = m_table [k].blue  () - m_table [k-1].blue  ();
        d1a [k] = m_table [k].alpha () - m_table [k-1].alpha ();
      }

      m_first.setPos (0, m_table [0].alpha ());
      m_first.setColor (m_table [0]);

      QTransferKnob * knob = &m_first;
      for (int k = 1; k < 255; ++k)
      {
        int d1r0 = d1r [k-1], d1r1 = d1r [k];
        int d1g0 = d1g [k-1], d1g1 = d1g [k];
        int d1b0 = d1b [k-1], d1b1 = d1b [k];
        int d1a0 = d1a [k-1], d1a1 = d1a [k];

#if 0
        cout << d1r0 << ':' << d1r1 << ' '
             << d1g0 << ':' << d1g1 << ' '
             << d1b0 << ':' << d1b1 << ' '
             << d1a0 << ':' << d1a1 << endl;
#endif

        if (d1r0 * d1r1 < 0 || fabs (d1r1 - d1r0) > 1
         || d1g0 * d1g1 < 0 || fabs (d1g1 - d1g0) > 1
         || d1b0 * d1b1 < 0 || fabs (d1b1 - d1b0) > 1
         || d1a0 * d1a1 < 0 || fabs (d1a1 - d1a0) > 1)
        {
          knob = new QTransferKnob (this,
                                    knob, &m_last,
                                    QPointF (k + 0.5, m_table [k].alpha ()),
                                    m_table [k],
                                    QTransferKnob::NORMAL);

          m_scene.addItem (knob);
        }
      }

      m_last.setPos (256, m_table [255].alpha ());
      m_last.setColor (m_table [255]);

      compute ();
    }

    void QTransferFunction::resizeEvent (QResizeEvent *)
    {
      static const int MARGIN = QTransferKnob::RADIUS * 4;
      this->fitInView (-MARGIN, -MARGIN, 256 + MARGIN, 256 + MARGIN, Qt::KeepAspectRatio);
    }

    void QTransferFunction::mouseMoveEvent (QMouseEvent * e)
    {
      QGraphicsView::mouseMoveEvent (e);

      if (m_magic != NULL)
      {
        m_scene.removeItem (m_magic);
        delete m_magic;
        m_magic = NULL;
      }

      if (e->buttons () == Qt::NoButton)
      {
        QTransferKnob * p0 = NULL;
        QPointF q = this->mapToScene (e->pos ());
        for (QTransferKnob * k = &m_first; k != &m_last; k = k->next ())
        {
          QTransferKnob * p1 = this->project (k, k->next (), q);
          if (p1 != NULL)
          {
            if (p0 != NULL)
            {
              qreal d0 = QLineF (q, p0->pos ()).length ();
              qreal d1 = QLineF (q, p1->pos ()).length ();
              if (d1 < d0) p0 = p1; else delete p1;
            }
            else
              p0 = p1;
          }
        }

        if (p0 != NULL)
        {
          m_magic = p0;
          m_scene.addItem (m_magic);
        }

        e->accept ();
      }
    }

  }
}

