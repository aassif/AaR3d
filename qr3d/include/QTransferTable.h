#ifndef AA_R3D_TRANSFER_TABLE__H
#define AA_R3D_TRANSFER_TABLE__H

#include <AaVector>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>

namespace Aa
{
  namespace R3d
  {

    class QTransferTable;

    class QTransferCell : public QGraphicsRectItem
    {
      private:
        QTransferTable * m_table;
        const AaUInt8    m_id;
        QColor           m_color;

      public:
        QTransferCell (QTransferTable *, AaUInt8 x, AaUInt8 y);

        AaUInt8 id () const {return m_id;}

        void set_color (const QColor &);

      protected:
        virtual void mouseDoubleClickEvent (QGraphicsSceneMouseEvent *);
    };

    class QTransferTable : public QGraphicsView
    {
      Q_OBJECT
      friend class QTransferCell;

      private:
        QGraphicsScene           m_scene;
        QVector<QTransferCell *> m_cells;
        QVector<QColor>          m_table;

      public:
        QTransferTable (QWidget * parent = 0);
        //~QTransferTable ();

        QVector<QColor> table () const {return m_table;}

      private slots:
        void set_color (AaUInt8 index, const QColor &);

      public slots:
        //void read (const QString &);
        //void write (const QString &) const;

      signals:
        void computed (const QVector<QColor> &);

      protected:
        virtual void resizeEvent (QResizeEvent *);        
    };

  }
}

#endif // AA_R3D_TRANSFER_TABLE__H

