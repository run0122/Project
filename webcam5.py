import cv2
import serial

# 시리얼 포트 설정
ser = serial.Serial('COM4', 9600)

cap = cv2.VideoCapture(0)

while True:
    # 프레임 읽어들이기
    ret, frame = cap.read()

    # frame = cv2.resize(frame, (0, 0), fx=0.5, fy=0.5)

    # 프레임의 가로와 세로 크기 구하기
    height, width, channels = frame.shape

    # 중앙에 위치한 영역 추출하기
    crop_width = int(width/4)
    crop_height = int(height/4)
    start_x = int(width/2) - crop_width
    start_y = int(height/2) - crop_height
    end_x = int(width/2) + crop_width
    end_y = int(height/2) + crop_height
    cropped_frame = frame[start_y:end_y, start_x:end_x]

    # 그레이스케일 이미지로 변환하기
    gray = cv2.cvtColor(cropped_frame, cv2.COLOR_BGR2GRAY)

    # 이미지 이진화하기
    _, thresh = cv2.threshold(gray, 127, 255, cv2.THRESH_BINARY_INV)

    # 컨투어 찾기
    contours, _ = cv2.findContours(
        thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    # 컨투어 중 가장 큰 영역 찾기
    max_area = 0
    max_contour = None
    for contour in contours:
        area = cv2.contourArea(contour)
        if area > max_area:
            max_area = area
            max_contour = contour

    # 가장 큰 영역의 중심점 찾기
    if max_contour is not None:
        M = cv2.moments(max_contour)
        if M["m00"] != 0:
            cx = int(M["m10"] / M["m00"])
            cy = int(M["m01"] / M["m00"])

            # 중심점 그리기
            cv2.circle(thresh, (cx, cy), 5, (0, 0, 255), -1)

            # 중심점 정보 시리얼 통신으로 전송하기
            # if cx < 300:
            #     ser.write(b'L')
            # elif cx > 340:
            #     ser.write(b'R')
            # else:
            #     ser.write(b'F')

            print("Center point: ({}, {})".format(cx, cy))

    # 프레임 보여주기
    cv2.imshow("Line Tracer", thresh)

    # 종료 키 검사하기
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# 자원 해제하기
cap.release()
cv2.destroyAllWindows()
