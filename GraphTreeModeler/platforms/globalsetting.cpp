#include "globalsetting.h"

//// 代码内调试用
//QString GlobalSetting::m_path_leaf_textures = "D:/Projects/UnityProjects/SelfOrganizationTree/Assets/Materials/LeafTextures/HD/base/";
//QString GlobalSetting::m_path_bark_textures = "D:/Projects/UnityProjects/SelfOrganizationTree/Assets/Materials/BarkTextures/";

//QString GlobalSetting::m_buffer_jsonNode_path = "D:/Projects/QtProjects/SpaceColonizationTree_Qt_Client/build/release/tmpJsonNodes.txt";
//QString GlobalSetting::m_buffer_jsonEdge_path = "D:/Projects/QtProjects/SpaceColonizationTree_Qt_Client/build/release/tmpJsonEdges.txt";
//QString GlobalSetting::m_path_savedGraph = "D:/Projects/QtProjects/SpaceColonizationTree_Qt_Client/res/trees/TreeGraph/";

//// 发布用
QString GlobalSetting::m_path_leaf_textures = "./Assets/Res/LeafTextures/";
QString GlobalSetting::m_path_bark_textures =  "./Assets/Res/BarkTextures/";

QString GlobalSetting::m_buffer_jsonNode_path = "./cache/tmpJsonNodes.txt";
QString GlobalSetting::m_buffer_jsonEdge_path =  "./cache/tmpJsonEdges.txt";
QString GlobalSetting::m_path_savedGraph = "./SavedGraphs/";

//////////////////////////////////////

QString GlobalSetting::m_default_leaf_texture = "_default.png";
QString GlobalSetting::m_default_bark_texture = "default_bark.png";

QString GlobalSetting::command_header_GenerateNew = "GenerateNew ";
QString GlobalSetting::command_header_RandSeed = "RandSeed ";
QString GlobalSetting::command_header_GenerateForest = "GenerateForest ";


//////////////////////////////////////

QString GlobalSetting::command_header_predictDepth = "A ";
QString GlobalSetting::command_header_genTwigs = "B ";
QString GlobalSetting::command_header_updateParams = "C ";
QString GlobalSetting::command_header_userInteract = "D ";
QString GlobalSetting::command_header_operation = "E ";


