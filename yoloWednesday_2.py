import cv2

# 웹캠에서 영상을 가져오기 위한 VideoCapture 객체 생성
cap = cv2.VideoCapture(0)

while True:
    # 영상 한 프레임씩 읽기
    ret, frame = cap.read()

    # 영상 크기 조정
    frame = cv2.resize(frame, (640, 480))

    # 그레이 스케일 이미지로 변환
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # 이진화 수행
    _, thresh = cv2.threshold(gray, 127, 255, cv2.THRESH_BINARY)

    # 컨투어 찾기
    contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # 가장 큰 컨투어 찾기
    largest_contour = max(contours, key=cv2.contourArea)

    # 가장 큰 컨투어의 바운딩 박스 구하기
    x, y, w, h = cv2.boundingRect(largest_contour)

    # 바운딩 박스 그리기
    cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

    # 가장 큰 컨투어의 중심점 찾기
    M = cv2.moments(largest_contour)
    cx = int(M["m10"] / M["m00"])
    cy = int(M["m01"] / M["m00"])

    # 중심점에 서클 그리기
    cv2.circle(frame, (cx, cy), 5, (0, 0, 255), -1)

    # 영상 출력
    cv2.imshow('frame', frame)

    # 종료 조건
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# 사용한 자원 해제
cap.release()
cv2.destroyAllWindows()