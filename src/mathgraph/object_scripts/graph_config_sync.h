#pragma once
#include "object_scripts.h"
#include "math_graph.h"

struct GraphConfigSyncOptions {
	std::string configName;
	MathGraph* graph = nullptr;
	std::string xAxisScaleKey = "x_scale";
	std::string yAxisScaleKey = "y_scale";
	std::string showGridKey = "show_grid";
	std::string showPointKey = "show_point";
	bool savedChangesOnly = false;
	bool updateGraphOnInit = true;
	bool useOnConfigEntryUpdate = true;
};

class GraphConfigSync : public RZUF3_ObjectScript {
public:
	GraphConfigSync(std::string configName, MathGraph* graph);
	GraphConfigSync(GraphConfigSyncOptions options);
	~GraphConfigSync();

	void init();
	void deinit();

	void setConfigName(std::string configName);
	void setSavedChangesOnly(bool savedChangesOnly);
	void setUseOnConfigEntryUpdate(bool useOnConfigEntryUpdate);

	MathGraphAxisScale stringToAxisScale(std::string& str);
	void updateGraphXAxisScale();
	void updateGraphYAxisScale();
	void updateGraphShowGrid();
	void updateGraphShowPoint();

	std::string getConfigName() const { return m_options.configName; }
	bool getSavedChangesOnly() const { return m_options.savedChangesOnly; }
	bool getUseOnConfigEntryUpdate() const { return m_options.useOnConfigEntryUpdate; }

protected:
	void onConfigEntryUpdate(RZUF3_ConfigEntryUpdateEvent* event);

	GraphConfigSyncOptions mp_options;

	GraphConfigSyncOptions m_options;
	RZUF3_ConfigFile* m_configFile = nullptr;
	bool m_hasOnConfigEntryUpdateListener = false;

	_DECLARE_LISTENER(ConfigEntryUpdate)
};