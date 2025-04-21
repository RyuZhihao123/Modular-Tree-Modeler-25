using UnityEngine;
using System.Collections.Generic;


 public partial class CameraCtrl : MonoBehaviour
{
    // ����ƶ����
    public float moveVerticalSpeed = 10.0f;//ˮƽ�ƶ��ٶ�
    public float moveHorizontalSpeed = 10.0f;//��ֱ�ƶ��ٶ�
    public float moveJumpSpeed = 10.0f;//��ֱ�ƶ��ٶ�
    private float angleX = 0.0f;
    private float angleY = 0.0f;
    private float movement_dist = 0.1f;

    // �Ƿ���ʾGraphģʽ
    public bool isGraphRenderMode = false;

    // ���ƽ������
    Vector3 last_pos;
    bool m_left_mouseBtn_down = false;
    bool m_right_mouseBtn_down = false;
    Internode m_curBranch = null;
    int m_curLeafID = -1;
    int m_curFlowerID = -1;

    Camera m_camera; // ���

    // ������ȡPlant����Main.Plant)
    public GameObject m_main_obj;  // main�Ǹ��ű���Ӧ������
    Main m_main;                   // main�ű�����

    public void ResetState()
    {
        // �����ʱ�Ѿ�ѡ����һ��Ҷ�ӣ���ô����mesh
        if (m_curBranch != null && m_curLeafID != -1)
        {
            Leaf lf = (Leaf)m_curBranch.kits[m_curLeafID];
            lf.isSelected = false;
            m_main.ConstructPlantGameObjects();
        }

        // �����ʱ�Ѿ�ѡ����һ�����䣬��ô����mesh
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



        this.UpdateCameraTransform();  // ��ʼ�����λ��
    }

    void Update()
    {
        DoMouseEvent();  // ��꽻���¼�
        DoKey_MoveEvent();
       

        if (TCP_Qt_Server.InteractionInfo.interaction_type == "Rotate")  // ���ִ������ת����
        {
            camera_angleX -= TCP_Qt_Server.InteractionInfo.interaction_dx / 100.0f;
            camera_angleY += TCP_Qt_Server.InteractionInfo.interaction_dy / 500.0f;

            this.UpdateCameraTransform();

            TCP_Qt_Server.InteractionInfo.interaction_type = "none";
        }
        if (TCP_Qt_Server.InteractionInfo.interaction_type == "Zoom")  // ���ִ�������Ų���
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


    // �������λ��
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



    // �����¼�
    bool m_camera_move_flag = false;  // ����Ƿ����ƶ�

    void DoKey_MoveEvent()
    {
        // ZX-CV-BN: ����ѡ��С���ʱ������XYZ���ƶ�
        // ��XYZ����ת��
        // ��ѡ��һ���ڵ�Ļ����� -> ���������alt�������϶�һ������


        ///////////////// �ƶ���� WASD /////////////////
        if (Input.GetKeyDown(KeyCode.LeftControl))  // control����������/�ر�
        {
            m_camera_move_flag = !m_camera_move_flag;
        }


        if (m_camera_move_flag)
        {
            float Ztranslation = Input.GetAxis("Vertical") * moveVerticalSpeed * Time.deltaTime;
            float Xtranslation = Input.GetAxis("Horizontal") * moveHorizontalSpeed * Time.deltaTime;
            float Ytranslation = Input.GetAxis("Jump") * moveJumpSpeed * Time.deltaTime;

            //���¼�����ת�Ƕ�
            angleX += Input.GetAxis("Mouse X");  // ˮƽ����ת���ĽǶ� [-width, width]
            angleY += Input.GetAxis("Mouse Y");  // ��ֱ����ת���ĽǶ� [-height, height]

            if (angleX < 0)
                angleX += 360;
            if (angleY > 360)
                angleX -= 360;
            angleY = Mathf.Clamp(angleY, -89, 89);

            //----------------------��ת�ӽ�---------------------------
            transform.forward = new Vector3(
                1.0f * Mathf.Cos(3.141592653f * angleY / 180.0f) * Mathf.Sin(3.141592653f * angleX / 180.0f),
                1.0f * Mathf.Sin(3.141592653f * angleY / 180.0f),
                1.0f * Mathf.Cos(3.141592653f * angleY / 180.0f) * Mathf.Cos(3.141592653f * angleX / 180.0f)
                ).normalized;  // ����Ĭ�϶���(0,0,1)

            // --------------WASD�ƶ� & space����------------------
            transform.Translate(0, 0, Ztranslation);
            transform.Translate(Xtranslation, 0, 0);
            transform.Translate(0, Ytranslation, 0);
        }
    }



}
