import cv2
import numpy as np
import threading
import time

cam = cv2.VideoCapture(1)
values = [(100, 100), (100, 210), (100, 320), (210, 100), (210, 210), (210, 320), (320, 100), (320, 210), (320, 320)]
colors = ['R', 'R', 'G', 'B', 'Y', 'O']
finals = ''
lower = [[0, 50, 100], [170, 100, 86], [58, 27, 100], [100, 100, 100], [24, 50, 100], [7, 50, 100]]
upper = [[6, 255, 255], [179, 255, 255], [83, 255, 255], [130, 255, 255], [42, 255, 255], [22, 255, 255]]

running = True
key = 0

s = ''

def capture_and_process():
    global s, finals, running, key
    while running:
        _, frame = cam.read()
        # print("Camera read")
        regionhsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        s = ''
        for j in values:
            color = 'W'
            for i in range(6):
                lower_bound = np.array(lower[i])
                upper_bound = np.array(upper[i])
                mask = cv2.inRange(regionhsv, lower_bound, upper_bound)
                region = mask[j[0] + 50][j[1] + 50]
                if region > 0:
                    color = colors[i]
                    break
            cv2.rectangle(frame, j, (j[0] + 100, j[1] + 100), (255, 0, 0), 3)
            s += color
        cv2.imshow('myCam', frame)
        cv2.moveWindow('myCam', 0, 0)
        cv2.rectangle(frame, (100, 100), (200, 200), (255, 0, 0), 3)
        key = cv2.waitKey(1) 
        if key == ord('q'):
            running = False
            print("key1=", key)
            break


# Start processing thread
processing_thread = threading.Thread(target=capture_and_process)
print("the thread has started 1\n\n")
processing_thread.start()
print("the thread has started 2\n\n")

# Main thread waits for 'v' key
while running:
    #print("the thread has started 3\n\n")
    time.sleep(0.001)
    if key == ord('v'):
        key = 0
        print(s)
        finals += s
        if len(finals) == 54:
            running = False            

# Wait for the processing thread to finish
processing_thread.join()
cam.release()
cv2.destroyAllWindows()

# Save the final output
with open('input.txt', 'w') as f:
    f.write(finals)
