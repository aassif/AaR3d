#include <QGraphicsRectItem>
#include <QColorDialog>
#include "QTransferTable.h"

using namespace std;

namespace Aa
{
  namespace R3d
  {

    QTransferCell::QTransferCell (QTransferTable * table,
                                  AaUInt8 x, AaUInt8 y) :
      QGraphicsRectItem (16 * x, 16 * y, 16.0, 16.0),
      m_table (table),
      m_id (y * 16 + x),
      m_color (Qt::transparent)
    {
      QPen pen (Qt::black, 1.5);
      pen.setJoinStyle (Qt::MiterJoin);
      setPen (pen);
    }

    void QTransferCell::set_color (const QColor & c)
    {
      m_color = c;
      setBrush (c);
      m_table->set_color (m_id, c);
    }

    void QTransferCell::mouseDoubleClickEvent (QGraphicsSceneMouseEvent * e)
    {
      QColor c = QColorDialog::getColor (m_color, NULL, "Select Color", QColorDialog::ShowAlphaChannel);
      if (c.isValid ()) this->set_color (c);
    }

    QTransferTable::QTransferTable (QWidget * parent) :
      QGraphicsView (parent),
      m_scene (),
      m_cells (256),
      m_table (256)
    {
      // Checker.
      QImage checker (16, 16, QImage::Format_ARGB32);
      QPainter p (&checker);
      p.fillRect (0, 0, 16, 16, Qt::lightGray);
      p.fillRect (0, 8,  8,  8, Qt::darkGray);
      p.fillRect (8, 0,  8,  8, Qt::darkGray);
      m_scene.setBackgroundBrush (checker);

      for (AaUInt8 i = 0; i < 16; ++i)
        for (AaUInt8 j = 0; j < 16; ++j)
        {
          QTransferCell * cell = new QTransferCell (this, j, i);
          m_scene.addItem (cell);
          m_cells [cell->id ()] = cell;
        }

      for (AaUInt k = 0; k < 256; ++k)
        m_cells [k]->set_color (QColor (k, k, k, k));

      scale (1, -1);
      setScene (&m_scene);
      setSceneRect (0, 0, 256, 256);
      setRenderHints (QPainter::Antialiasing);
      setCacheMode (QGraphicsView::CacheBackground);
    }

#if 0
    QTransferTable::~QTransferTable ()
    {
    }
#endif

    void QTransferTable::set_color (AaUInt8 k, const QColor & c)
    {
      m_table [k] = c;
      emit computed (m_table);
    }

    void QTransferTable::resizeEvent (QResizeEvent *)
    {
      static const int MARGIN = 12;
      this->fitInView (-MARGIN, -MARGIN, 256 + MARGIN, 256 + MARGIN, Qt::KeepAspectRatio);
    }

  }
}
