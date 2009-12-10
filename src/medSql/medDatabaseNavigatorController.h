#ifndef MEDDATABASENAVIGATORCONTROLLER_H
#define MEDDATABASENAVIGATORCONTROLLER_H

#include <Qt>
#include <QtCore>

class medDatabaseNavigatorController : public QObject
{
public:
    static medDatabaseNavigatorController *instance(void);

    Qt::Orientation orientation(void);

    qreal itemWidth(void) const;
    qreal itemHeight(void) const;
    qreal itemSpacing(void) const;

    qreal groupWidth(void) const;
    qreal groupHeight(void) const;
    qreal groupSpacing(void) const;

    qreal selectorWidth(void) const;
    qreal selectorHeight(void) const;
    qreal selectorSpacing(void) const;

    qreal queryOffset(void) const;

    void setItemSize(qreal width, qreal height);

    void setOrientation(Qt::Orientation orientation);

protected:
    static medDatabaseNavigatorController *s_instance;

private:
     medDatabaseNavigatorController(void);
    ~medDatabaseNavigatorController(void);

private:
    qreal m_width;
    qreal m_height;

    Qt::Orientation m_orientation;
};

#endif // MEDDATABASENAVIGATORCONTROLLER_H
