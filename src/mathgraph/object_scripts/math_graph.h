#pragma once
#include "../common.h"
#include "../math_resolver/math_expression.h"

class User_SetMathExpressionEvent;

enum class MathGraphAxisScale {
	Decimal,
	Logarithmic,
	PiBased
};

struct MathGraphOptions {
	std::string expression;
	RZUF3_RectD posZoom = { 0.0, 0.0, 1.0, 1.0 };
	SDL_Rect rect = { 0, 0, 128, 128 };
	SDL_Color bgColor = { 0, 0, 0, 0 };
	SDL_Color lineColor = { 255, 255, 255, 255 };
	SDL_Color axisColor = { 200, 128, 128, 255 };
	SDL_Color gridColor = { 40, 40, 40, 255 };
	SDL_Color pointColor = { 255, 255, 255, 255 };
	double gridSpacing = 0.1;
	int gridSpacingZoomOutFactor = 10;
	int gridSpacingZoomOutThreshold = 20;
	RZUF3_TextRendererStyle markerTextStyle;
	RZUF3_TextRendererStyle pointTextStyle;
	int pointSize = 8;
	int pointTextOffset = 8;
	int markerSize = 8;
	int markerTextOffset = 8;
	MathGraphAxisScale xAxisScale = MathGraphAxisScale::Decimal;
	MathGraphAxisScale yAxisScale = MathGraphAxisScale::Decimal;
	bool showGrid = true;
	bool showPoint = true;
	int pointMaxDistance = 40;

	MathGraphOptions() {
		markerTextStyle.color = axisColor;
		pointTextStyle.color = pointColor;
	}

	void setFontFilepath(std::string filepath) {
		markerTextStyle.fontFilepath = filepath;
		pointTextStyle.fontFilepath = filepath;
	}
};

static struct MathGraphPoint {
	double x;
	double y;
} mathGraphPoint;

class MathGraph : public RZUF3_ObjectScript {
public:
	MathGraph(MathGraphOptions options);
	~MathGraph();

	void init();
	void deinit();

	bool setExpression(std::string expression);
	void setRect(SDL_Rect rect);
	void setPosZoom(RZUF3_RectD posZoom);
	void setXAxisScale(MathGraphAxisScale scale);
	void setYAxisScale(MathGraphAxisScale scale);
	void setShowGrid(bool showGrid);
	void setShowPoint(bool showPoint);

	MathError* getError() { return m_error; }

protected:
	void onDraw(RZUF3_DrawEvent* event);
	void onSetMathExpression(User_SetMathExpressionEvent* event);
	void onSetRect(RZUF3_SetRectEvent* event);
	void onDraggableUpdate(RZUF3_DraggableUpdateEvent* event);

	void removeExpression();
	void removeTextures();
	void createTextures(int width, int height);
	void updateHelpersTexture();
	void updateLineTexture();
	void removeText();
	void createText();
	void removeClickable();
	void createClickable();
	void drawPoint();

	std::string getMarkerText(bool isX, double coef);
	std::string doubleToShortString(double value, size_t maxDecimals = 3);
	void posToValueX(double posX, double& valueX) const;
	void posToValueXY(double posX, double posY, double& valueX, double& valueY) const;
	bool solveForX(double valueX, double& valueY);
	void valueToPosY(double valueY, double& posY) const;
	void valueToPosXY(double valueX, double valueY, double& posX, double& posY) const;
	MathGraphPoint getClosestCachedPoint(double x, double y);

	MathGraphOptions mp_options;

	MathGraphOptions m_options;
	MathExpression* m_expression = nullptr;
	MathError* m_error = nullptr;
	SDL_Texture* m_helpersTexture = nullptr;
	bool m_helpersTextureNeedsUpdate = false;
	SDL_Texture* m_lineTexture = nullptr;
	bool m_lineTextureNeedsUpdate = false;
	RZUF3_TextRenderer* m_markerTextX = nullptr;
	RZUF3_TextRenderer* m_markerTextY = nullptr;
	RZUF3_TextRenderer* m_pointText = nullptr;
	RZUF3_Clickable* m_clickable = nullptr;
	MathGraphPoint* m_cachedPoints = nullptr;
	int m_cachedPointsCount = 0;

	_DECLARE_LISTENER(Draw)
	_DECLARE_LISTENER(SetMathExpression)
	_DECLARE_LISTENER(SetRect)
	_DECLARE_LISTENER(DraggableUpdate)
};