#pragma once
#include "../common.h"
#include "../math_resolver/math_expression.h"

class User_SetMathExpressionEvent;

struct MathGraphOptions {
	std::string expression;
	RZUF3_RectD posZoom = { 0.0, 0.0, 1.0, 1.0 };
	SDL_Rect rect = { 0, 0, 128, 128 };
	SDL_Color bgColor = { 0, 0, 0, 0 };
	SDL_Color lineColor = { 255, 255, 255, 255 };
	SDL_Color axisColor = { 200, 128, 128, 255 };
	SDL_Color markerTextColor = { 200, 128, 128, 255 };
	SDL_Color gridColor = { 40, 40, 40, 255 };
	double gridSpacing = 0.1;
	int gridSpacingZoomOutFactor = 10;
	int gridSpacingZoomOutThreshold = 20;
	std::string textFontFilepath;
	int markerTextSize = 16;
	int markerTextStyle = TTF_STYLE_NORMAL;
	int markerTextOffset = 8;
	int markerSize = 8;
};

class MathGraph : public RZUF3_ObjectScript {
public:
	MathGraph(MathGraphOptions options);
	~MathGraph();

	void init();
	void deinit();

	bool setExpression(std::string expression);
	void setRect(SDL_Rect rect);
	void setPosZoom(RZUF3_RectD posZoom);

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
	void createText();
	void removeText();

	std::string doubleToShortString(double value);

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

	_DECLARE_LISTENER(Draw)
	_DECLARE_LISTENER(SetMathExpression)
	_DECLARE_LISTENER(SetRect)
	_DECLARE_LISTENER(DraggableUpdate)
};