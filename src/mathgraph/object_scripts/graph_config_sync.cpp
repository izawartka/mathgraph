#include "graph_config_sync.h"

GraphConfigSync::GraphConfigSync(std::string configName, MathGraph* graph)
{
	mp_options.configName = configName;
	mp_options.graph = graph;
}

GraphConfigSync::GraphConfigSync(GraphConfigSyncOptions options)
{
	mp_options = options;
}

GraphConfigSync::~GraphConfigSync()
{
}

void GraphConfigSync::init()
{
	m_options = mp_options;

	setConfigName(m_options.configName);
	setUseOnConfigEntryUpdate(m_options.useOnConfigEntryUpdate);

	if (m_options.updateGraphOnInit) {
		updateGraphXAxisScale();
		updateGraphYAxisScale();
		updateGraphShowGrid();
		updateGraphShowPoint();
	}
}

void GraphConfigSync::deinit()
{
	setUseOnConfigEntryUpdate(false);

	m_configFile = nullptr;
}

void GraphConfigSync::setConfigName(std::string configName)
{
	m_options.configName = configName;
	m_configFile = g_game->getConfigFile(configName);
}

void GraphConfigSync::setSavedChangesOnly(bool savedChangesOnly)
{
	m_options.savedChangesOnly = savedChangesOnly;
}

void GraphConfigSync::setUseOnConfigEntryUpdate(bool useOnConfigEntryUpdate)
{
	m_options.useOnConfigEntryUpdate = useOnConfigEntryUpdate;
	RZUF3_EventsManager* eventsManager = g_scene->getEventsManager();

	if (useOnConfigEntryUpdate && !m_hasOnConfigEntryUpdateListener) {
		_ADD_LISTENER(eventsManager, ConfigEntryUpdate);
		m_hasOnConfigEntryUpdateListener = true;
	}
	else if (!useOnConfigEntryUpdate && m_hasOnConfigEntryUpdateListener) {
		_REMOVE_LISTENER(eventsManager, ConfigEntryUpdate);
		m_hasOnConfigEntryUpdateListener = false;
	}
}

MathGraphAxisScale GraphConfigSync::stringToAxisScale(std::string& str)
{
	if(str == "decimal") return MathGraphAxisScale::Decimal;
	if(str == "log") return MathGraphAxisScale::Logarithmic;
	if(str == "pi_based") return MathGraphAxisScale::PiBased;

	return MathGraphAxisScale::Decimal;
}

void GraphConfigSync::updateGraphXAxisScale()
{
	if (!m_configFile) return;
	if (!m_options.graph) return;

	std::string value;
	if(!m_configFile->getValue(m_options.xAxisScaleKey, value)) return;

	MathGraphAxisScale xScale = stringToAxisScale(value);
	m_options.graph->setXAxisScale(xScale);
}

void GraphConfigSync::updateGraphYAxisScale()
{
	if (!m_configFile) return;
	if (!m_options.graph) return;

	std::string value;
	if(!m_configFile->getValue(m_options.yAxisScaleKey, value)) return;

	MathGraphAxisScale yScale = stringToAxisScale(value);
	m_options.graph->setYAxisScale(yScale);
}

void GraphConfigSync::updateGraphShowGrid()
{
	if (!m_configFile) return;
	if (!m_options.graph) return;

	bool value;
	if(!m_configFile->getValue(m_options.showGridKey, value)) return;

	m_options.graph->setShowGrid(value);
}

void GraphConfigSync::updateGraphShowPoint()
{
	if (!m_configFile) return;
	if (!m_options.graph) return;

	bool value;
	if(!m_configFile->getValue(m_options.showPointKey, value)) return;

	m_options.graph->setShowPoint(value);
}

void GraphConfigSync::onConfigEntryUpdate(RZUF3_ConfigEntryUpdateEvent* event)
{
	if (!m_options.useOnConfigEntryUpdate) return;
	if (m_options.savedChangesOnly && !event->isSaved()) return;
	if(event->getConfigFile() != m_configFile) return;

	std::string key = event->getKey();

	if(key == m_options.xAxisScaleKey) return updateGraphXAxisScale();
	else if(key == m_options.yAxisScaleKey) return updateGraphYAxisScale();
	else if(key == m_options.showGridKey) return updateGraphShowGrid();
	else if(key == m_options.showPointKey) return updateGraphShowPoint();
}
