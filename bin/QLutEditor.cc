#include <cmath>
#include <iostream>
#include <QColorDialog>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <AaXml>
#include "QLutEditor.h"

#include <QMessageBox> // debug

using namespace std;

namespace Aa
{
  namespace R3d
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::R3d::QLutKnob ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    QLutKnob::QLutKnob (QLutEditor * editor,
                        qreal x, qreal y,
                        const QColor & color,
                        QLutKnob * prev,
                        QLutKnob * next,
                        int mode) :
      QGraphicsEllipseItem (-RADIUS, -RADIUS, 2*RADIUS + 1, 2*RADIUS + 1),
      m_editor (editor),
      m_color (),
      m_prev (prev),
      m_next (next),
      m_mode (mode)
    {
      setPos (x, y);
      setFlag (QGraphicsItem::ItemIsMovable);

      setColor (color);
      setToolTip ();

      if (m_mode != MAGIC)
      {
        if (m_prev != NULL) m_prev->m_next = this;
        if (m_next != NULL) m_next->m_prev = this;
      }
    }

    QLutKnob::~QLutKnob ()
    {
      if (m_mode != MAGIC)
      {
        if (m_prev != NULL) m_prev->m_next = m_next;
        if (m_next != NULL) m_next->m_prev = m_prev;
      }
    }

    void QLutKnob::setToolTip ()
    {
      static const QString FORMAT = QString ("(%1, %2)");
      QGraphicsItem::setToolTip (FORMAT.arg (this->x ()).arg (this->y ()));
    }

    void QLutKnob::setColor (const QColor & c)
    {
      int a = (m_mode == MAGIC ? c.alpha () : 255);
      m_color = QColor (c.red (), c.green (), c.blue (), a);

      setBrush (m_color);
      setPen (QPen (QColor (0, 0, 0, a), 1.5));
      setZValue (m_mode == MAGIC ? 2 : 3);
    }

    QColor QLutKnob::color () const
    {
      return QColor (m_color.red (),
                     m_color.green (),
                     m_color.blue (),
                     qBound (0.0, this->y (), 255.0));
    }

    void QLutKnob::validate ()
    {
      if (m_mode == MAGIC)
      {
        m_mode = NORMAL;
        setColor (m_color);
        if (m_prev != NULL) m_prev->m_next = this;
        if (m_next != NULL) m_next->m_prev = this;
      }
    }

    void QLutKnob::mouseMoveEvent (QGraphicsSceneMouseEvent * e)
    {
      QPointF p = this->pos ();
      QGraphicsItem::mouseMoveEvent (e);

      if (m_mode == VERTICAL)
        this->setPos (p.x (), this->y ());
      else
      {
        if (this->x () < m_prev->x ()) this->setPos (m_prev->x (), this->y ());
        if (this->x () > m_next->x ()) this->setPos (m_next->x (), this->y ());
      }

      if (this->y () <   0) this->setPos (this->x (),   0);
      if (this->y () > 256) this->setPos (this->x (), 256);

      this->setToolTip ();
      m_editor->compute ();
    }

    void QLutKnob::mousePressEvent (QGraphicsSceneMouseEvent * e)
    {
      QGraphicsItem::mousePressEvent (e);
      if (m_mode == MAGIC) m_editor->add ();
      //e->accept ();
    }

    void QLutKnob::mouseReleaseEvent (QGraphicsSceneMouseEvent * e)
    {
      QGraphicsItem::mouseReleaseEvent (e);
      //e->accept ();
    }

    void QLutKnob::mouseDoubleClickEvent (QGraphicsSceneMouseEvent *)
    {
      QColor c = QColorDialog::getColor (m_color);
      if (c.isValid ())
      {
        setColor (c);
        m_editor->compute ();
      }
      //e->accept ();
    }

    void QLutKnob::contextMenuEvent (QGraphicsSceneContextMenuEvent * e)
    {
      QMenu menu;
      QAction * remove = menu.addAction ("Remove");
      remove->setEnabled (m_mode != VERTICAL);
      QAction * action = menu.exec (e->screenPos ());
      if (action == remove) m_editor->remove (this);
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::R3d::QLutEditor /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


    QLutEditor::QLutEditor (qreal min, qreal max, QWidget * parent) :
      QGraphicsView (parent),
      m_scene (),
      m_first (this, min,   0, Qt::red,  NULL,     NULL, QLutKnob::VERTICAL),
      m_last  (this, max, 256, Qt::blue, &m_first, NULL, QLutKnob::VERTICAL),
      m_background (0, 0, 256, 256),
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

      // Background.
      m_background.setZValue (0);
      m_background.setPen (QPen (Qt::black, 1.5));

      // Path.
      m_path.setZValue (1);
      m_path.setPen (QPen (Qt::black, 1.5));

      m_scene.addItem (&m_background);
      m_scene.addItem (&m_path);
      m_scene.addItem (&m_first);
      m_scene.addItem (&m_last);

      scale (1, -1);
      setScene (&m_scene);
      setSceneRect (0, 0, 256, 256);
      setRenderHints (QPainter::Antialiasing);
      setCacheMode (QGraphicsView::CacheBackground);
      setMouseTracking (true);
      setWindowTitle ("QLutEditor");

      compute ();
    }

    QLutEditor::~QLutEditor ()
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

    QLutEditor::ConstKnobPair QLutEditor::find (qreal x) const
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

    void QLutEditor::add ()
    {
      m_magic->validate ();
      m_magic = NULL;
      compute ();
    }

    void QLutEditor::remove (QLutKnob * knob)
    {
      delete knob;
      compute ();
    }

    QLutKnob * QLutEditor::project (QLutKnob * k0, QLutKnob * k1, const QPointF & p)
    {
      static const qreal r = QLutKnob::RADIUS;
      static const qreal R = QLutKnob::RADIUS * 8;

      QLineF segment (k0->pos (), k1->pos ());
      QLineF n = segment.normalVector ().unitVector ();
      QLineF line = QLineF (QPointF (p.x () - R * n.dx (), p.y () - R * n.dy ()),
                            QPointF (p.x () + R * n.dx (), p.y () + R * n.dy ()));

      QPointF proj;
      QLineF::IntersectType type = segment.intersect (line, &proj);
      if (type != QLineF::BoundedIntersection) return NULL;

      QColor c = QLutEditor::Mix (k0, k1, proj.x ());
      qreal a0 = qBound (0.0, (QLineF (p, k0->pos ()).length () - 2 * r) / r, 1.0);
      qreal a1 = qBound (0.0, (QLineF (p, k1->pos ()).length () - 2 * r) / r, 1.0);
      qreal a  = qBound (0.0, (a0 * a1) - (QLineF (p, proj).length () - r) / R, 1.0);
      return new QLutKnob (this,
                           proj.x (), proj.y (),
                           QColor (c.red (), c.green (), c.blue (), 255.0 * a),
                           k0, k1, QLutKnob::MAGIC);
    }

    QColor QLutEditor::Mix (const QColor & c0, const QColor & c1, qreal t)
    {
      return QColor (qRound ((1 - t) * c0.red   () + t * c1.red   ()),
                     qRound ((1 - t) * c0.green () + t * c1.green ()),
                     qRound ((1 - t) * c0.blue  () + t * c1.blue  ()),
                     qRound ((1 - t) * c0.alpha () + t * c1.alpha ()));
    }

    QColor QLutEditor::Mix (const QLutKnob * k0, const QLutKnob * k1, qreal x)
    {
      if (k0 == NULL) return (k1 != NULL) ? k1->color () : Qt::black;
      if (k1 == NULL) return (k0 != NULL) ? k0->color () : Qt::black;

      qreal x0 = k0->x ();
      qreal x1 = k1->x ();
      qreal t = (x0 != x1) ? (x - x0) / (x1 - x0) : 0.5;
      return QLutEditor::Mix (k0->color (), k1->color (), t);
    }

#if 0
    QPointF QLutEditor::CatmullRom (const QLutKnob * k1, const QLutKnob * k2, qreal t)
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

    QColor QLutEditor::color (qreal x) const
    {
      ConstKnobPair f = this->find (x);
      return QLutEditor::Mix (f.first, f.second, x);
    }

    void QLutEditor::compute ()
    {
      //this->write (cout);

      QPainterPath path (QPointF (m_first.x (), m_first.y ()));
      for (Knob * k0 = &m_first; k0 != &m_last; k0 = k0->next ())
      {
        Knob * k1 = k0->next ();
        path.lineTo (k1->x (), k1->y ());
      }
      m_path.setPath (path);

      QImage image (256, 1, QImage::Format_ARGB32);
      Knob * k = &m_first;
      for (int i = 0; i < 256; ++i)
      {
        qreal x = i + 0.5;
        while (x > k->next ()->x ()) k = k->next ();
        QColor c = QLutEditor::Mix (k, k->next (), x);
        m_table [i] = c;
        image.setPixel (i, 0, c.rgba ());
      }
      m_background.setBrush (QBrush (image));

      emit computed (m_table);
    }

    void QLutEditor::read (istream &)
    {
      // TODO
      compute ();
    }

    void QLutEditor::write (ostream & os) const
    {
      os << "<knobs>\n";
      for (const Knob * k = &m_first; k != NULL; k = k->next ())
      {
        const QColor & c = k->color ();
        os << "  <knob "
           <<     "x=\"" << k->x () << "\" "
           <<     "y=\"" << k->y () << "\" "
           <<     "color=\"" << qPrintable (c.name ()) << "\" />\n";
      }
      os << "</knobs>\n";
    }

    void QLutEditor::import (const QColor table [256])
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

      QLutKnob * knob = &m_first;
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
          knob = new QLutKnob (this, k, m_table [k].alpha (), m_table [k], knob, &m_last, QLutKnob::NORMAL);
          m_scene.addItem (knob);
        }
      }

      m_last.setPos (256, m_table [255].alpha ());
      m_last.setColor (m_table [255]);

      compute ();
    }

#if 0
    void QLutEditor::write (ostream & o) const
    {
      for (int k = 0; k < 256; ++k)
      {
        QColor c = this->color (k);
        o << c.red   () << ' '
          << c.green () << ' '
          << c.blue  () << ' '
          << c.alpha () << std::endl;
      }
    }
#endif

    void QLutEditor::resizeEvent (QResizeEvent *)
    {
      static const int MARGIN = 40;
      this->fitInView (-MARGIN, -MARGIN, 256 + MARGIN, 256 + MARGIN, Qt::KeepAspectRatio);
    }

    void QLutEditor::mouseMoveEvent (QMouseEvent * e)
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
        QLutKnob * p0 = NULL;
        QPointF q = this->mapToScene (e->pos ());
        for (QLutKnob * k = &m_first; k != &m_last; k = k->next ())
        {
          QLutKnob * p1 = this->project (k, k->next (), q);
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

#if 0
    void QLutEditor::keyPressEvent (QKeyEvent * e)
    {
      std::ostringstream oss;
      this->dump (oss);
      QMessageBox::information (this, "QLutEditor", oss.str ().c_str ());
    }
#endif
  }
}

//#include "QLutEditor.moc"

