using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class CameraCtrl_Orbit : MonoBehaviour
{
    private Vector3 targetPos = new Vector3(0.0f, 5.0f, 0.0f); // Ŀ�����壨������ԭ�㣩
    private float distance = 20f; // ��ʼ����
    private float zoomSpeed = 2f;
    private float rotationSpeed = 3f;

    private float currentX = -90f;
    private float currentY = 0f;
    private float minY = -10f;
    private float maxY = 80f;

    void Update()
    {
        // ��������ת
        if (Input.GetMouseButton(0))
        {
            currentX += Input.GetAxis("Mouse X") * rotationSpeed;
            currentY -= Input.GetAxis("Mouse Y") * rotationSpeed;
            currentY = Mathf.Clamp(currentY, minY, maxY);
        }

        // ����������
        float scroll = Input.GetAxis("Mouse ScrollWheel");
        distance -= scroll * zoomSpeed;
        distance = Mathf.Clamp(distance, 1f, 100f);
    }

    void LateUpdate()
    {

        Quaternion rotation = Quaternion.Euler(currentY, currentX, 0);
        Vector3 dir = new Vector3(0, 0, -distance);
        transform.position = targetPos + rotation * dir;

        if (transform.position.y <= 0.05f)
            transform.position = new Vector3(transform.position.x, 0.05f, transform.position.z);
        transform.LookAt(targetPos);
    }
}