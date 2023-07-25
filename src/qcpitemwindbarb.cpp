#include "qcpitemwindbarb.h"

QCPItemWindBarb::QCPItemWindBarb(QCustomPlot *parentPlot) :
	QCPAbstractItem(parentPlot),
	start(createPosition(QLatin1String("start")))
{
	start->setCoords(0, 0);
	mWindSpeed = 0;
	mWindDirection = 0;
	mMainLineLength = 30;
	mLongLineLength = 16;

	setPen(QPen(Qt::black));
	setSelectedPen(QPen(Qt::blue, 2));
}

QCPItemWindBarb::~QCPItemWindBarb()
{
}

void QCPItemWindBarb::setPen(const QPen &pen)
{
	mPen = pen;
}

void QCPItemWindBarb::setSelectedPen(const QPen &pen)
{
	mSelectedPen = pen;
}

void QCPItemWindBarb::setWindSpeed(double ws)
{
	mWindSpeed = ws;
}

void QCPItemWindBarb::setWindDirection(double wd)
{
	mWindDirection = wd;
}

void QCPItemWindBarb::setMainLineLength(double len)
{
	mMainLineLength = len;
}

void QCPItemWindBarb::setLongLineLength(double len)
{
	mLongLineLength = len;
}

double QCPItemWindBarb::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
	Q_UNUSED(details)
	if (onlySelectable && !mSelectable)
		return -1;

	//TODO
	//使用这个mBoundingRect可能并不合适，还应该把旋转考虑进去
	if (mBoundingRect.contains(pos))
		return mParentPlot->selectionTolerance() * 0.99;

	return -1;
}

void QCPItemWindBarb::draw(QCPPainter *painter)
{
	double lineLen = mMainLineLength;
	double longLen = mLongLineLength;
	double shortLen = longLen / 2;
	QPointF ptBegin = start->pixelPosition();

	QPainterPath path;
	if (qFuzzyCompare(mWindSpeed, 0))
		path.addEllipse(QRectF(ptBegin - QPoint(2, 2), QSize(4, 4)));
	else
	{
        double angel_pole = (90 - mWindDirection) * M_PI / 180;
        double angel_value = (-mWindDirection)*M_PI / 180;
        double cosAngel_pole = qCos(angel_pole);
        double sinAngel_pole = qSin(angel_pole);
        double cosAngel_value = qCos(angel_value);
        double sinAngel_value = qSin(angel_value);
        QPointF ptEnd = ptBegin + QPointF(cosAngel_pole, -sinAngel_pole) * lineLen;
		path.moveTo(ptBegin);//主线，风向杆
		path.lineTo(ptEnd);
		int iSpeed = ceil(fabs(mWindSpeed));
		if (iSpeed != 1)
		{
			iSpeed = ceil(iSpeed*1.0 / 2) * 2;
			int iCount_fILLtrigon = iSpeed / 50;
			iSpeed %= 50;
			int iCount_trigon = iSpeed / 20;
			iSpeed %= 20;
			int iCount_LongLine = iSpeed / 4;
			int iCount_ShortLine = 0;
			if (iSpeed % 4 != 0)
			{
				iCount_ShortLine = 1;
			}
			//划风速线
			int iTmp = iCount_fILLtrigon + iCount_trigon + iCount_LongLine + iCount_ShortLine + 2;
			int k = 0;
			int i = 0;
			for (; i < iCount_fILLtrigon; i++, k++)
			{
				QPolygonF polygon;
                QPointF ptEnd_1 = ptBegin + QPointF(cosAngel_pole*lineLen*(iTmp - k) / iTmp, -sinAngel_pole * lineLen*(iTmp - k) / iTmp);
                QPointF ptEnd_2 = ptEnd_1 + QPointF(cosAngel_value*longLen, -sinAngel_value * longLen);
                QPointF ptEnd_3 = ptBegin + QPointF(cosAngel_pole*lineLen*(iTmp - k - 1) / iTmp, -sinAngel_pole * lineLen*(iTmp - k - 1) / iTmp);
				polygon.push_back(ptEnd_1);
				polygon.push_back(ptEnd_2);
				polygon.push_back(ptEnd_3);

				painter->save();
				painter->setPen(mainPen());
				painter->setBrush(mainPen().color());
				painter->drawPolygon(polygon);
				painter->restore();
			}
			i = 0;
			for (; i < iCount_trigon; i++, k++)
			{
				QPolygonF polygon;
                QPointF ptEnd_1 = ptBegin + QPointF(cosAngel_pole*lineLen*(iTmp - k) / iTmp, -sinAngel_pole * lineLen*(iTmp - k) / iTmp);
                QPointF ptEnd_2 = ptEnd_1 + QPointF(cosAngel_value*longLen, -sinAngel_value * longLen);
                QPointF ptEnd_3 = ptBegin + QPointF(cosAngel_pole*lineLen*(iTmp - k - 1) / iTmp, -sinAngel_pole * lineLen*(iTmp - k - 1) / iTmp);
				polygon.push_back(ptEnd_1);
				polygon.push_back(ptEnd_2);
				polygon.push_back(ptEnd_3);
				path.addPolygon(polygon);
			}
			i = 0;
			for (; i < iCount_LongLine; i++, k++)
			{
                QPointF ptEnd_1 = ptBegin + QPointF(cosAngel_pole*lineLen*(iTmp - k) / iTmp, -sinAngel_pole * lineLen*(iTmp - k) / iTmp);
                QPointF ptEnd_2 = ptEnd_1 + QPointF(cosAngel_value*longLen, -sinAngel_value * longLen);
				path.moveTo(ptEnd_1);
				path.lineTo(ptEnd_2);
			}
			if (iCount_ShortLine)
			{
                QPointF ptEnd_1 = ptBegin + QPointF(cosAngel_pole*lineLen*(iTmp - k) / iTmp, -sinAngel_pole * lineLen*(iTmp - k) / iTmp);
                QPointF ptEnd_2 = ptEnd_1 + QPointF(cosAngel_value*shortLen, -sinAngel_value * shortLen);
				path.moveTo(ptEnd_1);
				path.lineTo(ptEnd_2);
			}
		}
	}
	painter->save();
	painter->setPen(mainPen());
	painter->drawPath(path);
	mBoundingRect = path.boundingRect();
	painter->restore();
}

QPen QCPItemWindBarb::mainPen() const
{
	return mSelected ? mSelectedPen : mPen;
}
