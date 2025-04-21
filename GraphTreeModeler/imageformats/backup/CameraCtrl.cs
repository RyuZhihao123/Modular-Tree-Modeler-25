using UnityEngine;
using System.Collections.Generic;


 public partial class CameraCtrl : MonoBehaviour
{
    // 相机移动相关
    public float moveVerticalSpeed = 10.0f;//水平移动速度
    public float moveHorizontalSpeed = 10.0f;//垂直移动速度
    public float moveJumpSpeed = 10.0f;//垂直移动速度
    private float angleX = 0.0f;
    private float angleY = 0.0f;
    private float movement_dist = 0.1f;

    // 是否显示Graph模式
    public bool isGraphRenderMode = false;

    // 绘制交互相关
    Vector3 last_pos;
    bool m_left_mouseBtn_down = false;
    bool m_right_mouseBtn_down = false;
    Internode m_curBranch = null;
    int m_curLeafID = -1;
    int m_curFlowerID = -1;

    Camera m_camera; // 相机

    // 用来获取Plant对象（Main.Plant)
    public GameObject m_main_obj;  // main那个脚本对应的物体
    Main m_main;                   // main脚本对象

    public void ResetState()
    {
        // 如果此时已经选中了一个叶子，那么重置mesh
        if (m_curBranch != null && m_curLeafID != -1)
        {
            Leaf lf = (Leaf)m_curBranch.kits[m_curLeafID];
            lf.isSelected = false;
            m_main.ConstructPlantGameObjects();
        }

        // 如果此时已经选中了一个花朵，那么重置mesh
        if (m_curBranch != null && m_curFlowerID != -1)
        {
            Flower fl = (Flower)m_curBranch.kits[m_curFlowerID];
            fl.isSelected = false;
            m_main.ConstructPlantGameObjects();
        }
        m_curLeafID = -1;
        m_curFlowerID = -1;
        m_curBranch = null;

        m_left_mouseBtn_down = false;
        m_right_mouseBtn_down = false;
    }


    void Start()
    {
        m_camera = GetComponent<Camera>();
        m_main = m_main_obj.GetComponent<Main>();



        this.UpdateCameraTransform();  // 初始化相机位置
    }

    void Update()
    {
        DoMouseEvent();  // 鼠标交互事件
        DoKey_MoveEvent();
       

        if (TCP_Qt_Server.InteractionInfo.interaction_type == "Rotate")  // 如果执行了旋转操作
        {
            camera_angleX -= TCP_Qt_Server.InteractionInfo.interaction_dx / 100.0f;
            camera_angleY += TCP_Qt_Server.InteractionInfo.interaction_dy / 500.0f;

            this.UpdateCameraTransform();

            TCP_Qt_Server.InteractionInfo.interaction_type = "none";
        }
        if (TCP_Qt_Server.InteractionInfo.interaction_type == "Zoom")  // 如果执行了缩放操作
        {
            camera_dist -= TCP_Qt_Server.InteractionInfo.interaction_zoom / 600.0f;

            if (camera_dist < 0.1f)
                camera_dist = 0.1f;

            this.UpdateCameraTransform();

            TCP_Qt_Server.InteractionInfo.interaction_type = "none";
        }
    }

    float camera_dist = 6.0f;
    float camera_angleX = Mathf.PI * 0.0f / 180.0f;
    float camera_angleY = Mathf.PI * 15.0f / 180.0f;


    // 更新相机位置
    public void UpdateCameraTransform()
    {
        Transform transf = this.GetComponent<Transform>();

        if (camera_angleY >= Mathf.PI / 2.0f)
            camera_angleY = Mathf.PI / 2.0f - 0.01f;
        if (camera_angleY <= -Mathf.PI / 2.0f)
            camera_angleY = Mathf.PI / 2.0f + 0.01f;

        transf.position = new Vector3(
            camera_dist * Mathf.Cos(camera_angleY) * Mathf.Cos(camera_angleX),
            camera_dist * Mathf.Sin(camera_angleY),
            camera_dist * Mathf.Cos(camera_angleY) * Mathf.Sin(camera_angleX)
            );

        transf.position += m_main.plant.m_plant_center;
        transf.forward = (m_main.plant.m_plant_center - transf.position).normalized;
    }

    void DoMouseEvent()
    {
       
    }



    // 键盘事件
    bool m_camera_move_flag = false;  // 相机是否发生移动

    void DoKey_MoveEvent()
    {
        // ZX-CV-BN: （在选定小球的时候）沿着XYZ轴移动
        // 绕XYZ轴旋转。
        // 在选择一个节点的基础上 -> （如果按下alt）可以拖动一个线条


        ///////////////// 移动相机 WASD /////////////////
        if (Input.GetKeyDown(KeyCode.LeftControl))  // control键用来开启/关闭
        {
            m_camera_move_flag = !m_camera_move_flag;
        }


        if (m_camera_move_flag)
        {
            float Ztranslation = Input.GetAxis("Vertical") * moveVerticalSpeed * Time.deltaTime;
            float Xtranslation = Input.GetAxis("Horizontal") * moveHorizontalSpeed * Time.deltaTime;
            float Ytranslation = Input.GetAxis("Jump") * moveJumpSpeed * Time.deltaTime;

            //重新计算旋转角度
            angleX += Input.GetAxis("Mouse X");  // 水平方向转过的角度 [-width, width]
            angleY += Input.GetAxis("Mouse Y");  // 竖直方向转过的角度 [-height, height]

            if (angleX < 0)
                angleX += 360;
            if (angleY > 360)
                angleX -= 360;
            angleY = Mathf.Clamp(angleY, -89, 89);

            //----------------------旋转视角---------------------------
            transform.forward = new Vector3(
                1.0f * Mathf.Cos(3.141592653f * angleY / 180.0f) * Mathf.Sin(3.141592653f * angleX / 180.0f),
                1.0f * Mathf.Sin(3.141592653f * angleY / 180.0f),
                1.0f * Mathf.Cos(3.141592653f * angleY / 180.0f) * Mathf.Cos(3.141592653f * angleX / 180.0f)
                ).normalized;  // 正向默认都是(0,0,1)

            // --------------WASD移动 & space上移------------------
            transform.Translate(0, 0, Ztranslation);
            transform.Translate(Xtranslation, 0, 0);
            transform.Translate(0, Ytranslation, 0);
        }
    }



}
