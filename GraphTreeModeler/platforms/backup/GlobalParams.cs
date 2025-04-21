using System.Collections;
using System.Collections.Generic;
using UnityEngine;




/// 刘志浩的 "L-系统" 快乐手册
/// 
/// + - : 绕Z轴 正/负旋转 
/// \ / : 绕Y轴 正/负旋转
/// & ^ : 绕X轴 正/负旋转
///
/// F : 绘制树枝-移动Turtle（参数: len）
/// J : 绘制树叶（参数: w,h,horAng,VerAng）
/// 
/// ! ? : 正向/反向 缩放Turtle移动的步长 （与p_branch_len_factor对应）
/// ; @ : 正向/反向 缩放枝干旋转角度（与p_angX_factorXYZ对应），注意 ";"是分号
/// 



public class GlobalParams
{
    //public static string p_default_flower = "flower2";
    public static string p_default_flower = "flower2";
    public static string p_default_leaf = "leaf6.png";
    
    // [1] Plant类：
    // 树干相关
    public static float p_base_radius = 0.003f;  // 枝干最末端半径 (default=0.003f，重建3的树是0.005f)
    public static float p_radius_factor = 1.02f; // 枝干半径放大系数
                                                 //     ---默认参数；(0.008f, 1.02f)
                                                 //     ---03树木 (绿色) 0.01f, 1.04f;
                                                 //     ---椰子树:0.023f 1.04f
                                                 //     ---01树木 (红色) 0.01f, 1.035f
    // 树叶相关
    public static float p_leaf_witdth = 0.50f;  // 树叶宽度
    public static float p_leaf_height = 0.50f;  // 树叶高度
    public static float p_leaf_horBend_Ang = 14f;  // 树叶水平卷曲弯度
    public static float p_leaf_verBend_Ang = 15f;  // 树叶竖直卷曲弯度
    // 花朵相关
    public static float p_flower_scale = 0.3f; // 花朵的缩放系数

    //[2] GrammarParser类：
    // 默认旋转参数 (+-\/&^)
    public static float p_angX = 25.0f;   // x方向旋转角度
    public static float p_angY = 25.0f;   // y方向旋转角度
    public static float p_angZ = 25.0f;   // z方向旋转角度
    public static float p_angX_factor = 1.0f;  // 迭代缩放系数-x方向旋转角度
    public static float p_angY_factor = 1.0f;  // 迭代缩放系数-y方向旋转角度
    public static float p_angZ_factor = 1.0f;  // 迭代缩放系数-z方向旋转角度


    // 默认枝干参数 (F)
    public static float p_branch_len = 0.20f;  // 默认枝干长度 0.2f
                                               //     ---数据集展示用: 0.24
    public static float p_branch_len_factor = 0.9f;  // 迭代缩放系数-枝干长度

    // 随机性控制
    public static float p_prob_branch = 0.7f;  // 产生式的生成概率
    public static float p_prob_leaf = 0.8f;    // 叶片的生辰概率   (未使用)
}

public class GlobalSketchSetting
{
    // 用户端传来的参数设置
    // 常规参数
    public static float p_stem_len_ratio = 1.4f;  // C++端绘制的长度变换到Unity坐标系的缩放系数


    // 树叶相关
    public static string m_default_leaf_textures = "default.png";   // 树叶纹理
    public static string m_default_bark_textures = "default.png";   // 树冠纹理

    // 树干相关 (base len
    public static float p_twig_base_len = 0.09f;  // 新生成的twig的基础长度
    public static float p_twig_density = 1.0f;    // 新生成的twig的密度90%
    public static float p_twig_gravity_factor = 0.0f;
    public static float p_twig_base_dir_factor = 0.0f;

    public static float p_leaf_density = 0.15f;    // 叶片密度
    public static float p_leaf_size = 0.2f;       // 叶片尺寸
    public static bool p_leaf_is_crossed = true;   // 树叶是否是交叉模式

    public static float p_leaf_gravity_factor = 0.0f;
    public static float p_leaf_base_dir_factor = 0.0f;
}

public class GlobalPath
{
    // Python.exe的路径(UnityPlant虚拟环境)
    static public string p_python_exe = "D:/IDE/Anaconda/envs/UnityPlant/python.exe";
    static public string p_python_server_script = "C:/Users/liuzh/PycharmProjects/PlantAssemblyPythonLib/Plant_Server.py";
}

public class Interactive_OP
{
    static public string p_current_leaf_tex_name;
    static public string p_current_flower_name;

    static public bool p_isMoveSubTree;  // 是否移动子树

    static public float p_node_move_offset;
    static public float p_node_rotate_angle;

    static public float p_leaf_rotate_angle;
    static public float p_leaf_delta_size;
    static public float p_leaf_delta_angle;
}



public class Global_Interactive_Mode
{
    static public bool m_mode;  // 操作状态

    static public float p_rotate_angle = 0.0f;
    static public bool p_is_rotate = false;
    static public float p_rotate_delta = 0.1f;
    static public bool p_is_only_1_round = true;
}


public class Global_Materials
{
    // Preset Hierarchical boxes.
    static public bool m_new_box_material_mode = false;  // False:(旧的bbox的绘图模式), True: (新的bbox的绘图模式) 
    static public Material m_mat_box_ActiveNode = null;
    static public Material m_mat_box_Branch = null;
    static public Material m_mat_box_Flower = null;
    static public Material m_mat_box_Leaf = null;

    // 默认颜色
    static float p_box_transparent_alpha = 0.2f;
    static Color m_color_box_Flower = new Color(0.9f, 0.0f, 0.0f, p_box_transparent_alpha);  // 花
    static Color m_color_box_Leaf = new Color(0.71f, 1.0f, 0.56f, p_box_transparent_alpha); // 叶子
    static Color m_color_box_Branch = new Color(1.0f, 0.5f, 0.0f, 0.5f);  // 末-枝干
    static Color m_color_box_ActiveNode = new Color(0.2f, 0.4f, 0.4f, p_box_transparent_alpha);  // 中间-box(可扩展)


    static public void InitializeMats()
    {
        m_mat_box_ActiveNode = Resources.Load<Material>("BoxMaterials/MatBox_ActiveNode");
        m_mat_box_Branch = Resources.Load<Material>("BoxMaterials/MatBox_Branch");
        m_mat_box_Flower = Resources.Load<Material>("BoxMaterials/MatBox_Flower");
        m_mat_box_Leaf = Resources.Load<Material>("BoxMaterials/MatBox_Leaf");


        TCP_Client.SendMessage("A mat");
    }

    static public Material GetMaterialFromBoxType(MinBoundingBox.BoxType type)
    {
        if (type == MinBoundingBox.BoxType.endBranch)
            return m_mat_box_Branch;
        if (type == MinBoundingBox.BoxType.flower)
            return m_mat_box_Flower;
        if (type == MinBoundingBox.BoxType.leaf)
            return m_mat_box_Leaf;
        return m_mat_box_ActiveNode;
    }
}
