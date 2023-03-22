#!/usr/bin/env python
# -*- coding:utf-8 -*-
# @FileName  :find_face_landmarks.py
# @Author    :Ouyang Bin

import cv2
import sys
import dlib

def find_face(filename):
    face_detector = dlib.get_frontal_face_detector()  # 创建HOG人脸检测器
    image = cv2.imread(filename)
    detected_faces = face_detector(image, 1)
    location = []
    for i, face_rect in enumerate(detected_faces):
        location.append(face_rect.left())
        location.append(face_rect.top())
        location.append(face_rect.right())
        location.append(face_rect.bottom())

    return location
        

def find_face_landmarks(filename):
    # 人脸81个关键点模型位置 
    predictor_model = r"E:\Face_Landmarks\shape_predictor_81_face_landmarks.dat"

    face_detector = dlib.get_frontal_face_detector()  # 创建HOG人脸检测器
    face_pose_predictor = dlib.shape_predictor(predictor_model)  # 创建人脸特征点检测器

    image = cv2.imread(filename)

    detected_faces = face_detector(image, 1)

    mark = []

    for i, face_rect in enumerate(detected_faces):
        pose_landmarks = face_pose_predictor(image, face_rect)  # 获取面部的姿势
        for j in range(81):
            mark.append((pose_landmarks.part(j).x, pose_landmarks.part(j).y))
            
    return mark