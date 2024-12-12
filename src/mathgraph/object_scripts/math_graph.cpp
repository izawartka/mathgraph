#include "math_graph.h"
#include "../math_resolver/math_parser.h"
#include "../math_resolver/math_solver.h"
#include "../events/events.h"

MathGraph::MathGraph(MathGraphOptions options)
{
	mp_options = options;
}

MathGraph::~MathGraph()
{

}

void MathGraph::init()
{
	m_options = mp_options;
	createTextures(m_options.rect.w, m_options.rect.h);
	createText();
	setExpression(m_options.expression);
	createClickable();

	RZUF3_EventsManager* eventsManager = g_scene->getEventsManager();
	_ADD_LISTENER(eventsManager, Draw);
	_ADD_LISTENER_CL(eventsManager, SetMathExpression, User);

	RZUF3_EventsManager* objEventsManager = m_object->getEventsManager();
	_ADD_LISTENER(objEventsManager, SetRect);
	_ADD_LISTENER(objEventsManager, DraggableUpdate);
}

void MathGraph::deinit()
{
	RZUF3_EventsManager* eventsManager = g_scene->getEventsManager();
	_REMOVE_LISTENER(eventsManager, Draw);
	_REMOVE_LISTENER_CL(eventsManager, SetMathExpression, User);

	RZUF3_EventsManager* objEventsManager = m_object->getEventsManager();
	_REMOVE_LISTENER(objEventsManager, SetRect);
	_REMOVE_LISTENER(objEventsManager, DraggableUpdate);

	removeExpression();
	removeTextures();
	removeText();
	removeClickable();
}

bool MathGraph::setExpression(std::string expressionStr)
{
	removeExpression();
	m_lineTextureNeedsUpdate = true;

	m_expression = new MathExpression;
	m_error = new MathError;

	MathParser::parseExpression(expressionStr, *m_expression, *m_error);

	MathErrorType errorType = m_error->type;
	if (errorType)
	{
		spdlog::warn("Error while parsing expression. Code: {}", (int)errorType);
		delete m_expression;
		m_expression = nullptr;
	}
	else {
		spdlog::info("Expression parsed successfully");
		delete m_error;
		m_error = nullptr;

		// MathParser::debugPrintExpression(*m_expression);

		int varCount = m_expression->variables.size();
		if (varCount > 1 || varCount == 1 && m_expression->variables[0] != "x") {
			spdlog::warn("Expression has more than one variable or variable is not 'x'");
			errorType = TOO_MUCH_VARIABLES;

			delete m_expression;
			m_expression = nullptr;
		}
	}

	RZUF3_EventsManager* eventsManager = g_scene->getEventsManager();
	User_MathExpressionErrorEvent errorEvent(errorType);
	eventsManager->dispatchEvent(&errorEvent);

	return errorType == NO_ERROR;
}

void MathGraph::setRect(SDL_Rect rect)
{
	m_options.rect = rect;

	removeTextures();
	createTextures(rect.w, rect.h);
	m_lineTextureNeedsUpdate = true;
	m_helpersTextureNeedsUpdate = true;

	if(m_clickable) m_clickable->setRect(rect);
}

void MathGraph::setPosZoom(RZUF3_RectD posZoom)
{
	m_options.posZoom = posZoom;
	m_lineTextureNeedsUpdate = true;
	m_helpersTextureNeedsUpdate = true;
}

void MathGraph::setXAxisScale(MathGraphAxisScale scale)
{
	m_options.xAxisScale = scale;
	m_helpersTextureNeedsUpdate = true;
	m_lineTextureNeedsUpdate = true;
}

void MathGraph::setYAxisScale(MathGraphAxisScale scale)
{
	m_options.yAxisScale = scale;
	m_helpersTextureNeedsUpdate = true;
	m_lineTextureNeedsUpdate = true;
}

void MathGraph::setShowGrid(bool showGrid)
{
	m_options.showGrid = showGrid;
	m_helpersTextureNeedsUpdate = true;
}

void MathGraph::setShowPoint(bool showPoint)
{
	m_options.showPoint = showPoint;
}

void MathGraph::onDraw(RZUF3_DrawEvent* event)
{
	if (m_helpersTexture) {
		if (m_helpersTextureNeedsUpdate) updateHelpersTexture();

		g_renderer->setAlign(RZUF3_Align_TopLeft);
		g_renderer->drawTexture(m_object, m_helpersTexture, nullptr, m_options.rect);
	}

	if (m_lineTexture == nullptr) return;
	if (m_lineTextureNeedsUpdate) updateLineTexture();

	g_renderer->setAlign(RZUF3_Align_TopLeft);
	g_renderer->drawTexture(m_object, m_lineTexture, nullptr, m_options.rect);

	drawPoint();
}

void MathGraph::onSetMathExpression(User_SetMathExpressionEvent* event)
{
	setExpression(event->getText());
}

void MathGraph::onSetRect(RZUF3_SetRectEvent* event)
{
	setRect(event->getRect());
}

void MathGraph::onDraggableUpdate(RZUF3_DraggableUpdateEvent* event)
{
	setPosZoom(event->getAsRectD());
}

void MathGraph::removeExpression()
{
	if(m_expression != nullptr) delete m_expression;
	if(m_error != nullptr) delete m_error;

	m_expression = nullptr;
	m_error = nullptr;
	m_lineTextureNeedsUpdate = true;

	m_options.expression = "";
}

void MathGraph::removeTextures()
{
	if (m_lineTexture != nullptr) SDL_DestroyTexture(m_lineTexture);
	m_lineTexture = nullptr;

	if (m_helpersTexture != nullptr) SDL_DestroyTexture(m_helpersTexture);
	m_helpersTexture = nullptr;
}

void MathGraph::createTextures(int width, int height)
{
	if (m_lineTexture == nullptr) {
		m_lineTexture = SDL_CreateTexture(g_renderer->getSDLRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
		SDL_SetTextureBlendMode(m_lineTexture, SDL_BLENDMODE_BLEND);

		m_lineTextureNeedsUpdate = true;
	}

	if (m_helpersTexture == nullptr) {
		m_helpersTexture = SDL_CreateTexture(g_renderer->getSDLRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
		SDL_SetTextureBlendMode(m_helpersTexture, SDL_BLENDMODE_BLEND);

		m_helpersTextureNeedsUpdate = true;
	}
}

void MathGraph::updateHelpersTexture()
{
	if (!m_helpersTexture || !m_markerTextX || !m_markerTextY) return;
	m_helpersTextureNeedsUpdate = false;

	double centerX = m_options.rect.w / 2.0;
	double centerY = m_options.rect.h / 2.0;

	int axisXY = std::round(centerY + m_options.posZoom.y * m_options.posZoom.h);
	int axisYX = std::round(centerX + m_options.posZoom.x * m_options.posZoom.w);

	SDL_Renderer* renderer = g_renderer->getSDLRenderer();
	SDL_SetRenderTarget(renderer, m_helpersTexture);
	SDL_SetRenderDrawColor(renderer, m_options.bgColor.r, m_options.bgColor.g, m_options.bgColor.b, m_options.bgColor.a);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, m_options.gridColor.r, m_options.gridColor.g, m_options.gridColor.b, m_options.gridColor.a);

	double gridSpacingX = m_options.gridSpacing * m_options.posZoom.w;
	while(gridSpacingX < m_options.gridSpacingZoomOutThreshold) gridSpacingX *= m_options.gridSpacingZoomOutFactor;

	double gridSpacingY = m_options.gridSpacing * m_options.posZoom.h;
	while (gridSpacingY < m_options.gridSpacingZoomOutThreshold) gridSpacingY *= m_options.gridSpacingZoomOutFactor;

	if (m_options.showGrid) {
		double gridStartX = std::fmod(m_options.posZoom.x * m_options.posZoom.w, gridSpacingX);
		int gridCountX = m_options.rect.w / gridSpacingX + 1;

		for (int i = -gridCountX; i <= gridCountX; i++)
		{
			int x = std::round(centerX + gridStartX + i * gridSpacingX);

			if (x == axisYX) continue;
			if (x >= 0 && x < m_options.rect.w) SDL_RenderDrawLine(renderer, x, 0, x, m_options.rect.h);
		}

		double gridStartY = std::fmod(m_options.posZoom.y * m_options.posZoom.h, gridSpacingY);
		int gridCountY = m_options.rect.h / gridSpacingY + 1;

		for (int i = -gridCountY; i <= gridCountY; i++)
		{
			int y = std::round(centerY + gridStartY + i * gridSpacingY);

			if (y == axisXY) continue;
			if (y >= 0 && y < m_options.rect.h) SDL_RenderDrawLine(renderer, 0, y, m_options.rect.w, y);
		}
	}

	SDL_SetRenderDrawColor(renderer, m_options.axisColor.r, m_options.axisColor.g, m_options.axisColor.b, m_options.axisColor.a);
	g_renderer->setUseObjectPos(false);

	if (axisXY >= 0 && axisXY < m_options.rect.h) {
		SDL_RenderDrawLine(renderer, 0, axisXY, m_options.rect.w, axisXY);

		int markerXStartY = std::round(axisXY - m_options.markerSize / 2.0);
		int markerXEndY = std::round(axisXY + m_options.markerSize / 2.0);
		int markerXX = std::round(axisYX + gridSpacingX);
		SDL_RenderDrawLine(renderer, markerXX, markerXStartY, markerXX, markerXEndY);

		std::string markerXText = getMarkerText(true, gridSpacingX / m_options.posZoom.w);
		int markerTextXY = std::round(axisXY + m_options.markerTextOffset);

		m_markerTextX->setText(markerXText);
		m_markerTextX->setDstPos(markerXX, markerTextXY);
		m_markerTextX->draw();
	}

	if (axisYX >= 0 && axisYX < m_options.rect.w) {
		SDL_RenderDrawLine(renderer, axisYX, 0, axisYX, m_options.rect.h);

		int markerYStartX = std::round(axisYX - m_options.markerSize / 2.0);
		int markerYEndX = std::round(axisYX + m_options.markerSize / 2.0);
		int markerYY = std::round(axisXY - gridSpacingY);
		SDL_RenderDrawLine(renderer, markerYStartX, markerYY, markerYEndX, markerYY);

		std::string markerYText = getMarkerText(false, gridSpacingY / m_options.posZoom.h);
		int markerTextYX = std::round(axisYX - m_options.markerTextOffset);

		m_markerTextY->setText(markerYText);
		m_markerTextY->setDstPos(markerTextYX, markerYY);
		m_markerTextY->draw();
	}

	g_renderer->setUseObjectPos(true);
	SDL_SetRenderTarget(renderer, nullptr);
}

void MathGraph::updateLineTexture()
{
	if (!m_lineTexture) return;
	m_lineTextureNeedsUpdate = false;

	SDL_Renderer* renderer = g_renderer->getSDLRenderer();
	SDL_SetRenderTarget(renderer, m_lineTexture);
	SDL_SetRenderDrawColor(renderer, m_options.bgColor.r, m_options.bgColor.g, m_options.bgColor.b, m_options.bgColor.a);
	SDL_RenderClear(renderer);
	SDL_SetRenderTarget(renderer, nullptr);

	if (m_expression == nullptr) return;
	if (m_error != nullptr) return;

	double centerX = m_options.rect.w / 2.0;
	double centerY = m_options.rect.h / 2.0;

	SDL_SetRenderDrawColor(renderer, m_options.lineColor.r, m_options.lineColor.g, m_options.lineColor.b, m_options.lineColor.a);
	SDL_SetRenderTarget(renderer, m_lineTexture);

	SDL_Point* points = new SDL_Point[m_options.rect.w];
	int pointCount = 0;
	double lastY = 0;
	bool lastInBounds = false;
	bool lastOk = false;
	bool lastIsError = true;

	for (int i = 0; i <= m_options.rect.w; i++)
	{
		if (!lastOk || i >= m_options.rect.w)
		{
			if (pointCount > 1)
			{
				SDL_RenderDrawLines(renderer, points, pointCount);
			}

			pointCount = 0;

			if (i >= m_options.rect.w) break;
		}

		double valueX, valueY, x, y;
		x = i;
		posToValueX(x, valueX);
		if (!solveForX(valueX, valueY)) {
			lastIsError = true;
			continue;
		}
		valueToPosY(valueY, y);

		if (y < 0) y = -1;
		if (y >= m_options.rect.h) y = m_options.rect.h;

		bool inBounds = (y >= 0 && y < m_options.rect.h);
		bool ok = inBounds || lastInBounds;

		if (!lastOk && ok && !lastIsError) points[pointCount++] = { (int)x-1, (int)lastY };
		if (ok) points[pointCount++] = { (int)x, (int)y };

		lastY = y;
		lastInBounds = inBounds;
		lastOk = ok;
		lastIsError = false;
	}

	delete[] points;
	SDL_SetRenderTarget(renderer, nullptr);
}

void MathGraph::removeText()
{
	if (m_markerTextX != nullptr) {
		m_object->removeScript(m_markerTextX);
		delete m_markerTextX;
		m_markerTextX = nullptr;
	}

	if (m_markerTextY != nullptr) {
		m_object->removeScript(m_markerTextY);
		delete m_markerTextY;
		m_markerTextY = nullptr;
	}

	if (m_pointText != nullptr) {
		m_object->removeScript(m_pointText);
		delete m_pointText;
		m_pointText = nullptr;
	}
}

void MathGraph::createText()
{
	RZUF3_TextRendererOptions options;
	options.text = "";
	options.useOnDraw = false;
	options.useLangFile = false;
	options.style = m_options.markerTextStyle;

	if (m_markerTextX == nullptr) {
		options.alignment = RZUF3_Align_Top;
		m_markerTextX = new RZUF3_TextRenderer(options);
		m_object->addScript(m_markerTextX);
	}

	if (m_markerTextY == nullptr) {
		options.alignment = RZUF3_Align_Right;
		m_markerTextY = new RZUF3_TextRenderer(options);
		m_object->addScript(m_markerTextY);
	}

	if (m_pointText == nullptr) {
		options.style = m_options.pointTextStyle;
		options.alignment = RZUF3_Align_Bottom;
		m_pointText = new RZUF3_TextRenderer(options);
		m_object->addScript(m_pointText);
	}
}

void MathGraph::removeClickable()
{
	if (m_clickable == nullptr) return;

	RZUF3_Object* clickableObject = m_clickable->getObject();
	g_scene->removeObject(clickableObject->getName());

	delete m_clickable;
	m_clickable = nullptr;
}

void MathGraph::createClickable()
{
	if (m_clickable != nullptr) return;

	RZUF3_ClickableOptions options;
	options.alignment = RZUF3_Align_TopLeft;
	options.useOnHoverCursor = false;
	options.useOnSetRect = false;
	options.rect = m_options.rect;

	m_clickable = new RZUF3_Clickable(options);

	RZUF3_ObjectDefinition objectDef;
	objectDef.name = m_object->getName() + "_clickable";
	objectDef.pos = { 0, 0 };
	objectDef.scripts = { m_clickable };

	RZUF3_Object* object = g_scene->addObject(objectDef);

	object->setParent(m_object);
	object->init();
}

void MathGraph::drawPoint()
{
	if (!m_clickable || !m_clickable->isInside()) return;
	if (!m_options.showPoint) return;

	int x, y;
	m_clickable->getLastMousePos(x, y);

	double posX, valueX, valueY, posY;
	posX = x;
	posToValueX(posX, valueX);
	if (!solveForX(valueX, valueY)) return;
	valueToPosY(valueY, posY);
	if(posY < 0 || posY >= m_options.rect.h || isnan(posY)) return;

	posX += m_options.rect.x;
	posY += m_options.rect.y;

	m_pointText->setText("(" + doubleToShortString(valueX) + ", " + doubleToShortString(valueY) + ")");
	m_pointText->setDstPos(posX, posY - m_options.pointTextOffset);
	m_pointText->draw();

	SDL_Rect pointRect = { posX, posY, m_options.pointSize, m_options.pointSize };

	g_renderer->setAlign(RZUF3_Align_Center);
	g_renderer->setColor(m_options.pointColor);
	g_renderer->fillCircle(m_object, pointRect);
}

void MathGraph::posToValueX(double posX, double& valueX) const
{
	double centerX = m_options.rect.w / 2.0;

	double input = (posX - centerX) / m_options.posZoom.w - m_options.posZoom.x;
	if (m_options.xAxisScale == MathGraphAxisScale::Logarithmic) input = std::pow(10, input);
	else if (m_options.xAxisScale == MathGraphAxisScale::PiBased) input = input * M_PI;

	valueX = input;
}

bool MathGraph::solveForX(double valueX, double& valueY)
{
	if (m_expression == nullptr) return false;

	MathError error;
	MathSolver::solveExpression(*m_expression, { valueX }, valueY, error);

	return error.type == NO_ERROR;
}

void MathGraph::valueToPosY(double value, double& posY) const
{
	double centerY = m_options.rect.h / 2.0;

	if (m_options.yAxisScale == MathGraphAxisScale::Logarithmic) value = std::log10(value);
	else if (m_options.yAxisScale == MathGraphAxisScale::PiBased) value = value / M_PI;

	posY = centerY + (m_options.posZoom.y - value) * m_options.posZoom.h;
}

std::string MathGraph::getMarkerText(bool isX, double coef)
{
	MathGraphAxisScale scale = isX ? m_options.xAxisScale : m_options.yAxisScale;
	std::string text;

	switch (scale) {
	case MathGraphAxisScale::Decimal:
		return doubleToShortString(coef);
	case MathGraphAxisScale::Logarithmic:
		return "10^" + doubleToShortString(coef);
	case MathGraphAxisScale::PiBased:
		return doubleToShortString(coef) + "\xCF\x80";
	default:
		return "";
	}
}

std::string MathGraph::doubleToShortString(double value, size_t maxDecimals)
{
	std::string result = std::to_string(value);
	size_t decimalPos = result.find('.');
	size_t pos = std::min(result.find_last_not_of('0') + 1, decimalPos + maxDecimals + 1);
	result.erase(pos, std::string::npos);
	if (!result.empty() && result.back() == '.') {
		result.pop_back();
	}

	return result;
}