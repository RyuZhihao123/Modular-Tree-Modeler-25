#ifndef GLOBALSETTING_H
#define GLOBALSETTING_H

#include <QString>

class GlobalSetting
{
public:
    static QString m_path_leaf_textures;
    static QString m_path_bark_textures;

    static QString m_default_leaf_texture;
    static QString m_default_bark_texture;

    static QString m_buffer_jsonNode_path;
    static QString m_buffer_jsonEdge_path;

    static QString command_header_GenerateNew;
    static QString command_header_RandSeed;
    static QString command_header_GenerateForest;

    static QString m_path_savedGraph;
    // old

    static QString command_header_predictDepth;
    static QString command_header_genTwigs;
    static QString command_header_updateParams;
    static QString command_header_userInteract;


    static QString command_header_operation;
};



#endif // GLOBALSETTING_H
