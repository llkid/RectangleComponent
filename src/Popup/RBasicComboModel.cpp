#include "RBasicComboModel.h"

RBasicComboModel::RBasicComboModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int RBasicComboModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : modelData.count();
}

QVariant RBasicComboModel::data(const QModelIndex &index, int role) const
{
    return (index.isValid() && role == Qt::DisplayRole)
            ? modelData.at(index.row())
            : QVariant();
}

QList<QString> RBasicComboModel::getModelData() const
{
    return modelData;
}

void RBasicComboModel::setModelData(const QList<QString> &datas)
{
    beginResetModel();
    modelData = datas;
    endResetModel();
}

int RBasicComboModel::getDataRow(const QString &text)
{
    return modelData.indexOf(text);
}
