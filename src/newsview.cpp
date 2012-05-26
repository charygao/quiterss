#include "newsview.h"
#include "delegatewithoutfocus.h"

NewsView::NewsView(QWidget * parent)
  : QTreeView(parent)
{
  setObjectName("newsView_");
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  setMinimumWidth(120);
  setSortingEnabled(true);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setSelectionMode(QAbstractItemView::ExtendedSelection);
  DelegateWithoutFocus *itemDelegate = new DelegateWithoutFocus(this);
  setItemDelegate(itemDelegate);
  //    setAlternatingRowColors(true);
  setContextMenuPolicy(Qt::CustomContextMenu);
}

/*virtual*/ void NewsView::mousePressEvent(QMouseEvent *event)
{
  QModelIndex index = indexAt(event->pos());
  QSqlTableModel *model_ = (QSqlTableModel*)model();
  if (event->buttons() & Qt::LeftButton) {
    if (index.column() == model_->fieldIndex("starred")) {
      if (model_->index(index.row(), model_->fieldIndex("starred")).data(Qt::EditRole).toInt() == 0) {
        emit signalSetItemStar(index, 1);
      } else {
        emit signalSetItemStar(index, 0);
      }
      event->ignore();
      return;
    } else if (index.column() == model_->fieldIndex("read")) {
      if (model_->index(index.row(), model_->fieldIndex("read")).data(Qt::EditRole).toInt() == 0) {
        emit signalSetItemRead(index, 1);
      } else {
        emit signalSetItemRead(index, 0);
      }
      event->ignore();
      return;
    }
  }
  QTreeView::mousePressEvent(event);
}

/*virtual*/ void NewsView::mouseMoveEvent(QMouseEvent *event)
{
  Q_UNUSED(event)
}

/*virtual*/ void NewsView::mouseDoubleClickEvent(QMouseEvent *event)
{
  emit signalDoubleClicked(indexAt(event->pos()));
}

/*virtual*/ void NewsView::keyPressEvent(QKeyEvent *event)
{
  if (!event->modifiers()) {
    if (event->key() == Qt::Key_Up) emit pressKeyUp();
    else if (event->key() == Qt::Key_Down) emit pressKeyDown();
  } else if ((event->modifiers() == Qt::ControlModifier) &&
             (event->key() == Qt::Key_A)) {
    QTreeView::keyPressEvent(event);
  }
}