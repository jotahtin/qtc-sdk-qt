#include "stdafx.h"

#include <QDebug>

#include "QEnginioCollectionprivate.h"

/* Public */
QEnginioCollectionObject::QEnginioCollectionObject(QObject *parent)
    : QObject(parent)
{

}

/* Private */
QEnginioCollectionPrivate::QEnginioCollectionPrivate(QObject *parent)
    : QEnginioCollectionObject(parent)
{}

QString QEnginioCollectionPrivate::collectionName() const
{
    return iCollectionName;
}

void QEnginioCollectionPrivate::foo()
{
    qDebug() << "FOO";



}
