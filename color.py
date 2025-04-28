import cv2
import numpy as np
import time

values = [
    (100, 100), (100, 210), (100, 320),
    (210, 100), (210, 210), (210, 320),
    (320, 100), (320, 210), (320, 320)
]

color_labels = {
    'R': (0, 0, 255),
    'O': (0, 128, 255),
    'G': (0, 255, 0),
    'B': (255, 0, 0),
    'Y': (0, 255, 255),
    'W': (255, 255, 255),
}

finals = ''
faces_scanned = 0
scanned_faces = []
s = ''
running = True
clicked_btn = None

scan_btn = (470, 100, 650, 170)
quit_btn = (470, 200, 650, 270)

center_colors = ['R', 'G', 'W', 'O', 'B', 'Y']

def classify_color(avg_hsv):
    h, s_, v = int(avg_hsv[0]), int(avg_hsv[1]), int(avg_hsv[2])
    if (h <= 10 or h >= 170) and s_ > 100 and v > 100:
        return 'R'
    if 11 <= h <= 20 and s_ > 100 and v > 100:  
        return 'O'
    if 21 <= h <= 40 and s_ > 100 and v > 120:  
        return 'Y'
    if 41 <= h <= 85 and s_ > 80 and v > 50:
        return 'G'
    if 90 <= h <= 130 and s_ > 80 and v > 50:
        return 'B'
    if s_ < 60 and v > 150:
        return 'W'
    return 'W'

def draw_buttons(frame):
    cv2.rectangle(frame, scan_btn[:2], scan_btn[2:], (0, 200, 0), -1 if clicked_btn == 'scan' else 2)
    cv2.putText(frame, "Scan", (scan_btn[0]+35, scan_btn[1]+50), cv2.FONT_HERSHEY_SIMPLEX, 1.5, (255,255,255), 3)
    cv2.rectangle(frame, quit_btn[:2], quit_btn[2:], (0, 0, 200), -1 if clicked_btn == 'quit' else 2)
    cv2.putText(frame, "Quit", (quit_btn[0]+35, quit_btn[1]+50), cv2.FONT_HERSHEY_SIMPLEX, 1.5, (255,255,255), 3)

def is_inside_btn(x, y, btn):
    return btn[0] <= x <= btn[2] and btn[1] <= y <= btn[3]

def mouse_callback(event, x, y, flags, param):
    global clicked_btn, running, finals, faces_scanned, scanned_faces, s
    if event == cv2.EVENT_LBUTTONDOWN:
        if is_inside_btn(x, y, scan_btn):
            clicked_btn = 'scan'
        elif is_inside_btn(x, y, quit_btn):
            clicked_btn = 'quit'
    elif event == cv2.EVENT_LBUTTONUP:
        if clicked_btn == 'scan' and is_inside_btn(x, y, scan_btn):
            if len(s) == 9 and faces_scanned < 6:
                fixed = list(s)
                fixed[4] = center_colors[faces_scanned]
                s_fixed = ''.join(fixed)
                print(s_fixed)
                finals += s_fixed
                scanned_faces.append(list(s_fixed))
                faces_scanned += 1
                if len(finals) == 54:
                    running = False
        elif clicked_btn == 'quit' and is_inside_btn(x, y, quit_btn):
            running = False
        clicked_btn = None

def draw_cube_net_on_frame(frame, scanned_faces, current_face=None, pos=(30,30), sticker=40):
    # pos: (x, y) top-left corner of net
    # sticker: size of each sticker
    face_pos = {
        0: (0, 3),  # Red (top)
        1: (3, 0),  # Blue (left)
        2: (3, 3),  # White (center)
        3: (6, 3),  # Orange (bottom)
        4: (3, 6),  # Green (right)
        5: (3, 9),  # Yellow (far right)
    }
    net_h, net_w = sticker*9, sticker*12
    net_img = np.ones((net_h, net_w, 3), dtype=np.uint8) * 40  # dark background

    # Draw scanned faces
    for face_idx, face in enumerate(scanned_faces):
        if face_idx not in face_pos:
            continue
        base_row, base_col = face_pos[face_idx]
        for i in range(3):
            for j in range(3):
                color = face[i*3 + j]
                bgr = color_labels[color]
                y1 = (base_row + i) * sticker
                x1 = (base_col + j) * sticker
                cv2.rectangle(net_img, (x1, y1), (x1+sticker-2, y1+sticker-2), bgr, -1)
                cv2.rectangle(net_img, (x1, y1), (x1+sticker-2, y1+sticker-2), (0, 0, 0), 2)
    # Draw preview face
    if current_face is not None and len(scanned_faces) < 6:
        face_idx = len(scanned_faces)
        if face_idx in face_pos:
            base_row, base_col = face_pos[face_idx]
            for i in range(3):
                for j in range(3):
                    color = current_face[i*3 + j]
                    bgr = color_labels[color]
                    y1 = (base_row + i) * sticker
                    x1 = (base_col + j) * sticker
                    cv2.rectangle(net_img, (x1, y1), (x1+sticker-2, y1+sticker-2), bgr, -1)
                    cv2.rectangle(net_img, (x1, y1), (x1+sticker-2, y1+sticker-2), (100, 100, 100), 2)

    # Overlay net_img onto frame at pos
    x0, y0 = pos
    h, w = net_img.shape[:2]
    # Resize if needed to fit in frame
    if y0+h > frame.shape[0] or x0+w > frame.shape[1]:
        scale = min((frame.shape[0]-y0)/h, (frame.shape[1]-x0)/w, 1.0)
        net_img = cv2.resize(net_img, (int(w*scale), int(h*scale)), interpolation=cv2.INTER_AREA)
        h, w = net_img.shape[:2]

    # Alpha blending for better look (optional)
    alpha = 0.8
    roi = frame[y0:y0+h, x0:x0+w]
    blended = cv2.addWeighted(roi, 1-alpha, net_img, alpha, 0)
    frame[y0:y0+h, x0:x0+w] = blended

    return frame

def main():
    global s, running

    cam = cv2.VideoCapture(1)  # Change to 0 if needed
    if not cam.isOpened():
        print("Cannot open camera")
        return

    cv2.namedWindow('myCam')
    cv2.setMouseCallback('myCam', mouse_callback)

    font = cv2.FONT_HERSHEY_SIMPLEX

    while running:
        ret, frame = cam.read()
        if not ret:
            print("Failed to grab frame")
            break

        frame = cv2.GaussianBlur(frame, (5, 5), 0)
        regionhsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        detected_colors = [''] * 9

        for idx, (y, x) in enumerate(values):
            if idx == 4 and faces_scanned < 6:
                color = center_colors[faces_scanned]
            else:
                sample = regionhsv[y+20:y+80, x+20:x+80]
                avg_hsv = np.median(sample.reshape(-1, 3), axis=0)
                color = classify_color(avg_hsv)
            border_color = color_labels[color]
            cv2.rectangle(frame, (x, y), (x+100, y+100), border_color, 3)
            text_color = (0, 0, 0) if color == 'W' else (255, 255, 255)
            cv2.putText(frame, color, (x+35, y+65), font, 1.8, text_color, 3, cv2.LINE_AA)
            cv2.putText(frame, str(idx+1), (x+10, y+30), font, 0.7, (0, 0, 0), 2, cv2.LINE_AA)
            detected_colors[idx] = color

        s = ''.join(detected_colors)

        # Draw UI
        draw_buttons(frame)
        cv2.putText(frame, f"Detected: {s}", (30, 70), font, 1.2, (255, 255, 255), 2, cv2.LINE_AA)
        cv2.putText(frame, f"Face {faces_scanned+1}/6", (505, 60), font, 1.0, (255,255,0), 2)
        cv2.putText(frame, "Align cube face, click Scan", (30, 470), font, 0.8, (255, 255, 0), 2)

        # Overlay the live net in the top right corner (new size)
        frame = draw_cube_net_on_frame(frame, scanned_faces, list(s) if faces_scanned < 6 else None, pos=(frame.shape[1]-500-20, 20), sticker=40)

        cv2.imshow('myCam', frame)
        key = cv2.waitKey(1) & 0xFF
        if key == ord('q'):
            running = False

        if len(finals) == 54:
            running = False

    cam.release()
    cv2.destroyAllWindows()

    if len(finals) == 54:
        with open('input.txt', 'w') as f:
            f.write(finals)
        print("Scan complete! Saved to input.txt")
    else:
        print("Scan incomplete or cancelled.")

if __name__ == "__main__":
    main()
