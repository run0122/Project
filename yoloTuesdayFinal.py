import cv2
import numpy as np
import torch
import serial

ser = serial.Serial('COM3', 9600)

model = torch.hub.load('ultralytics/yolov5', 'yolov5s', pretrained=True)

cap = cv2.VideoCapture(0)  # 0번 카메라 연결
while True:
    ret, frame = cap.read()  # 프레임 읽기
    if not ret:
        break
    results = model(frame)  # 객체 감지 수행

    # 결과를 처리합니다.
    cx_list = []
    cy_list = []
    for detection in results.xyxy[0]:
        # 객체의 위치와 클래스 정보를 가져옵니다.
        x1, y1, x2, y2, conf, cls = detection
        label = model.names[int(cls)]

        # person 객체일 때만 그리기
        if label == 'person':
            # 객체의 위치에 사각형을 그려줍니다.
            cv2.rectangle(frame, (int(x1), int(y1)), (int(x2), int(y2)), (0, 255, 0), 2)
            # 객체의 클래스 이름과 정확도를 화면에 출력합니다.
            cv2.putText(frame, f'{label} {conf:.2f}', (int(x1), int(y1 - 10)), cv2.FONT_HERSHEY_SIMPLEX, 0.9,
                        (0, 255, 0), 2)

            # 객체의 중심점(cx, cy) 계산
            cx = int((x1 + x2) / 2)
            cy = int((y1 + y2) / 2)
            cx_list.append(cx)
            cy_list.append(cy)

            cv2.circle(frame, (cx, cy), 5, (0, 0, 255), -1)

    # 여러 명의 person 객체의 중심점 평균 계산
    if len(cx_list) > 0 and len(cy_list) > 0:
        avg_cx = int(np.mean(cx_list))
        avg_cy = int(np.mean(cy_list))

        # 중심점 정보 시리얼 통신으로 전송하기
        if avg_cx < 160:
            ser.write(b'R')
        elif avg_cx > 480:
            ser.write(b'L')
        else:
            ser.write(b'F')

        cv2.circle(frame, (avg_cx, avg_cy), 5, (255, 0, 0), -1)

        print("Average Center point: ({}, {})".format(avg_cx, avg_cy))

    cv2.imshow('YOLOv5', frame)  # 화면에 프레임을 출력합니다.

    # 'q' 키를 눌러 종료합니다.
    if cv2.waitKey(1) == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()