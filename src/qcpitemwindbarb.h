#pragma once
#include "qcustomplot.h"

class QCPItemWindBarb : public QCPAbstractItem
{
	Q_OBJECT
public:
	explicit QCPItemWindBarb(QCustomPlot *parentPlot);
	virtual ~QCPItemWindBarb() Q_DECL_OVERRIDE;

	// getters:
	QPen pen() const { return mPen; }
	QPen selectedPen() const { return mSelectedPen; }
	double windSpeed() const { return mWindSpeed; }
	double windDirection() const { return mWindDirection; }
	double mainLineLength() const { return mMainLineLength; }
	double longLineLength() const { return mLongLineLength; }

	// setters;
	void setPen(const QPen &pen);
	void setSelectedPen(const QPen &pen);
	void setWindSpeed(double ws);
	void setWindDirection(double wd);
	void setMainLineLength(double len);
	void setLongLineLength(double len);

	// reimplemented virtual methods:
	virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details = nullptr) const Q_DECL_OVERRIDE;

	QCPItemPosition * const start;

protected:
	double mWindSpeed;
	double mWindDirection;

	double mMainLineLength;
	double mLongLineLength;

	QRectF mBoundingRect;

	// property members:
	QPen mPen, mSelectedPen;

	// reimplemented virtual methods:
	virtual void draw(QCPPainter *painter) Q_DECL_OVERRIDE;

	QPen mainPen() const;
};
